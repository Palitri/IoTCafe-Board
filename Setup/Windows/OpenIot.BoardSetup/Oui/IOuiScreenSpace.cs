using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Oui
{
    interface IOuiScreenSpace
    {
        public float GetScreenWidth(float x);
        public float GetScreenHeight(float y);
        public PointF GetScreenCoords(float x, float y);
        public PointF GetScreenCoords(PointF p);

        public float GetUnitWidth(float x);
        public float GetUnitHeight(float y);
        public PointF GetUnitCoords(float x, float y);
        public PointF GetUnitCoords(PointF p);
    }
}
