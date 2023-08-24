using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Flashing
{
    delegate void BeganFlashingDelegate();

    internal interface IFlashProgrammer
    {
        Task<bool> FlashBoardAsync();

        event BeganFlashingDelegate BeganFlashing;
    }
}
