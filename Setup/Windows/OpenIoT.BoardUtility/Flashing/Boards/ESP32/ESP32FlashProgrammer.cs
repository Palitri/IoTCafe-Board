using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.ESP32
{
    internal class ESP32FlashProgrammer : FlashProgrammersList
    {
        public static string BoardName = "ESP32";

        public ESP32FlashProgrammer(string comPort, IFlashLog log)
        {
            this.Add(new ESP32FlashEraser(comPort, log));
            this.Add(new ESP32FlashWriter(comPort, log));
        }
    }
}
