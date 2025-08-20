using System.Diagnostics;

namespace Palitri.OpenIoT.Setup.Shared.Flashing
{
    public class ExternalFlashProgrammer : IFlashProgrammer
    {
        protected IFlashLog log;
        protected Process flashingProcess;

        protected string baseDir, exeName, commandLine;

        public event BeganFlashingDelegate BeganFlashing;
        public event MessageReceivedDelegate MessageReceived;


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

                    this.flashingProcess.OutputDataReceived += FlashingProcess_OutputDataReceived;
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
                this.OnMessageReceived(e.Data, FlashMessageType.Error);
        }

        private void FlashingProcess_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
                this.OnMessageReceived(e.Data, FlashMessageType.Info);
        }

        public virtual void OnMessageReceived(string message, FlashMessageType messageType)
        {
            this.log.Log(message, true, messageType);

            this.MessageReceived?.Invoke(message, messageType);
        }

        protected void OnBeganFlashing()
        {
            this.BeganFlashing?.Invoke();
        }
    }
}
