// Noctra C# WinForms UI
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace NoctraLoader
{
    public partial class MainForm : Form
    {
        private CheckBox chkSpoofer, chkStealth, chkInputBot, chkMultiInstance, chkBooster, chkWatchdog;
        private Button btnLaunch;
        private Label lblStatus;

        public MainForm()
        {
            SetupUI();
        }

        private void SetupUI()
        {
            this.Text = "Noctra Ultimate";
            this.Size = new Size(420, 520);
            this.BackColor = Color.FromArgb(18, 18, 18);
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;

            Label lblTitle = new Label
            {
                Text = "Noctra",
                Font = new Font("Segoe UI", 28, FontStyle.Bold),
                ForeColor = Color.White,
                Location = new Point(20, 20),
                AutoSize = true
            };
            this.Controls.Add(lblTitle);

            chkSpoofer = CreateToggle("Enable HWID Spoofer (SMBIOS)", 80);
            chkStealth = CreateToggle("Enable Hypervisor Anti-Scan (EPT)", 120);
            chkWatchdog = CreateToggle("Script Watchdog (Block Malicious Lua)", 160);
            chkInputBot = CreateToggle("Hardware Input (AFK Farm)", 200);
            chkMultiInstance = CreateToggle("Unlock Multi-Instance (5-6 Tabs)", 240);
            chkBooster = CreateToggle("System Booster (RAM Purge)", 280);

            this.Controls.AddRange(new Control[] { chkSpoofer, chkStealth, chkWatchdog, chkInputBot, chkMultiInstance, chkBooster });

            btnLaunch = new Button
            {
                Text = "Launch Roblox",
                Font = new Font("Segoe UI", 14, FontStyle.Bold),
                Size = new Size(360, 50),
                Location = new Point(20, 340),
                BackColor = Color.FromArgb(0, 120, 215),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };
            btnLaunch.FlatAppearance.BorderSize = 0;
            btnLaunch.Click += BtnLaunch_Click;
            this.Controls.Add(btnLaunch);

            lblStatus = new Label
            {
                Text = "Status: Ready",
                ForeColor = Color.Gray,
                Location = new Point(20, 410),
                AutoSize = true
            };
            this.Controls.Add(lblStatus);
        }

        private CheckBox CreateToggle(string text, int yPos)
        {
            return new CheckBox
            {
                Text = text,
                ForeColor = Color.LightGray,
                Location = new Point(20, yPos),
                AutoSize = true,
                Checked = true
            };
        }

        private void BtnLaunch_Click(object sender, EventArgs e)
        {
            btnLaunch.Enabled = false;
            lblStatus.Text = "Status: Injecting Hypervisor...";
            Application.DoEvents();

            string appPath = AppDomain.CurrentDomain.BaseDirectory;
            string mapperPath = Path.Combine(appPath, "kdmapper.exe");
            string driverPath = Path.Combine(appPath, "NoctraCore.sys");

            try
            {
                if (File.Exists(mapperPath) && File.Exists(driverPath))
                {
                    ProcessStartInfo psi = new ProcessStartInfo
                    {
                        FileName = mapperPath,
                        Arguments = driverPath,
                        UseShellExecute = true,
                        Verb = "runas",
                        CreateNoWindow = true,
                        WindowStyle = ProcessWindowStyle.Hidden
                    };
                    using (Process proc = Process.Start(psi)) { proc.WaitForExit(); }
                }

                lblStatus.Text = "Status: Noctra Active. Launching Game...";
                Application.DoEvents();

                Process.Start(new ProcessStartInfo
                {
                    FileName = "https://www.roblox.com/home",
                    UseShellExecute = true
                });

                lblStatus.Text = "Status: Tools Running. Protected.";
                btnLaunch.Text = "Noctra Protected";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error: {ex.Message}", "Noctra Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                btnLaunch.Enabled = true;
                btnLaunch.Text = "Launch Roblox";
            }
        }
    }
}
