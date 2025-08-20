using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.ESP8266
{
    internal class ESP8266FlashEraser : ExternalFlashProgrammer
    {
        public ESP8266FlashEraser(string comPort, IFlashLog log)
            : base(
                  log,
                  "./Boards/",
                  "esptool",
                  "--chip esp8266 --port " + comPort + " erase_flash")
        {
            //Example command line: esptool --chip esp8266 --port COM9 erase_flash
        }

        public override void OnMessageReceived(string message, FlashMessageType messageType)
        {
            base.OnMessageReceived(message, messageType);

            if (message.Contains("Erasing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }

    }
}
