using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsMouseCallback
{
	public class SampleCsMouseCallbackCommand : Command
	{
		readonly SampleMouseCallback smcb = new SampleMouseCallback();
		public SampleCsMouseCallbackCommand()
		{
			// Rhino only creates one instance of each command class defined in a
			// plug-in, so it is safe to store a refence in a static property.
			Instance = this;
		}

		///<summary>The only instance of this command.</summary>
		public static SampleCsMouseCallbackCommand Instance
		{
			get; private set;
		}

		///<returns>The command name as it appears on the Rhino command line.</returns>
		public override string EnglishName
		{
			get { return "SampleCsMouseCallbackCommand"; }
		}

		protected override Result RunCommand(RhinoDoc doc, RunMode mode)
		{
			smcb.Enabled = !smcb.Enabled;
			return Result.Success;
		}
	}
}
