"""
Character avatar data subsegment (included by the entityAnimationScripts parent).

Generates src/data/animation/characterAvatars/characterAvatars.inc.c, the
`characterAvatarCodesToAnimations` table mapping avatar codes to animation
indices. Unlike animationScriptData (packed u16 animation scripts), this is a
flat u8 table, so it gets its own segment type. The parent animationScripts
group emits an `#include` for it so the whole region links as one object.
"""

from pathlib import PurePosixPath
from typing import Optional

from splat.util import options
from splat.segtypes.common.codesubsegment import CommonSegCodeSubsegment


class N64SegCharacterAvatarData(CommonSegCodeSubsegment):
    """Flat u8 character-avatar table, written as a .inc.c."""

    def __init__(self, rom_start, rom_end, type, name, vram_start, args, yaml):
        super().__init__(rom_start, rom_end, type, name, vram_start, args=args, yaml=yaml)

        # Included by the parent, not compiled/linked on its own.
        self.has_linker_entry = False
        self.file_text: Optional[str] = None

    def out_path(self):
        return options.opts.src_path / self.dir / f"{self.name}.inc.c"

    def scan(self, rom_bytes: bytes):
        if self.rom_start is None or self.rom_end is None:
            return

        data = rom_bytes[self.rom_start:self.rom_end]

        lines = [
            '#include "common.h"',
            "",
            '#include "characterAvatars.h"',
            "",
            "u8 characterAvatarCodesToAnimations[] = {",
        ]

        # 16 bytes per row, tab-indented, trailing comma (matches the rest of
        # the generated animation data styling).
        for i in range(0, len(data), 16):
            row = ", ".join(str(b) for b in data[i:i + 16])
            lines.append(f"\t{row},")

        lines.append("};")
        lines.append("")

        self.file_text = "\n".join(lines)

    def split(self, rom_bytes: bytes):
        if not self.file_text or not self.out_path():
            return

        self.out_path().parent.mkdir(parents=True, exist_ok=True)
        with open(self.out_path(), "w", newline="\n") as f:
            f.write(self.file_text)

        self.log(f"Generated character avatar data: {self.out_path()}")

    def should_scan(self) -> bool:
        return True

    def should_split(self) -> bool:
        return self.extract

    @staticmethod
    def is_data() -> bool:
        return True
