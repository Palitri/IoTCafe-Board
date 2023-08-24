using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OpenIot.BoardSetup.Flashing
{
    internal class ExternalFlashProgrammer : IFlashProgrammer
    {
        protected IFlashLog log;
        protected Process flashingProcess;

        protected string baseDir, exeName, commandLine;

        public event BeganFlashingDelegate BeganFlashing;

        public string FullWorkingDir => Path.IsPathFullyQualified(this.baseDir) ? this.baseDir : Path.GetFullPath(this.baseDir);

        public string FullExeName => Path.Combine(this.FullWorkingDir, this.exeName);

        public ExternalFlashProgrammer(IFlashLog log, string baseDir, string exeName, string commandLine)
        {
            this.log = log;
            this.baseDir = baseDir;
            this.exeName = exeName;
            this.commandLine = commandLine;
        }

        public async Task<bool> FlashBoardAsync()
        {

            using (this.flashingProcess = new Process())
            {
                try
                {
                    string workingDir = Path.IsPathFullyQualified(this.baseDir) ? this.baseDir : Path.GetFullPath(this.baseDir);

                    this.flashingProcess.StartInfo.WorkingDirectory = workingDir;
                    this.flashingProcess.StartInfo.FileName = Path.Combine(workingDir, this.exeName);
                    this.flashingProcess.StartInfo.Arguments = this.commandLine;
                    this.flashingProcess.StartInfo.UseShellExecute = false;
                    this.flashingProcess.StartInfo.RedirectStandardOutput = true;
                    this.flashingProcess.StartInfo.RedirectStandardError = true;
                    this.flashingProcess.StartInfo.CreateNoWindow = true;
                    this.flashingProcess.Start();

                    this.flashingProcess.OutputDataReceived +=FlashingProcess_OutputDataReceived;
                    this.flashingProcess.BeginOutputReadLine();

                    this.flashingProcess.ErrorDataReceived += FlashingProcess_ErrorDataReceived;
                    this.flashingProcess.BeginErrorReadLine();

                    await this.flashingProcess.WaitForExitAsync();

                    return this.flashingProcess.ExitCode == 0;
                }
                catch (Exception ex)
                {
                    this.log.Log(ex.Message, true, FlashMessageType.Error);
                    return false;
                }
            }
        }

        private void FlashingProcess_ErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
                this.MessageReceived(e.Data, FlashMessageType.Error);
        }

        private void FlashingProcess_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
                this.MessageReceived(e.Data, FlashMessageType.Info);
        }

        public virtual void MessageReceived(string message, FlashMessageType messageType)
        {
            this.log.Log(message, true, messageType);
        }

        internal void OnBeganFlashing()
        {
            this.BeganFlashing?.Invoke();
        }
    }
}
