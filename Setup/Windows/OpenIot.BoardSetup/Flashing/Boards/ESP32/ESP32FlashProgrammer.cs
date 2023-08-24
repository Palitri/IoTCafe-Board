using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Flashing.Boards.ESP32
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
