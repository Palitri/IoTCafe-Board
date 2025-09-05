using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Palitri.OpenIoT.BoardFlash.Flashing.Boards.STM32
{
    public enum FlashingResult
    {
        Success = 0,
        GeneralError = 1,
        CommunicationOpeningError = 2,
        CommunicationProtocolError = 3,
        FlashingError = 4,
    }
}
