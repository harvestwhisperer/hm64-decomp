"""
HM64 data files package.

Contains CSV address files and other data used by the extraction tools.
"""

from pathlib import Path

DATA_DIR = Path(__file__).parent

SPRITE_ADDRESSES_CSV = DATA_DIR / "sprite_addresses.csv"
TEXT_ADDRESSES_CSV = DATA_DIR / "text_addresses.csv"
TEXT_SELECTION_SEGMENTS_CSV = DATA_DIR / "text_selection_segments.csv"
MAP_ADDRESSES_CSV = DATA_DIR / "map_addresses.csv"
ANIMATION_SCRIPTS_ADDRESSES_CSV = DATA_DIR / "animation_scripts_addresses.csv"
CUTSCENE_ADDRESSES_CSV = DATA_DIR / "cutscene_addresses.csv"
CUTSCENE_ENTRY_POINTS_CSV = DATA_DIR / "cutscene_entry_points.csv"
DIALOGUE_BYTECODE_ADDRESSES_CSV = DATA_DIR / "dialogue_bytecode_addresses.csv"

SPRITE_ADDRESSES_JP_CSV = DATA_DIR / "sprite_addresses.jp.csv"
TEXT_ADDRESSES_JP_CSV = DATA_DIR / "text_addresses.jp.csv"
MAP_ADDRESSES_JP_CSV = DATA_DIR / "map_addresses.jp.csv"
ANIMATION_SCRIPTS_ADDRESSES_JP_CSV = DATA_DIR / "animation_scripts_addresses.jp.csv"
CUTSCENE_ADDRESSES_JP_CSV = DATA_DIR / "cutscene_addresses.jp.csv"
DIALOGUE_BYTECODE_ADDRESSES_JP_CSV = DATA_DIR / "dialogue_bytecode_addresses.jp.csv"

from .symbols import ROM_SEGMENT_START_SYMBOLS, ROM_SEGMENT_END_SYMBOLS
