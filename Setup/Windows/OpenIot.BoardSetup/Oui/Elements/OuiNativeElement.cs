using OpenIot.BoardSetup.Oui.Resources;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal abstract class OuiNativeElement : OuiElement
    {
        internal Control parent;

        public OuiNativeElement(Control parent)
            : base()
        {
            this.parent = parent;
        }

        internal bool rendered;

        public abstract void Show();
        public abstract void Hide();
    }
}
