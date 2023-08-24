using OpenIot.BoardSetup.Oui.Common;
using OpenIot.BoardSetup.Oui.Events;
using OpenIot.BoardSetup.Oui.Resources;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal class OuiText : OuiElement
    {
        public PointF Position { get; set; }

        public PointF PositionMiddle { get { return new PointF(this.Position.X + this.Metrics.Size.X / 2, this.Position.Y - this.Font.Metrics.Baseline + this.Metrics.Size.Y / 2); } }
        
        public OuiTextMetrics Metrics { get; private set; }
        
        public OuiFont Font { get; private set; }

        private string displayedText;
        private string _text;
        public string Text { get { return this._text; } set { this._text = value; this.displayedText = value; this.UpdateMetrics(); } }

        private float? maxWidth;
        private float? maxHeight;

        public OuiText(OuiFont font, PointF position, string text, float? maxWidth = null, float? maxHeight = null)
            : base()
        {
            this.Position = position;

            this.Font = font;
            this.Text = text;

            this.maxWidth = maxWidth;
            this.maxHeight = maxHeight;
        }

        public void AppendText(string text)
        {
            if (String.IsNullOrEmpty(text))
                return;

            this._text += text;

            string[] lines = this._text.Split(Environment.NewLine);

            List<string> processedLines = new List<string>();

            if (this.maxWidth != null)
            {
                foreach (string line in lines)
                {
                    int start = 0;
                    int end = line.Length - 1;
                    do
                    {
                        while (this.Font.GetTextMetrics(line.Substring(start, end - start + 1)).Size.X > this.maxWidth)
                        {
                            end = line.LastIndexOf(' ', end);
                            if ((end == -1) || (end <= start))
                            {
                                end = line.Length - 1;
                                break; // TODO: split the word by characters
                            }
                            else
                                end--;
                        }
                        processedLines.Add(line.Substring(start, end - start + 1));
                        start = end + 2;
                        end = line.Length - 1;
                    }
                    while (start < end);
                }
            }



            int renderLines = this.maxHeight != null ? Math.Min((int)(this.maxHeight / this.Font.Metrics.LineHeight), processedLines.Count) : processedLines.Count;
            int startLine = processedLines.Count - renderLines;
            int endLine = startLine + renderLines - 1;

            StringBuilder result = new StringBuilder();
            for (int i = startLine; i <= endLine; i++)
            {
                if (i != startLine)
                    result.Append(Environment.NewLine);
                
                result.Append(processedLines[i]);
            }

            this.displayedText = result.ToString();
        }

        public override void Render(IOuiRendering renderer)
        {
            renderer.Draw(Font, this.displayedText, Position);
        }

        public override bool Intersect(PointF coords)
        {
            float x = this.Position.X;
            float y = this.Position.Y - this.Font.Metrics.Baseline;

            return
                (coords.X >= x) && (coords.X <= x + this.Metrics.Size.X) &&
                (coords.Y >= y) && (coords.Y <= y + this.Metrics.Size.Y);
        }

        private void UpdateMetrics()
        {
            this.Metrics = this.Font.GetTextMetrics(this.Text);
        }

    }
}
