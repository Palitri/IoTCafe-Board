using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.SetupConfigModels
{
    internal class FirmwareSetupInfo
    {
        public string Name { get; set; }
        public string Version { get; set; }
        public string Vendor { get; set; }
        public List<FirmwareSetupConfig> Configs { get; set; }

        public static FirmwareSetupInfo FromJson(string json)
        {
            return JsonSerializer.Deserialize<FirmwareSetupInfo>(json, new JsonSerializerOptions { PropertyNamingPolicy = JsonNamingPolicy.CamelCase });
        }

        public FirmwareSetupConfig GetSetupConfigForBoardAndOs(string board, string os)
        {
            return Configs.FirstOrDefault(c => c.Board.Equals(board, StringComparison.OrdinalIgnoreCase) && c.OS.Equals(os, StringComparison.OrdinalIgnoreCase));
        }
        public FirmwareSetupConfig GetSetupConfigsForOs(string os)
        {
            return Configs.FirstOrDefault(c => c.OS.Equals(os, StringComparison.OrdinalIgnoreCase));
        }
    }
}
