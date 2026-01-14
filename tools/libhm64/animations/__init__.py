"""
Animation asset handling for HM64.

Animations consist of:
- Metadata: Frame counts, durations, sprite references, anchor positions
- Scripts: Animation type flags and metadata offsets for sprite directions
- Frame textures: Individual sprite images composited into animation frames

Modules:
    scripts      - Animation script extraction to C source files
    metadata     - Animation metadata extraction from ROM
    gif_builder  - GIF composition from animation frames
"""
