using OpenIot.BoardSetup.Oui.Common;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal class OuiElementCollection : OuiElement
    {
        private bool passDownEvents;

        public List<IOuiElement> Elements { get; set; }

        public OuiElementCollection(List<IOuiElement> elements = null, bool passDownEvents = true)
            : base()
        {
            this.Elements = elements != null ? elements : new List<IOuiElement>();

            this.passDownEvents = passDownEvents;
        }

        public IOuiElement AddElement(IOuiElement ouiObject)
        {
            this.Elements.Add(ouiObject);
            return ouiObject;
        }

        // TODO: Inefficient
        public override void OnHovered(PointF coords)
        {
            base.OnHovered(coords);

            if (this.passDownEvents)
            {
                foreach (IOuiElement item in this.Elements)
                {
                    if (item.Visible)
                    {
                        item.OnHovered(coords);
                        if (item.Intersect(coords))
                            item.State = OuiElementState.Hover;
                        else
                            item.State = OuiElementState.Default;
                    }
                }
            }
        }

        // TODO: Inefficient
        public override void OnClicked(PointF coords)
        {
            base.OnClicked(coords);

            if (this.passDownEvents)
            {
                foreach (IOuiElement item in this.Elements)
                {
                    if (item.Visible)
                        if (item.Intersect(coords))
                            item.OnClicked(coords);
                }
            }
        }

        public override bool Intersect(PointF coords)
        {
            return this.Elements.Any(i => i.Intersect(coords));
        }

        public override void Render(IOuiRendering renderer)
        {
            foreach (IOuiElement item in this.Elements)
                if (item.Visible)
                    item.Render(renderer);
        }
    }
}
