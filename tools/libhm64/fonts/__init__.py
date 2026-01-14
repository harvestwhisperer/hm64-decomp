"""
Font asset handling for HM64.

The game uses CI2 format fonts (2-bit color indexed, 4 colors per palette).
Each character is 14x14 pixels stored as 64 bytes of packed data.

Modules:
    extractor - Extract font assets from ROM
    assembler - Assemble font assets back to bin files
"""
