using OpenIot.BoardSetup.Oui.Common;
using OpenIot.BoardSetup.Oui.Events;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui.Elements
{
    internal abstract class OuiElement : IOuiElement
    {
        private int flows;

        public event EventHandler Clicked;
        public event EventHandler Hovered;

        public virtual OuiElementState State { get; set; }

        public bool Visible { get; set; }
        
        public OuiElement()
        {
            this.Visible = true;
            this.flows = 0;
        }

        public virtual void AddToFlow(int flow)
        {
            this.flows |= flow;
        }

        public virtual void RemoveFromFlow(int flow)
        {
            this.flows &= ~flow;
        }

        public virtual bool BelongsToFlow(int flow)
        {
            return (this.flows & flow) != 0;
        }

        public virtual void OnClicked(PointF coords)
        {
            this.Clicked?.Invoke(this, new OuiElementClickedEventArgs(coords));
        }

        public virtual void OnHovered(PointF coords)
        {
            this.Hovered?.Invoke(this, new OuiElementClickedEventArgs(coords));
        }

        public abstract bool Intersect(PointF coords);

        public abstract void Render(IOuiRendering renderer);
    }
}
