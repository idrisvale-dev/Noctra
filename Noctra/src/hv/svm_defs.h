// language: C++, file: src/hv/svm_defs.h
#pragma once
#include <ntddk.h>

#define SVM_EXIT_CPUID 0x72
#define SVM_EXIT_MSR   0x7C

typedef struct _VMCB_SEGMENT {
    unsigned short Selector;
    unsigned short Attrib;
    unsigned int Limit;
    unsigned __int64 Base;
} VMCB_SEGMENT;

typedef struct _VMCB_STATE_SAVE {
    VMCB_SEGMENT Es;
    VMCB_SEGMENT Cs;
    VMCB_SEGMENT Ss;
    VMCB_SEGMENT Ds;
    VMCB_SEGMENT Fs;
    VMCB_SEGMENT Gs;
    VMCB_SEGMENT Gdtr;
    VMCB_SEGMENT Ldtr;
    VMCB_SEGMENT Idtr;
    VMCB_SEGMENT Tr;
    unsigned char Reserved1[43];
    unsigned char Cpl;
    unsigned char Reserved2[4];
    unsigned __int64 Efer;
    unsigned char Reserved3[104];
    unsigned __int64 Cr4;
    unsigned __int64 Cr3;
    unsigned __int64 Cr0;
    unsigned __int64 Dr7;
    unsigned __int64 Dr6;
    unsigned __int64 Rflags;
    unsigned __int64 Rip;
    unsigned char Reserved4[88];
    unsigned __int64 Rsp;
    unsigned char Reserved5[24];
    unsigned __int64 Rax;
    unsigned __int64 Rcx;
    unsigned __int64 Rdx;
    unsigned __int64 Rbx;
    unsigned __int64 Rbp;
    unsigned __int64 Rsi;
    unsigned __int64 Rdi;
    unsigned __int64 R8;
    unsigned __int64 R9;
    unsigned __int64 R10;
    unsigned __int64 R11;
    unsigned __int64 R12;
    unsigned __int64 R13;
    unsigned __int64 R14;
    unsigned __int64 R15;
} VMCB_STATE_SAVE;

typedef struct _VMCB_CONTROL_AREA {
    unsigned int InterceptCrRead;
    unsigned int InterceptCrWrite;
    unsigned int InterceptDrRead;
    unsigned int InterceptDrWrite;
    unsigned int InterceptException;
    unsigned int InterceptIntr;
    unsigned int InterceptCpuid; // Bit 0 = intercept CPUID
    unsigned int InterceptMsr;   // Bit 0 = intercept RDMSR/WRMSR
    unsigned int InterceptIo;
    unsigned int InterceptHlt;
    unsigned int InterceptShutdown;
    unsigned int InterceptNmi;
    unsigned int InterceptSmi;
    // ... additional fields skipped to reach exit code offsets
    unsigned char Reserved1[40];
    unsigned __int64 ExitCode;
    unsigned __int64 ExitInfo1;
    unsigned __int64 ExitInfo2;
    unsigned __int64 ExitIntInfo;
    unsigned __int64 NestedCtl;
    unsigned char Reserved2[16];
    unsigned __int64 EventInject;
    unsigned __int64 NestedCr3;
    unsigned __int64 LbrVirtualization;
    unsigned __int64 VmcbCleanBits;
    unsigned __int64 NextRip;
    unsigned char Reserved3[8];
    unsigned __int64 NumBytes;
    unsigned char Reserved4[16];
    unsigned __int64 TscOffset;
    unsigned __int64 Asid;
    unsigned __int64 TlbCtl;
    unsigned __int64 VIntrCtl;
    unsigned __int64 VIntrVector;
    unsigned char Reserved5[24];
    unsigned __int64 PauseFilterCount;
    unsigned __int64 PauseFilterThreshold;
    unsigned __int64 IopmBasePa;
    unsigned __int64 MsrpmBasePa;
    unsigned __int64 TscOffsetPauseCtl;
    unsigned __int64 PauseFilterThresh;
    unsigned __int64 InterceptShadow;
    unsigned __int64 Cr3TargetCount;
    unsigned char Reserved6[8];
    unsigned __int64 Cr3TargetList[4];
} VMCB_CONTROL_AREA;

typedef struct _VMCB {
    VMCB_CONTROL_AREA ControlArea;
    VMCB_STATE_SAVE StateSaveArea;
    unsigned char Reserved[1024];
} VMCB, *PVMCB;
