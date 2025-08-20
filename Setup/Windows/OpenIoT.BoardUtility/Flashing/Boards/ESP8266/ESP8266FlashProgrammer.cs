using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.ESP8266
{
    internal class ESP8266FlashProgrammer : FlashProgrammersList
    {
        public static string BoardName = "ESP8266";

        public ESP8266FlashProgrammer(string comPort, IFlashLog log)
        {
            this.Add(new ESP8266FlashEraser(comPort, log));
            this.Add(new ESP8266FlashWriter(comPort, log));
        }
    }
}
