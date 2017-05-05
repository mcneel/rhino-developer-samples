using Rhino;
using Rhino.Commands;

namespace PluginCommunicationTwo
{
	public class PluginCommunicationTwoCommand : Command
	{
		public PluginCommunicationTwoCommand()
		{
			Instance = this;
		}

		public static PluginCommunicationTwoCommand Instance
		{
			get; private set;
		}

		public override string EnglishName
		{
			get { return "PluginCommunicationTwoCommand"; }
		}
		protected override Result RunCommand(RhinoDoc doc, RunMode mode)
		{
			(PlugIn as PluginCommunicationTwoPlugIn).ReadSetting();
			return Result.Success;
		}
	}
}
