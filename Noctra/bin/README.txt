================================================================================
                              NOCTRA ULTIMATE
                         QUICK START & SETUP GUIDE
================================================================================

[ ! ] IMPORTANT PREREQUISITES (MUST READ BEFORE RUNNING)
--------------------------------------------------------------------------------
Failure to follow these steps will result in a system crash (BSOD).
  1. Disable Windows Defender Real-Time Protection (add this folder to exclusions).
  2. Disable Core Isolation / Memory Integrity in Windows Security.
  3. Disable Hyper-V, Windows Sandbox, and Virtual Machine Platform.
  4. Enable Intel VT-x or AMD-V in your motherboard BIOS/UEFI.

[ > ] HOW TO USE
--------------------------------------------------------------------------------
  1. Right-click "NoctraLoader.exe" and select "Run as Administrator".
  2. Toggle your desired features in the UI (Spoofer, Watchdog, Hypervisor, etc.).
  3. Click the "Launch Roblox" button.
  4. Your browser will open the Roblox homepage. Log in and choose your game.

[ ? ] TROUBLESHOOTING
--------------------------------------------------------------------------------
  * BSOD on Launch: 
    Hyper-V or Core Isolation is still enabled. Turn them off and reboot your PC.
    
  * Driver Failed to Load: 
    Ensure kdmapper.exe and NoctraCore.sys are in the same folder as the loader.
    
  * Antivirus Deletes Files: 
    Your AV is flagging kdmapper.exe. Add the Noctra folder to your AV exclusions.

================================================================================
  Copyright (c) 2026 Noctra Team. Licensed under the MIT License.
================================================================================
