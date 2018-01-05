using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.PlugIns;
using Rhino.Render;

namespace MockingBirdModal
{
	public class MockingBirdModalPlugIn : RenderPlugIn

	{
		public MockingBirdModalPlugIn()
		{
			if(Instance == null) Instance = this;
		}

		public static MockingBirdModalPlugIn Instance
		{
			get; private set;
		}

		protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
		{
			// initialise our render context
			MockingRenderContext rc = new MockingRenderContext();

			// initialise our pipeline implementation
			RenderPipeline pipeline = new MockingRenderPipeline(doc, mode, this, rc);

			// get the render window where we will put our render result
			rc.RenderWindow = pipeline.GetRenderWindow(true);

			// now fire off render thread.
			var renderCode = pipeline.Render();

			// note that the rendering isn't complete yet, rather the pipeline.Render()
			// call starts a rendering thread. Here we essentially check whether
			// starting that thread went ok.
			if (renderCode != RenderPipeline.RenderReturnCode.Ok)
			{
				RhinoApp.WriteLine("Rendering failed:" + rc.ToString());
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