using OpenIot.BoardSetup.Flashing;
using OpenIot.BoardSetup.Oui.Elements;
using OpenIot.BoardSetup.UI;
using System;
using System.Drawing;
using System.Windows.Forms;
using OpenIoT.Lib.Board.Scanner;
using OpenIoT.Lib.Composite;
using OpenIoT.Lib.Board.Api;
using OpenIoT.Lib.Board.Transmission.Com;
using System.Linq;
using System.Threading;
using System.IO.Ports;
using OpenIoT.Lib.Board.Protocol.Events;
using Microsoft.VisualBasic.ApplicationServices;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;
using OpenIoT.Lib.Web.Api;
using System.CodeDom.Compiler;
using OpenIoT.Lib.Board.Protocol;
using OpenIoT.Lib.Web.Models;
using OpenIot.BoardSetup.Flashing.Boards.ESP8266;
using OpenIot.BoardSetup.Flashing.Boards.ESP32;
using OpenIot.BoardSetup.Flashing.Boards.STM32;

namespace OpenIot.BoardSetup
{
    public partial class MainForm : Form
    {
        internal SetupUI ui;

        private BoardScanner boardScanner;

        private string userToken = String.Empty;

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

        class BoardDescription
        {
            public string Name { get; set; }
            public bool IsInternetEnabled { get; set; }
            public bool RequiresButtonPressToFlash { get; set; }
            public bool RequiresButtonHoldToFlash { get; set; }
        }

        private Dictionary<string, BoardDescription> boardsDescription = new Dictionary<string, BoardDescription>()
        {
            { ESP32FlashProgrammer.BoardName,       new BoardDescription() { Name = ESP32FlashProgrammer.BoardName,      IsInternetEnabled = true,   RequiresButtonPressToFlash = true,  RequiresButtonHoldToFlash = true } },
            { ESP8266FlashProgrammer.BoardName,     new BoardDescription() { Name = ESP8266FlashProgrammer.BoardName,    IsInternetEnabled = true,   RequiresButtonPressToFlash = true,  RequiresButtonHoldToFlash = true } },
            { STM32F103CFlashProgrammer.BoardName,  new BoardDescription() { Name = STM32F103CFlashProgrammer.BoardName, IsInternetEnabled = false,  RequiresButtonPressToFlash = true,  RequiresButtonHoldToFlash = false } }
        };

        public MainForm()
        {
            InitializeComponent();

            this.Width = Screen.PrimaryScreen.WorkingArea.Width / 4;
            this.Height = this.Width * 9 / 8;
            this.Left = (Screen.PrimaryScreen.WorkingArea.Width - this.Width) / 2;
            this.Top = (Screen.PrimaryScreen.WorkingArea.Height - this.Height) / 2;

            this.ui = new SetupUI(this.pbBackground, new PointF(960.0f, 1080.0f), this.boardsDescription.Select(p => p.Key).ToArray());
            this.ui.OnFlashInitiated = Ui_OnFlashInitiated;
            this.ui.OnMinimized = Ui_OnMinimized;
            this.ui.OnClosed = Ui_OnClosed;
            this.ui.OnStartDeviceSearch = Ui_OnStartDeviceSearch;
            this.ui.OnEndDeviceSearch = Ui_OnEndDeviceSearch;
            this.ui.OnBoardTypeSelected = Ui_OnBoardTypeSelected;
            this.ui.OnAccountCredentialsEntered = Ui_OnAccountCredentialsEntered;
            this.ui.SetSize(this.Width, this.Height);
            this.ui.Log = new OuiTextLog(this.ui.logText, this.pbBackground);


            this.boardScanner = new BoardScanner();
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

                IFlashProgrammer programmer = null;
                if (boardType == STM32F103CFlashProgrammer.BoardName)
                    programmer = new STM32F103CFlashProgrammer(comPort, this.ui.Log);

                else if (boardType == ESP8266FlashProgrammer.BoardName)
                    programmer = new ESP8266FlashProgrammer(comPort, this.ui.Log);

                else if (boardType == ESP32FlashProgrammer.BoardName)
                    programmer = new ESP32FlashProgrammer(comPort, this.ui.Log);

                if (programmer != null)
                {
                    programmer.BeganFlashing += Programmer_BeganFlashing;
                    
                    bool flashSuccessfull = await programmer.FlashBoardAsync();

                    programmer.BeganFlashing -= Programmer_BeganFlashing;

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
            this.boardScanner.ScanContinuously();
        }

        private void Ui_OnEndDeviceSearch(object sender)
        {
            this.BeginInvoke(delegate
            {
                this.boardScanner.OnPortAvailable = null;
                this.boardScanner.StopScan(false);
            });
        }

        private void Ui_OnBoardTypeSelected(object sender, string boardType)
        {
            BoardDescription boardDescription;
            if (!this.boardsDescription.TryGetValue(boardType, out boardDescription))
                return;

            if (boardDescription.IsInternetEnabled)
                this.ui.Flow |= SetupUI.Flow_InternetEnabled;

            if (boardDescription.RequiresButtonPressToFlash)
            {
                if (boardDescription.RequiresButtonHoldToFlash)
                    this.ui.Flow |= SetupUI.Flow_PressAndHold;
                else
                    this.ui.Flow |= SetupUI.Flow_Press;
            }
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
                        board.requestDeviceName();

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

        public override void onDeviceNameReceived(object sender, string name)
        {
            this.IsResponsive = true;
            this.board.resetLogic();
        }

        public override void onResetLogic(object sender)
        {
            this.board.requestSetDeviceName(this.deviceName);
        }

        public override void onDevicePropertiesSet(object sender, Dictionary<int, byte[]> properties)
        {
            if (properties != null)
            {
                if (properties.ContainsKey(OpenIoTProtocol.DevicePropertyId_Name))
                {
                    if (this.embedWeblink)
                        this.board.requestSetUserId(this.userId);
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
            //byte[] weblink = "34,0,0,1,1,2,0,0,112,66".Split(',').Select(i => Byte.Parse(i)).ToArray();
            this.board.uploadSchemeLogic(this.GenerateSchemeCode());
        }

        public override void onSchemeLogicUploaded(object sender)
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
