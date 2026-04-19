---
description: Look up the extracted PNG(s) for a sprite segment's animation script entry (PACK_ANIM_DATA). Usage -- /lookup-sprite <segment> <anim_index> [--paths-only] [--frame N]
---

You are looking up extracted PNG assets for a Harvest Moon 64 animation script entry.

Arguments: `$ARGUMENTS`

Steps:

1. If `$ARGUMENTS` is empty, stop and ask the user for `<segment> <anim_index>` (e.g. `holdableItems 103`). The segment is a label from `tools/libhm64/data/sprite_addresses.csv` and `anim_index` is the `N` in `PACK_ANIM_DATA(N, ...)`.
2. Run: `python3 tools/research/lookup_sprite.py $ARGUMENTS`
3. Show the script's output verbatim to the user. Do not summarize or rename files.
4. If the script errors (unknown segment, missing animation JSON, etc.), pass the error through and suggest the likely fix:
   - Unknown segment: point them to `tools/libhm64/data/sprite_addresses.csv` for the list of valid labels.
   - Missing JSON: suggest running `make extract-animations` (or the equivalent extraction target) if assets haven't been extracted.
5. Do not modify any files. This is a read-only lookup.
