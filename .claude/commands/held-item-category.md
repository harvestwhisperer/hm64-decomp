---
description: Resolve a VAR_ITEM_BEING_HELD dialogue category to raw item IDs (and back). Usage -- /held-item-category [category | --item <id|name>]
---

You are resolving VAR_ITEM_BEING_HELD dialogue categories against the `heldItemIndices[]` remap table in HM64.

Arguments: `$ARGUMENTS`

Steps:

1. If `$ARGUMENTS` is empty, run `python3 tools/research/held_item_category.py` to dump the full pivot table.
2. Otherwise run `python3 tools/research/held_item_category.py $ARGUMENTS`. Accepted forms:
   - `<category>` — a dialogue-category integer (decimal or `0xNN`), e.g. `52` or `0x34`.
   - `--item <id|name>` — a raw item ID (decimal or `0xNN`) or a `#define` name from the `/* item indices */` block of `src/game/items.h` (e.g. `TURNIP_HELD_ITEM`, `BABY_HELD_ITEM`).
3. Show the script's output verbatim. Do not summarize or reformat entries.
4. If the user is trying to name a segment based on the result, remind them that a handful of cutscene sites in `src/game/cutscenes.c` write `gItemBeingHeld` directly (bypassing `getHeldItemIndex`) — so a category that only makes sense in a cutscene context may not come through the lookup table. Also note that category `0` means "no dialogue category" (the raw item is ignored by dialogue dispatch).
5. Do not modify any files. This is a read-only lookup.
