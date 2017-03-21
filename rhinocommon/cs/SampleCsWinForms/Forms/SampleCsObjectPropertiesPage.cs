using System.Drawing;
using System.Windows.Forms;
using Rhino.DocObjects;
using Rhino.UI;

namespace SampleCsWinForms.Forms
{
  class SampleCsObjectPropertiesPage : ObjectPropertiesPage
  {
    private SampleCsObjectPropertiesUserControl m_control;

    public override Icon Icon
    {
      get { return SampleCsWinForms.Properties.Resources.Property; }
    }

    public override Control PageControl
    {
      get { return m_control ?? (m_control = new SampleCsObjectPropertiesUserControl()); }
    }

    public override string EnglishPageTitle
    {
      get { return "Sample"; }
    }

    public override bool ShouldDisplay(RhinoObject rhinoObject)
    {
      var rc = false;
      // One object selected
      if (null != rhinoObject)
      {
        rc = true;
      }
      else
      {
        // Multiple objects selected
        foreach (var rh_obj in SelectedObjects)
        {
          rc = true;
          break;
        }
      }
      return rc;
    }

    public override void InitializeControls(RhinoObject rhObj)
    {
      // TODO...
    }
  }
}
