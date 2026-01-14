"""
Sprite address parsing and lookup utilities.

Parses the sprite_addresses.csv file and provides cached lookup of sprite info.
"""

import csv
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional

from ..common import rom
from ..data import SPRITE_ADDRESSES_CSV

_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_CSV_PATH = SPRITE_ADDRESSES_CSV
DEFAULT_OUTPUT_DIR = _REPO_DIR / "assets" / "sprites"

# Cached data
_sprites: Optional[List['SpriteInfo']] = None
_label_index: Optional[Dict[str, 'SpriteInfo']] = None


@dataclass
class SpriteInfo:
    """Information about a sprite asset from the CSV."""
    sprite_type: str  # 'type-1' or 'type-2'
    addr_base: int
    addr_index: int
    label: str
    subdir: str
    # Type-1 specific
    spritesheet_index_base: Optional[int] = None
    spritesheet_index_end: Optional[int] = None
    # Type-2 specific
    addr_extra: Optional[int] = None
    # Row index in CSV (for animation utilities compatibility)
    csv_row: int = 0

    @property
    def is_type1(self) -> bool:
        return self.sprite_type == 'type-1'

    @property
    def is_type2(self) -> bool:
        return self.sprite_type == 'type-2'


def _load_sprites(csv_path: Path = DEFAULT_CSV_PATH) -> None:
    """Load and cache sprite data from CSV."""
    global _sprites, _label_index

    _sprites = []
    _label_index = {}

    with open(csv_path, newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        for row_idx, row in enumerate(reader):
            row = [c.strip() for c in row]

            if len(row) == 6:
                # Type-1: base, index, spritesheet_index_base, spritesheet_index_end, label, subdir
                info = SpriteInfo(
                    sprite_type='type-1',
                    addr_base=int(row[0], 16),
                    addr_index=int(row[1], 16),
                    spritesheet_index_base=int(row[2], 16),
                    spritesheet_index_end=int(row[3], 16),
                    label=row[4],
                    subdir=row[5],
                    csv_row=row_idx
                )
            elif len(row) == 5:
                # Type-2: base, index, extra, label, subdir
                info = SpriteInfo(
                    sprite_type='type-2',
                    addr_base=int(row[0], 16),
                    addr_index=int(row[1], 16),
                    addr_extra=int(row[2], 16),
                    label=row[3],
                    subdir=row[4],
                    csv_row=row_idx
                )
            else:
                continue

            _sprites.append(info)
            _label_index[info.label] = info


def get_all_sprites(csv_path: Path = DEFAULT_CSV_PATH) -> List[SpriteInfo]:
    """Get list of all sprite info from CSV."""
    if _sprites is None:
        _load_sprites(csv_path)
    return _sprites


def get_sprite_by_label(label: str, csv_path: Path = DEFAULT_CSV_PATH) -> Optional[SpriteInfo]:
    """Get sprite info by label name."""
    if _label_index is None:
        _load_sprites(csv_path)
    return _label_index.get(label)


def clear_cache() -> None:
    """Clear cached sprite data."""
    global _sprites, _label_index
    _sprites = None
    _label_index = None


def get_asset_offsets(addr_index: int) -> List[int]:
    """Read 8 u32 offsets from asset index."""
    return [rom.read_u32_be(addr_index + i * 4) for i in range(8)]


def get_spritesheet_offsets(info: SpriteInfo) -> List[int]:
    """Get spritesheet offset table for a sprite."""
    if info.is_type1:
        return _get_spritesheet_offsets_type1(info)
    else:
        return _get_spritesheet_offsets_type2(info)


def _get_spritesheet_offsets_type1(info: SpriteInfo) -> List[int]:
    """Get spritesheet offset table for type-1 sprites."""
    base = info.spritesheet_index_base
    end = info.spritesheet_index_end
    count = (end - base) // 4
    offsets = [rom.read_u32_be(base + i * 4) for i in range(count)]

    # Special case: festivalFlowers has embedded spritesheet index at start of texture
    if info.label == 'festivalFlowers':
        embedded_index_size = count * 4
        last_nonzero = len(offsets) - 1
        while last_nonzero >= 0 and offsets[last_nonzero] == 0:
            last_nonzero -= 1
        offsets = [
            off + embedded_index_size if i <= last_nonzero else 0
            for i, off in enumerate(offsets)
        ]

    return offsets


def _get_spritesheet_offsets_type2(info: SpriteInfo) -> List[int]:
    """Get spritesheet offset table for type-2 sprites."""
    base = info.addr_base
    first_offset = rom.read_u32_be(base)

    # Validate: first_offset should be reasonable (< 64KB) and aligned
    if first_offset == 0 or first_offset > 0x10000 or first_offset % 4 != 0:
        return []

    count = first_offset // 4
    # Sanity check: max 1024 sprites per asset
    if count > 1024:
        return []

    return [rom.read_u32_be(base + i * 4) for i in range(count)]


def get_palette_offsets(addr_base: int, asset_offsets: List[int]) -> List[int]:
    """Get palette offset table."""
    palette_section = addr_base + asset_offsets[1]
    first_offset = rom.read_u32_be(palette_section)

    # Validate: first_offset should be reasonable (< 4KB) and aligned
    if first_offset == 0 or first_offset > 0x1000 or first_offset % 4 != 0:
        return []

    count = first_offset // 4
    # Sanity check: max 256 palettes
    if count > 256:
        return []

    return [rom.read_u32_be(palette_section + i * 4) for i in range(count)]


def get_animation_offsets(addr_base: int, asset_offsets: List[int]) -> List[int]:
    """Get animation offset table."""
    anim_section = addr_base + asset_offsets[2]
    first_offset = rom.read_u32_be(anim_section)

    if first_offset == 0:
        return []

    count = first_offset // 4
    return [rom.read_u32_be(anim_section + i * 4) for i in range(count)]


def get_sprite_to_palette_map(addr_base: int, asset_offsets: List[int], sprite_count: int) -> List[int]:
    """Get sprite-to-palette mapping array."""
    map_base = addr_base + asset_offsets[3] + 4  # Skip header
    rom_data = rom.get_rom()
    return list(rom_data[map_base:map_base + sprite_count])
