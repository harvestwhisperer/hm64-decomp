"""
HM64 Dialogues package.

Provides tools for transpiling dialogue bytecode.

Modules:
    opcodes: Dialogue opcode definitions and constants
    transpiler: Transpile DSL to assembly
"""

from .opcodes import DialogueOpcode, COMMAND_SPECS, BUILTIN_CONSTANTS

__all__ = [
    'DialogueOpcode',
    'COMMAND_SPECS',
    'BUILTIN_CONSTANTS',
]
