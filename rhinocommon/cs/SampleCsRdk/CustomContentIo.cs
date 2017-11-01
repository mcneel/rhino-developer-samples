using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Rhino.Render;
/*
namespace rdktest_cs
{
  [CustomRenderContentIo("myext", RenderContentKind.Material, true, false)]
  public class CustomContentIo : IOPlugIn
  {
    public override string EnglishDescription
    {
      get { return "CustomCSharpIo"; }
    }

    public override RenderContent Load(String pathToFile)
    {
      RenderContent content = new CustomMaterial();
      var color = new Rhino.Display.Color4f(System.Drawing.Color.IndianRed);
      content.Fields.Set("color", color, RenderContent.ChangeContexts.Ignore);
      return content;
    }

    public override bool Write(String pathToFile, RenderContent rc, CreatePreviewEventArgs ss)
    {
      return false;
    }

  }
}
*/