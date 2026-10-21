// Hardware-level PS/2 input injection for AFK
#include <ntddk.h>
#include <intrin.h>

void InjectHardwareKey(unsigned char scan_code, bool key_up) {
    unsigned char data = key_up ? (scan_code | 0x80) : scan_code;
    
    // Wait for buffer clear
    int timeout = 100000;
    while ((__inbyte(0x64) & 2) && --timeout);
    
    // Send key
    __outbyte(0x60, data);
}
