using Rhino.PlugIns;

namespace PluginCommunicationOne
{
	public class PluginCommunicationOnePlugIn : Rhino.PlugIns.PlugIn

	{
		public PluginCommunicationOnePlugIn()
		{
			Instance = this;
			// NOTE: Don't access plug-in settings in the constructor.
			// This will create a faulty settings manager behind the scenes
			// due to the plug-in ID not being correct at this time.
			// It is set only after the constructor has completed properly.
		}
		public override PlugInLoadTime LoadTime => PlugInLoadTime.AtStartup;
		protected override LoadReturnCode OnLoad(ref string errorMessage)
		{
			Settings.SetInteger("SomeSetting", 0);
			return LoadReturnCode.Success;
		}

		public static PluginCommunicationOnePlugIn Instance
		{
			get; private set;
		}
	}
}