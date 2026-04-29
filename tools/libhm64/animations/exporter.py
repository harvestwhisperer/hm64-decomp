"""
Regenerate `src/data/animation/*` C source from the authoritative JSON in
`assets/animationScripts/`.

Per-script and per-asset files are rewritten wholesale; the three aggregator
files (`entityAnimationScripts.c`, `entityAnimationScripts.h`,
`entityAnimationLabels.h`) are hand-maintained because the `.c` aggregator's
`#include` order dictates ROM layout.

The output is compile-equivalent to the original hand-written C (same bytes
after linking), not necessarily byte-identical C source. Matching-build
invariants are preserved because:
  1. Every PACK_ANIM_DATA entry keeps its offset/type/flip values.
  2. Trailing zero padding is reproduced exactly from `trailing_zeros`.
  3. Array storage class and name are unchanged.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from libhm64.sprites.addresses import get_all_sprites, get_sprite_by_label

REPO_ROOT = Path(__file__).resolve().parents[3]
SPRITES_ROOT = REPO_ROOT / "assets" / "sprites"
JSON_AVATAR = SPRITES_ROOT / "overlayScreens" / "characterAvatars" / "characterAvatars.json"

C_ENTITY_DIR = REPO_ROOT / "src" / "data" / "animation" / "entityAnimationScripts"
C_LABELS_DIR = C_ENTITY_DIR / "animationLabels"
C_AVATAR_DIR = REPO_ROOT / "src" / "data" / "animation" / "characterAvatars"
C_STANDALONE_DIR = REPO_ROOT / "src" / "data" / "animation" / "standalone"

ANIMATION_SCRIPTS_CSV = REPO_ROOT / "tools" / "libhm64" / "data" / "animation_scripts_addresses.csv"


def _entity_include_order() -> list[str]:
    """Return entity script names in ROM address order.

    Excludes `characterAvatars` which shares the ROM region but belongs
    to the separate avatar aggregator, not this one.
    """
    import csv
    names: list[str] = []
    with ANIMATION_SCRIPTS_CSV.open() as f:
        for row in csv.reader(f):
            if not row:
                continue
            name = row[2]
            if name == "characterAvatars":
                continue
            names.append(name)
    return names

ANIM_TYPE_C = {
    "ONE_SHOT": "ANIM_TYPE_ONE_SHOT",
    "LOOP": "ANIM_TYPE_LOOP",
    "DESTROY_ON_END": "ANIM_TYPE_DESTROY_ON_END",
}


# --- entity script emission ----------------------------------------------

def emit_entity_inc_c(data: dict, script_name: str) -> str:
    array_name = f"{script_name}AnimationScripts"
    animations = data["animations"]
    trailing_zeros = data["trailing_zeros"]

    lines: list[str] = [
        '#include "common.h"',
        "",
        '#include "entityAnimationScripts.h"',
        "",
        f"u16 {array_name}[] = {{",
    ]

    total = len(animations) + trailing_zeros
    for i, entry in enumerate(animations):
        is_last = (i + 1 == total)
        comma = "" if is_last else ","
        anim_type = ANIM_TYPE_C[entry["anim_type"]]
        flip = "FLIP_HORIZONTAL" if entry["flip"] else "0"
        lines.append(
            f"\tPACK_ANIM_DATA({entry['metadata_index']}, {anim_type}, {flip}){comma}"
        )
    for j in range(trailing_zeros):
        is_last = (len(animations) + j + 1 == total)
        comma = "" if is_last else ","
        lines.append(f"\t0{comma}")
    lines.append("};")
    return "\n".join(lines) + "\n"


def emit_entity_labels_h(data: dict, script_name: str) -> str:
    script_upper = _camel_to_snake(script_name)
    prefix = f"{script_upper}_ANIMATION"

    guard = f"_{script_upper}_ANIMATION_MACROS_H_"
    lines: list[str] = [
        f"#ifndef {guard}",
        f"#define {guard}",
        "",
    ]
    for flat_idx, entry in enumerate(data["animations"]):
        macro = f"{prefix}_{entry['label']}"
        lines.append(f"#define {macro:<40s} {flat_idx}")
    lines.append("")
    lines.append("#endif")
    return "\n".join(lines) + "\n"


def emit_entity_scripts_aggregator_c() -> str:
    lines = [
        '#include "common.h"',
        "",
        '#include "entityAnimationScripts.h"',
        "",
    ]
    for name in _entity_include_order():
        lines.append(f'#include "{name}.inc.c"')
    return "\n".join(lines) + "\n"


def emit_entity_scripts_aggregator_h() -> str:
    lines = [
        "#ifndef _ENTITY_ANIMATION_SCRIPTS_H_",
        "#define _ENTITY_ANIMATION_SCRIPTS_H_",
        "",
        '#include "common.h"',
        "",
        '#include "entityAnimationLabels.h"',
        "",
        "#define ANIM_TYPE_ONE_SHOT           0x0000  // type 0xFF",
        "#define ANIM_TYPE_LOOP               0x2000  // type 0xFE",
        "#define ANIM_TYPE_DESTROY_ON_END     0x4000  // type 0xFD",
        "",
        "#define FLIP_HORIZONTAL              0x8000",
        "",
        "#define PACK_ANIM_DATA(metadata_offset, anim_type, horizontal_flip) \\",
        "    ((metadata_offset) | (anim_type) | (horizontal_flip))",
        "",
    ]
    for name in _entity_include_order():
        lines.append(f"extern u16 {name}AnimationScripts[];")
    lines.append("")
    lines.append("#endif")
    return "\n".join(lines) + "\n"


def emit_entity_labels_aggregator_h() -> str:
    lines = [
        "#ifndef _ENTITY_ANIMATION_MACROS_H_",
        "#define _ENTITY_ANIMATION_MACROS_H_",
        "",
    ]
    for name in _entity_include_order():
        lines.append(f'#include "animationLabels/{name}.h"')
    lines.append("")
    lines.append("#endif")
    return "\n".join(lines) + "\n"


def _camel_to_snake(camel: str) -> str:
    out: list[str] = []
    for ch in camel:
        if ch.isupper() and out and out[-1] != "_":
            out.append("_")
        out.append(ch.upper())
    return "".join(out)


# --- avatar mapping emission ---------------------------------------------

def emit_avatar_inc_c(data: dict) -> str:
    lines: list[str] = [
        '#include "common.h"',
        "",
        '#include "characterAvatars.h"',
        "",
        "u8 characterAvatarCodesToAnimations[] = {",
    ]
    nums = [str(e["metadata_index"]) for e in data["entries"]]
    for i in range(0, len(nums), 16):
        chunk = nums[i:i+16]
        is_last = (i + len(chunk) >= len(nums))
        joined = ", ".join(chunk)
        lines.append(f"\t{joined}" + ("" if is_last else ","))
    lines.append("};")
    return "\n".join(lines) + "\n"


def emit_avatar_h(data: dict) -> str:
    lines: list[str] = [
        "#ifndef _CHARACTER_AVATARS_H_",
        "#define _CHARACTER_AVATARS_H_",
        "",
        "extern u8 characterAvatarCodesToAnimations[];",
        "",
    ]
    for flat_idx, entry in enumerate(data["entries"]):
        macro = f"DIALOGUE_AVATARS_{entry['label']}"
        lines.append(f"#define {macro:<40s} {flat_idx}")
    lines.append("")
    lines.append("#endif")
    return "\n".join(lines) + "\n"


# --- standalone seeding + emission --------------------------------------

def _metadata_indices_for(sprite_dir: Path) -> list[int]:
    animations_dir = sprite_dir / "animations"
    if not animations_dir.is_dir():
        return []
    indices: list[int] = []
    for entry in sorted(animations_dir.iterdir()):
        if entry.suffix == ".json" and entry.stem.isdigit():
            indices.append(int(entry.stem))
    return indices


def seed_standalone_if_missing(sprite_dir: Path, sprite_name: str) -> dict:
    json_path = sprite_dir / "animationLabels.json"
    if json_path.is_file():
        return json.loads(json_path.read_text())
    indices = _metadata_indices_for(sprite_dir)
    data = {
        "kind": "standalone",
        "sprite": sprite_name,
        "labels": [
            {"metadata_index": idx, "label": str(idx)}
            for idx in indices
        ],
    }
    json_path.write_text(json.dumps(data, indent=2) + "\n")
    return data


def emit_standalone_h(data: dict) -> str:
    sprite = data["sprite"]
    sprite_upper = _camel_to_snake(sprite)
    guard = f"_{sprite_upper}_FRAME_MACROS_H_"
    lines: list[str] = [
        f"#ifndef {guard}",
        f"#define {guard}",
        "",
    ]
    for entry in data["labels"]:
        macro = f"{sprite_upper}_FRAME_{entry['label']}"
        lines.append(f"#define {macro:<40s} {entry['metadata_index']}")
    lines.append("")
    lines.append("#endif")
    return "\n".join(lines) + "\n"


# --- driver --------------------------------------------------------------

def export_all() -> None:
    C_ENTITY_DIR.mkdir(parents=True, exist_ok=True)
    C_LABELS_DIR.mkdir(parents=True, exist_ok=True)
    C_AVATAR_DIR.mkdir(parents=True, exist_ok=True)
    C_STANDALONE_DIR.mkdir(parents=True, exist_ok=True)

    entity_count = 0
    avatar_count = 0
    standalone_count = 0
    skipped_standalone = 0

    for info in get_all_sprites():
        sprite_dir = SPRITES_ROOT / info.subdir / info.label
        if info.kind == "entity":
            for json_path in sorted(sprite_dir.glob("animationScripts/*.json")):
                data = json.loads(json_path.read_text())
                name = json_path.stem
                (C_ENTITY_DIR / f"{name}.inc.c").write_text(emit_entity_inc_c(data, name))
                (C_LABELS_DIR / f"{name}.h").write_text(emit_entity_labels_h(data, name))
                entity_count += 1
        elif info.kind == "avatar":
            json_path = sprite_dir / "characterAvatars.json"
            if not json_path.is_file():
                continue
            data = json.loads(json_path.read_text())
            (C_AVATAR_DIR / "characterAvatars.inc.c").write_text(emit_avatar_inc_c(data))
            (C_AVATAR_DIR / "characterAvatars.h").write_text(emit_avatar_h(data))
            avatar_count += 1
        elif info.kind == "standalone":
            if not sprite_dir.is_dir():
                # Sprite has not been extracted yet (animations/ metadata absent).
                # Skip — re-run exporter after extract-sprites to pick it up.
                skipped_standalone += 1
                continue
            data = seed_standalone_if_missing(sprite_dir, info.label)
            (C_STANDALONE_DIR / f"{info.label}.h").write_text(emit_standalone_h(data))
            standalone_count += 1

    # Aggregators (entity only; no standalone aggregator per spec).
    (C_ENTITY_DIR / "entityAnimationScripts.c").write_text(emit_entity_scripts_aggregator_c())
    (C_ENTITY_DIR / "entityAnimationScripts.h").write_text(emit_entity_scripts_aggregator_h())
    (C_ENTITY_DIR / "entityAnimationLabels.h").write_text(emit_entity_labels_aggregator_h())

    print(f"Entity: {entity_count}, Avatar: {avatar_count}, Standalone: {standalone_count} (skipped {skipped_standalone} missing)")


def export_sprite(name: str) -> None:
    info = get_sprite_by_label(name)
    if info is None:
        raise SystemExit(f"sprite {name!r} not in CSV")

    sprite_dir = SPRITES_ROOT / info.subdir / info.label
    if info.kind == "entity":
        for json_path in sorted(sprite_dir.glob("animationScripts/*.json")):
            data = json.loads(json_path.read_text())
            n = json_path.stem
            (C_ENTITY_DIR / f"{n}.inc.c").write_text(emit_entity_inc_c(data, n))
            (C_LABELS_DIR / f"{n}.h").write_text(emit_entity_labels_h(data, n))
    elif info.kind == "avatar":
        data = json.loads((sprite_dir / "characterAvatars.json").read_text())
        (C_AVATAR_DIR / "characterAvatars.inc.c").write_text(emit_avatar_inc_c(data))
        (C_AVATAR_DIR / "characterAvatars.h").write_text(emit_avatar_h(data))
    elif info.kind == "standalone":
        if not sprite_dir.is_dir():
            raise SystemExit(f"sprite {name!r} directory does not exist yet: {sprite_dir}")
        data = seed_standalone_if_missing(sprite_dir, info.label)
        (C_STANDALONE_DIR / f"{info.label}.h").write_text(emit_standalone_h(data))


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(prog="libhm64.animations.exporter")
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--all", action="store_true", help="Export all sprites + regenerate aggregators")
    g.add_argument("--sprite", metavar="NAME", help="Export a single sprite (no aggregator changes)")
    args = p.parse_args(argv)

    if args.all:
        export_all()
    else:
        export_sprite(args.sprite)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
