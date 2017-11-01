using System.Windows.Forms;
using Rhino.Render;
using Rhino.Render.UI;
using Rhino.UI;

namespace rdktest_cs
{
  // TODO: You must provide a unique Guid for this class
  [System.Runtime.InteropServices.Guid("FDBFCDD3-ED4C-4490-B26E-3879A3E72BC8")]
  public partial class CustomMaterialUserInterfaceSection2 : RhinoWindows.Controls.RenderContentUserControlCollapsibleSection
  {
    public CustomMaterialUserInterfaceSection2()
    {
      InitializeComponent();

      m_view_model = new CustomMaterialViewModel(this);
    }
     

    public override bool Hidden
    {
      get
      {
        bool? bShow = m_view_model.ShowSection2;
        if (null == bShow)
          return true;

        return !bShow.Value;
      }
    }

    public override LocalizeStringPair Caption
    {
      get
      {
        return new LocalizeStringPair("Section2", "Section2");
      }
    }

    public override int SectionHeight
    {
      get
      {
        return 100;
      }
    }

    CustomMaterialViewModel m_view_model;
  }
}
