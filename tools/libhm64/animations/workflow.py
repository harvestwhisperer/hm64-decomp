"""
Animation extraction workflow for animation_viewer and other tools.

Provides high-level functions for extracting animation assets including:
- GIF generation from extracted sprites

NOTE: Sprites and animations are extracted together via the sprite extractor:
    python -m libhm64.sprites.extractor extract_all

This extracts to assets/sprites/{subdir}/{asset}/:
    textures/{index:03d}.png  - Sprite images
    animations/{index}.json   - Animation metadata
    manifest.json             - Asset manifest

GIFs can then be generated:
    python -m libhm64.animations.gif_builder
"""

import json
from pathlib import Path
from typing import Dict, Any, Optional

from .gif_builder import build_gifs_for_asset, build_all_gifs

# Default paths
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
SPRITES_DIR = _REPO_DIR / "assets" / "sprites"


def find_asset_dir(asset_name: str) -> Optional[Path]:
    """
    Find the asset directory in assets/sprites/.

    Args:
        asset_name: Name of the asset (e.g., "player")

    Returns:
        Path to asset directory, or None if not found
    """
    if not SPRITES_DIR.exists():
        return None

    for subdir in SPRITES_DIR.iterdir():
        if not subdir.is_dir():
            continue
        asset_dir = subdir / asset_name
        if asset_dir.is_dir():
            return asset_dir

    return None


def build_gifs_for_single_asset(
    asset_name: str,
    force: bool = False,
    verbose: bool = False
) -> Dict[str, Any]:
    """
    Build GIFs for a single asset from extracted sprites.

    This is the main workflow function used by animation_viewer.

    Prerequisites:
        Run 'python -m libhm64.sprites.extractor extract_all' first to extract
        sprites and animation metadata.

    Args:
        asset_name: Name of the asset (e.g., "player")
        force: If True, rebuild GIFs even if they exist
        verbose: Print progress information

    Returns:
        Dict with statistics:
        {
            "status": "success" | "error",
            "asset": asset_name,
            "gifs_built": int,
            "message": str  # If error
        }
    """
    asset_dir = find_asset_dir(asset_name)

    if asset_dir is None:
        return {
            "status": "error",
            "message": f"Asset '{asset_name}' not found in {SPRITES_DIR}. "
                       "Run 'python -m libhm64.sprites.extractor extract_all' first."
        }

    animations_dir = asset_dir / "animations"
    textures_dir = asset_dir / "textures"

    if not animations_dir.exists():
        return {
            "status": "error",
            "message": f"No animations directory found for '{asset_name}'."
        }

    if not textures_dir.exists():
        return {
            "status": "error",
            "message": f"No textures directory found for '{asset_name}'."
        }

    # Check if already built (unless force=True)
    if not force:
        existing_gifs = list(animations_dir.glob("*.gif"))
        if existing_gifs:
            return {
                "status": "cached",
                "asset": asset_name,
                "gifs_built": len(existing_gifs)
            }

    # Build GIFs
    if verbose:
        print(f"Building GIFs for {asset_name}...")

    gifs_built = build_gifs_for_asset(asset_dir)

    if verbose:
        print(f"Built {gifs_built} GIFs for {asset_name}")

    return {
        "status": "success",
        "asset": asset_name,
        "gifs_built": gifs_built
    }


# Backwards compatibility alias
def extract_single_asset(
    asset_name: str,
    force: bool = False,
    verbose: bool = False
) -> Dict[str, Any]:
    """
    DEPRECATED: Use build_gifs_for_single_asset instead.

    Sprites and animations are now extracted together via the sprite extractor.
    This function only builds GIFs from already-extracted data.
    """
    return build_gifs_for_single_asset(asset_name, force, verbose)


def fetch_sprites_for_animations(
    asset_name: Optional[str] = None,
    verbose: bool = False
) -> None:
    """
    DEPRECATED: Sprite textures are now extracted via the sprite extractor.

    Run `python -m libhm64.sprites.extractor extract_all` to extract sprites,
    then `python -m libhm64.animations.gif_builder` to build GIFs.

    This function is kept for backwards compatibility but does nothing.
    """
    if verbose:
        print("Note: Sprites are now extracted via the sprite extractor. "
              "Run 'python -m libhm64.sprites.extractor extract_all' first.")


def make_gifs_from_animations(asset_name: Optional[str] = None) -> None:
    """
    Create GIF animations from extracted sprite/animation data.

    Args:
        asset_name: If provided, only process this asset. Otherwise process all.
    """
    if asset_name:
        result = build_gifs_for_single_asset(asset_name, force=True, verbose=True)
        if result.get("status") == "error":
            print(f"Error: {result.get('message')}")
    else:
        build_all_gifs(SPRITES_DIR)
