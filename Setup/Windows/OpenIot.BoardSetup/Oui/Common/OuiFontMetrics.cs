using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Common
{
    internal class OuiFontMetrics
    {
        public float Baseline { get; private set; }
        public float Ascent { get; private set; }
        public float LineSpacing { get; private set; }
        public float LineHeight { get; private set; }

        public OuiFontMetrics(float baseline, float ascent, float lineSpacing, float lineHeight)
        {
            this.Baseline = baseline;
            this.Ascent = ascent;
            this.LineSpacing = lineSpacing;
            this.LineHeight = lineHeight;
        }
    }
}
