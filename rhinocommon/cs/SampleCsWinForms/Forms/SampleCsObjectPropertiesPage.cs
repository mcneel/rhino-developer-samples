using System.Drawing;
using System.Windows.Forms;
using Rhino.DocObjects;
using Rhino.UI;

namespace SampleCsWinForms.Forms
{
  class SampleCsObjectPropertiesPage : ObjectPropertiesPage
  {
    private SampleCsObjectPropertiesUserControl m_control;

    public override System.Drawing.Icon PageIcon(System.Drawing.Size sizeInPixels)
    {
      var icon = Rhino.UI.DrawingUtilities.LoadIconWithScaleDown(
        "SampleCsWinForms.Resources.Property.ico",
        sizeInPixels.Width,
        GetType().Assembly);
      return icon;
    }

    public override object PageControl => m_control ?? (m_control = new SampleCsObjectPropertiesUserControl());

    public override string EnglishPageTitle => "Sample";

    public override bool ShouldDisplay(ObjectPropertiesPageEventArgs e)
    {
      var rc = false;
      // One object selected
      if (1 == e.ObjectCount)
      {
        rc = true;
      }
      else
      {
        // Multiple objects selected
        foreach (var rh_obj in e.Objects)
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
