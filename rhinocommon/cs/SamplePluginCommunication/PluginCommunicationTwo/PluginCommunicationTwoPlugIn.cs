using Rhino.PlugIns;

namespace PluginCommunicationTwo
{
	public class PluginCommunicationTwoPlugIn : PlugIn

	{
		public PluginCommunicationTwoPlugIn()
		{
			Instance = this;
		}
		public static PluginCommunicationTwoPlugIn Instance
		{
			get; private set;
		}
		public override PlugInLoadTime LoadTime => PlugInLoadTime.AtStartup;

		protected override LoadReturnCode OnLoad(ref string errorMessage)
		{
			var pi = Find(new System.Guid("8abebc35-fca5-4e4f-a096-4b0f178d1c7e"));
			if(pi!=null)
			{
				pi.SettingsSaved += Pi_SettingsSaved;
			}
			return LoadReturnCode.Success;
		}

		private void Pi_SettingsSaved(object sender, Rhino.PersistentSettingsSavedEventArgs e)
		{
			Rhino.RhinoApp.WriteLine("Settings in PluginCommunicationOne saved");
			ReadSetting();
		}

		public void ReadSetting()
		{
			var pi = Find(new System.Guid("8abebc35-fca5-4e4f-a096-4b0f178d1c7e"));
			if (pi!=null)
			{
				var i = pi.Settings.GetInteger("SomeSetting", -1);
				Rhino.RhinoApp.WriteLine($"Setting from PluginCommunicationOne > {i}");
			}
		}
	}
}