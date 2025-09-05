using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Palitri.OpenIoT.BoardUtility.CommandLine
{
    /// <summary>
    /// Parses command line in a format like
    /// <TokenName> [TokenValue1 TokenValue2 ... TokenValueN]
    /// where N is defined in the TokensInfo dictionary.
    /// 
    /// Allows for the same token to appear multiple time, in which case each occurrence adds values to the record of the specific token, for example
    /// <Token> [Value1] <Token> [Value2]
    /// will result in a record like
    /// { Token, {Value1, Value2} }
    /// </summary>
    internal class CommandLineArguments : Dictionary<CommandLineToken, List<string>>
    {
        public static Dictionary<string, Tuple<CommandLineToken, int, string>> TokensInfo => new Dictionary<string, Tuple<CommandLineToken, int, string>>()
        {
            { "--flash", new Tuple<CommandLineToken, int, string>(CommandLineToken.Flash, 0, "Flashes the specified board") },
            { "--board",    new Tuple<CommandLineToken, int, string>(CommandLineToken.Board, 1, "Specifies the board type to be flashed") },
            { "--port",     new Tuple<CommandLineToken, int, string>(CommandLineToken.Port, 1, "Specifies the port at which the board can be found") },
            { "--firmware", new Tuple<CommandLineToken, int, string>(CommandLineToken.FirmwareFile, 1, "Specifies firmware file to flash the board with") },
            { "--help", new Tuple<CommandLineToken, int, string>(CommandLineToken.Help, 0, "Displays general help information") },
            { "--version", new Tuple<CommandLineToken, int, string>(CommandLineToken.Version, 0, "Displays app version") },
            { "--boards", new Tuple<CommandLineToken, int, string>(CommandLineToken.Boards, 0, "Displays a list of the supported board types") },
        };

        public static Dictionary<CommandLineToken, string> TokenNames => TokensInfo.ToDictionary(t => t.Value.Item1, t => t.Key);

        public CommandLineArguments(string[] args)
        {
            int index = 0;

            while (index < args.Count())
            {
                string tokenName = args[index].ToLower();
                index++;

                if (TokensInfo.TryGetValue(tokenName, out var tokenInfo))
                {
                    if (index + tokenInfo.Item2 <= args.Count())
                    {
                        var tokenValues = args.Take(new Range(index, index + tokenInfo.Item2)).ToList();
                        index += tokenInfo.Item2;

                        if (this.TryGetValue(tokenInfo.Item1, out var existingValues))
                            existingValues.AddRange(tokenValues);
                        else
                            this.Add(tokenInfo.Item1, tokenValues);
                    }
                }
            }
        }
    }
}
