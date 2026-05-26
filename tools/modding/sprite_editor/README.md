# HM64 Sprite Editor

A standalone desktop editor for Harvest Moon 64 sprites

---

## Contents

- [HM64 Sprite Editor](#hm64-sprite-editor)
  - [Contents](#contents)
  - [Setup](#setup)
    - [1. Get a workspace with extracted assets](#1-get-a-workspace-with-extracted-assets)
    - [2. Launch the editor](#2-launch-the-editor)
    - [3. Getting your changes into a ROM](#3-getting-your-changes-into-a-rom)
  - [Terminology](#terminology)
  - [Tour of the window](#tour-of-the-window)
  - [Creating a new sprite](#creating-a-new-sprite)
  - [Editing an existing sprite](#editing-an-existing-sprite)
    - [Palettes](#palettes)
    - [Textures](#textures)
    - [Animations](#animations)
    - [Animation scripts and labels](#animation-scripts-and-labels)
      - [Animation labeling](#animation-labeling)
      - [Directional groups (entity only)](#directional-groups-entity-only)
  - [Fonts](#fonts)
    - [Glyph editing](#glyph-editing)
    - [Font palette editing](#font-palette-editing)
    - [Creating a new font](#creating-a-new-font)
  - [Asset formats reference](#asset-formats-reference)
    - [Per-sprite directory layout](#per-sprite-directory-layout)
    - [Sprite type 1 vs. type 2](#sprite-type-1-vs-type-2)
    - [CI4, CI8, and CI2](#ci4-ci8-and-ci2)
    - [Animation metadata: frames and layers](#animation-metadata-frames-and-layers)
    - [Entity animation scripts](#entity-animation-scripts)
    - [Export bin file layout](#export-bin-file-layout)
      - [Sprite bins](#sprite-bins)
        - [`<name>Texture.bin`](#nametexturebin)
        - [`<name>AssetsIndex.bin`](#nameassetsindexbin)
        - [`<name>SpritesheetIndex.bin` (type-1 only)](#namespritesheetindexbin-type-1-only)
      - [Font bins](#font-bins)
  - [License](#license)

---

## Setup

### 1. Get a workspace with extracted assets

The editor needs an **asset workspace** — a directory laid out like the
decomp's repo root, with `assets/sprites/...` and (optionally) `assets/font/`
underneath it. You can produce one two ways:

**Option A — extract directly from a ROM.** Click **Extract ROM…** in the
toolbar, point it at an unmodified US Harvest Moon 64 ROM (.z64/byteswapped), and choose
an empty workspace folder. The editor verifies the ROM and then populates `assets/sprites/` and
`assets/font/`.

**Option B — use the decomp repo.** Clone the HM64 decomp and check out the
`dev` or `dev-qol` branch and run `make extract` from the repo root. Then
open the repo root in the editor. Any edits made in the editor to sprite assets will be picked up on subsequent builds.

### 2. Launch the editor

Download the release for your platform from the GitHub Releases page and
run it. On first launch, click **Open…** and pick your workspace
directory. The choice is remembered across sessions.

### 3. Getting your changes into a ROM

There are two ways to ship new assets into the game.

**Pathway A — Export bins (no build environment required).** With a
sprite (or font) loaded, click **Export bins…** and choose an output
folder. The editor writes ROM-ready binary files:

- For type-1 sprites: `<name>Texture.bin`, `<name>AssetsIndex.bin`,
  `<name>SpritesheetIndex.bin`.
- For type-2 sprites: `<name>Texture.bin`, `<name>AssetsIndex.bin`
  (the spritesheet index is embedded in the texture bin).
- For fonts: `fontTexture.bin` plus one `fontPaletteN.bin` per selected
  palette.

See [Export bin file layout](#export-bin-file-layout) for what's inside
each file.

These match the layout the original ROM uses, so anyone with a working
HM64 decomp build (modding branch) can drop them into the appropriate
segment of their build and produce a patched ROM. This is the right
pathway if you're authoring graphics for someone else to integrate —
you don't need WSL/Docker, the build toolchain, or any of the decomp setup.

**Pathway B — Save asset files and rebuild yourself.** If you have the decomp repo set up, click **Save
Sprite** to write the loaded asset back to the workspace. Files get saved to `assets/sprites/<category>/<sprite>/` (palettes, textures,
animations, manifest) and, for entity sprites,
`src/data/animationScripts/<name>.inc.c`. The next `make` on the
modding branch (`dev` or `dev-qol`) picks them up, creates the necessary bin files, and links them into the ROM. This requires the full decomp build environment (WSL2 +
`mips-linux-gnu-` cross toolchain on Windows or native tools on Debian/Ubuntu/macOS).

With either approach, net new entity sprite assets still need the small
manual C / `spec` wiring listed at the end of
[Creating a new sprite](#creating-a-new-sprite) before the game can
address them.

---

## Terminology

**Sprite asset.** The whole bundle that the editor treats as one unit
and that the game addresses as one entity: textures, palettes,
animation metadata, and the lookup tables that tie them together. A sprite asset corresponds to an `assets/sprites/<category>/<name>/` directory; in a ROM
build, it's the 2–3 `.bin` files that **Export bins…** writes
(`<name>Texture.bin`, `<name>AssetsIndex.bin`, and — type-1 only —
`<name>SpritesheetIndex.bin`). Whenever this README says "sprite" in
the context of the browser, the **+ New Sprite** wizard, or
**Save Sprite**, it means a sprite *asset*. Not to be confused with the
lowercase, individual-image meaning below.

**Texture (a.k.a. "sprite," lowercase).** A single image: one
`textures/NNN.ci` raw-index file plus its `NNN.png` indexed-PNG copy.
The game never references textures/images directly — they are reached *only* through the animation system,
which addresses them by spritesheet index. The editor's UI uses "texture" to keep the
distinction clear.

**Palette.** An array of RGBA color entries (16 for CI4 textures, 256
for CI8, 16 for fonts — only the first 4 of which actually render).
Stored as `.pal` files (RGBA5551 big-endian, 2 bytes per color). A
texture stores palette *indices*; the palette turns those indices into
actual colors at render time.

**Animation / animation metadata.** A timeline of frames, stored as
`animations/NN.json`. Each frame holds a duration and one or more
**layers**; each layer points at a texture by spritesheet index and
positions it via `(anchor_x, anchor_y)`. The single word "animation"
gets used loosely in this codebase — it can mean a multi-frame
sequence, a single composite frame (when only one frame matters), or
just the set of textures reachable from that JSON via its layer
indices. The phrase "animation metadata" specifically means the JSON
file and its data.

**Animation script.** Entity sprites-only. A flat list of packed 16-bit values (in the code, this looks like `PACK_ANIM_DATA(...)` entries in the exported `.inc.c`) that wraps
animation metadata with playback flags: which animation to play
(by `metadata_index`), how it loops (`ONE_SHOT` / `LOOP` /
`DESTROY_ON_END`), whether to flip horizontally. Game code calls into an entry by its
byte offset inside the script — that offset is what the exported `.h`
file's macros expand to. (See more at 
[Animation scripts and labels](#animation-scripts-and-labels)).

---

## Tour of the window

```
┌─ Toolbar ────────────────────────────────────────────────────────────┐
│  + New Sprite   Delete   Open…   Extract ROM…   Save Sprite          │
│  Export bins…   New Font…   Undo   Redo                    Scale ▾  │
├─ Sprite browser ────┬─ Tabs ─────────────────────────────────────────┤
│  entitySprites      │  Textures │ Palettes │ Animations │ Script    │
│   ├ animals         │  ┌──────────────────────────────────────────┐  │
│   │   ├ bunny  ◀── │  │                                           │  │
│   │   └ cow         │  │  Active tab content                      │  │
│   ├ npc             │  │                                          │  │
│   └ player          │  └──────────────────────────────────────────┘  │
│  mapObjects         │                                                │
│  effects            │                                                │
│  overlayScreens     │                                                │
│  font               │                                                |
└─────────────────────┴────────────────────────────────────────────────┘
```

- **Sprite browser (left dock).** Browses every sprite directory under
  `assets/sprites/` plus any font directory. Click a leaf to load it.
  Filter narrows the tree to matching names.
- **Tabs (right).** When a sprite is loaded: **Textures**, **Palettes**,
  **Animations**, **Script**. When a font is loaded: **Glyphs** and
  **Palettes**.
- **Save Sprite.** Writes every dirty domain (palettes, textures,
  animations, animation scripts) for the loaded asset. Tabs with unsaved
  changes show a star. Undo / Redo are global, with a 500 ms merge window
  so drags coalesce into single undo steps.

---

## Creating a new sprite

1. Click **+ New Sprite** in the toolbar.
2. Pick a **category** — one of:
   - `entitySprites` — moving game-world entities (NPCs, animals, the
     player, expression bubbles). Need an animation script.
   - `mapObjects` — static map objects (mountain boulder) and water effects. These do not include foragable items or billboarded sprites that are included in the map data itself (trees, fences, signs). 
   - `effects` — particle / overlay sprites (smoke, sparks, etc.).
   - `overlayScreens` — UI / HUD imagery, items, and character dialogue portraits/avatars.
3. Pick a **type** (`type-1` or `type-2`). See
   [Sprite type 1 vs. type 2](#sprite-type-1-vs-type-2). When in doubt,
   follow the type used by the same asset category (type 1 for entities, etc.)
4. Optionally tick **Seed from indexed PNG(s)** and pick one or more 4-bit
   or 8-bit indexed PNGs (PNGs MUST be indexed). The wizard imports each picked PNG as a starting
   texture with its embedded palette. Duplicate palettes are detected and only created once
5. Click **Create**. The wizard scaffolds a fresh asset directory at
   `assets/sprites/<category>/<name>/` with `manifest.json` and empty
   `textures/`, `palettes/`, and `animations/` subdirectories.

**Manual build wiring.** Adding a brand-new sprite
the *game* will reference requires five additional code and build changes the editor
doesn't handle:

1. Add a segment entry for the sprite's binary in the necessary sprite `spec` file (i.e., `assets/sprites/entitySprites/entitySprites.spec`).
2. For entities, add the exported animation script files ("<name>.inc.c" and "animatonLabels/<name>.h") to the `src/data/animation/entityAnimationScripts` directory.
3. Add `#include "<name>.inc.c"` to
   `src/data/animationScripts/animationScripts.c`.
4. Add `extern u16 <name>AnimationScripts[];` to
   `src/data/animationScripts/animationScripts.h`.
5. Add the new `ENTITY_ASSET_*` entry in `src/game/initialize.c`.

For standalone sprites (UI, map objects, etc.), just add the new sprite asset bin files to the relevant spec files (`assets/sprtes/mapObjects/mapObjects.spec`, etc.). These sprites are accessed via direct sprite API calls (in `level.c`, `overlayScreens.c`, etc.)

For character avatars, replace `characterAvatars.inc.c` and `characterAvatars.h` in the `src/data/animation/characterAvatars` directory.

TODO: update with latest state of text transpiler

After that, `make` (on a modding branch based off of dev or dev-qol) builds a ROM that can use
the new sprite.

---

## Editing an existing sprite

Click a sprite in the browser to load it. The four tabs cover everything
on disk for that sprite.

### Palettes

A palette is a fixed-size table of colors that textures reference by
index. CI4 palettes hold 16 colors; CI8 palettes hold 256. Sprites can
own multiple palettes, and each texture is bound to exactly one.

The **Palettes** tab has three panes:

- **Palette list (left).** All palettes for this sprite. The buttons
  here are:
  - **+ New** — create an empty palette in the current sprite's format.
  - **+ Import…** — load colors from a `.pal` file (RGBA5551 big-endian,
    the format the extractor produces) or an indexed PNG. The palette's
    color count is inferred from the file.
  - **Delete** — only enabled when no texture still references the
    palette.
- **Color grid (center).** A 16×N grid of color cells. Click a cell to
  open the color picker (HSV pad, hue slider, alpha slider, `#RRGGBB`
  hex entry). Picker edits are applied as you adjust; clicking outside
  the picker closes it.
- **Texture preview (right).** Live thumbnails of every texture in the
  sprite that uses this palette. Changing a color updates all of them
  immediately.

### Textures

A texture is an image stored as **palette indices** — each pixel is a
small integer that picks a color from the texture's bound palette. The
on-disk file (`textures/NNN.ci`) is the raw index data; the sibling
`NNN.png` is a copy of the same image as an indexed PNG, kept as a
human-viewable preview. The editor reads from and writes to both.

The **Textures** tab shows:

- **Gallery (left).** All textures for the sprite as thumbnails.
- **Preview + actions (right).** The currently selected texture rendered
  through its palette. Buttons:
  - **Import…** — add a new texture from an indexed PNG. If the PNG's
    palette doesn't match any existing palette in the sprite, the editor
    will offer to add it.
  - **Replace…** — replace the selected texture's pixels (and optionally
    its palette) from a PNG. Width/height/format must remain compatible
    with the sprite's existing dimensions.
  - **Delete** — remove the texture. Any animation frame or script entry
    referencing the index is shown in the **Usage** list below so you
    can fix references first.
  - **Palette dropdown** — re-bind the texture to a different palette in
    the same sprite (must have a compatible color count).
- **Usage panel.** Lists every animation frame and every script entry
  that references the selected texture's spritesheet index. Click a row
  to jump there.

### Animations

An **animation** is the per-sprite playback timeline that animation
scripts (or the avatar table) reference by `metadata_index`. One
`animations/NN.json` file stores one animation. The number in the
filename is its index.

The **Animations** tab has:

- **Gallery (left).** All animations for this sprite, with a thumbnail
  of frame 0 and the animation's index. **+ New** creates an empty
  animation slot.
- **Playback preview (right).** Plays the selected animation through
  its bound palettes. Frame controls let you scrub a single frame at a
  time.
- **Frame editor (below).** Each frame row is expandable and shows:
  - **Duration** (game ticks). The runtime advances frames using a
    fixed-tick timer.
  - **Has audio** (yes/no) — a flag the runtime checks to decide
    whether to trigger a sound on this frame; the editor only preserves
    the value.
  - **Layers** — one or more `(spritesheet_index, anchor_x, anchor_y)`
    triples. See
    [Animation metadata: frames and layers](#animation-metadata-frames-and-layers)
    for what these mean.

Layers can be added, removed, or reordered. Anchors are signed pixel
offsets relative to the entity's origin (positive Y goes down on screen
in the editor preview).

### Animation scripts and labels

There are tree types of "scripts" based on the sprite type:

| Sprite kind  | What you edit on the Script tab                            | Where it lives on disk                                              |
|--------------|------------------------------------------------------------|---------------------------------------------------------------------|
| `entity`     | One or more entity animation scripts                       | `<sprite>/animationScripts/<name>.json` (+ exported `*.inc.c`)      |
| `avatar`     | The character portrait table that maps character+animation label (array index) to animation index                     | `assets/sprites/overlayScreens/characterAvatars/characterAvatars.json` |
| `standalone` | A flat list of `(metadata_index, label)` pairs (optional)  | `<sprite>/animationLabels.json`                                     |

The tab's layout adapts to the kind:

- **Script picker** (entity only). Most entity sprites have one script,
  but some have several. The dropdown selects which one you're editing.
- **Entry list.** Each row is one animation-script entry: an offset
  number, the bound animation's metadata index, type, flip flag, label
  text, and (entity-only) a `directional` marker.
- **Entry editor.** Edit the selected entry's fields:
  - **Offset** — display-only ordering hint (preserved verbatim).
  - **Animation type** — `ONE_SHOT`, `LOOP`, or `DESTROY_ON_END`. See
    [Entity animation scripts](#entity-animation-scripts).
  - **Flip** — when set, the runtime renders the sprite mirrored on the
    X axis. Reuses the same animation, halving texture cost.
  - **Label** — free-text human name for the entry. For avatar entries,
    `label` splits into **character** and **expression** fields (e.g.
    `MARIA` + `HAPPY`).
  - **Directional** — entity only. Marks the entry as part of an
    8-direction group (see below).
- **Preview pane.** Plays the animation referenced by the selected entry,
  flipped if the entry has `flip` set.
- **Buttons:**
  - **Add / Clone / Remove** entries.
  - **Move up / down**.
  - **Import** — bulk-replace the script from a `.inc.c` file. Useful
    for round-tripping from the legacy Python pipeline.
  - **Export inc.c** — write the active entity script back to its
    `.inc.c` file for the build.
  - **Export .h** — emit the macro header (`#define <LABEL> <index>`)
    that game code uses to refer to entries by name.

#### Animation labeling

Labels exist purely for human readability. Labels get exported as `.h` files
so game code can write a named constant
(`PLAYER_ANIMATION_WALKING_S`) instead of a raw integer offset. The
three sprite asset types label different things:

**Entity sprites — per-entry or per-group labels.** Set a label on each
script entry. Entries *outside* a directional group get one macro each
named `<SCRIPT>_ANIMATION_<LABEL>`, where `<SCRIPT>` is the script
filename (e.g. `PLAYER_ANIMATION_NAPPING` → that entry's byte offset).
Entries *inside* a directional group share a single label set on the
group's base entry, and the exporter then emits nine macros from it:

- `<SCRIPT>_ANIMATION_<LABEL>_DIRECTIONAL` — the group's base offset.
  This is the constant passed to `setEntityDirectionalAnimation()`,
  which then plays `base + direction` at runtime.
- `<SCRIPT>_ANIMATION_<LABEL>_S`, `_SW`, `_W`, `_NW`, `_N`, `_NE`, `_E`,
  `_SE` — the eight individual direction offsets, in the slot order
  described in [Directional groups](#directional-groups-entity-only).

So a `WALKING` group on the `player` script produces
`PLAYER_ANIMATION_WALKING_DIRECTIONAL` plus
`PLAYER_ANIMATION_WALKING_S` through `PLAYER_ANIMATION_WALKING_SE`.

**Standalone sprites — per-animation labels.** Each entry in the
`animationLabels.json` file labels one animation by its metadata index.
Useful for sprite assets that bundle a set of unrelated single-frame
images — for example, a shared UI sprite containing one "animation" (frame)
per button. The macro is `<SPRITE>_<LABEL>` and resolves to the
animation's metadata index directly. The "script" here is purely for labeling/code readability.

**Character dialogue avatars/portraits — array-index labels.** The
entries in `characterAvatars.json` form a mapping table from
`character + expression` to the underlying animation. Each entry stores
a `metadata_index` (the actual animation in
`characterAvatars/animations/`), but the **macro value the `.h`
exporter writes is the *array index* of the entry, not the metadata
index**. The game code does a lookup on the avatar mapping table with the array index to
get the right animation metadata index (individual portrait frame).

The macro is `DIALOGUE_AVATARS_<CHARACTER>_<EXPRESSION>`. For example,
if array index `0` holds `(character: "MARIA", expression: "DEFAULT",
metadata_index: 7)`, the exporter emits:

```c
#define DIALOGUE_AVATARS_MARIA_DEFAULT 0
```

#### Directional groups (entity only)

A directional group is an 8-entry run of script entries that share the
same logical animation across the eight on-screen directions. When the
game calls `setEntityDirectionalAnimation(base)` it plays
`base + direction` where `direction` advances through the slots in
order:

```
slot 0 = S     slot 4 = N
slot 1 = SW    slot 5 = NE
slot 2 = W     slot 6 = E
slot 3 = NW    slot 7 = SE
```

To form a group, select a row that has 8 entries available below it
and toggle **directional** ON. The eight rows are then visually
collapsible under a header showing the group's base offset and label.
A group can start at any offset (not only multiples of 8) and groups
cannot overlap.

> The compass labels above are **world** directions — the direction the
> entity is *facing* in world coordinates. Under HM64's south-west
> isometric projection, the slot 0 (`S`) art often looks "south-facing"
> on screen because the camera sits at world-SW.

---

## Fonts

The font category in the sprite browser opens the **Glyphs** and
**Palettes** tabs instead of the four sprite tabs.

### Glyph editing

The **Glyphs** tab shows the entire font as a horizontal contact sheet
plus a 280-pixel single-glyph editor on the right.

- Click a glyph in the contact sheet to load it into the editor.
- Pick an **active color** (0, 1, 2, or 3) from the radio list.
- **Left-click** a pixel in the editor to paint it with the active
  color; **right-click** to paint with color 0 (background).
- The **Palette dropdown** at the top picks which of the three font
  palettes (1, 2, 3) is used for the on-screen preview. Pixel data is
  shared across palettes — palettes recolor the same glyphs.

### Font palette editing

The **Palettes** tab works like the sprite palette editor but is
restricted to 4 visible colors per palette (color index 0 is the
runtime background and is ignored by the renderer). The remaining 12
slots in each palette are unused at runtime but preserved on disk so
loading and saving produces byte-identical output.

### Creating a new font

1. Click **New Font…** in the toolbar.
2. Enter a **name**.
3. Set a **glyph count**. The default `700` matches the original font
   and is what `src/system/message.c` hard-codes. Any other count
   requires patching that file before the new font will render in-game
   — a warning appears in the wizard when you change the value.
4. Choose an **initial palette** (Black, Greyscale, or Copy from
   default).
5. Click **Create**. A new directory is added under `assets/font/` with
   a fresh `fontTexture.ci2` and one palette file.

---

## Asset formats reference

This section describes what's actually on disk. You don't need to read
it to use the editor, but it helps when debugging round-trips or hand-
editing files.

### Per-sprite directory layout

```
assets/sprites/<category>/<sprite_name>/
├── manifest.json
├── palettes/
│   ├── 00.json    ← per-palette metadata (color count, format)
│   ├── 00.pal     ← raw colors: RGBA5551, big-endian, 2 bytes per color
│   ├── 01.json
│   └── 01.pal
├── textures/
│   ├── 000.ci     ← raw palette-index bytes (CI4 packed, CI8 flat)
│   ├── 000.png    ← indexed PNG copy of the same image
│   └── …
├── animations/
│   ├── 00.json    ← one animation per file, filename = index
│   └── …
├── animationScripts/                   (entity sprites only)
│   └── <name>.json
└── animationLabels.json                (standalone sprites only)
```

`manifest.json` records the sprite's category, type, sprite count,
palette count, per-texture format, and the sprite-to-palette mapping.
The editor maintains it automatically; manual edits aren't usually
needed.

### Sprite type 1 vs. type 2

The two types differ in how a sprite's binary section is laid out in
ROM and, correspondingly, how many `.bin` files **Export bins…** emits.
They are not interchangeable on a per-sprite basis without adjusting
the sprite's address-table entry.

**Type 1** — the spritesheet index (the table of per-texture offsets)
lives in a separate `<name>SpritesheetIndex.bin` file. The address
table uses a `spritesheet_index_base..end` range to point external
code at it. Three bin files: `Texture.bin`, `AssetsIndex.bin`,
`SpritesheetIndex.bin`.

**Type 2** — the spritesheet index is embedded inline at the start of
the texture bin's spritesheet section. The address table uses an
`addr_extra` pointer instead of a range. Two bin files only:
`Texture.bin` and `AssetsIndex.bin`.

Empirically, the two types correlate cleanly with what the sprite is
for, not with a "default vs. fallback" relationship. Of the 161
sprites in the original ROM, ~28% are type-2 and they are all
`kind: standalone` — sprites the engine renders without an animation
script driving them. The split looks like this:

| Use case                                                   | Type   |
|------------------------------------------------------------|--------|
| Anything with an animation script (NPCs, animals, player)  | Type-1 |
| Map objects (`mapObjects`)                                 | Type-1 |
| Character avatar / portrait sheet                          | Type-1 |
| UI / HUD / overlay surfaces (`overlayScreens`)             | Type-2 |
| Particle and ambient effects (`effects`)                   | Type-2 |
| Sprites driven by external C code rather than a script     | Type-2 |

So **all `effects` are type-2** (`fairies`, `fireflies`, `chimneySmoke`,
`sparklers`, `balloons`, `funeralIntro`, …) and **nearly every
`overlayScreens` sprite is type-2** (`inventoryScreen`, `messageBox`,
`calendar`, `dialogueButtonIcons`, `titleSprites`, `controllerIcons`,
and so on). `entitySprites` is almost entirely type-1, the only
exception being `shadows` (which has no animation script).

When in doubt, pick the type used by other sprites in the same category
— the wizard's category dropdown is a strong hint at which is correct.

### CI4, CI8, and CI2

All three are **palette-indexed** color formats: pixels store small
integers that look up colors in a palette table.

- **CI4** — 4 bits per pixel, 16-color palette. Two pixels per byte
  (high nibble first). The on-disk byte count is
  `ceil(width * height / 2)`. Used by sprites that need to be small.
- **CI8** — 8 bits per pixel, 256-color palette. One byte per pixel.
  The on-disk byte count is `width * height`. Used when a sprite needs
  more than 16 distinct colors.
- **CI2** — 2 bits per pixel, 4 visible colors. **Only the font uses
  it.** Each glyph is 14×14 pixels stored in a fixed 64-byte slot
  (14 rows × 4 bytes per row + 8 trailing padding bytes). The packing
  is unusual: each row's 4 bytes are read in reverse, each byte's two
  nibbles are unpacked separately, and the lower nibble of byte 0 in
  each row is unused. The editor packs and unpacks this for you and
  preserves the runtime-ignored bytes byte-for-byte across a load /
  save round-trip.

Palettes for CI4 and CI8 are stored as `.pal` files (RGBA5551 big-
endian, 2 bytes per color). Fonts use the same `.pal` format but
always have 16 entries per palette (of which 4 are actually rendered).

### Animation metadata: frames and layers

`animations/NN.json` describes a single animation as a sequence of
frames. The runtime plays one frame at a time, holding each for its
`duration` value (in game ticks) before advancing.

```jsonc
{
  "frame_count": 4,          // = frames.length
  "unused": 0,               // preserved verbatim
  "frames": [
    {
      "duration": 24,        // ticks
      "has_audio": 0,        // 0 or 1
      "sprites": [           // one or more "layers"
        { "spritesheet_index": 2, "anchor_x":  0, "anchor_y": -8 }
      ]
    },
    { "…": "next frame" }
  ],
  "index": 0                 // matches the filename stem
}
```

**Consecutive frames** are simply the entries in the `frames` array,
played top-to-bottom. The runtime loops, runs once, or destroys the
entity at the end depending on the animation type set by the *script
entry* that referenced this animation — the animation file itself
doesn't know which behavior applies.

**Layers** are the `sprites` array inside a frame. Each layer
contributes one texture composited at `(anchor_x, anchor_y)` offsets
relative to the entity's origin. Most frames use exactly one layer; a
few use several to assemble a sprite from sub-pieces (e.g. body + head
that animate at different rates by referencing the layers from
different frames). Each layer's `spritesheet_index` selects a texture
by its index in `manifest.json`.

### Entity animation scripts

An entity animation script is a flat array of u16 entries packed by the
`PACK_ANIM_DATA(metadata_index, anim_type, flip)` macro:

```c
u16 bunnyAnimationScripts[] = {
    PACK_ANIM_DATA(14, ANIM_TYPE_LOOP, 0),
    PACK_ANIM_DATA(10, ANIM_TYPE_LOOP, 0),
    PACK_ANIM_DATA(20, ANIM_TYPE_LOOP, FLIP_HORIZONTAL),
    /* … */
    0, 0, 0, 0,                   // trailing zero padding found in original rom; kept to preserve matching, but unneeded in modding builds
};
```

Per entry:

- **`metadata_index`** — the animation file under `animations/` (e.g.
  `14` references `animations/14.json`).
- **`anim_type`** — playback behavior:
  - `ANIM_TYPE_ONE_SHOT` — play through frames once and stop on the last
    frame.
  - `ANIM_TYPE_LOOP` — play forever, restarting after the last frame.
  - `ANIM_TYPE_DESTROY_ON_END` — play once, then destroy the entity.
- **`flip`** — `0` or `FLIP_HORIZONTAL`. When set, the runtime mirrors
  the sprite on the X axis at render time, so a single texture can cover both left and right directions.

Game code references entries by **byte offset** (the position in the
array × 2), which is what the exported `.h` file's macros expand to.
The editor's "Offset" column shows that offset and the `.h` exporter
writes one `#define` per labeled entry.

Trailing zeros are preserved verbatim across load / save so the
exported `.inc.c` byte-matches the input file. Group breaks (blank
lines between entries in the source `.inc.c`) are also preserved.

> Avatar and standalone sprites don't use this format. Avatars use the
> 143-entry table in `characterAvatars.json`; standalone sprites only
> store optional human labels in `animationLabels.json`. Neither needs
> a `.inc.c` exported.

### Export bin file layout

The files produced by **Export bins…** are the raw byte layouts the
engine expects in ROM.

#### Sprite bins

A sprite produces 2 or 3 files depending on whether it's type 1 or type 2. All offsets and
index entries below are 32-bit **big-endian**; texture / animation
fields are little-endian. The offsets in `<name>AssetsIndex.bin` are
how the runtime locates each section inside `<name>Texture.bin`.

##### `<name>Texture.bin`

The main bundle. Four sections concatenated end-to-end (the offsets
listed below are recorded in `AssetsIndex.bin`):

1. **Spritesheet section.** Each texture in turn, prefixed by an
   8-byte header: `34 36 64` + a format byte (`0x10` for CI4, `0x00`
   for CI8), then width and height as little-endian `s16`. The raw
   palette-index pixel bytes follow.
   - **Type-2 only:** an additional table sits at the very start of
     this section — one BE `u32` per sprite pointing at that sprite's
     header. (Type-1 sprites put that table in a separate
     `SpritesheetIndex.bin` file instead.)
2. **Palette section.** A palette index table (one BE `u32` per
   palette, each pointing at that palette's block within this section)
   followed by the palette blocks themselves. Each block is
   `34 36 64` + format byte, then the colors as RGBA5551 big-endian
   (2 bytes per color), terminated by `FF FF FF FF`.
3. **Animation section.** An animation index table (BE `u32` offsets)
   followed by animation blocks. Each block opens with
   `34 36 64 00 <frame_count u16 LE> 00 00`, then for each frame:
   `<layer_count u16 LE> <duration u8> <has_audio u8>` and 8 bytes per
   layer — spritesheet index (`u16 LE`), reserved zero (`u16 LE`),
   `anchor_x` (`s16 LE`), `anchor_y` (`s16 LE`).
4. **Sprite-to-palette section.** `34 36 64 00` + one `u8` per sprite
   giving the palette index that sprite is bound to, padded to
   alignment with `0xFF` bytes.

##### `<name>AssetsIndex.bin`

A fixed 32-byte block: eight big-endian `u32` values pointing at the
section starts inside `<name>Texture.bin`:

| Word | Meaning                                                  |
|------|----------------------------------------------------------|
| 0    | Start of the spritesheet section                         |
| 1    | Start of the palette section                             |
| 2    | Start of the animation section                           |
| 3    | Start of the sprite-to-palette section                   |
| 4    | Start of the assets index itself (= end of the bundle)   |
| 5–7  | Zero padding (reserved)                                  |

The code reads this to jump straight into each section without
scanning; the file is always 32 bytes long.

##### `<name>SpritesheetIndex.bin` (type-1 only)

One BE `u32` per sprite plus a trailing terminator entry, each holding
the offset of that texture's header inside the spritesheet section of
`Texture.bin`. Padded to a 16-byte boundary with zeros. Type-2 sprites
omit this file because the equivalent table is embedded inline at the
start of their spritesheet section.

#### Font bins

A font produces:

- **`fontTexture.bin`** — concatenated packed-CI2 glyph slots, 64
  bytes per glyph, in glyph order. The per-slot packing is described
  in [CI4, CI8, and CI2](#ci4-ci8-and-ci2).
- **`fontPaletteN.bin`** (one per selected palette, with `N = 1, 2,`
  or `3`) — a 4-byte header `34 36 64 10` followed by 16 RGBA5551
  big-endian colors, padded to a 16-byte boundary with zeros.

---

## License

GPL-3.0-or-later. See [`LICENSE`](LICENSE).
