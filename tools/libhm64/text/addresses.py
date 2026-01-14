"""
Text bank address parsing for HM64.

Parses text_addresses.csv to locate text banks in ROM and
text_selection_segments.csv to identify selection menu segments.
"""

import csv
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Set, Tuple

from ..data import TEXT_ADDRESSES_CSV, TEXT_SELECTION_SEGMENTS_CSV


# Banks where the LAST segment contains junk data (not real text)
# Used in modding mode to skip extraction of ROM artifacts
JUNK_DATA_BANKS = {
    'ann', 'elli', 'farmVisits', 'jeff', 'maria',
    'popuri', 'shop', 'text54', 'text65', 'tv'
}


@dataclass
class TextBankInfo:
    """Information about a text bank in the ROM."""
    name: str
    index_start: int
    index_end: int
    text_start: int
    game_index: int
    csv_row: int = 0

    @property
    def index_size(self) -> int:
        """Size of the index table in bytes."""
        return self.index_end - self.index_start

    @property
    def index_count(self) -> int:
        """Number of entries in the index table."""
        return self.index_size // 4

    @property
    def text_end(self) -> int:
        """End of text data (starts at index table)."""
        return self.index_start


# Cache for text banks
_text_banks_cache: Optional[List[TextBankInfo]] = None

# Cache for selection segments
_selection_segments_cache: Optional[Set[Tuple[str, int]]] = None


def _load_text_banks() -> List[TextBankInfo]:
    """Load text bank addresses from CSV."""
    global _text_banks_cache

    if _text_banks_cache is not None:
        return _text_banks_cache

    _text_banks_cache = []

    if not TEXT_ADDRESSES_CSV.exists():
        print(f"Warning: Text addresses CSV not found: {TEXT_ADDRESSES_CSV}")
        return _text_banks_cache

    with open(TEXT_ADDRESSES_CSV, 'r', newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)

        for row_num, row in enumerate(reader, start=2):
            try:
                bank = TextBankInfo(
                    name=row['name'].strip(),
                    index_start=int(row['index_start'], 16),
                    index_end=int(row['index_end'], 16),
                    text_start=int(row['text_start'], 16),
                    game_index=int(row.get('game_index', 0)),
                    csv_row=row_num
                )
                _text_banks_cache.append(bank)
            except (ValueError, KeyError) as e:
                print(f"Warning: Error parsing row {row_num}: {e}")
                continue

    return _text_banks_cache


def _load_selection_segments() -> Set[Tuple[str, int]]:
    """Load selection segment indices from CSV."""
    global _selection_segments_cache

    if _selection_segments_cache is not None:
        return _selection_segments_cache

    _selection_segments_cache = set()

    if not TEXT_SELECTION_SEGMENTS_CSV.exists():
        return _selection_segments_cache

    with open(TEXT_SELECTION_SEGMENTS_CSV, 'r', newline='', encoding='utf-8') as f:
        # Filter out comment lines
        lines = [line for line in f if not line.startswith('#')]
        reader = csv.DictReader(lines)

        for row in reader:
            try:
                bank_name = row['bank_name'].strip()
                segment_index = int(row['segment_index'])
                _selection_segments_cache.add((bank_name, segment_index))
            except (ValueError, KeyError):
                continue

    return _selection_segments_cache


def get_all_text_banks() -> List[TextBankInfo]:
    """Get all text banks from CSV."""
    return _load_text_banks()


def get_text_bank_by_name(name: str) -> Optional[TextBankInfo]:
    """Get a specific text bank by name."""
    banks = _load_text_banks()
    for bank in banks:
        if bank.name == name:
            return bank
    return None


def get_text_bank_by_game_index(game_index: int) -> Optional[TextBankInfo]:
    """Get a text bank by its game index."""
    banks = _load_text_banks()
    for bank in banks:
        if bank.game_index == game_index:
            return bank
    return None


def is_selection_segment(bank_name: str, segment_index: int) -> bool:
    """Check if a segment is a selection menu (pink dialogue box)."""
    selection_segments = _load_selection_segments()
    return (bank_name, segment_index) in selection_segments


def get_selection_segments() -> Set[Tuple[str, int]]:
    """Get all selection segment (bank_name, index) pairs."""
    return _load_selection_segments()


def clear_cache():
    """Clear cached data (useful for testing)."""
    global _text_banks_cache, _selection_segments_cache
    _text_banks_cache = None
    _selection_segments_cache = None
