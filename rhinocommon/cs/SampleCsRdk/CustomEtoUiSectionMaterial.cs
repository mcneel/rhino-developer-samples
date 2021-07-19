using System.ComponentModel;
using System.Globalization;
using System.Runtime.InteropServices;
using System;
using System.Collections.Generic;
using Eto.Drawing;
using Eto.Forms;
using Rhino;
using Rhino.Display;
using Rhino.Render;
using Rhino.UI.Controls;
using RhinoWindows.ViewModels;
using Rhino.UI;

namespace SampleCsRdk
{
  internal class ColorObject : ViewModelBase
  {
    public Color4f Color { get; set; }
    public NumberFormatInfo NumberFormatInfo { get; set; }

    public ColorObject()
    {
      NumberFormatInfo = CultureInfo.InvariantCulture.NumberFormat;
    }

    public string ColorAsString
    {
      get { return String.Format("{0} {1} {2}", Color.R, Color.G, Color.B); }
      set
      {
        try
        {
          var floats = value;
          floats = floats.Trim();
          floats = floats.Replace("  ", " ");
          floats = floats.Replace(",", ".");

          var fs = floats.Split(' ');
          var realfloats = new float[fs.Length];
          for (var i = 0; i < fs.Length; i++)
          {
            realfloats[i] = float.Parse(fs[i], NumberFormatInfo);
          }

          if (realfloats.Length > 2)
          {
            Color = new Color4f(realfloats[0], realfloats[1], realfloats[2], 1.0f);
            RaisePropertyChanged("ColorAsString");
          }
        }
        catch (Exception e)
        {
          // Just put it out there so we know there was something amiss.
          // Not really the point here. What I find is that backspaces in the
          // text box get into this setter, but other textual input doesn't.
          RhinoApp.WriteLine(e.Message);
        }
      }
    }
  }

  [Guid("10E83715-82F1-439B-83B6-1CA1EF8C054E")]
  public class CustomEtoCyclesUISection : EtoCollapsibleSection
  {
    public CustomEtoCyclesUISection()
    {
      Content = new TableLayout
      {
        Spacing = new Size(5, 5),
        Padding = new Padding(10, 10, 10, 10),
        Rows =
        {
          new TableRow(new Label {Text = "The Color:"}, DataContextBinding()),
          null
        }
      };

      var cob = new ColorObject { ColorAsString = "1.0 2.0 3.0" };
      cob.PropertyChanged += CustomEtoCyclesUISection_PropertyChanged;
      DataContext = cob;
    }

    void CustomEtoCyclesUISection_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      if (DataContext is ColorObject cob)
      {
        var c = cob.Color;
        RhinoApp.WriteLine(String.Format("Custom Eto Section property changed: {0} {1},{2},{3}", e.PropertyName, c.R, c.G, c.B));
      }
    }

    private TextBox DataContextBinding()
    {
      var label = new TextBox
      {
        Enabled = true
      };
      label.TextBinding.BindDataContext<ColorObject>(r => r.ColorAsString);

      return label;
    }

    public override LocalizeStringPair Caption => new LocalizeStringPair("Test Eto Ui Section", "Test Eto Ui Section");

    public override int SectionHeight => 200;
  }

  [Guid("2FEE12AE-1D16-4338-9D28-0BBD6EFE763D")]
  public class CustomEtoUiSectionMaterial : RenderMaterial
  {
    public override string TypeName        { get { return "Test Eto Section Material"; } }
    public override string TypeDescription { get { return "Test Eto Section Material"; } }

    public CustomEtoUiSectionMaterial()
    {
      Fields.Add("diffuse_color", Color4f.White, "Diffuse Color");
    }

    static readonly List<ICollapsibleSection> m_sections_to_keep_alive = new List<ICollapsibleSection>();

    protected override void OnAddUserInterfaceSections()
    {
      AddAutomaticUserInterfaceSection("Parameters", 0);

      var s1 = new CustomEtoCyclesUISection();

      //This line and the list will be unnecessary after Rhino 6.9
      m_sections_to_keep_alive.Add(s1);

      AddUserInterfaceSection(s1);
    }

    public override void SimulateMaterial(ref Rhino.DocObjects.Material simulatedMaterial, bool forDataOnly)
    {
      base.SimulateMaterial(ref simulatedMaterial, forDataOnly);

      if (Fields.TryGetValue("diffuse_color", out Color4f color))
      {
        simulatedMaterial.DiffuseColor = color.AsSystemColor();
      }
    }
  }
}
