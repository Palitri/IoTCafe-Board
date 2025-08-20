using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.API.Models
{
    internal class VersionInfoResponse : BaseApiResponse
    {
        public VersionInfo Version { get; set; }
    }
}
