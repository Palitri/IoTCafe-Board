using Palitri.OpenIoT.BoardFlash.Flashing.Boards.STM32;
using Palitri.OpenIoT.BoardUtility.CommandLine;
using Palitri.OpenIoT.Setup.Shared.Flashing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Palitri.OpenIoT.BoardUtility.Flashing.Boards.STM32
{
    internal class STM32F103CFlashProgrammerDispatcher : IFlashProgrammerDispatcher
    {
        public string BoardName => "STM32F103C";

        public IFlashProgrammer? Dispatch(CommandLineArguments arguments, IFlashLog log)
        {
            if (!arguments.TryGetValue(CommandLineToken.Port, out var port))
            {
                log.Log(string.Format("Please provide a {0} parameter", CommandLineArguments.TokenNames[CommandLineToken.Port]), true, FlashMessageType.Error);

                return null;
            }

            if (!arguments.TryGetValue(CommandLineToken.FirmwareFile, out var firmware))
            {
                log.Log(string.Format("Please provide a {0} parameter", CommandLineArguments.TokenNames[CommandLineToken.FirmwareFile]), true, FlashMessageType.Error);

                return null;
            }

            return new STM32F103CFlashProgrammer(port[0], firmware[0], log);
        }
    }
}
