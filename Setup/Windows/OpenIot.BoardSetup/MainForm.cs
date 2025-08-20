using OpenIot.BoardSetup.Oui.Elements;
using OpenIot.BoardSetup.UI;
using System;
using System.Drawing;
using System.Windows.Forms;
using System.Linq;
using System.Threading;
using System.Collections.Generic;
using System.Threading.Tasks;
using Palitri.OpenIoT.Board.Scanner;
using Palitri.OpenIoT.Web.Api;
using Palitri.OpenIoT.Board.Api;
using Palitri.OpenIoT.Board.Protocol.Events;
using Palitri.OpenIoT.Board.Protocol;
using Palitri.OpenIoT.Board.Transmission.Com;
using OpenIot.BoardSetup.API;
using OpenIot.BoardSetup.API.Models;
using OpenIot.BoardSetup.SetupConfigModels;
using OpenIot.BoardSetup.ResourceDownload;
using System.IO;
using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace OpenIot.BoardSetup
{
    public partial class MainForm : Form
    {
        private const string OS = "Windows";
        private const string FirmwaresDownloadDirectory = "Firmwares";

        internal SetupUI ui;

        private OpenIoTBoardScanner boardScanner;

        private string userToken = String.Empty;

        private FirmwareSetupInfo setupInfo;
        private FirmwareSetupConfig setupConfig;
        private IFlashProgrammer programmer;

        class OuiTextLog : IFlashLog
        {
            private OuiText text;
            private PictureBox pictureBox;

            public OuiTextLog(OuiText text, PictureBox pictureBox)
            {
                this.text = text;
                this.pictureBox = pictureBox;
            }

            public void Log(string message, bool newLine = true, FlashMessageType messageType = FlashMessageType.Info, int messageId = 0)
            {
                if (newLine && !String.IsNullOrWhiteSpace(this.text.Text))
                    this.text.AppendText(Environment.NewLine);

                if (message != null)
                    this.text.AppendText(message);

                this.pictureBox.Invalidate();
            }
        }

        public MainForm()
        {
            InitializeComponent();

            Task.Run(async () =>
            {
                VersionInfoResponse response = await VersionsAPI.ReadCurrentVersionAsync();

                this.setupInfo = FirmwareSetupInfo.FromJson(response.Version.Notes);
            }).Wait();

            this.Width = Screen.PrimaryScreen.WorkingArea.Width / 4;
            this.Height = this.Width * 9 / 8;
            this.Left = (Screen.PrimaryScreen.WorkingArea.Width - this.Width) / 2;
            this.Top = (Screen.PrimaryScreen.WorkingArea.Height - this.Height) / 2;

            this.ui = new SetupUI(this.pbBackground, new PointF(960.0f, 1080.0f), setupInfo.Configs.Select(c => c.Board));
            this.ui.OnFlashInitiated = Ui_OnFlashInitiated;
            this.ui.OnMinimized = Ui_OnMinimized;
            this.ui.OnClosed = Ui_OnClosed;
            this.ui.OnStartDeviceSearch = Ui_OnStartDeviceSearch;
            this.ui.OnEndDeviceSearch = Ui_OnEndDeviceSearch;
            this.ui.OnStartFirmwareDownload = Ui_OnStartFirmwareDownload;
            this.ui.OnBoardTypeSelected = Ui_OnBoardTypeSelected;
            this.ui.OnAccountCredentialsEntered = Ui_OnAccountCredentialsEntered;
            this.ui.SetSize(this.Width, this.Height);
            this.ui.Log = new OuiTextLog(this.ui.logText, this.pbBackground);


            this.boardScanner = new OpenIoTBoardScanner();
            this.boardScanner.ScanInterval = TimeSpan.FromSeconds(0.5);
            this.boardScanner.ScanPorts();
            //this.boardScanner.ScanOnce();


        }

        private void OnPortAvailable(object sender, BoardPortEventArgs args)
        {
            this.ui.SetComPort(args.Port);
        }

        private async void Ui_OnFlashInitiated(object sender, string comPort, string boardType)
        {
            try
            {
                this.ui.logText.Text = String.Empty;

                if (this.programmer != null)
                {
                    this.Programmer_BeganFlashing();

                    this.programmer.BeganFlashing += Programmer_BeganFlashing;

                    bool flashSuccessfull = await this.programmer.FlashBoardAsync();

                    this.programmer.BeganFlashing -= Programmer_BeganFlashing;

                    if (flashSuccessfull)
                    {
                        this.ui.Log.Log("Resetting device");

                        this.ui.Log.Log(await this.ResetDevice() ? "Success" : "Failed");
                    }

                    this.ui.FinishFlashing(flashSuccessfull);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "OpenIoT setup", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Programmer_BeganFlashing()
        {
            this.ui.BeganFlashing();
        }

        private void Ui_OnMinimized(object sender)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void Ui_OnClosed(object sender)
        {
            this.Close();
        }

        private void Ui_OnStartDeviceSearch(object sender)
        {
            this.boardScanner.OnPortAvailable = OnPortAvailable;
            this.boardScanner.StartScanning();
        }

        private void Ui_OnEndDeviceSearch(object sender)
        {
            this.BeginInvoke(delegate
            {
                this.boardScanner.OnPortAvailable = null;
                this.boardScanner.StopScanning(false);
            });
        }

        private void Ui_OnStartFirmwareDownload(object sender)
        {
            Task.Run(async () =>
            {
                this.ui.textUpdateFirmwareStatus.Text = "Downloading firmware";
                this.pbBackground.Invalidate();

                this.setupConfig = setupInfo.GetSetupConfigForBoardAndOs(this.ui.SelectedBoardType, OS);

                Dictionary<string, string> downloadedFiles = await ResourceDownloader.DownloadFilesAsync(setupConfig.Files.Values, Path.Combine(AppDomain.CurrentDomain.BaseDirectory, FirmwaresDownloadDirectory));

                var commandParams = setupConfig.TranslateFiles(downloadedFiles);
                commandParams.Add("PORT", this.ui.ComPort);
                commandParams.Add("BOARD", setupConfig.Board);

                var commands = setupConfig.TranslateCommands(commandParams);

                this.programmer = new CommandLineProgrammer(this.ui.Log, commands, FirmwaresDownloadDirectory);

                this.ui.textUpdateFirmwareStatus.Text = "";
                this.pbBackground.Invalidate();

                this.ui.FinishFirmwareUpdate();
            }).Wait();
        }

        private void Ui_OnBoardTypeSelected(object sender, string boardType)
        {
            FirmwareSetupConfig config =  this.setupInfo.Configs.FirstOrDefault(c => c.Board.Equals(boardType, StringComparison.OrdinalIgnoreCase));

            if (config == null)
                return;

            if (config.IsInternetEnabled)
                this.ui.Flow |= SetupUI.Flow_InternetEnabled;

            if (config.FlashRequiresBootButtonHold)
                this.ui.Flow |= SetupUI.Flow_PressAndHold;
            else if (config.FlashRequiresBootButtonPress)
                this.ui.Flow |= SetupUI.Flow_Press;
            else
                this.ui.Flow |= SetupUI.Flow_NoPress;
        }

        private bool Ui_OnAccountCredentialsEntered(object sender, string name, string password)
        {
            this.userToken = null;
            if (!Task.Run(async () => { this.userToken = (await new OpenIoTService().RequestUserLogin(name, password))?.User?.UserId; }).Wait(5000))
            {
                MessageBox.Show("Could not connect to server.\r\nPlease check your internet connection", "Internet connection error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (String.IsNullOrWhiteSpace(this.userToken))
            {
                MessageBox.Show("The credentials you entered seem to be wrong", "Wrong credentials", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //this.boardScanner.StopScan();
        }

        private Task<bool> ResetDevice()
        {
            TaskCompletionSource<bool> tcs1 = new TaskCompletionSource<bool>();
            Task<bool> t1 = tcs1.Task;

            Task.Factory.StartNew(() =>
            {
                OpenIoTBoard board = new OpenIoTBoard(new ComTransmissionChannel(this.ui.ComPort));
                BoardResetEvents events = new BoardResetEvents(board, this.ui.EmbedWeblink, this.userToken, this.ui.DeviceName, this.ui.WifiNetworkName, this.ui.WifiNetworkPassword);
                board.EventHandlers.Add(events);

                board.Open();

                int i = 0;
                while (!events.IsDone && (i++ < 10))
                {
                    if (!events.IsResponsive)
                        board.RequestDeviceName();

                    Thread.Sleep(500);
                }

                board.Close(true);

                tcs1.SetResult(events.IsDone);
            });

            return t1;
        }
    }

    internal class BoardResetEvents : OpenIoTProtocolEventsHandler
    {
        OpenIoTBoard board;

        bool embedWeblink;
        string userId;
        string wifiNetworkName, wifiNetworkPassword;
        string deviceName;

        public bool IsResponsive { get; private set; }
        public bool IsDone { get; private set; }

        public BoardResetEvents(OpenIoTBoard board, bool embedWeblink, string userId, string deviceName, string wifiNetworkName, string wifiNetworkPassword)
            : base()
        {
            this.board = board;
            this.embedWeblink = embedWeblink;
            this.userId = userId;
            this.wifiNetworkName = wifiNetworkName;
            this.wifiNetworkPassword = wifiNetworkPassword;
            this.deviceName = deviceName;

            this.IsDone = false;
            this.IsResponsive = false;
        }

        public override void OnDeviceNameReceived(object sender, string name)
        {
            this.IsResponsive = true;
            this.board.ResetLogic();
        }

        public override void OnResetLogic(object sender)
        {
            this.board.RequestSetDeviceName(this.deviceName);
        }

        public override void OnDevicePropertiesSet(object sender, Dictionary<int, byte[]> properties)
        {
            if (properties != null)
            {
                if (properties.ContainsKey(OpenIoTProtocol.DevicePropertyId_Name))
                {
                    if (this.embedWeblink)
                        this.board.RequestSetUserId(this.userId);
                    else
                        this.IsDone = true;
                }
                else if (properties.ContainsKey(OpenIoTProtocol.DevicePropertyId_UserUid))
                {
                    this.UploadWeblink();
                }
            }
        }

        public void UploadWeblink()
        {
            this.board.UploadSchemeLogic(this.GenerateSchemeCode());
        }

        public override void OnSchemeLogicUploaded(object sender)
        {
            this.IsDone = true;
        }

        private byte[] GenerateSchemeCode()
        {
            List<byte> bytes = new List<byte>();

            // Generic Wifi
            bytes.Add(43);

            bytes.Add(0);
            bytes.Add((byte)(this.wifiNetworkName.Length + 1));
            bytes.AddRange(this.wifiNetworkName.Select(c => (byte)c));
            bytes.Add(0);

            bytes.Add(1);
            bytes.Add((byte)(this.wifiNetworkPassword.Length + 1));
            bytes.AddRange(this.wifiNetworkPassword.Select(c => (byte)c));
            bytes.Add(0);

            // Weblink
            bytes.AddRange(new byte[] { 34, 2, 1, 3, 1, 4, 0, 0, 32, 65 });

            return bytes.ToArray();
        }
    }
}
