using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Resources
{
    internal class OuiBitmap : IOuiResource
    {
        public PointF Size { get; private set; }

        internal Bitmap bitmap;

        public OuiBitmap(IOuiScreenSpace screenSpace, Bitmap bitmap)
        {
            this.bitmap = bitmap;
            this.Size = screenSpace.GetUnitCoords(this.bitmap.Width, this.bitmap.Height);
        }

        public void Resize(IOuiScreenSpace screenSpace)
        {

        }

    }
}
