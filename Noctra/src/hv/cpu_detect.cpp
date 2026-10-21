// language: C++, file: src/hv/cpu_detect.cpp
#include "../include/common.h"
#include <intrin.h>

typedef enum _CPU_VENDOR {
    CPU_UNKNOWN = 0,
    CPU_INTEL,
    CPU_AMD
} CPU_VENDOR;

CPU_VENDOR GetCpuVendor() {
    int cpu_info[4];
    __cpuid(cpu_info, 0);
    
    // Check for "GenuineIntel"
    if (cpu_info[1] == 0x756E6547 && cpu_info[3] == 0x49656E69 && cpu_info[2] == 0x6C65746E)
        return CPU_INTEL;
        
    // Check for "AuthenticAMD"
    if (cpu_info[1] == 0x68747541 && cpu_info[3] == 0x69746E65 && cpu_info[2] == 0x444D4163)
        return CPU_AMD;
        
    return CPU_UNKNOWN;
}
