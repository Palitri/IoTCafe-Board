using Palitri.OpenIoT.Setup.Shared.Flashing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Palitri.OpenIoT.BoardFlash
{
    internal class ConsoleLog : IFlashLog
    {
        public void Log(string message, bool newLine = true, FlashMessageType messageType = FlashMessageType.Info, int messageId = 0)
        {
            if (newLine)
                Console.WriteLine(message);
            else
                Console.Write(message);
        }
    }
}
