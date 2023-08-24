using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Flashing
{
    public interface IFlashLog
    {
        void Log(string message, bool newLine = true, FlashMessageType messageType = FlashMessageType.Info, int messageId = 0);
    }
}
