"""
Custom Splat extension for animation script data extraction.

Generates:
    - src/data/animationScripts.c (base file containing includes)
    - src/data/animationScripts/player.inc.c
    - src/data/animationScripts/maria.inc.c
    - etc.
"""

from pathlib import Path, PurePosixPath
from typing import Optional, List
import struct

from splat.util import options, log
from splat.segtypes.common.group import CommonSegGroup
from splat.segtypes.common.codesubsegment import CommonSegCodeSubsegment


class N64SegAnimationScripts(CommonSegGroup):
    """
    Parent segment that manages animation script subsegments.
    Generates a base C file that includes all subsegments.
    This segment appears in the linker script.
    """

    def __init__(self, rom_start, rom_end, type, name, vram_start, args, yaml):
        super().__init__(rom_start, rom_end, type, name, vram_start, args=args, yaml=yaml)

    def out_path(self) -> Optional[Path]:
        """
        Returns path to the base C file that will be compiled and linked.
        This file includes all the .inc.c subsegments.
        """
        return options.opts.src_path / self.dir / f"{self.name}.c"

    def split(self, rom_bytes: bytes):
        """
        Split all subsegments first, then generate the base include file.
        """

        # Let subsegments generate their .inc.c files
        super().split(rom_bytes)

        # Generate base C file that includes all subsegments
        if not self.out_path():
            return

        self.out_path().parent.mkdir(parents=True, exist_ok=True)

        lines = [
            '#include "common.h"',
            "",
            '#include "animationScripts.h"',
            "",
        ]

        # Add includes for each subsegment
        for sub in self.subsegments:
            if hasattr(sub, 'type') and sub.type == "animationScriptData":
                basename = PurePosixPath(sub.name).name
                lines.append(f'#include "{basename}.inc.c"')

        lines.append("")

        with open(self.out_path(), "w", newline="\n") as f:
            f.write("\n".join(lines))

        self.log(f"Generated base animation scripts file: {self.out_path()}")

    def get_linker_section(self) -> str:
        return ".data"

    def get_linker_entries(self):
        """
        Override to include the parent's base C file in the linker script.
        """

        from splat.segtypes.linker_entry import LinkerEntry

        if not self.has_linker_entry:
            return []

        path = self.out_path()
        if path:
            return [
                LinkerEntry(
                    self,
                    [path],
                    path,
                    self.get_linker_section_order(),
                    self.get_linker_section_linksection(),
                    self.is_noload(),
                )
            ]
        return []

    @staticmethod
    def is_data() -> bool:
        return True