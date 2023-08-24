using OpenIot.BoardSetup.Oui.Common;
using System.Drawing;

namespace OpenIot.BoardSetup.Oui
{
    internal interface IOuiElement
    {
        OuiElementState State { get; set; }

        bool Visible { get; set; }

        void AddToFlow(int flow);
        void RemoveFromFlow(int flow);
        bool BelongsToFlow(int flow);

        void Render(IOuiRendering renderer);

        bool Intersect(PointF coords);

        void OnClicked(PointF coords);

        void OnHovered(PointF coords);
    }
}
