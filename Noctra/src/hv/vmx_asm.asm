; language: x86-64 MASM, file: src/hv/svm_asm.asm
section .text

global AsmSvmLaunch
extern SvmExitHandler_C

; RCX = Physical Address of VMCB
AsmSvmLaunch:
    ; Save host state
    push rbx
    push rbp
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15

    ; Store host RSP in RBP
    mov rbp, rsp
    
    ; Load VMCB
    mov rax, rcx
    vmload rax

    ; Guest state is now loaded. Run guest.
    vmrun rax

AsmSvmExitHandler:
    ; On VMEXIT, RAX contains the VMCB physical address
    mov r15, rax

    ; Save guest state from VMCB (optional, but needed for C++ handler)
    ; Switch back to host stack
    mov rsp, rbp

    ; Call C++ handler with VMCB pointer in RCX
    mov rcx, r15
    sub rsp, 32
    call SvmExitHandler_C
    add rsp, 32

    ; Resume guest
    mov rax, r15
    vmload rax
    vmrun rax
    jmp AsmSvmExitHandler
