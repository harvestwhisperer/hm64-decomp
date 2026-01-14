"""
HM64 Cutscenes package.

Provides tools for extracting and transpiling cutscene bytecode.

Modules:
    opcodes: Cutscene opcode definitions and constants
    extractor: Extract ROM bytecode to assembly
    transpiler: Transpile DSL to assembly
    asm_to_dsl: Convert assembly back to DSL
"""

from .opcodes import CutsceneOpcode, COMMAND_SPECS, COMMAND_SPECS_BY_NAME, ASM_COMMAND_SPECS

__all__ = [
    'CutsceneOpcode',
    'COMMAND_SPECS',
    'COMMAND_SPECS_BY_NAME',
    'ASM_COMMAND_SPECS',
]
