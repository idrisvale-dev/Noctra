; VMX Entry and Exit Handler
section .text

global AsmVmxLaunch
AsmVmxLaunch:
    mov rax, rcx
    push rax
    vmlaunch
    pop rax
    ret

global AsmVmExitHandler
extern VmExitHandler_C
AsmVmExitHandler:
    ; Save guest registers
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    
    ; Call C handler
    mov rcx, rsp 
    sub rsp, 32
    call VmExitHandler_C
    add rsp, 32
    
    ; Restore guest registers
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ; Resume guest execution
    vmresume
    jmp AsmVmExitHandler
