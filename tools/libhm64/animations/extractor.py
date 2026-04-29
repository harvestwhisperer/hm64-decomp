"""
Extract animation script data, character-avatar mapping, and standalone
animation labels from the ROM into JSON under `assets/sprites/...`.

Modding branches (`dev`, `dev-qol`) drop the splat animation-script and
character-avatar segments and run this extractor inline with
`make extract-sprites` to bootstrap the JSON source-of-truth files. The
exporter then regenerates the C/H files from JSON on every build.

Idempotent by default: any JSON already on disk is left alone so editor
edits aren't clobbered. Pass `--force` to re-extract from the ROM.

Inputs:
  - `tools/libhm64/data/animation_scripts_addresses.csv`  (entity + avatar ROM ranges)
  - `tools/libhm64/data/sprite_addresses.csv`             (sprite kinds and subdirs, via SpriteInfo)
  - `baserom.us.z64`                                       (the ROM)
  - `assets/sprites/<subdir>/<sprite>/animations/*.json`   (standalone metadata indices, populated by extract-sprites)

Outputs:
  - `assets/sprites/<subdir>/<sprite>/animationScripts/<name>.json`   (entity)
  - `assets/sprites/overlayScreens/characterAvatars/characterAvatars.json` (avatar)
  - `assets/sprites/<subdir>/<sprite>/animationLabels.json`           (standalone)

Standalone seeding requires `make extract-sprites` to have already populated
the per-sprite `animations/` directory; sprites without that directory are
skipped with a count printed at the end.
"""

from __future__ import annotations

import argparse
import csv
import json
from dataclasses import dataclass
from pathlib import Path

from libhm64.common import rom
from libhm64.sprites.addresses import get_all_sprites, get_sprite_by_label

REPO_ROOT = Path(__file__).resolve().parents[3]
SPRITES_ROOT = REPO_ROOT / "assets" / "sprites"
ANIMATION_SCRIPTS_CSV = REPO_ROOT / "tools" / "libhm64" / "data" / "animation_scripts_addresses.csv"

# Some animation scripts share a sprite directory (e.g. the four "holdable"
# scripts all live under the `holdableItems` sprite). Mirrors the migrator.
SCRIPT_SPRITE_OVERRIDES: dict[str, str] = {
    "expressionBubbles": "holdableItems",
    "heldItems": "holdableItems",
    "thrownItems": "holdableItems",
    "toolEffects": "holdableItems",
}

AVATAR_SCRIPT_NAME = "characterAvatars"
# The avatar table is 143 entries; the splat segment range (F7118-F71B0) is
# 152 bytes because the next segment is 8-byte aligned. The 9 trailing bytes
# are alignment padding emitted by the linker, not part of the C array.
AVATAR_COUNT = 143

MASK_METADATA_OFFSET = 0x1FFF
MASK_ANIMATION_TYPE = 0x6000
MASK_HORIZONTAL_FLIP = 0x8000

ANIM_TYPE_FROM_BITS = {
    0x0000: "ONE_SHOT",
    0x2000: "LOOP",
    0x4000: "DESTROY_ON_END",
}


@dataclass
class ScriptRange:
    rom_start: int
    rom_end: int
    name: str


def _load_script_ranges() -> list[ScriptRange]:
    ranges: list[ScriptRange] = []
    with ANIMATION_SCRIPTS_CSV.open() as f:
        for row in csv.reader(f):
            if not row:
                continue
            ranges.append(ScriptRange(int(row[0], 16), int(row[1], 16), row[2]))
    return ranges


# --- entity script extraction -------------------------------------------

def _extract_entity_data(rom_start: int, rom_end: int) -> tuple[list[dict], int]:
    """Decode u16 entries; trailing zeros are tracked separately for matching."""
    words: list[int] = []
    for pos in range(rom_start, rom_end, 2):
        words.append(rom.read_u16_be(pos))

    last_nonzero = len(words)
    while last_nonzero > 0 and words[last_nonzero - 1] == 0:
        last_nonzero -= 1
    trailing_zeros = len(words) - last_nonzero

    animations: list[dict] = []
    for idx, word in enumerate(words[:last_nonzero]):
        metadata_index = word & MASK_METADATA_OFFSET
        anim_type = ANIM_TYPE_FROM_BITS.get(word & MASK_ANIMATION_TYPE, "ONE_SHOT")
        flip = (word & MASK_HORIZONTAL_FLIP) != 0
        animations.append({
            "label": str(idx),
            "metadata_index": metadata_index,
            "anim_type": anim_type,
            "flip": flip,
            "directional": False,
        })
    return animations, trailing_zeros


def _build_entity_json(name: str, sprite: str, rom_start: int, rom_end: int) -> dict:
    animations, trailing_zeros = _extract_entity_data(rom_start, rom_end)
    return {
        "kind": "entity",
        "sprite": sprite,
        "animations": animations,
        "trailing_zeros": trailing_zeros,
    }


# --- avatar mapping extraction ------------------------------------------

def _build_avatar_json(rom_start: int) -> dict:
    # `character` is left blank for the editor (or the modder) to fill in
    # later; `expression` is seeded with the entry index so the table is at
    # least disambiguable post-extraction.
    entries = [
        {
            "character": "",
            "expression": str(i),
            "metadata_index": rom.read_u8(rom_start + i),
        }
        for i in range(AVATAR_COUNT)
    ]
    return {"kind": "avatar", "entries": entries}


# --- standalone label seeding -------------------------------------------

def _metadata_indices_from_animations_dir(sprite_dir: Path) -> list[int] | None:
    """Returns sorted metadata indices found under `animations/`, or `None` if
    the directory is missing entirely (signal to run extract-sprites first).
    An empty list means the sprite genuinely has no animation metadata."""
    animations_dir = sprite_dir / "animations"
    if not animations_dir.is_dir():
        return None
    indices: list[int] = []
    for entry in sorted(animations_dir.iterdir()):
        if entry.suffix == ".json" and entry.stem.isdigit():
            indices.append(int(entry.stem))
    return indices


def _build_standalone_json(sprite_name: str, indices: list[int]) -> dict:
    return {
        "kind": "standalone",
        "sprite": sprite_name,
        "labels": [{"metadata_index": idx, "label": str(idx)} for idx in indices],
    }


# --- writing ------------------------------------------------------------

def _write_json(path: Path, data: dict, force: bool) -> bool:
    if path.is_file() and not force:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + "\n")
    return True


# --- driver -------------------------------------------------------------

def extract_all(force: bool = False) -> None:
    entity_written = 0
    avatar_written = 0
    standalone_written = 0
    skipped_existing = 0
    standalone_skipped_no_animations = 0
    unknown_sprite = 0

    for r in _load_script_ranges():
        sprite_name = SCRIPT_SPRITE_OVERRIDES.get(r.name, r.name)
        info = get_sprite_by_label(sprite_name)
        if info is None:
            print(f"WARN: sprite {sprite_name!r} not in sprite_addresses.csv; skipping {r.name}")
            unknown_sprite += 1
            continue
        sprite_dir = SPRITES_ROOT / info.subdir / info.label

        if r.name == AVATAR_SCRIPT_NAME:
            json_path = sprite_dir / "characterAvatars.json"
            if _write_json(json_path, _build_avatar_json(r.rom_start), force):
                avatar_written += 1
            else:
                skipped_existing += 1
        else:
            json_path = sprite_dir / "animationScripts" / f"{r.name}.json"
            data = _build_entity_json(r.name, sprite_name, r.rom_start, r.rom_end)
            if _write_json(json_path, data, force):
                entity_written += 1
            else:
                skipped_existing += 1

    for info in get_all_sprites():
        if info.kind != "standalone":
            continue
        sprite_dir = SPRITES_ROOT / info.subdir / info.label
        json_path = sprite_dir / "animationLabels.json"
        if json_path.is_file() and not force:
            skipped_existing += 1
            continue
        indices = _metadata_indices_from_animations_dir(sprite_dir)
        if indices is None:
            standalone_skipped_no_animations += 1
            continue
        data = _build_standalone_json(info.label, indices)
        if _write_json(json_path, data, force):
            standalone_written += 1

    print(
        f"entity: {entity_written} written, "
        f"avatar: {avatar_written} written, "
        f"standalone: {standalone_written} written"
    )
    if skipped_existing or standalone_skipped_no_animations or unknown_sprite:
        notes = []
        if skipped_existing:
            notes.append(f"{skipped_existing} skipped (already exist; use --force)")
        if standalone_skipped_no_animations:
            notes.append(f"{standalone_skipped_no_animations} standalone skipped (no animations/ dir yet — run extract-sprites first)")
        if unknown_sprite:
            notes.append(f"{unknown_sprite} script(s) had no matching sprite in CSV")
        print("  " + "; ".join(notes))


def extract_sprite(name: str, force: bool = False) -> None:
    info = get_sprite_by_label(name)
    if info is None:
        raise SystemExit(f"sprite {name!r} not in sprite_addresses.csv")
    sprite_dir = SPRITES_ROOT / info.subdir / info.label

    if info.kind == "avatar":
        for r in _load_script_ranges():
            if r.name == AVATAR_SCRIPT_NAME:
                json_path = sprite_dir / "characterAvatars.json"
                if _write_json(json_path, _build_avatar_json(r.rom_start), force):
                    print(f"wrote {json_path}")
                else:
                    print(f"skip {json_path} (exists; use --force)")
                return
        raise SystemExit("characterAvatars range not found in animation_scripts_addresses.csv")

    if info.kind == "entity":
        wrote = 0
        skipped = 0
        for r in _load_script_ranges():
            mapped = SCRIPT_SPRITE_OVERRIDES.get(r.name, r.name)
            if mapped != info.label:
                continue
            json_path = sprite_dir / "animationScripts" / f"{r.name}.json"
            data = _build_entity_json(r.name, info.label, r.rom_start, r.rom_end)
            if _write_json(json_path, data, force):
                wrote += 1
                print(f"wrote {json_path}")
            else:
                skipped += 1
                print(f"skip {json_path} (exists; use --force)")
        if wrote == 0 and skipped == 0:
            print(f"no animation scripts found in CSV for sprite {name!r}")
        return

    # standalone
    json_path = sprite_dir / "animationLabels.json"
    if json_path.is_file() and not force:
        print(f"skip {json_path} (exists; use --force)")
        return
    indices = _metadata_indices_from_animations_dir(sprite_dir)
    if indices is None:
        print(f"no animations/ dir under {sprite_dir}; run extract-sprites first")
        return
    data = _build_standalone_json(info.label, indices)
    _write_json(json_path, data, force)
    print(f"wrote {json_path}")


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(prog="libhm64.animations.extractor")
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--all", action="store_true", help="Extract all sprites (entity + avatar + standalone)")
    g.add_argument("--sprite", metavar="NAME", help="Extract a single sprite by label")
    p.add_argument("--force", action="store_true", help="Overwrite existing JSON files")
    args = p.parse_args(argv)

    if args.all:
        extract_all(force=args.force)
    else:
        extract_sprite(args.sprite, force=args.force)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
