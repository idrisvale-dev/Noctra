; VMFUNC for EPTP Switching
section .text

global AsmVmFunc
AsmVmFunc:
    ; RCX contains the EPTP index (0 for clean, 1 for cheat)
    mov rax, 0
    vmfunc
    ret
