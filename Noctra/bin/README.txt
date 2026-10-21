==================================================
NOCTRA ULTIMATE - QUICK START GUIDE
==================================================

REQUIRED PREREQUISITES (READ BEFORE RUNNING):
1. Disable Windows Defender Real-Time Protection. 
   (Defender flags kdmapper.exe as a HackTool. Add the folder to Exclusions).
2. Disable Core Isolation / Memory Integrity in Windows Security.
3. Disable Hyper-V, Windows Sandbox, and Virtual Machine Platform in Windows Features.
4. Enable Intel VT-x or AMD-V in your BIOS/UEFI settings.

HOW TO USE:
1. Right-click "NoctraLoader.exe" and select "Run as Administrator".
2. Toggle your desired features in the UI (HWID Spoofer, Watchdog, Hypervisor, etc.).
3. Click the "Launch Roblox" button.
4. Your default browser will open the Roblox homepage. Log in and choose your game.

TROUBLESHOOTING:
- BSOD (Blue Screen of Death) on Launch: 
  This means Hyper-V or Core Isolation is still enabled. Turn them off, reboot your PC, and try again.
- Driver failed to load: 
  Make sure kdmapper.exe and NoctraCore.sys are in the exact same folder as NoctraLoader.exe.
  
==================================================
