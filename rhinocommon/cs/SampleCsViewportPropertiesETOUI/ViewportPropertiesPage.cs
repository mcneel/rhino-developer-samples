using System;
using Rhino;
using Rhino.Commands;
using Rhino.UI.Controls;
using Eto.Forms;
using Rhino.UI;
using System.Drawing;
using Rhino.Display;
using Rhino.DocObjects;

namespace SampleViewportPropertiesETOUI
{
    public class ViewportPropertiesPage : ObjectPropertiesPage
    {

        public ViewportPropertiesPage()
        {

        }

        public override string EnglishPageTitle => "Viewport";

        // If no icon then the Title will be displayed in the Tab
        public override System.Drawing.Icon PageIcon(Size size)
        {
            return null;
        }

        public override PropertyPageType PageType => PropertyPageType.View;

        public sealed override object PageControl
        {
            get
            {
                if (CollapsibleSectionHolder == null)
                {
                    CollapsibleSectionHolder = NewPanel();
                }
                return CollapsibleSectionHolder;
            }
        }

        public override bool ShouldDisplay(ObjectPropertiesPageEventArgs e)
        {
            if (e.View == null)
            {
                if (CollapsibleSectionHolder == null)
                    return false;

                // Update page here and tell it no view
                // is selected.

                return false;
            }

            return true;
        }

        public override void UpdatePage(ObjectPropertiesPageEventArgs e)
        {
            if (CollapsibleSectionHolder == null)
                return;

            ViewInfo vi = new ViewInfo(e.Viewport);

            if (vi != null)
            {
                // Pass the view info to the page
            }
            else
            {
                // Tell the ui that there is no view.
            }
        }

        private uint[] m_selected_serial_numbers = { };
        private CollapsibleSectionUIPanel m_page;
        private CollapsibleSectionUIPanel CollapsibleSectionHolder { get { return m_page; } set { m_page = value; } }

        private CollapsibleSectionUIPanel NewPanel() => new CollapsibleSectionUIPanel();
    }
}

