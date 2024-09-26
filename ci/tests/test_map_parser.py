import os
import sys
import unittest
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).resolve().parent.absolute()
UNO = HERE / "uno"
OUTPUT = HERE / "output"
ELF_FILE = UNO / "firmware.elf"


@dataclass
class Tools:
    as_path: Path
    ld_path: Path
    objcopy_path: Path
    objdump_path: Path


def load_tools() -> Tools:
    as_path = UNO / "avr-as"
    ld_path = UNO / "avr-ld"
    objcopy_path = UNO / "avr-objcopy"
    objdump_path = UNO / "avr-objdump"
    if sys.platform == "win32":
        as_path = as_path.with_suffix(".exe")
        ld_path = ld_path.with_suffix(".exe")
        objcopy_path = objcopy_path.with_suffix(".exe")
        objdump_path = objdump_path.with_suffix(".exe")
    out = Tools(as_path, ld_path, objcopy_path, objdump_path)
    tools = [as_path, ld_path, objcopy_path, objdump_path]
    for tool in tools:
        if not tool.exists():
            raise FileNotFoundError(f"Tool not found: {tool}")
    return out


TOOLS = load_tools()


class TestMapParser(unittest.TestCase):
    def test_bin_to_elf_conversion(self):
        # os.system("uv run fpvgcc ci/tests/uno/firmware.map --lmap root")

        cmds = [
            f"uv run fpvgcc {UNO}/firmware.map --sar",
            f"uv run fpvgcc {UNO}/firmware.map --lmap root",
            f"uv run fpvgcc {UNO}/firmware.map --uf",
            f"uv run fpvgcc {UNO}/firmware.map --uregions",
            # --usections
            f"uv run fpvgcc {UNO}/firmware.map --usections",
            f"uv run fpvgcc {UNO}/firmware.map --la",
        ]
        for cmd in cmds:
            print("\nRunning command: ", cmd)
            os.system(cmd)


if __name__ == "__main__":
    unittest.main()
