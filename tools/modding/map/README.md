# Blender Tooling

This tool extracts all necessary map data from the rom for use with Blender rndering script.

## Requirements

- Python 3.x
- Dependencies from `tools/requirements.txt`
- `baserom.us.z64` in the project root directory

## Usage

From the `tools/modding/map` directory:

```bash
python prep_blender.py
```

## Output Structure

Assets are extracted to `assets/maps/` in the project root:

```
assets/maps/
├── ranch-spring/
│   ├── ranch-spring.bin                 # Raw binary map data
│   ├── tiles/                  # Tile textures
│   │   ├── tile-0.png
│   │   ├── tile-1.png
│   │   └── ...
│   └── core_map_objects/       # Map object textures
│       ├── map_object-0.png
│       ├── map_object-1.png
│       └── ...
├── ranch-summer/
│   └── ...
└── ...
```

## Integration with Blender

The extracted assets follow the directory structure expected by the Blender map rendering script:

```
{base_path}/assets/maps/{specific_map_directory}/
```

To use with Blender, simply copy the `assets` directory to your desired path and update the `BASE_PATH` variable to be the absolute path of the `assets` directory:

```
# Replace with absolute path to map assets directory
BASE_PATH = rf"..\..\..\assets\maps"
```

On Windows, this will look something like:

```
BASE_PATH = rf"C:\Users\{username}\Desktop\assets\maps" # or whatever your absolute path is
```

## Running in Blender

Inside Blender, select the "Scripting" tab, then select "Text", and paste the contents of the `blender_import.py` script into the text editor. Ensure the `BASE_PATH` variable is updated and then hit the play button to run the script. The model should be rendered to the scene, and you can enable camera view to emulate how orthographic projection used by the game.

To change the map, simply change the `MAP_NAME` string to be any of the maps found in `./tools/map_addresses.csv` (except for the empty maps).

## Blender script options

Currently, the Blender rendering script supports enabling/disabling tile texturing and enabling/disabling "core map objects." 

Enabling/disabling texturing is useful if you want to see and work with only the core mesh geometry.

The "core map objects" are sprites overlayed on top of the main map and can either be billboarded sprites (with a pre-rendered isometric view) that are tied to the orthographic camera view or flat sprites (sprites that appear on walls). These billboarded sprites will only look as expected if camera mode is enabled.

To toggle these rendering options, change these flags:

```
APPLY_TEXTURES = True # or False
RENDER_CORE_MAP_OBJECTS = True # or False
```
