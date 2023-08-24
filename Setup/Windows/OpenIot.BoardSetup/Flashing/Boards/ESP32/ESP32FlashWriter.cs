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
    internal class ESP32FlashWriter : ExternalFlashProgrammer
    {
        public ESP32FlashWriter(string comPort, IFlashLog log)
            : base(
                  log,
                  "./../../../Boards/",
                  "esptool",
                  "--chip esp32 --port " + comPort + " --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 \"ESP32\\OpenIoT.ino.bootloader.bin\" 0x8000 \"ESP32\\OpenIoT.ino.partitions.bin\" 0xe000 \"ESP32\\boot_app0.bin\" 0x10000 \"ESP32\\OpenIoT.ino.esp32da.bin\"")
        {
            //Example command line: esptool --chip esp32 --port COM9 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 "ESP32\OpenIoT.ino.bootloader.bin" 0x8000 "ESP32\OpenIoT.ino.partitions.bin" 0xe000 "ESP32\boot_app0.bin" 0x10000 "ESP32\OpenIoT.ino.esp32da.bin"
        }

        public override void MessageReceived(string message, FlashMessageType messageType)
        {
            base.MessageReceived(message, messageType);

            if (message.Contains("Writing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }

    }
}
