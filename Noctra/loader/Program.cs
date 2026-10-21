// language: C#, file: Program.cs
using System;
using System.Windows.Forms;

namespace NoctraLoader
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
