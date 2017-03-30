using System;
using Rhino;
using Rhino.Commands;
using Rhino.UI.Controls;
using Eto.Forms;
using Rhino.UI;
using System.Drawing;

namespace SampleViewportPropertiesETOUI
{
  public class ViewportPropertiesPage : ObjectPropertiesPage
  {
    private CollapsibleSectionUIPanel m_sections;
    private uint[] m_selected_serial_numbers = { };

    public ViewportPropertiesPage()
    {
      m_sections = new CollapsibleSectionUIPanel();
    }

    public override string EnglishPageTitle => "Viewport";

    // If no icon then the Title will be displayed in the Tab
    public override System.Drawing.Icon Icon => null;

    public override PropertyPageType PageType => PropertyPageType.View;

    public sealed override object PageControl => CollapsibleSectionHolder;

    CollapsibleSectionUIPanel CollapsibleSectionHolder { get { return m_sections; } }


  }
}
