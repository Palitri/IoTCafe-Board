using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Flashing.Boards.ESP8266
{
    internal class ESP8266FlashWriter : ExternalFlashProgrammer
    {
        public ESP8266FlashWriter(string comPort, IFlashLog log)
            : base(
                  log,
                  "./../../../Boards/",
                  "esptool",
                  "--chip esp8266 --port " + comPort + " --baud 115200 --before default_reset --after hard_reset write_flash 0x0 \"ESP8266\\OpenIoT.ino.generic.bin\"")
        {
        }

        public override void MessageReceived(string message, FlashMessageType messageType)
        {
            base.MessageReceived(message, messageType);

            Debug.WriteLine(message);

            if (message.Contains("Writing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }
    }
}
