using Rhino;
using Rhino.Commands;

namespace PluginCommunicationOne
{
	public class PluginCommunicationOneCommand : Command
	{
		public PluginCommunicationOneCommand()
		{
			Instance = this;
		}

		public static PluginCommunicationOneCommand Instance
		{
			get; private set;
		}

		public override string EnglishName
		{
			get { return "PluginCommunicationOneCommand"; }
		}

		protected override Result RunCommand(RhinoDoc doc, RunMode mode)
		{
			var pi = PlugIn;
			var i = pi.Settings.GetInteger("SomeSetting");
			pi.Settings.SetInteger("SomeSetting", ++i);
			RhinoApp.WriteLine($"SomeSetting is now {pi.Settings.GetInteger("SomeSetting", -2)}");
			pi.SaveSettings();

			return Result.Success;
		}
	}
}
