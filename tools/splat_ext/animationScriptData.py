"""
Animation script data subsegment (included by animationScripts parent).
"""

from pathlib import Path, PurePosixPath
from typing import Optional, List
import struct

from splat.util import options, log
from splat.segtypes.common.codesubsegment import CommonSegCodeSubsegment


class N64SegAnimationScriptData(CommonSegCodeSubsegment):
    """
    Individual animation script data segment.
    Generates .inc.c files that are included by the parent segment.
    """

    def __init__(self, rom_start, rom_end, type, name, vram_start, args, yaml):
        
        super().__init__(rom_start, rom_end, type, name, vram_start, args=args, yaml=yaml)

        # Force no linker entry - these files are included, not compiled separately
        self.has_linker_entry = False
        self.file_text: Optional[str] = None

    def out_path(self) -> Optional[Path]:
        """
        Returns path to the .inc.c file for this animation script.
        Path is relative to parent's directory.
        """
        return options.opts.src_path / self.dir / f"{self.name}.inc.c"

    def parse_animation_data(self, data: bytes) -> List[dict]:
        """
        Parse animation script data from raw bytes.

        Bit layout:
        - metadata_offset: bits 0-12 (0x1FFF)
        - anim_type: bits 13-14 (0x6000)
        - horizontal_flip: bit 15 (0x8000)
        """

        MASK_METADATA_OFFSET = 0x1FFF
        MASK_ANIMATION_TYPE = 0x6000
        MASK_HORIZONTAL_FLIP = 0x8000

        anim_type_map = {
            0x0000: "ANIM_TYPE_ONE_SHOT",
            0x2000: "ANIM_TYPE_LOOP",
            0x4000: "ANIM_TYPE_DESTROY_ON_END",
        }

        entries = []

        for i in range(0, len(data), 2):

            if i + 2 > len(data):
                break

            # Read as big-endian u16
            packed_value = struct.unpack(">H", data[i:i+2])[0]

            metadata_offset = packed_value & MASK_METADATA_OFFSET
            anim_type_bits = packed_value & MASK_ANIMATION_TYPE
            horizontal_flip = (packed_value & MASK_HORIZONTAL_FLIP) != 0

            # Map animation type bits to constant name
            anim_type = anim_type_map.get(anim_type_bits, f"(/*UNKNOWN_TYPE*/0x{anim_type_bits:04X})")

            entries.append({
                'raw_value': packed_value,
                'metadata_offset': metadata_offset,
                'anim_type': anim_type,
                'horizontal_flip': horizontal_flip,
                'is_zero': packed_value == 0,
                'index': i // 2,  # Entry index (for grouping)
            })

        return entries

    def scan(self, rom_bytes: bytes):
        """
        Parse the animation data during the scan phase and generate C files with the data
        """

        if self.rom_start is None or self.rom_end is None:
            return

        data = rom_bytes[self.rom_start:self.rom_end]
        entries = self.parse_animation_data(data)

        if not entries:
            self.warn(f"No animation data found for {self.name}")
            return

        basename = PurePosixPath(self.name).name

        lines = [
            '#include "common.h"',
            "",
            '#include "animationScripts.h"',
            "",
            f"u16 {basename}AnimationScripts[] = {{",
            "        ",
        ]

        # Generate array entries with grouping by 8
        total = len(entries)

        for i, entry in enumerate(entries):

            # Add tab/newline grouping every 8 entries (per 8 sprite directions)
            if i > 0 and i % 8 == 0:
                lines.append("\t")

            is_last = (i == total - 1)
            comma = "" if is_last else ","

            if entry['is_zero']:
                line = f"0{comma}"
            else:
                flip_str = "FLIP_HORIZONTAL" if entry['horizontal_flip'] else "0"
                line = f"PACK_ANIM_DATA({entry['metadata_offset']}, {entry['anim_type']}, {flip_str}){comma}"

            lines.append("\t" + line)

        lines.append("")
        lines.append("};")
        lines.append("")

        self.file_text = "\n".join(lines)

    def split(self, rom_bytes: bytes):
        """
        Write the generated .inc.c file.
        """

        if not self.file_text or not self.out_path():
            return

        self.out_path().parent.mkdir(parents=True, exist_ok=True)

        with open(self.out_path(), "w", newline="\n") as f:
            f.write(self.file_text)

        self.log(f"Generated animation script: {self.out_path()}")

    def should_scan(self) -> bool:
        return True

    def should_split(self) -> bool:
        return self.extract

    @staticmethod
    def is_data() -> bool:
        return True
