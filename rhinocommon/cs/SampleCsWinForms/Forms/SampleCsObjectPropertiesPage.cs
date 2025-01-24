using Rhino.DocObjects;
using Rhino.UI;
using System.Drawing;

namespace SampleCsWinForms.Forms
{
  class SampleCsObjectPropertiesPage : ObjectPropertiesPage
  {
    private SampleCsObjectPropertiesUserControl m_control;

    public override System.Drawing.Icon PageIcon(System.Drawing.Size sizeInPixels)
    {
      Icon icon = Rhino.UI.DrawingUtilities.LoadIconWithScaleDown(
        "SampleCsWinForms.Resources.Property.ico",
        sizeInPixels.Width,
        GetType().Assembly);
      return icon;
    }

    public override object PageControl => m_control ?? (m_control = new SampleCsObjectPropertiesUserControl());

    public override string EnglishPageTitle => "Sample";

    public override bool ShouldDisplay(ObjectPropertiesPageEventArgs e)
    {
      bool rc = false;
      // One object selected
      if (1 == e.ObjectCount)
      {
        rc = true;
      }
      else
      {
        // Multiple objects selected
        foreach (RhinoObject rh_obj in e.Objects)
        {
          rc = true;
          break;
        }
      }
      return rc;
    }

    public override void UpdatePage(ObjectPropertiesPageEventArgs e)
    {
      // TODO...
    }
  }
}
