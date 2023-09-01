using OpenIoT.BoardSetup.Transmission;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OpenIot.BoardSetup.Flashing.Boards.ESP32
{
    internal class ESP32FlashEraser : ExternalFlashProgrammer
    {
        public static string BoardName = "ESP32";

        public ESP32FlashEraser(string comPort, IFlashLog log)
            : base(
                  log,
                  "./Boards/",
                  "esptool",
                  "--chip esp32 --port " + comPort + " erase_flash")
        {
            //Example command line: esptool --chip esp32 --port COM9 erase_flash
        }

        public override void MessageReceived(string message, FlashMessageType messageType)
        {
            base.MessageReceived(message, messageType);

            if (message.Contains("Erasing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }

    }
}
