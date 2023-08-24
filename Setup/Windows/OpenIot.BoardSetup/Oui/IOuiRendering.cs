using OpenIot.BoardSetup.Oui.Resources;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui
{
    interface IOuiRendering
    {
        void Draw(OuiBitmap bitmap, PointF position, PointF size);
        void Draw(OuiFont font, string text, PointF position);
    }
}
