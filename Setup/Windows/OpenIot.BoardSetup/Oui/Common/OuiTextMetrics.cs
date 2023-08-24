using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Common
{
    internal class OuiTextMetrics
    {
        public PointF Size { get; private set; }

        public OuiTextMetrics(PointF size)
        {
            this.Size = size;
        }
    }
}
