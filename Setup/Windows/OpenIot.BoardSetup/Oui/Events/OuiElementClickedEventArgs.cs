using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Events
{
    internal class OuiElementClickedEventArgs : EventArgs
    {
        public PointF Coords { get; private set; }

        public OuiElementClickedEventArgs(PointF coords)
        {
            this.Coords = coords;
        }
    }
}
