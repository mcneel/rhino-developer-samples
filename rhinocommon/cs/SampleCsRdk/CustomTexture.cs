using System;
using System.Collections.Generic;
using Rhino.Render;

namespace SampleCsRdk
{
  [System.Runtime.InteropServices.Guid("57BF48A5-C656-4996-920D-41B5BD63C57A")]
  public class CustomTexture : TwoColorRenderTexture
  {
    public override string TypeName { get { return "C# Custom Texture"; } }
    public override string TypeDescription { get { return "My first custom .NET Texture"; } }

    //StringField m_script = new StringField("script", "Script", "");
    //BoolField m_bool = new BoolField("bool", "Yes/No", false);
    //IntField m_int = new IntField("int", "Integer", 4);
    //DoubleField m_double = new DoubleField("double", "Double", 2.0);
    //ColorField m_color = new ColorField("color", "Color", Rhino.Display.Color4f.Black);

    public CustomTexture()
    {
      Fields.Add("script", string.Empty, "Script");
      Fields.Add("bool", false, "Yes/No");
      Fields.Add("int", 4, "Integer");
      Fields.Add("double", 2.0, "Double");
      Fields.Add("color", Rhino.Display.Color4f.Black, "Color");

      //AddAutomaticUiField(m_script);
      //AddAutomaticUiField(m_bool);
      //AddAutomaticUiField(m_int);
      //AddAutomaticUiField(m_double);
      //AddAutomaticUiField(m_color);
    }

    public override TextureEvaluator CreateEvaluator(TextureEvaluatorFlags evaluatorFlags)
    {
        return new CustomTextureEvaluator(this, evaluatorFlags);
    }

    //Used for testing only
    //public override void SimulateTexture(SimulatedTexture sim, bool bForDataOnly)
    //{
    //  sim.Filename = "C:\\Users\\Omistaja\\Desktop\\_Images\\AtoZ.bmp";
      //base.SimulateTexture(sim, bForDataOnly);
    //}

      protected override void AddAdditionalUISections()
      {
        AddAutomaticUserInterfaceSection("Parameters", 0);
          //AddAutomaticUISection("Parameters", 0);
      }
  }

  class CustomTextureEvaluator : TextureEvaluator
  {
      public CustomTextureEvaluator(CustomTexture tex, RenderTexture.TextureEvaluatorFlags evaluatorFlags)
          : base(evaluatorFlags)
      {
          m_color1 = new Rhino.Display.Color4f(tex.Color1);
          m_color2 = new Rhino.Display.Color4f(tex.Color2);
          m_mapping = tex.LocalMappingTransform;        //TODO - evaluator flags needs to be checked for efDisableLocalMapping
          m_bSwapColors = tex.SwapColors;

          m_bTexture1On = tex.Texture1On && tex.Texture1Amount > 0.0;
          m_bTexture2On = tex.Texture2On && tex.Texture2Amount > 0.0;

          m_dTexture1Amount = tex.Texture1Amount;
          m_dTexture2Amount = tex.Texture2Amount;

          if (m_bTexture1On)
          {
              RenderTexture child = tex.FindChild(tex.ChildSlotNameFromParamName("color-one")) as RenderTexture;
              if (child != null)
              {
                  textureEvaluatorOne = child.CreateEvaluator(evaluatorFlags);
              }
          }

          if (m_bTexture2On)
          {
              RenderTexture child = tex.FindChild(tex.ChildSlotNameFromParamName("color-two")) as RenderTexture;
              if (child != null)
              {
                  textureEvaluatorTwo = child.CreateEvaluator(evaluatorFlags);
              }
          }
      }

      private Rhino.Display.Color4f m_color1;
      private Rhino.Display.Color4f m_color2;
      private Rhino.Geometry.Transform m_mapping;
      private bool m_bSwapColors;
      private TextureEvaluator textureEvaluatorOne;
      private TextureEvaluator textureEvaluatorTwo;
      private bool m_bTexture1On;
      private bool m_bTexture2On;
      private double m_dTexture1Amount;
      private double m_dTexture2Amount;


    static Rhino.Runtime.PythonScript m_python_script;
    static Rhino.Runtime.PythonScript Script
    {
      get
      {
        return m_python_script ?? (m_python_script = Rhino.Runtime.PythonScript.Create());
      }
    }

    public override Rhino.Display.Color4f GetColor(Rhino.Geometry.Point3d _uvw, Rhino.Geometry.Vector3d duvwdx, Rhino.Geometry.Vector3d duvwdy)
    {
      //Apply the mapping transform to get tiling and offset to work
      Rhino.Geometry.Point3d uvw = new Rhino.Geometry.Point3d(m_mapping * _uvw);

      double d = uvw.X * 20;
      int i = (int)d;

      bool useColorOne = false;
      if (i % 2 == 0)
      {
          useColorOne = true;
      }
      else
      {
          d = uvw.Y * 20;
          i = (int)d;
          if (i % 2 == 0)
              useColorOne = true;
      }

      if (m_bSwapColors)
          useColorOne = !useColorOne;

      bool textureOn = useColorOne ? m_bTexture1On : m_bTexture2On;
      double textureAmount = useColorOne ? m_dTexture1Amount : m_dTexture2Amount;
        TextureEvaluator texEval = useColorOne ? textureEvaluatorOne : textureEvaluatorTwo;

      Rhino.Display.Color4f color = useColorOne ? m_color1 : m_color2;

      if (textureOn && texEval != null)
      {
          //Ensure that the original UVW is passed into the child texture evaluator
          color = color.BlendTo((float)textureAmount, texEval.GetColor(_uvw, duvwdx, duvwdy));
      }

      return new Rhino.Display.Color4f(color);
    }
  }
}
