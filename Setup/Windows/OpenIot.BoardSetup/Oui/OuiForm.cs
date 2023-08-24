using OpenIot.BoardSetup.Oui.Common;
using OpenIot.BoardSetup.Oui.Elements;
using OpenIot.BoardSetup.Oui.Events;
using OpenIot.BoardSetup.Oui.Resources;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace OpenIot.BoardSetup.Oui
{
    internal class OuiForm : IOuiScreenSpace, IOuiRendering
    {
        public event EventHandler Invalidated;

        public PointF PixelSize { get; set; }
        public System.Drawing.Graphics graphics;

        public List<IOuiResource> Resources { get; set; }
        public List<IOuiElement> Elements { get; set; }

        private List<OuiNativeElement> nativeElements = new List<OuiNativeElement>();

        private PointF cursor;
        private bool cursorDown;

        public OuiForm(System.Drawing.Graphics graphics, PointF size)
        {
            this.graphics = graphics;
            this.PixelSize = size;
            this.Resources = new List<IOuiResource>();
            this.Elements = new List<IOuiElement>();

            this.cursor = new PointF(0.5f, 0.5f);
            this.cursorDown = false;
        }

        public IOuiResource AddResource(IOuiResource resource)
        {
            this.Resources.Add(resource);
            return resource;
        }

        public IOuiElement AddElement(IOuiElement ouiObject)
        {
            this.Elements.Add(ouiObject);
            return ouiObject;
        }

        public void SetSize(float width, float height)
        {
            if ((width == this.PixelSize.X) && (height == this.PixelSize.Y))
                return;

            this.PixelSize = new PointF(width, height);

            foreach (IOuiResource resource in this.Resources)
            {
                resource.Resize(this);
            }
        }

        public void Render()
        {
            foreach (IOuiElement item in this.Elements)
                if (item.Visible)
                    item.Render(this);

            this.ProcessNativeElements(this.Elements);
        }

        private volatile bool processingNativeElements;
        private void ProcessNativeElements(List<IOuiElement> elements)
        {
            if (this.processingNativeElements)
                return;

            this.processingNativeElements = true;

            this.TraceNativeElements(elements);

            for (int i = 0; i < this.nativeElements.Count; i++)
            {
                OuiNativeElement item = this.nativeElements[i];
                if (!item.rendered)
                {
                    item.Hide();
                    this.nativeElements.RemoveAt(i);
                    i--;
                }

                item.rendered = false;
            }

            this.processingNativeElements = false;
        }

        private void TraceNativeElements(List<IOuiElement> elements)
        {
            foreach (IOuiElement item in elements)
            {
                if (item is OuiElementCollection)
                    this.TraceNativeElements((item as OuiElementCollection).Elements);
                else if (item is OuiNativeElement)
                {
                    OuiNativeElement nativeItem = item as OuiNativeElement;
                    nativeItem.rendered = true;
                    if (!this.nativeElements.Contains(nativeItem))
                    {
                        this.nativeElements.Add(nativeItem);
                        nativeItem.Show();
                    }
                }
            }
        }

        public void SetCursorScreenPosition(float x, float y)
        {
            this.cursor = this.GetUnitCoords(x, y);

            foreach (IOuiElement item in this.Elements)
            {
                if (item.Visible)
                {
                    item.OnHovered(this.cursor);
                    if (item.Intersect(this.cursor))
                        item.State = OuiElementState.Hover;
                    else
                        item.State = OuiElementState.Default;
                }
            }
        }

        public void SetCursorState(bool down)
        {
            this.cursorDown = down;

            if (down)
            {
                IOuiElement clickedElement = this.GetElementAt(this.cursor);
                if (clickedElement != null)
                {
                    clickedElement.OnClicked(this.cursor);
                }
            }
        }

        public IOuiElement GetElementAt(PointF coords)
        {
            return this.Elements.LastOrDefault(i => i.Intersect(coords));
        }

        public virtual void OnInvalidated()
        {
            this.Invalidated?.Invoke(this, new EventArgs());
        }

        #region IOuiRendering

        public void Draw(OuiBitmap bitmap, PointF position, PointF size)
        {
            PointF screenSize = this.GetScreenCoords(size);
            PointF screenPos = this.GetScreenCoords(position);
            screenPos.X -= screenSize.X / 2;
            screenPos.Y -= screenSize.Y / 2;
            this.graphics.DrawImage(bitmap.bitmap, screenPos.X, screenPos.Y, screenSize.X, screenSize.Y);
        }

        public void Draw(OuiFont font, string text, PointF position)
        {
            PointF screenPos = this.GetScreenCoords(new PointF(position.X, position.Y - font.Metrics.Baseline));

            this.graphics.DrawString(text, font.font, font.brush, screenPos);
        }

        #endregion

        #region IScreenSpace

        public float GetScreenWidth(float x)
        {
            return this.PixelSize.X * x;
        }

        public float GetScreenHeight(float y)
        {
            return this.PixelSize.Y * y;
        }

        public PointF GetScreenCoords(float x, float y)
        {
            return new PointF(this.GetScreenWidth(x), this.GetScreenHeight(y));
        }

        public PointF GetScreenCoords(PointF p)
        {
            return new PointF(this.GetScreenWidth(p.X), this.GetScreenHeight(p.Y));
        }

        public float GetUnitWidth(float x)
        {
            return x / this.PixelSize.X;
        }

        public float GetUnitHeight(float y)
        {
            return y / this.PixelSize.Y;
        }

        public PointF GetUnitCoords(float x, float y)
        {
            return new PointF(this.GetUnitWidth(x), this.GetUnitHeight(y));
        }

        public PointF GetUnitCoords(PointF p)
        {
            return new PointF(this.GetUnitWidth(p.X), this.GetUnitHeight(p.Y));
        }

        #endregion

    }
}
