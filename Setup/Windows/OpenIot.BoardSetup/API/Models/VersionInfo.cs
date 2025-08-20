using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.API.Models
{
    internal class VersionInfo
    {
        public String Product { get; set; }
        public String Version { get; set; }
        public string Date { get; set; }
        public String Notes { get; set; }
    }
}
