using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;

namespace MockingBirdIntro
{

	[CommandStyle(Style.ScriptRunner)]
	public class MockingBirdIntroCommand : Command
	{
		public MockingBirdIntroCommand()
		{
			// Rhino only creates one instance of each command class defined in a
			// plug-in, so it is safe to store a refence in a static property.
			Instance = this;
		}

		///<summary>The only instance of this command.</summary>
		public static MockingBirdIntroCommand Instance
		{
			get; private set;
		}

		///<returns>The command name as it appears on the Rhino command line.</returns>
		public override string EnglishName
		{
			get { return "ActivateMockingBirdIntro"; }
		}

		protected override Result RunCommand(RhinoDoc doc, RunMode mode)
		{
			RhinoApp.RunScript("-_SetCurrentRenderPlugIn \"MockingBird (intro)\"", true);

			return Result.Success;
		}
	}
}
