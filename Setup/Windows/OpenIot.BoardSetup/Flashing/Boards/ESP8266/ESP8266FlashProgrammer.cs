using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Flashing.Boards.ESP8266
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
