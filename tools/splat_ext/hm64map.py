from pathlib import Path
from typing import Optional

from splat.segtypes.common.bin import CommonSegBin


class N64SegHm64map(CommonSegBin):
    def out_path(self) -> Optional[Path]:
        return Path("assets") / self.dir / f"{self.name}.hm64map"

    def split(self, rom_bytes: bytes):
        if self.rom_start is None or self.rom_end is None:
            return

        path = self.out_path()
        if path:
            path.parent.mkdir(parents=True, exist_ok=True)

        super().split(rom_bytes)
