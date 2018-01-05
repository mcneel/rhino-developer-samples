using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.PlugIns;
using Rhino.Render;

namespace MockingBirdChangeQueue
{
	///<summary>
	/// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
	/// class. DO NOT create instances of this class yourself. It is the
	/// responsibility of Rhino to create an instance of this class.</para>
	/// <para>To complete plug-in information, please also see all PlugInDescription
	/// attributes in AssemblyInfo.cs (you might need to click "Project" ->
	/// "Show All Files" to see it in the "Solution Explorer" window).</para>
	///</summary>
	public class MockingBirdChangeQueuePlugIn : Rhino.PlugIns.RenderPlugIn
	{
		public MockingBirdChangeQueuePlugIn()
		{
			if(Instance == null) Instance = this;
		}

		public static MockingBirdChangeQueuePlugIn Instance
		{
			get; private set;
		}

		protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
		{
			// initialise our render context
			MockingRenderContext rc = new MockingRenderContext(this, doc);

			// initialise our pipeline implementation
			RenderPipeline pipeline = new MockingRenderPipeline(doc, mode, this, rc);

			// query for render resolution
			var renderSize = RenderPipeline.RenderSize(doc);

			// set up view info
			ViewInfo viewInfo = new ViewInfo(doc.Views.ActiveView.ActiveViewport);

			// set up render window
			rc.RenderWindow = pipeline.GetRenderWindow();
			// add a wireframe channel for curves/wireframes/annotation etc.
			rc.RenderWindow.AddWireframeChannel(doc, viewInfo.Viewport, renderSize, new Rectangle(0, 0, renderSize.Width, renderSize.Height));
			// set correct size
			rc.RenderWindow.SetSize(renderSize);

			// prime the ChangeQueue. We do it here, since this *has* to
			// happen on the main thread.
			rc.ChangeQueue.CreateWorld();

			// now fire off render thread.
			var renderCode = pipeline.Render();

			// note that the rendering isn't complete yet, rather the pipeline.Render()
			// call starts a rendering thread. Here we essentially check whether
			// starting that thread went ok.
			if (renderCode != RenderPipeline.RenderReturnCode.Ok)
			{
				RhinoApp.WriteLine("Rendering (mockingbird modal+changequeue) failed:" + rc.ToString());
				return Result.Failure;
			}

			// all ok, so we are apparently rendering.
			return Result.Success;
		}

		protected override Result RenderWindow(RhinoDoc doc, RunMode mode, bool fastPreview, RhinoView view, Rectangle rect, bool inWindow)
		{
			return Result.Success;
		}

	}
}