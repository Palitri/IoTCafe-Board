using OpenIot.BoardSetup.Oui.Common;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal class OuiStatesElement : OuiElement
    {
        private OuiForm oui;

        public Dictionary<OuiElementState, OuiElementCollection> Elements { get; set; }

        public override OuiElementState State
        {
            get { return base.State; }

            set
            {
                OuiElementCollection oldStateElement = this.StateElement;

                base.State = value;

                if (this.StateElement != oldStateElement)
                {
                    this.oui.OnInvalidated();
                }
            }
        }

        public OuiElementCollection StateElement { get { return this.Elements.ContainsKey(this.State) ? this.Elements[this.State] : this.Elements[OuiElementState.Default]; } }

        public OuiStatesElement(OuiForm oui, Dictionary<OuiElementState, OuiElementCollection> elements = null)
            : base()
        {
            this.oui = oui;
            this.Elements = elements != null ? elements : new Dictionary<OuiElementState, OuiElementCollection>();
        }

        public override bool Intersect(PointF coords)
        {
            return this.StateElement.Visible && this.StateElement.Intersect(coords);
        }

        public override void Render(IOuiRendering renderer)
        {
            if (this.StateElement.Visible)
                this.StateElement.Render(renderer);
        }
    }
}
