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
using RhinoWindows.ViewModels;
using RhinoWindows.Controls;

namespace rdktest_cs
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

	public class CustomEtoCyclesUISection : Eto.Forms.Panel
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
	}

	[Guid("B8AF5769-E57B-4760-8F54-F9187DF3A6EA")]
	public class CustomEtoCyclesUiSectionHost : EtoElementHost, IUserInterfaceSection
	{
		public CustomEtoCyclesUiSectionHost()
			: base(new CustomEtoCyclesUISection(), null)
		{
		}
		public void UserInterfaceDisplayData(UserInterfaceSection userInterfaceSection, RenderContent[] renderContentList)
		{
			RhinoApp.WriteLine("in CustomEtoCyclesUiSectionHost.UserInterfaceDisplayData");
			foreach (var tm in renderContentList.OfType<CustomEtoUiSectionMaterial>())
			{
				Color4f color;
				tm.Fields.TryGetValue("diffuse_color", out color);

				var ui_section = Control as CustomEtoCyclesUISection;
				if (ui_section != null)
				{
					var co = ui_section.DataContext as ColorObject;
					if (co != null)
					{
						co.ColorAsString = String.Format("{0} {1} {2}", color.R, color.B, color.G);
					}
				}
			}
		}
        bool IUserInterfaceSection.Hidden { get { return false;} }
		public void OnUserInterfaceSectionExpanding(UserInterfaceSection userInterfaceSection, bool expanding)
		{
		}
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

		protected override void OnAddUserInterfaceSections()
		{
			AddAutomaticUserInterfaceSection("Parameters", 0);
			var type = typeof(CustomEtoCyclesUiSectionHost);
			AddUserInterfaceSection(type, "(Eto) Custom Material", true, true);
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
