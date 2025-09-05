using NAudio.Codecs;
using Palitri.OpenIoT.BoardFlash;
using Palitri.OpenIoT.BoardFlash.Flashing.Boards.STM32;
using Palitri.OpenIoT.BoardUtility.CommandLine;
using Palitri.OpenIoT.BoardUtility.Flashing;
using Palitri.OpenIoT.BoardUtility.Flashing.Boards.STM32;
using Palitri.OpenIoT.Setup.Shared.Flashing;
using System.Text;

namespace Palitri.OpenIoT.BoardSetup;

class Program
{
    const string version = "0.891";

    static List<IFlashProgrammerDispatcher> dispatchers = new List<IFlashProgrammerDispatcher>()
    {
        new STM32F103CFlashProgrammerDispatcher(),
    };

    static async Task<int> Main(string[] args)
    {
        IFlashLog log = new ConsoleLog();

        CommandLineArguments arguments = new CommandLineArguments(args);

        if (arguments.ContainsKey(CommandLineToken.Help) || arguments.Count == 0)
        {
            log.Log("OpenIoT Board Flashing utility");
            log.Log(string.Empty);

            log.Log("Version:");
            PrintVersion(log);
            log.Log(string.Empty);

            log.Log("Commands:");
            PrintCommands(log);
            log.Log(string.Empty);

            log.Log("Supported boards:");
            PrintSupportedBoards(log);
            log.Log(string.Empty);
        }

        if (arguments.ContainsKey(CommandLineToken.Version))
        {
            PrintVersion(log);
        }

        if (arguments.ContainsKey(CommandLineToken.Boards))
        {
            PrintSupportedBoards(log);
        }

        if (arguments.ContainsKey(CommandLineToken.Flash))
        {
            if (!arguments.ContainsKey(CommandLineToken.Board))
            {
                log.Log("Please specify a board", true, FlashMessageType.Error);
                return -1;
            }

            var dispatcher = dispatchers.FirstOrDefault(d => d.BoardName.Equals(arguments[CommandLineToken.Board][0], StringComparison.OrdinalIgnoreCase));
            if (dispatcher == null)
            {
                log.Log($"Unsupported board {arguments[CommandLineToken.Board][0]}", true, FlashMessageType.Error);
                return -1;
            }

            IFlashProgrammer? programmer = dispatcher.Dispatch(arguments, log);

            if (programmer == null)
            {
                log.Log("Flashing finished with an error", true, FlashMessageType.Error);
                return -1;
            }

            bool result = await programmer.FlashBoardAsync();

            if (result)
                log.Log("Flashing finished successfully", true, FlashMessageType.Info);
            else
                log.Log("Flashing finished with an error", true, FlashMessageType.Error);

            return result ? 0 : -1;
        }

        return 0;
    }

    static void PrintVersion(IFlashLog log)
    {
        log.Log(version);
    }
    static void PrintCommands(IFlashLog log)
    {
        foreach (var tokenInfo in CommandLineArguments.TokensInfo)
        {
            StringBuilder commandInfo = new StringBuilder();

            commandInfo.Append(tokenInfo.Key);
            commandInfo.Append(" ");

            if (tokenInfo.Value.Item2 == 1)
            {
                commandInfo.Append("<value> ");
            }
            else
            {
                for (int i = 0; i < tokenInfo.Value.Item2; i++)
                {
                    commandInfo.Append($"<value{i + 1}> ");
                }
            }

            commandInfo.Append("- ");
            commandInfo.Append(tokenInfo.Value.Item3);

            log.Log(commandInfo.ToString());
        }
    }

    static void PrintSupportedBoards(IFlashLog log)
    {
        foreach (IFlashProgrammerDispatcher dispatcher in dispatchers)
            log.Log(dispatcher.BoardName);
    }
}