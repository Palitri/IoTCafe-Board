using OpenIot.BoardSetup.Oui;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System;
using System.Diagnostics;
using System.Security.Policy;
using OpenIot.BoardSetup.Flashing;
using OpenIot.BoardSetup.Oui.Elements;
using OpenIot.BoardSetup.Oui.Resources;
using OpenIot.BoardSetup.Oui.Common;

namespace OpenIot.BoardSetup.UI
{
    public delegate void BasicEventDelegate(object sender);
    public delegate void BoardTypeSelectedDelegate(object sender, string boardType);
    public delegate void FlashInitiatedDelegate(object sender, string comPort, string boardType);
    public delegate bool AccountCredentialsEnteredDelegate(object sender, string name, string password);
    public delegate bool WifiNetworkCredentialsEntered(object sender, string name, string password);

    internal class SetupUI : OuiForm
    {
        public static int Flow_Default = 1 << 0;
        public static int Flow_InternetEnabled = 1 << 1;
        public static int Flow_EmbedWeblink = 1 << 2;
        public static int Flow_NoPress = 1 << 3;
        public static int Flow_Press = 1 << 4;
        public static int Flow_PressAndHold = 1 << 5;

        public int Flow { get; set; }

        private PictureBox pictureBox;
        private string[] boardTypes;

        private OuiStatesElement flashingContinueButton;
        private List<OuiElementCollection> pages;
        private OuiElementCollection pageDeviceSearch, pageHoldBootButton, pageFlashing;
        private OuiNativeTextBox tbAccountName, tbAccountPassword, tbWifiName, tbWifiPassword, tbDeviceName;

        public BoardTypeSelectedDelegate OnBoardTypeSelected;
        public FlashInitiatedDelegate OnFlashInitiated;
        public BasicEventDelegate OnClosed;
        public BasicEventDelegate OnMinimized;
        public BasicEventDelegate OnStartDeviceSearch;
        public BasicEventDelegate OnEndDeviceSearch;
        public AccountCredentialsEnteredDelegate OnAccountCredentialsEntered;
        public WifiNetworkCredentialsEntered OnWifiNetworkCredentialsEntered;

        public OuiText logText;

        public string ComPort { get; private set; }
        public string SelectedBoardType { get; private set; }
        public bool EmbedWeblink { get; private set; }
        public string AccountName { get; private set; }
        public string AccountPassword { get; private set; }
        public string WifiNetworkName { get; private set; }
        public string WifiNetworkPassword { get; private set; }
        public string DeviceName { get; private set; }

        public IFlashLog Log { get; set; }

        public SetupUI(PictureBox pictureBox, PointF size, string[] boardTypes)
            : base(pictureBox.CreateGraphics(), size)
        {
            this.pictureBox = pictureBox;
            this.pictureBox.MouseDown += PictureBox_MouseDown;
            this.pictureBox.MouseUp += PictureBox_MouseUp;
            this.pictureBox.MouseMove += PictureBox_MouseMove;
            this.pictureBox.Paint += PictureBox_Paint;

            this.boardTypes = boardTypes;

            this.pages = new List<OuiElementCollection>();

            this.Flow = Flow_Default;

            OuiFont whiteFont = (OuiFont)this.AddResource(new OuiFont(this, "Arial", this.GetUnitHeight(77.0f), Color.White));
            OuiFont grayFont = (OuiFont)this.AddResource(new OuiFont(this, "Arial", this.GetUnitHeight(77.0f), Color.FromArgb(179, 207, 230)));
            OuiFont infoHeaderFont = (OuiFont)this.AddResource(new OuiFont(this, "Arial", this.GetUnitHeight(50.0f), Color.White, true));
            OuiFont infoHintFont = (OuiFont)this.AddResource(new OuiFont(this, "Arial", this.GetUnitHeight(33.0f), Color.White));
            OuiFont linkFont = (OuiFont)this.AddResource(new OuiFont(this, "Arial", this.GetUnitHeight(33.0f), Color.FromArgb(194, 255, 139)));
            OuiFont warningFont = (OuiFont)this.AddResource(new OuiFont(this, "Arial", this.GetUnitHeight(33.0f), Color.FromArgb(255, 134, 100)));

            OuiBitmap closeImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Close));
            OuiBitmap closeSelectedImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Close_selected));
            OuiBitmap minimizeImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Minimize));
            OuiBitmap minimizeSelectedImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Minimize_selected));
            OuiBitmap underscoreLineImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Thin_Line_Underscore));
            OuiBitmap treeImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Tree));
            OuiBitmap pluginImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.Plug_in));
            OuiBitmap copyImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.copy_icon));
            OuiBitmap copySelectedImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.copy_icon_selected));
            OuiBitmap deviceButtonPressImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.button_push));
            OuiBitmap deviceButtonPressAndHoldImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.button_push_hold));
            OuiBitmap deviceButtonReleaseImage = (OuiBitmap)this.AddResource(new OuiBitmap(this, Properties.Resources.button_release));

            OuiStatesElement close = (OuiStatesElement)this.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { new OuiImage(closeImage, this.GetUnitCoords(918, 40)) }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { new OuiImage(closeSelectedImage, this.GetUnitCoords(918, 40)) }) },
            }));
            close.Clicked += Close_Clicked;

            OuiStatesElement minimize = (OuiStatesElement)this.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { new OuiImage(minimizeImage, this.GetUnitCoords(838, 40)) }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { new OuiImage(minimizeSelectedImage, this.GetUnitCoords(838, 40)) }) },
            }));
            minimize.Clicked += Minimize_Clicked;




            OuiElementCollection pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Device setup"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "This app will install OpenIoT firmware on a board device"));

            pageElement.AddElement(new OuiImage(treeImage, this.GetUnitCoords(480, 728)));

            OuiText txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Cancel");
            OuiStatesElement btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += Close_Clicked;

            OuiText txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Continue");
            OuiStatesElement btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnNext.Clicked += BtnPageNext_Clicked;

            pageElement.AddToFlow(Flow_Default);
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Plug in device"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Plug your device in to continue.\r\n\r\nTo retry, please plug the device out for a moment\r\nand then back in."));

            pageElement.AddElement(new OuiImage(pluginImage, this.GetUnitCoords(480, 780)));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            pageElement.AddToFlow(Flow_Default);
            this.pageDeviceSearch = pageElement;
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Select board"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Please select your board"));

            float y = 600;
            foreach (string boardType in this.boardTypes)
            {
                OuiText txtBoard = new OuiText(infoHintFont, this.GetUnitCoords(250, y), boardType);
                OuiStatesElement btnBoard = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
                {
                    { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBoard }) },
                    { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBoard, new OuiImage(underscoreLineImage, new PointF(txtBoard.Position.X + txtBoard.Metrics.Size.X / 2, txtBoard.Position.Y + txtBoard.Metrics.Size.Y * 0.3f), new PointF(txtBoard.Metrics.Size.X, underscoreLineImage.Size.Y)) }) },
                }));
                y += 70;

                btnBoard.Clicked += BtnBoardSelect_Clicked;
            }

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnBack.Clicked += BtnPageBack_Clicked;

            pageElement.AddToFlow(Flow_Default);
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Name"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Give a name to your device"));

            //pageElement.AddElement(new OuiImage(pluginImage, this.GetUnitCoords(480, 780)));

            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(100, 580), "Device name"));
            this.tbDeviceName = (OuiNativeTextBox)pageElement.AddElement(new OuiNativeTextBox(this.pictureBox, this, this.GetUnitCoords(100, 600), new PointF(0.7f, 0.06f), this.DeviceName));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Next");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnDeviceNamePageNext_Clicked;

            pageElement.AddToFlow(Flow_Default);
            //this.pageDeviceSearch = pageElement;
            this.pages.Add(pageElement);





            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Weblink"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Please choose whether to embed Weblink.\r\n\r\nWeblink will make your device\r\nimmediately accessible from the internet"));

            y = 720;
            OuiText txtWeblinkOption = new OuiText(infoHintFont, this.GetUnitCoords(250, y), "No");
            OuiStatesElement btnWeblinkOption = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtWeblinkOption }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtWeblinkOption, new OuiImage(underscoreLineImage, new PointF(txtWeblinkOption.Position.X + txtWeblinkOption.Metrics.Size.X / 2, txtWeblinkOption.Position.Y + txtWeblinkOption.Metrics.Size.Y * 0.3f), new PointF(txtWeblinkOption.Metrics.Size.X, underscoreLineImage.Size.Y)) }) },
            }));
            btnWeblinkOption.Clicked += BtnWeblinkOptionNo_Clicked;

            y += 80;
            txtWeblinkOption = new OuiText(infoHintFont, this.GetUnitCoords(250, y), "Yes");
            btnWeblinkOption = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtWeblinkOption }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtWeblinkOption, new OuiImage(underscoreLineImage, new PointF(txtWeblinkOption.Position.X + txtWeblinkOption.Metrics.Size.X / 2, txtWeblinkOption.Position.Y + txtWeblinkOption.Metrics.Size.Y * 0.3f), new PointF(txtWeblinkOption.Metrics.Size.X, underscoreLineImage.Size.Y)) }) },
            }));
            btnWeblinkOption.Clicked += BtnWeblinkOptionYes_Clicked;

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            pageElement.AddToFlow(Flow_InternetEnabled);
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Account Credentials"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Enter your IoT.Cafe credentials"));

            //pageElement.AddElement(new OuiImage(pluginImage, this.GetUnitCoords(480, 780)));

            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(100, 580), "Email"));
            this.tbAccountName = (OuiNativeTextBox)pageElement.AddElement(new OuiNativeTextBox(this.pictureBox, this, this.GetUnitCoords(100, 600), new PointF(0.7f, 0.06f), this.AccountName));

            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(100, 750), "Password"));
            this.tbAccountPassword = (OuiNativeTextBox)pageElement.AddElement(new OuiNativeTextBox(this.pictureBox, this, this.GetUnitCoords(100, 770), new PointF(0.7f, 0.06f), this.AccountPassword, true));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Next");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnAccountPageNext_Clicked;

            pageElement.AddToFlow(Flow_EmbedWeblink);
            //this.pageDeviceSearch = pageElement;
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "WiFi Connection"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Enter the WiFi network to use for internet connection"));

            //pageElement.AddElement(new OuiImage(pluginImage, this.GetUnitCoords(480, 780)));

            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(100, 580), "Network name"));
            this.tbWifiName = (OuiNativeTextBox)pageElement.AddElement(new OuiNativeTextBox(this.pictureBox, this, this.GetUnitCoords(100, 600), new PointF(0.7f, 0.06f), this.WifiNetworkName));

            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(100, 750), "Network password"));
            this.tbWifiPassword = (OuiNativeTextBox)pageElement.AddElement(new OuiNativeTextBox(this.pictureBox, this, this.GetUnitCoords(100, 770), new PointF(0.7f, 0.06f), this.WifiNetworkPassword, true));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Next");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnWifiPageNext_Clicked;

            pageElement.AddToFlow(Flow_EmbedWeblink);
            //this.pageDeviceSearch = pageElement;
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Install"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Setup is ready to install OpenIoT on your device."));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Install");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnFlash_Clicked;

            pageElement.AddToFlow(Flow_NoPress);
            this.pages.Add(pageElement);
            
            
            
            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Install"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Setup is ready to install OpenIoT on your device.\r\n\r\nPlease press the BOOT button\r\non the device and click Install"));

            pageElement.AddElement(new OuiImage(deviceButtonPressImage, this.GetUnitCoords(480, 780)));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Install");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnFlash_Clicked;

            pageElement.AddToFlow(Flow_Press);
            this.pages.Add(pageElement);



            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Install"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Please press AND HOLD the BOOT button\r\non the device and click Install\r\n\r\nKeep the BOOT button pressed."));

            pageElement.AddElement(new OuiImage(deviceButtonPressAndHoldImage, this.GetUnitCoords(480, 780)));

            txtBack = new OuiText(grayFont, this.GetUnitCoords(87, 1033), "Back");
            btnBack = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtBack }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtBack, new OuiImage(underscoreLineImage, new PointF(txtBack.Position.X + txtBack.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));
            btnBack.Clicked += BtnPageBack_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Install");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnFlash_Clicked;

            pageElement.AddToFlow(Flow_PressAndHold);
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Flashing"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Please keep the BOOT button pressed."));

            pageElement.AddElement(new OuiImage(deviceButtonPressAndHoldImage, this.GetUnitCoords(480, 780)));

            pageElement.AddToFlow(Flow_PressAndHold);
            this.pageHoldBootButton = pageElement;
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Flashing"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Please release the BOOT button\r\non your device and click Continue"));
            
            pageElement.AddElement(new OuiImage(deviceButtonReleaseImage, this.GetUnitCoords(480, 780)));

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Continue");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnPageNext_Clicked;

            pageElement.AddToFlow(Flow_PressAndHold);
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Flashing"));
            this.logText = (OuiText)pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 410), "Setup log", this.GetUnitWidth(855), this.GetUnitHeight(535)));

            OuiStatesElement btnCopy = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { new OuiImage(copyImage, this.GetUnitCoords(888, 888)) }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { new OuiImage(copySelectedImage, this.GetUnitCoords(888, 888)) }) },
            }));
            btnCopy.Clicked += BtnCopyLog_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Finish");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += BtnPageNext_Clicked;

            this.flashingContinueButton = btnNext;
            this.pageFlashing = pageElement;

            pageElement.AddToFlow(Flow_Default);
            this.pages.Add(pageElement);




            pageElement = new OuiElementCollection();

            pageElement.AddElement(new OuiText(infoHeaderFont, this.GetUnitCoords(82, 326), "Finished"));
            pageElement.AddElement(new OuiText(infoHintFont, this.GetUnitCoords(56, 480), "Setup has finished!\r\n\r\nTo create projects for your device, visit"));

            OuiText txtLink = new OuiText(linkFont, this.GetUnitCoords(56, 600), "https://iot.cafe");
            OuiStatesElement btnLink = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtLink }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtLink, new OuiImage(underscoreLineImage, new PointF(txtLink.Position.X + txtLink.Metrics.Size.X / 2, txtLink.Position.Y + txtLink.Metrics.Size.Y / 3)) }) },
            }));

            btnLink.Clicked += BtnLinkIoTCafe_Clicked;

            txtContinue = new OuiText(whiteFont, this.GetUnitCoords(600, 1033), "Close");
            btnNext = (OuiStatesElement)pageElement.AddElement(new OuiStatesElement(this, new Dictionary<OuiElementState, OuiElementCollection>()
            {
                { OuiElementState.Default, new OuiElementCollection(new List<IOuiElement>() { txtContinue }) },
                { OuiElementState.Hover, new OuiElementCollection(new List<IOuiElement>() { txtContinue, new OuiImage(underscoreLineImage, new PointF(txtContinue.Position.X + txtContinue.Metrics.Size.X / 2, this.GetUnitHeight(1054))) }) },
            }));

            btnNext.Clicked += Close_Clicked;

            pageElement.AddToFlow(Flow_Default);
            this.pages.Add(pageElement);



            this.AddElement(this.pages[0]);

        }

        public void SetSize(int width, int height)
        {
            base.SetSize(width, height);

            this.Invalidated += Oui_Invalidated;
        }

        private void BtnBoardSelect_Clicked(object sender, EventArgs e)
        {
            OuiStatesElement element = (OuiStatesElement)sender;
            this.SelectedBoardType = ((OuiText)element.Elements[OuiElementState.Default].Elements[0]).Text;

            this.OnBoardTypeSelected?.Invoke(this, this.SelectedBoardType);

            if (this.Log != null)
                this.Log.Log(String.Format("Selected board {0}", this.SelectedBoardType));

            this.NextPage();
        }

        private void BtnDeviceNamePageNext_Clicked(object sender, EventArgs e)
        {
            this.DeviceName = this.tbDeviceName.Text;

            this.NextPage();
        }

        private void BtnAccountPageNext_Clicked(object sender, EventArgs e)
        {
            this.AccountName = this.tbAccountName.Text;
            this.AccountPassword = this.tbAccountPassword.Text;

            if ((this.OnAccountCredentialsEntered == null) || this.OnAccountCredentialsEntered.Invoke(this, this.AccountName, this.AccountPassword))
                this.NextPage();
        }

        private void BtnWifiPageNext_Clicked(object sender, EventArgs e)
        {
            this.WifiNetworkName = this.tbWifiName.Text;
            this.WifiNetworkPassword = this.tbWifiPassword.Text;

            if ((this.OnWifiNetworkCredentialsEntered == null) || this.OnWifiNetworkCredentialsEntered.Invoke(this, this.WifiNetworkName, this.WifiNetworkPassword))
                this.NextPage();
        }

        private void BtnPageNext_Clicked(object sender, EventArgs e)
        {
            this.NextPage();
        }

        private void BtnPageBack_Clicked(object sender, EventArgs e)
        {
            this.PreviousPage();
        }

        private void BtnWeblinkOptionNo_Clicked(object sender, EventArgs e)
        {
            this.EmbedWeblink = false;
            this.NextPage();
        }

        private void BtnWeblinkOptionYes_Clicked(object sender, EventArgs e)
        {
            this.Flow |= Flow_EmbedWeblink;
            this.EmbedWeblink = true;
            this.NextPage();
        }

        private void BtnCopyLog_Clicked(object sender, EventArgs e)
        {
            Clipboard.SetText(this.logText.Text);
        }

        private void BtnLinkIoTCafe_Clicked(object sender, EventArgs e)
        {
            Process.Start(new System.Diagnostics.ProcessStartInfo()
            {
                FileName = "https://iot.cafe",
                UseShellExecute = true
            });
        }

        private void BtnFlash_Clicked(object sender, EventArgs e)
        {
            this.OnFlashInitiated?.Invoke(this, this.ComPort, this.SelectedBoardType);
            this.NextPage();
        }

        private void PictureBox_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            this.graphics = e.Graphics;
            this.Render();
        }

        private void PictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            this.SetCursorScreenPosition(e.X, e.Y);
        }

        private void PictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            this.SetCursorState(false);
        }

        private void PictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            this.SetCursorState(true);
        }

        private void Oui_Invalidated(object sender, EventArgs e)
        {
            this.pictureBox.Invalidate();
        }

        private void Minimize_Clicked(object sender, EventArgs e)
        {
            this.OnMinimized?.Invoke(this);
        }

        private void Close_Clicked(object sender, EventArgs e)
        {
            this.OnClosed?.Invoke(this);
        }

        public virtual void SetComPort(string comPort)
        {
            this.ComPort = comPort;

            if (this.Log != null)
                this.Log.Log(String.Format("Detected port {0}", this.ComPort));

            this.NextPage();            
        }

        public virtual void BeganFlashing()
        {
            if ((this.Flow & Flow_PressAndHold) != 0)
                if (this.Elements.Contains(this.pageHoldBootButton))
                    this.NextPage();
        }

        public virtual void FinishFlashing(bool success)
        {
            if (this.Log != null)
            {
                if (success)
                    this.Log.Log(Environment.NewLine + "Flash completed successfully");
                else
                    this.Log.Log(Environment.NewLine + "ERROR in flashing process. Check log for details.");
            }

            this.flashingContinueButton.Visible = true;
        }

        private void PreviousPage()
        {
            for (int i = 0; i < this.pages.Count; i++)
            {
                if (this.Elements.Contains(this.pages[i]))
                {
                    OuiElementCollection currentPage = this.pages[i];
                    while (i > 0)
                    {
                        i--;
                        OuiElementCollection newPage = this.pages[i];

                        if (newPage.BelongsToFlow(this.Flow))
                        {
                            this.ChangePage(currentPage, newPage);
                            break;
                        }
                    }

                    break;
                }
            }
        }

        private void NextPage()
        {
            for (int i = 0; i < this.pages.Count; i++)
            {
                if (this.Elements.Contains(this.pages[i]))
                {
                    OuiElementCollection currentPage = this.pages[i];

                    while (i < this.pages.Count - 1)
                    {
                        i++;
                        OuiElementCollection newPage = this.pages[i];

                        if (newPage.BelongsToFlow(this.Flow))
                        {
                            this.ChangePage(currentPage, newPage);
                            break;
                        }
                    }

                    break;
                }
            }
        }

        private void ChangePage(OuiElementCollection oldPage, OuiElementCollection newPage)
        {
            if (this.Elements.Contains(newPage))
                return;
            
            this.Elements.Remove(oldPage);
            this.Elements.Add(newPage);

            if (newPage == this.pageDeviceSearch)
                this.OnStartDeviceSearch?.Invoke(this);
            if (oldPage == this.pageDeviceSearch)
                this.OnEndDeviceSearch?.Invoke(this);

            if (newPage == this.pageFlashing)
                this.flashingContinueButton.Visible = false;

            this.OnInvalidated();
        }

    }
}
