using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.SetupConfigModels
{
    internal class FirmwareSetupConfig
    {
        public string Board { get; set; }
        public string OS { get; set; }
        public Dictionary<string, string> Files { get; set; }
        public List<string> Commands { get; set; }
        public List<string> Features { get; set; }

        public bool IsInternetEnabled => this.Features.Any(f => f.Equals("InternetEnabled", StringComparison.OrdinalIgnoreCase));
        public bool FlashRequiresBootButtonHold => this.Features.Any(f => f.Equals("FlashRequiresBootButtonHold", StringComparison.OrdinalIgnoreCase));
        public bool FlashRequiresBootButtonPress => this.Features.Any(f => f.Equals("FlashRequiresBootButtonPress", StringComparison.OrdinalIgnoreCase));

        public static string TranslateCommand(string command, Dictionary<string, string> parameters = null)
        {
            if (parameters != null)
                foreach (string key in parameters.Keys)
                    command = command.Replace("{" + key + "}", "\"" + parameters[key] + "\"");

            return command;
        }

        public List<string> TranslateCommands(Dictionary<string, string> parameters = null)
        {
            return this.Commands.Select(c => TranslateCommand(c, parameters)).ToList();
        }

        /// <summary>
        /// Produces a new dictionary by changing the values of the Files dictionary. The keys remain intact
        /// Can be used to replace the url values of files with local directory filename values
        /// </summary>
        /// <param name="alternatives">An <OldValue, NewValue></param>
        /// <returns>A new dictionary witht the same keys as Files, but with the provided alternative values</returns>
        public Dictionary<string, string> TranslateFiles(Dictionary<string, string> alternatives)
        {
            return this.Files.ToDictionary(e => e.Key, e => alternatives[e.Value]);
        }
    }
}
