# Noctra Ultimate

**Noctra** is an advanced, hypervisor-based system optimization and anti-cheat evasion tool designed for Roblox. By operating at Ring -1 (Hypervisor) and Ring 0 (Kernel), Noctra provides deep HWID spoofing, memory hiding, multi-instance support, and real-time script protection.

---

## ⚠️ IMPORTANT PREREQUISITES
Before running Noctra, you **MUST** configure your Windows environment, or your system will crash (BSOD) upon launching the hypervisor:
1. **Disable Hyper-V** (Windows Features -> uncheck Hyper-V, Windows Sandbox, Virtual Machine Platform).
2. **Disable Core Isolation / Memory Integrity** (Windows Security -> Device Security).
3. **Enable Intel VT-x or AMD-V** in your BIOS/UEFI settings.
4. **Add to Windows Defender Exclusions** (Noctra uses `kdmapper.exe` to load the unsigned driver, which Defender flags as a HackTool).
5. Disable **Tamper Protection** in Windows Security temporarily.

---

## ✨ Features
- **Hypervisor Stealth (Ring -1):** Uses EPT (Extended Page Tables) Shadow Pages to hide cheat memory from Byfron/Hyperion memory scans.
- **HWID Spoofer:** Modifies SMBIOS, Disk Serials, and MAC addresses to bypass hardware bans.
- **Script Watchdog:** Intercepts and blocks malicious Lua scripts (IP loggers, Discord webhooks) inside executors like Delta or Wave.
- **Multi-Instance Unlocker:** Bypasses `ROBLOX_singletonMutex` to allow 5-6 simultaneous Roblox instances.
- **Hardware Input Injection:** Emulates physical PS/2 keyboard interrupts for AFK farming, making it undetectable as a bot.
- **System Booster:** Clears the Windows standby memory list and purges cached RAM for smoother gameplay.

---

## 🚀 How to Use (For Users)
1. Go to the **[Releases]** tab on the right side of this repository.
2. Download the latest `Noctra.zip` file.
3. Extract the ZIP file to a folder.
4. Right-click `NoctraLoader.exe` and select **Run as Administrator**.
5. Toggle your desired features in the dark-mode UI.
6. Click **Launch Roblox**. The loader will silently map the driver, open the Roblox homepage, and protect your system in the background.

---

## 🛠️ How to Build (For Developers)
If you want to compile Noctra from source, you will need:
- **Visual Studio 2022** (with Desktop Development with C++ and .NET Desktop Development workloads).
- **Windows Driver Kit (WDK)** for Visual Studio.
- **NASM / MASM** (Macro Assembler) for `.asm` files.

### Build Steps:
1. Clone the repository: `git clone https://github.com/yourusername/Noctra.git`
2. Open `Noctra.sln` in Visual Studio.
3. Build the C++ Driver (`NoctraCore.sys`) and C# Loader (`NoctraLoader.exe`).
4. The Post-Build events will automatically copy the compiled `.sys` and `.exe` into the `/bin` folder.
5. Manually download `kdmapper.exe` and place it in the `/bin` folder.

---

## ⚖️ Disclaimer & License
This project is licensed under the **MIT License**. 
This tool is for educational and research purposes only. The developers are not responsible for any account bans, HWID bans, or system instability caused by improper use. Use at your own risk.
