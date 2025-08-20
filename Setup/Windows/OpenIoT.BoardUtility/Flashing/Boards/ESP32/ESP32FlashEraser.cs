using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.ESP32
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

        public override void OnMessageReceived(string message, FlashMessageType messageType)
        {
            base.OnMessageReceived(message, messageType);

            if (message.Contains("Erasing", StringComparison.OrdinalIgnoreCase))
                OnBeganFlashing();
        }

    }
}
