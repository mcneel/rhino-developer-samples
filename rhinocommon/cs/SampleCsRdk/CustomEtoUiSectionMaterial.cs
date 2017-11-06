using System.ComponentModel;
using System.Globalization;
using System.Runtime.InteropServices;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Eto.Drawing;
using Eto.Forms;
using Rhino;
using Rhino.Display;
using Rhino.Render;
using Rhino.Render.UI;
using Rhino.UI.Controls;
using RhinoWindows.ViewModels;
using RhinoWindows.Controls;
using Rhino.UI;

namespace SampleCsRdk
{
	internal class ColorObject : ViewModelBase
	{
		public ColorObject()
		{
			NumberFormatInfo = CultureInfo.InvariantCulture.NumberFormat;
		}

		private Color4f color;
		public NumberFormatInfo NumberFormatInfo { get; set; }

		public Color4f Color
		{
			get { return color; }
			set { color = value; }
		}

		public string ColorAsString
		{
			get { return String.Format("{0} {1} {2}", color.R, color.B, color.G); }
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
						color = new Color4f(realfloats[0], realfloats[1], realfloats[2], 1.0f);
						RaisePropertyChanged("ColorAsString");
					}
				}
				catch (Exception e)
				{
					// just put it out there so we know there was something amiss.
					// not really the point here. What I find is that backspaces in the
					// text box get into this setter, but other textual input doesnt.
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
				Spacing = new Size(5,5),
				Padding = new Padding(10, 10, 10, 10),
				Rows = {
					new TableRow(new Label {Text = "Teh Color:"}, DataContextBinding()),
					null
				}
			};

			var cob = new ColorObject {ColorAsString = "1.0 2.0 3.0"};

			cob.PropertyChanged += CustomEtoCyclesUISection_PropertyChanged;

			DataContext = cob;

		}

		void CustomEtoCyclesUISection_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			var cob = DataContext as ColorObject;
			if (cob != null)
			{
				var c = cob.Color;
				RhinoApp.WriteLine(String.Format("Custom Eto Section property changed: {0} {1},{2},{3}", e.PropertyName, c.R, c.G, c.B));
			}
		}

		private TextBox DataContextBinding()
		{
			var label = new TextBox ();
			label.Enabled = true;
			label.TextBinding.BindDataContext<ColorObject>(r => r.ColorAsString);

			return label;
		}
		public override LocalizeStringPair Caption => new LocalizeStringPair("Test Eto Ui Section", "Test Eto Ui Section");

		public override int SectionHeight => 200;
	}


	[Guid("2FEE12AE-1D16-4338-9D28-0BBD6EFE763D")]
	public class CustomEtoUiSectionMaterial : RenderMaterial
	{
		public override string TypeName { get { return "Test Eto Section Material"; } }
		public override string TypeDescription { get { return "Test Eto Section Material"; } }

		public CustomEtoUiSectionMaterial()
		{
			Fields.Add("diffuse_color", Color4f.White, "Diffuse Color");
		}

		private CustomEtoCyclesUISection m_sect = new CustomEtoCyclesUISection();

		protected override void OnAddUserInterfaceSections()
		{
			AddAutomaticUserInterfaceSection("Parameters", 0);
			AddUserInterfaceSection(m_sect);
		}

		public override void SimulateMaterial(ref Rhino.DocObjects.Material simulatedMaterial, bool forDataOnly)
		{
			base.SimulateMaterial(ref simulatedMaterial, forDataOnly);

			Color4f color;
			if (Fields.TryGetValue("diffuse_color", out color))
				simulatedMaterial.DiffuseColor = color.AsSystemColor();
		}
	}
}
