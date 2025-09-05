using Palitri.OpenIoT.BoardFlash.Flashing.Boards.STM32;
using Palitri.OpenIoT.BoardUtility.CommandLine;
using Palitri.OpenIoT.Setup.Shared.Flashing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Palitri.OpenIoT.BoardUtility.Flashing
{
    internal interface IFlashProgrammerDispatcher
    {
        public string BoardName { get; }
        public IFlashProgrammer? Dispatch(CommandLineArguments arguments, IFlashLog log);
    }
}
