using OpenIot.BoardSetup.Oui.Resources;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal class OuiNativeTextBox : OuiNativeElement
    {
        public OuiFont Font { get; private set; }

        internal System.Windows.Forms.TextBox nativeTextBox;
        private IOuiScreenSpace screenSpace;

        public OuiNativeTextBox(Control parent, IOuiScreenSpace screenSpace, PointF position, PointF metrics, string text = null, bool isMasked = false)
            : base(parent)
        {
            this.screenSpace = screenSpace;

            this.nativeTextBox = new TextBox();
            this.nativeTextBox.Text = text == null ? String.Empty : text;
            this.nativeTextBox.UseSystemPasswordChar = isMasked;

            this.Position = position;
            this.Metrics = metrics;
        }

        public PointF Position { get; set; }
        public PointF Metrics { get; set; }

        public string Text { get { return this.nativeTextBox.Text; } set { this.nativeTextBox.Text = value; } }

        private void ResizeToScreenSpaceSize(IOuiScreenSpace screenSpace, float desiredHeight)
        {
            float originalFontHeight = screenSpace.GetUnitHeight(this.nativeTextBox.Font.Height);
            float originalControlHeight = screenSpace.GetUnitHeight(this.nativeTextBox.Height);
            float desiredFontHeight = desiredHeight - (originalControlHeight - originalFontHeight);
            float sizeFactor = desiredFontHeight / originalFontHeight;

            this.nativeTextBox.Font = new Font(this.nativeTextBox.Font.FontFamily, this.nativeTextBox.Font.Size * sizeFactor);

            float resultingFontHeight = screenSpace.GetUnitHeight(this.nativeTextBox.Font.Height);
        }

        public override void Render(IOuiRendering renderer)
        {
        }

        public override bool Intersect(PointF coords)
        {
            return
                (coords.X >= this.Position.X) && (coords.X <= this.Position.X + this.Metrics.X) &&
                (coords.Y >= this.Position.Y) && (coords.Y <= this.Position.Y + this.Metrics.Y);
        }

        public override void Show()
        {
            PointF pos = this.screenSpace.GetScreenCoords(this.Position);
            PointF size = this.screenSpace.GetScreenCoords(this.Metrics);

            this.ResizeToScreenSpaceSize(this.screenSpace, this.Metrics.Y);

            this.nativeTextBox.Left = (int)pos.X;
            this.nativeTextBox.Top = (int)pos.Y;
            this.nativeTextBox.Width = (int)size.X;
            this.nativeTextBox.Height = (int)size.Y;

            this.nativeTextBox.Parent = this.parent;
            this.nativeTextBox.Show();
        }
        public override void Hide()
        {
            this.parent.Controls.Remove(this.nativeTextBox);
        }

    }
}
