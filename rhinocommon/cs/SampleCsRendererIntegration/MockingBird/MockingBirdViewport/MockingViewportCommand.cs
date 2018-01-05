using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace MockingBirdViewport
{
	public class MockingViewportCommand : Command
	{
		public MockingViewportCommand()
		{
			// Rhino only creates one instance of each command class defined in a
			// plug-in, so it is safe to store a refence in a static property.
			Instance = this;
		}

		///<summary>The only instance of this command.</summary>
		public static MockingViewportCommand Instance
		{
			get; private set;
		}

		///<returns>The command name as it appears on the Rhino command line.</returns>
		public override string EnglishName
		{
			get { return "ActivateMockingViewport"; }
		}

		protected override Result RunCommand(RhinoDoc doc, RunMode mode)
		{
			RhinoApp.RunScript("-_SetCurrentRenderPlugIn \"MockingBird (viewport)\"", true);


			return Result.Success;
		}
	}
}
