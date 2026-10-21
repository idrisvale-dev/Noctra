# Noctra Ultimate

Noctra is an advanced, hypervisor-based system optimization and anti-cheat evasion tool designed for Roblox. By operating at Ring -1 (Hypervisor) and Ring 0 (Kernel), Noctra provides deep HWID spoofing, memory hiding, multi-instance support, and real-time script protection.

## Requirements
Before running Noctra, you MUST configure your Windows environment, or your system will crash (BSOD) upon launching the hypervisor:
1. Disable Hyper-V (Windows Features -> uncheck Hyper-V, Windows Sandbox, Virtual Machine Platform).
2. Disable Core Isolation / Memory Integrity (Windows Security -> Device Security).
3. Enable Intel VT-x or AMD-V in your BIOS/UEFI settings.
4. Add to Windows Defender Exclusions (Noctra uses kdmapper.exe to load the unsigned driver).
5. Disable Tamper Protection in Windows Security temporarily.

## Features
- **Hypervisor Stealth (Ring -1):** Uses EPT Shadow Pages to hide cheat memory from Byfron.
- **HWID Spoofer:** Modifies SMBIOS, Disk Serials, and MachineGuid to bypass hardware bans.
- **Script Watchdog:** Intercepts and blocks malicious Lua scripts inside executors.
- **Unlimited Multi-Instance:** Bypasses `ROBLOX_singletonMutex` to allow opening unlimited Roblox tabs. You can open as many accounts as your PC hardware can handle.
- **Hardware Input Injection:** Emulates physical PS/2 keyboard interrupts for AFK farming.
- **System Booster:** Clears the Windows standby memory list to free up RAM and maximize the number of tabs you can open.

## How to Use (For Users)
1. Go to the [Releases] tab and download the latest Noctra.zip file.
2. Extract the ZIP file to a folder.
3. Right-click NoctraLoader.exe and select Run as Administrator.
4. Toggle your desired features in the UI.
5. Click Launch Roblox.

## How to Build (For Developers)
1. Clone the repository.
2. Open Noctra.sln in Visual Studio 2022.
3. Ensure the `.asm` files are set to Microsoft Macro Assembler in their properties.
4. Build the solution in x64 Release mode.
5. The Post-Build events will output NoctraCore.sys and NoctraLoader.exe to the /bin folder.
6. Manually download kdmapper.exe and place it in the /bin folder.

## License
This project is licensed under the MIT License. This tool is for educational and research purposes only.
