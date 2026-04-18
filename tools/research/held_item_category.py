#!/usr/bin/env python3
"""
Resolve VAR_ITEM_BEING_HELD dialogue categories to raw item IDs.

Dialogue bytecode branches on VAR_ITEM_BEING_HELD (dialogue variable 6), which
holds a remapped category number rather than the raw item ID. The remap lives
in heldItemIndices[] in src/game/items.c, and is applied by getHeldItemIndex()
before the value is written to gItemBeingHeld. Raw item IDs are named in the
/* item indices */ block of src/game/items.h.

Usage:
    held_item_category.py                    full table (category -> items)
    held_item_category.py <category>         items mapping to a specific category
                                             (decimal or 0xNN)
    held_item_category.py --item <id|name>   category a specific raw item maps to
"""

import argparse
import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
ITEMS_C = REPO_ROOT / "src/game/items.c"
ITEMS_H = REPO_ROOT / "src/game/items.h"


def parse_indices_table() -> list[int]:
    text = ITEMS_C.read_text()
    m = re.search(
        r'u8\s+heldItemIndices\[\]\s*=\s*\{(.*?)\};', text, re.DOTALL
    )
    if not m:
        sys.exit(f"could not find heldItemIndices[] in {ITEMS_C}")
    body = re.sub(r'//[^\n]*', '', m.group(1))
    body = re.sub(r'/\*.*?\*/', '', body, flags=re.DOTALL)
    return [int(tok, 0) for tok in re.findall(r'0x[0-9A-Fa-f]+|\d+', body)]


def parse_item_names() -> dict[int, list[str]]:
    """Parse the /* item indices */ block of items.h into {raw_id: [names]}."""
    text = ITEMS_H.read_text()
    m = re.search(
        r'/\*\s*item indices\s*\*/(.*?)/\*\s*bottle', text, re.DOTALL
    )
    if not m:
        sys.exit(f"could not find /* item indices */ block in {ITEMS_H}")
    names: dict[int, list[str]] = {}
    for line in m.group(1).splitlines():
        if re.match(r'\s*//', line):
            continue
        dm = re.match(r'\s*#define\s+(\w+)\s+(\d+)', line)
        if dm:
            names.setdefault(int(dm.group(2)), []).append(dm.group(1))
    return names


def build_reverse(
    indices: list[int], names: dict[int, list[str]]
) -> dict[int, list[tuple[int, list[str]]]]:
    out: dict[int, list[tuple[int, list[str]]]] = {}
    for raw_id, cat in enumerate(indices):
        out.setdefault(cat, []).append((raw_id, names.get(raw_id, [])))
    return out


def format_raw(raw_id: int, name_list: list[str]) -> str:
    names_str = ", ".join(name_list) if name_list else "(unnamed)"
    return f"    raw 0x{raw_id:02X} ({raw_id:>3})  {names_str}"


def cmd_full_table(
    indices: list[int], names: dict[int, list[str]]
) -> None:
    rev = build_reverse(indices, names)
    for cat in sorted(rev):
        if cat == 0:
            continue
        entries = rev[cat]
        print(f"category 0x{cat:02X} ({cat})  [{len(entries)} raw id(s)]:")
        for raw_id, name_list in entries:
            print(format_raw(raw_id, name_list))
        print()


def cmd_category(
    indices: list[int], names: dict[int, list[str]], cat_arg: str
) -> None:
    try:
        cat = int(cat_arg, 0)
    except ValueError:
        sys.exit(f"category must be an integer (got {cat_arg!r})")
    rev = build_reverse(indices, names)
    entries = rev.get(cat, [])
    if not entries:
        print(f"no raw item IDs map to category 0x{cat:02X} ({cat})")
        return
    print(f"category 0x{cat:02X} ({cat})  [{len(entries)} raw id(s)]:")
    for raw_id, name_list in entries:
        print(format_raw(raw_id, name_list))


def cmd_item(
    indices: list[int], names: dict[int, list[str]], item_arg: str
) -> None:
    try:
        raw_id = int(item_arg, 0)
    except ValueError:
        raw_id = None
        for rid, name_list in names.items():
            if item_arg in name_list:
                raw_id = rid
                break
        if raw_id is None:
            sys.exit(
                f"no item named {item_arg!r} in items.h /* item indices */ block"
            )
    if raw_id < 0 or raw_id >= len(indices):
        sys.exit(
            f"raw id {raw_id} out of range (table has {len(indices)} entries)"
        )
    cat = indices[raw_id]
    name_list = names.get(raw_id, [])
    names_str = ", ".join(name_list) if name_list else "(unnamed)"
    print(
        f"raw 0x{raw_id:02X} ({raw_id})  {names_str}"
        f"  ->  category 0x{cat:02X} ({cat})"
    )


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument(
        "category",
        nargs="?",
        help="dialogue category (decimal or 0xNN); omit for full table",
    )
    ap.add_argument(
        "--item", help="raw item ID (decimal/0xNN) or #define name from items.h"
    )
    args = ap.parse_args()

    indices = parse_indices_table()
    names = parse_item_names()

    if args.item is not None:
        cmd_item(indices, names, args.item)
    elif args.category is not None:
        cmd_category(indices, names, args.category)
    else:
        cmd_full_table(indices, names)


if __name__ == "__main__":
    main()
