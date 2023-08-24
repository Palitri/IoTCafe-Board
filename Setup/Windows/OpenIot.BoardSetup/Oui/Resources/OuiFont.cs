using OpenIot.BoardSetup.Oui.Common;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OpenIot.BoardSetup.Oui.Resources
{
    internal class OuiFont : IOuiResource
    {
        internal Font font;
        internal Brush brush;
        internal OuiFontMetrics Metrics { get; private set; }
        private float height;
        private IOuiScreenSpace screenSpace;
        private string fontName;
        private bool bold;

        internal OuiFont(IOuiScreenSpace screenSpace, string fontName, float height, Color color, bool bold = false)
        {
            this.screenSpace = screenSpace;
            this.height = height;
            this.brush = new SolidBrush(color);
            this.fontName = fontName;
            this.bold = bold;

            this.CreateFont();
        }

        public void Resize(IOuiScreenSpace screenSpace)
        {
            this.screenSpace = screenSpace;
            this.CreateFont();
        }

        internal void CreateFont()
        {
            FontFamily fontFamily = new FontFamily(fontName);
            this.font = new Font(fontFamily, this.screenSpace.GetScreenHeight(this.height), this.bold ? FontStyle.Bold : FontStyle.Regular, GraphicsUnit.Pixel);

            float lineSpacing = fontFamily.GetLineSpacing(font.Style);
            float ascent = fontFamily.GetCellAscent(font.Style);
            float baseline = this.font.GetHeight() * ascent / lineSpacing;
            float lineHeight = this.font.GetHeight();

            this.Metrics = new OuiFontMetrics(
                this.screenSpace.GetUnitHeight(baseline),
                this.screenSpace.GetUnitHeight(ascent),
                this.screenSpace.GetUnitHeight(lineSpacing),
                this.screenSpace.GetUnitHeight(lineHeight)
                );
        }

        internal OuiTextMetrics GetTextMetrics(string text)
        {
            Size size = TextRenderer.MeasureText(text, this.font);
            return new OuiTextMetrics(this.screenSpace.GetUnitCoords(new PointF(size.Width, size.Height)));
        }
    }
}
