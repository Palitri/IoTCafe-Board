using Palitri.OpenIoT.Setup.Shared.Flashing;
using System.Diagnostics;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.ESP8266
{
    internal class ESP8266FlashWriter : ExternalFlashProgrammer
    {
        public ESP8266FlashWriter(string comPort, IFlashLog log)
            : base(
                  log,
                  "./Boards/",
                  "esptool",
                  "--chip esp8266 --port " + comPort + " --baud 115200 --before default_reset --after hard_reset write_flash 0x0 \"ESP8266\\OpenIoT.ino.generic.bin\"")
        {
        }

        public override void OnMessageReceived(string message, FlashMessageType messageType)
        {
            base.OnMessageReceived(message, messageType);

            Debug.WriteLine(message);

            if (message.Contains("Writing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }
    }
}
