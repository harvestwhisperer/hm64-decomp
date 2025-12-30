#!/usr/bin/env python3

import argparse
import struct
from pathlib import Path

import ipl3checksum


def update_crc(rom_bytes: bytearray, cic: str | None = None):
    """Calculate CRC and update ROM header."""
    # Detect or use specified CIC
    if cic is None:
        cic_kind = ipl3checksum.detectCIC(rom_bytes)
        if cic_kind is None:
            print("Could not detect CIC, defaulting to 6102")
            cic_kind = ipl3checksum.CICKind.CIC_6102_7101
        else:
            print(f"Detected CIC: {cic_kind.name}")
    else:
        cic_map = {
            "6101": ipl3checksum.CICKind.CIC_6101,
            "6102": ipl3checksum.CICKind.CIC_6102_7101,
            "7101": ipl3checksum.CICKind.CIC_6102_7101,
            "6103": ipl3checksum.CICKind.CIC_6103_7103,
            "7103": ipl3checksum.CICKind.CIC_6103_7103,
            "6105": ipl3checksum.CICKind.CIC_6105_7105,
            "7105": ipl3checksum.CICKind.CIC_6105_7105,
            "6106": ipl3checksum.CICKind.CIC_6106_7106,
            "7106": ipl3checksum.CICKind.CIC_6106_7106,
        }
        cic_kind = cic_map.get(cic, ipl3checksum.CICKind.CIC_6102_7101)

    # Calculate checksum
    checksum = ipl3checksum.calculateChecksum(rom_bytes, cic_kind)
    if checksum is None:
        raise RuntimeError("Failed to calculate checksum")

    crc1, crc2 = checksum
    print(f"CRC: 0x{crc1:08X} 0x{crc2:08X}")

    # Write checksum to header
    struct.pack_into(">II", rom_bytes, 0x10, crc1, crc2)


def pad_rom(rom_bytes: bytearray, target_size: int = 0x1000000) -> bytearray:
    """Pad ROM to target size (default 16MB) with 0xFF."""
    size = len(rom_bytes)

    if size < target_size:
        rom_bytes.extend(b'\xFF' * (target_size - size))
        print(f"Padded: {size} -> {target_size} bytes (0x{size:X} -> 0x{target_size:X})")

    return rom_bytes


def main():
    parser = argparse.ArgumentParser(description='N64 ROM CRC and padding tool')
    parser.add_argument('rom', help='ROM file to process')
    parser.add_argument('-o', '--output', help='Output file (default: modify in place)')
    parser.add_argument('--cic', type=str,
                        choices=['6101', '6102', '7101', '6103', '7103',
                                 '6105', '7105', '6106', '7106'],
                        help='CIC type (default: auto-detect)')
    parser.add_argument('--pad', action='store_true',
                        help='Pad ROM after CRC update')

    args = parser.parse_args()
    rom_path = Path(args.rom)
    out_path = Path(args.output) if args.output else rom_path

    # Read ROM
    rom_bytes = bytearray(rom_path.read_bytes())

    # Pad first if requested (CRC is calculated over padded data)
    if args.pad:
        rom_bytes = pad_rom(rom_bytes)

    # Update CRC
    update_crc(rom_bytes, args.cic)

    # Write output
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(rom_bytes)


if __name__ == '__main__':
    main()
