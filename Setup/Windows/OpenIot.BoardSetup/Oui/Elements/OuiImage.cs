using OpenIot.BoardSetup.Oui.Events;
using OpenIot.BoardSetup.Oui.Resources;
using OpenIot.BoardSetup.Properties;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal class OuiImage : OuiElement
    {
        public PointF Position { get; set; }
        public PointF Size { get; set; }

        private OuiBitmap bitmap;

        public OuiImage(OuiBitmap bitmap, PointF position, PointF? size = null)
            : base()
        {
            this.bitmap = bitmap;
            this.Position = position;
            this.Size = size != null ? size.Value : bitmap.Size;
        }

        public override void Render(IOuiRendering renderer)
        {
            renderer.Draw(this.bitmap, this.Position, this.Size);
        }

        public override bool Intersect(PointF coords)
        {
            return 
                (coords.X >= this.Position.X - this.Size.X / 2) && (coords.X <= this.Position.X + this.Size.X / 2) &&
                (coords.Y >= this.Position.Y - this.Size.Y / 2) && (coords.Y <= this.Position.Y + this.Size.Y / 2);
        }
    }
}
