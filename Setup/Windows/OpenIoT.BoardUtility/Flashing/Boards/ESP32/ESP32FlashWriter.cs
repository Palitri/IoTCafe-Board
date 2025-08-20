using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.ESP32
{
    internal class ESP32FlashWriter : ExternalFlashProgrammer
    {
        public ESP32FlashWriter(string comPort, IFlashLog log)
            : base(
                  log,
                  "./Boards/",
                  "esptool",
                  "--chip esp32 --port " + comPort + " --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 \"ESP32\\OpenIoT.ino.bootloader.bin\" 0x8000 \"ESP32\\OpenIoT.ino.partitions.bin\" 0xe000 \"ESP32\\boot_app0.bin\" 0x10000 \"ESP32\\OpenIoT.ino.esp32da.bin\"")
        {
            //Example command line: esptool --chip esp32 --port COM9 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 "ESP32\OpenIoT.ino.bootloader.bin" 0x8000 "ESP32\OpenIoT.ino.partitions.bin" 0xe000 "ESP32\boot_app0.bin" 0x10000 "ESP32\OpenIoT.ino.esp32da.bin"
        }

        public override void OnMessageReceived(string message, FlashMessageType messageType)
        {
            base.OnMessageReceived(message, messageType);

            if (message.Contains("Writing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }

    }
}
