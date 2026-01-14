"""
Map address parsing and area/variant logic for HM64.

Maps are organized by area (e.g., farm, mountain, village) and variant
(e.g., seasonal variants like spring, summer, fall, winter).
"""

import csv
import re
from pathlib import Path
from typing import List, Tuple, Dict, Optional

import numpy as np

from ..common.rom import get_rom
from ..data import MAP_ADDRESSES_CSV

# Cache for CSV data
_csv_cache: Optional[List[Tuple[str, ...]]] = None


def _load_csv_cache() -> List[Tuple[str, ...]]:
    """Load and cache CSV data."""
    global _csv_cache
    if _csv_cache is None:
        with open(MAP_ADDRESSES_CSV, newline='', encoding='utf-8') as f:
            reader = csv.reader(f)
            _csv_cache = [tuple(row) for row in reader]
    return _csv_cache


def get_all_map_addresses() -> List[Tuple[str, ...]]:
    """Get all map addresses from CSV."""
    return _load_csv_cache()


def clear_cache():
    """Clear the cached CSV data."""
    global _csv_cache
    _csv_cache = None


# Explicit area mappings for non-obvious cases
_EXPLICIT_AREA_MAPPINGS = {
    # Ranch area
    'annsRoom': 'ranch',
    'ranchBarn': 'ranch',
    'ranchStore': 'ranch',
    'ranchHouse': 'ranch',
    # Bakery area
    'ellisRoom': 'bakery',
    'bakery': 'bakery',
    # Flower shop area
    'popurisRoom': 'flowerShop',
    'flowerShop': 'flowerShop',
    # Church (standalone)
    'church': 'church',
    # Souvenir shop (standalone)
    'souvenirShop': 'souvenirShop',
    # Rick's shop
    'ricksShop': 'rickShop',
    # Midwife
    'midwifesHouse': 'midwife',
    # Tavern (standalone)
    'tavern': 'tavern',
    # Library (standalone)
    'library': 'library',
    # Mayor's house area
    'mariasRoom': 'mayorsHouse',
    'mayorsHouse': 'mayorsHouse',
    # Potion shop area
    'potionShopBackroom': 'potionShop',
    'potionShop': 'potionShop',
    # Cave area
    'spriteCave': 'cave',
    'harvestSpriteCave': 'cave',
    'cave': 'cave',
    # Mine area
    'emptyMine': 'mine',
    'mine': 'mine',
    # Vineyard area
    'karensRoom': 'vineyard',
    'vineyardHouse': 'vineyard',
    'vineyardCellar': 'vineyard',
    'vineyardCellar1': 'vineyard',
    'vineyardCellar2': 'vineyard',
    # Farm area (player's farm)
    'greenhouse': 'farm',
    'barn': 'farm',
    'coop': 'farm',
    # House area (player's house)
    'house': 'house',
    'kitchen': 'house',
    'bathroom': 'house',
    # Carpenter/dumpling (standalone)
    'carpentersHut': 'carpenterHut',
    'carpenterHut': 'carpenterHut',
    'dumplingHouse': 'dumplingHouse',
    # Empty/unknown
    'emptyMap1': 'unknown',
    'emptyMap2': 'unknown',
    'emptyMap3': 'unknown',
    'emptyMap4': 'unknown',
}

# Nested area patterns (mountain1, village2, etc.)
_NESTED_PATTERNS = [
    (r'^mountain1', 'mountain/mountain1'),
    (r'^mountain2', 'mountain/mountain2'),
    (r'^topOfMountain1', 'mountain/topOfMountain1'),
    (r'^moonMountain', 'mountain/moonMountain'),
    (r'^village1', 'village/village1'),
    (r'^village2', 'village/village2'),
]

# Simple seasonal areas
_SIMPLE_AREAS = ['farm', 'beach', 'pond', 'road', 'square', 'vineyard',
                 'ranch', 'raceTrack']


def get_map_area(map_name: str) -> str:
    """
    Get the base area for a map (without variant/season).

    Examples:
        farmSpring -> farm
        mountain1Spring -> mountain/mountain1
        barn -> farm
        bakery -> bakery
    """
    if map_name in _EXPLICIT_AREA_MAPPINGS:
        return _EXPLICIT_AREA_MAPPINGS[map_name]

    for pattern, area in _NESTED_PATTERNS:
        if re.match(pattern, map_name):
            return area

    for area in _SIMPLE_AREAS:
        if map_name.startswith(area):
            return area

    # Default: use the map name as the area
    return map_name


def get_map_variant(map_name: str) -> str:
    """
    Get the variant/season subdirectory for a map.

    For seasonal maps, returns the season (lowercase).
    For non-seasonal maps in shared areas, returns the map name.
    For standalone maps, returns the map name.

    Examples:
        farmSpring -> spring
        mountain1Winter -> winter
        barn -> barn
        bakery -> bakery
    """
    seasonal_match = re.search(r'(Spring|Summer|Fall|Winter)$', map_name)
    if seasonal_match:
        return seasonal_match.group(1).lower()

    return map_name


def get_map_area_path(map_name: str) -> str:
    """
    Get the full directory path for a map (area + variant).

    Examples:
        farmSpring -> farm/spring
        mountain1Spring -> mountain/mountain1/spring
        barn -> farm/barn
        cave -> cave
        bakery -> bakery
    """
    area = get_map_area(map_name)
    variant = get_map_variant(map_name)

    # Avoid redundant paths like cave/cave or bakery/bakery
    if area == variant or area.endswith(f"/{variant}"):
        return area

    return f"{area}/{variant}"


def get_asset_offsets_array(row: Tuple[str, ...]) -> np.ndarray:
    """Get the asset offsets array for a map row."""
    rom = get_rom()
    asset_index_base = int(row[0], 16)
    # element 12 is always 0 padding
    return np.frombuffer(rom, dtype=np.dtype(">u4"), count=12, offset=asset_index_base)


def get_texture_offsets_array(map_start: int, texture_offsets_start: int) -> np.ndarray:
    """Get texture offsets array for a map section."""
    rom = get_rom()

    first_offset = np.frombuffer(
        rom, dtype=np.dtype(">u4"), count=1,
        offset=map_start + texture_offsets_start
    ).item()

    count = first_offset // 4
    offsets = np.frombuffer(
        rom, dtype=np.dtype(">u4"), count=count,
        offset=map_start + texture_offsets_start
    )

    # Handle offset arrays that have 0 padding at the end
    nonzeros = np.nonzero(offsets)[0]
    last = nonzeros[-1] if len(nonzeros) > 0 else 0

    return offsets[:last + 1]


def get_palette_offsets_array(map_start: int, palette_offsets_start: int) -> np.ndarray:
    """Get palette offsets array for a map section."""
    rom = get_rom()

    first_offset = np.frombuffer(
        rom, dtype=np.dtype(">u4"), count=1,
        offset=map_start + palette_offsets_start
    ).item()

    count = first_offset // 4
    offsets = np.frombuffer(
        rom, dtype=np.dtype(">u4"), count=count,
        offset=map_start + palette_offsets_start
    )

    # Handle offset arrays that have 0 padding at the end
    nonzeros = np.nonzero(offsets)[0]
    last = nonzeros[-1] if len(nonzeros) > 0 else 0

    return offsets[:last + 1]
