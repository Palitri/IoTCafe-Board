using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui
{
    interface IOuiResource
    {
        public void Resize(IOuiScreenSpace screenSpace);
    }
}
