using System;
using Rhino;
using Rhino.Commands;

namespace MockingBirdIntro
{
	///<summary>
	/// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
	/// class. DO NOT create instances of this class yourself. It is the
	/// responsibility of Rhino to create an instance of this class.</para>
	/// <para>To complete plug-in information, please also see all PlugInDescription
	/// attributes in AssemblyInfo.cs (you might need to click "Project" ->
	/// "Show All Files" to see it in the "Solution Explorer" window).</para>
	///</summary>
	public class MockingBirdIntroPlugIn : Rhino.PlugIns.RenderPlugIn

	{
		public MockingBirdIntroPlugIn()
		{
			if(Instance == null) Instance = this;
		}

		///<summary>Gets the only instance of the MockingBirdIntroPlugIn plug-in.</summary>
		public static MockingBirdIntroPlugIn Instance
		{
			get; private set;
		}

		/// <summary>
		/// Is called when the user calls the _Render command.
		/// </summary>
		/// <param name="doc">The document to be rendered.</param>
		/// <param name="mode">The run mode: interactive or scripted.</param>
		/// <param name="fastPreview">Whether the render is in preview-mode.</param>
		/// <returns>The result of the command.</returns>
		protected override Rhino.Commands.Result Render(RhinoDoc doc, Rhino.Commands.RunMode mode, bool fastPreview)
		{
			return Result.Success;
		}

		/// <summary>
		/// Is called when the user calls the _RenderWindow command.
		/// </summary>
		/// <param name="doc">The document to be rendered.</param>
		/// <param name="mode">The run mode: interactive or scripted.</param>
		/// <param name="fastPreview">Whether the render is in preview-mode.</param>
		/// <param name="view">The view being rendered.</param>
		/// <param name="rect">The rendering rectangle.</param>
		/// <param name="inWindow">Whether rendering should appear in the window.</param>
		/// <returns>The result of the command.</returns>
		protected override Rhino.Commands.Result RenderWindow(RhinoDoc doc, Rhino.Commands.RunMode mode, bool fastPreview, Rhino.Display.RhinoView view, System.Drawing.Rectangle rect, bool inWindow)
		{
			return Result.Success;
		}

		// You can override methods here to change the plug-in behavior on
		// loading and shut down, add options pages to the Rhino _Option command
		// and mantain plug-in wide options in a document.
	}
}