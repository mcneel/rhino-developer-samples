using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Rhino.Render;
using Rhino.Render.Fields;
using System.Drawing;
using System.Drawing.Imaging;

namespace SampleRdkMaterialAutoUI
{
  // The type id for the material
  [System.Runtime.InteropServices.Guid("61F4648D-C724-495C-8434-2004F86304E7")]
  public class SampleRdkMaterial : RenderMaterial
  {
    public override string TypeName { get { return "CSharpSampleAutomaticUIMaterial"; } }
    public override string TypeDescription { get { return "My first Sample Automatic UI Material"; } }

    public SampleRdkMaterial()
    {
      InitializeFields();
    }

    protected override void OnAddUserInterfaceSections()
    {
      // Add automatic UI section with Parameters title
      AddAutomaticUserInterfaceSection("Parameters", 0);
    }

    public override void SimulateMaterial(ref Rhino.DocObjects.Material simulatedMaterial, bool forDataOnly)
    {
      base.SimulateMaterial(ref simulatedMaterial, forDataOnly);

      Rhino.Display.Color4f color;
      if (Fields.TryGetValue("color", out color))
        simulatedMaterial.DiffuseColor = color.AsSystemColor();
    }

    public override bool Icon(System.Drawing.Size size, out System.Drawing.Bitmap bitmap)
    {
      // The icon is a red square
      bitmap = new Bitmap(size.Width, size.Height, PixelFormat.Format24bppRgb);
      using (var g = Graphics.FromImage(bitmap))
        g.Clear(Color.Red);
      return true;
    }

    private void InitializeFields()
    {
      // Visible fields in the section
      Fields.Add("bool", false, "Yes/No");
      Fields.AddTextured("color", Rhino.Display.Color4f.White, "Color");

      //Non visible fields to store texture on-ness and amount
      Field field = Fields.Add("color-texture-on", true);
      BindParameterToField("color", "texture-on", field, ChangeContexts.UI);

      field = Fields.Add("color-texture-amount", 100.0);
      BindParameterToField("color", "texture-amount", field, ChangeContexts.UI);
    }

  }
}
