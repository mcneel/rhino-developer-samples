using System.Drawing;
using System.Drawing.Imaging;
using Rhino.Render;
using Rhino.Render.Fields;
using Rhino.UI.Controls;

namespace SampleCsRdk
{
  // TODO: You must provide a unique Guid for this class
  [System.Runtime.InteropServices.Guid("ABE4059B-9BD7-451C-91B2-67C2F188860A")]
  public class CustomMaterial : RenderMaterial
  {
    public override string TypeName { get { return "CSharp Custom Material"; } }
    public override string TypeDescription { get { return "My first custom .NET material"; } }

    public CustomMaterial()
    {
      //ModifyRenderContentStyles(RenderContentStyles.None, RenderContentStyles.TextureSummary);
      Fields.Add("bool", false, "Yes/No");
      Fields.AddTextured("color", Rhino.Display.Color4f.White, "Color");

      //Non visible fields to store texture on-ness and amount
      Field field = Fields.Add("color-texture-on", true);
      BindParameterToField("color", "texture-on", field, ChangeContexts.UI);
      
      field = Fields.Add("color-texture-amount", 100.0);
      BindParameterToField("color", "texture-amount", field, ChangeContexts.UI);
    }

    protected override void OnAddUserInterfaceSections()
    {
      AddAutomaticUserInterfaceSection("Parameters", 0);
    }

    public override void SimulateMaterial(ref Rhino.DocObjects.Material simulatedMaterial, bool forDataOnly)
    {
      base.SimulateMaterial(ref simulatedMaterial, forDataOnly);

      Rhino.Display.Color4f color;
      if( Fields.TryGetValue("color", out color) )
        simulatedMaterial.DiffuseColor = color.AsSystemColor();
    }

    public override bool Icon(System.Drawing.Size size, out System.Drawing.Bitmap bitmap)
    {
      bitmap = new Bitmap(size.Width, size.Height, PixelFormat.Format24bppRgb);
      using (var g = Graphics.FromImage(bitmap))
        g.Clear(Color.Red);
      return true;
    }

    //public override IntPtr GetShader(Guid renderEngineId, IntPtr privateData)
    //{
    //  if (!IsCompatible(renderEngineId))
    //    return IntPtr.Zero;

    //  return IntPtr.Zero;
    //}


    //BoolField m_bool = new BoolField("bool", "Yes/No", false);
    //ColorField m_color = new ColorField("color", "Color", Rhino.Display.Color4f.White);
  }
  // TODO: You must provide a unique Guid for this class
  [System.Runtime.InteropServices.Guid("5A3CCE11-643B-4E4B-8BF7-CABFEE30B3CB")]
  public class CustomMaterialWithUserControl : CustomMaterial
  {
    public override string TypeName { get { return "CSharp Custom Material With UserControl"; } }
    public override string TypeDescription { get { return "Custom material with custom UserControl"; } }

    public static string ShowSectionKey { get { return "show-section-2"; } }
    public static string TrackBarPositionKey { get { return "trackbar-position"; } }

    public CustomMaterialWithUserControl()
    {
      Fields.Add(ShowSectionKey, true, "Yes/No");
      Fields.Add(TrackBarPositionKey, 0, "position");
    }


    ICollapsibleSection m_wpf = new CustomMaterialWpfUserInterfaceSectionHost();

    ICollapsibleSection m_one = new CustomMaterialUserInterfaceSection();

    ICollapsibleSection m_two = new CustomMaterialUserInterfaceSection2();

    protected override void OnAddUserInterfaceSections()
    {
      AddUserInterfaceSection(m_wpf);

      AddUserInterfaceSection(m_one);

      AddUserInterfaceSection(m_two);
    }
  }
}
