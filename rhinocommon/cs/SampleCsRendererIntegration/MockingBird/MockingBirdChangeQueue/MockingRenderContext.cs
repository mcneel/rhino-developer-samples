using System.Threading;
using Rhino;
using Rhino.Display;
using Rhino.PlugIns;
using Rhino.Render;
using Rhino.DocObjects;

namespace MockingBirdChangeQueue
{
	/// <summary>
	/// The render context essentially hosts our render engine. It'll contain the
	/// main render entry function that gets called by the RenderPipeline
	/// mechanism.
	/// </summary>
	public class MockingRenderContext : AsyncRenderContext
	{ 
		public bool Done { get; private set; }

		public MockingChangeQueue ChangeQueue { get; private set; }

		public MockingRenderContext(PlugIn plugIn, RhinoDoc doc)
		{
			// set up view info
			ViewInfo viewInfo = new ViewInfo(doc.Views.ActiveView.ActiveViewport);
			ChangeQueue = new MockingChangeQueue(plugIn.Id, doc.RuntimeSerialNumber, viewInfo);
		}

		// our main rendering function.
		public void Renderer()
		{
			RhinoApp.WriteLine("Starting modal MockingBird with ChangeQueue");

			Done = false;
			using (var channel = RenderWindow.OpenChannel(RenderWindow.StandardChannels.RGBA))
			{
				var size = RenderWindow.Size();
				var max = (float)size.Width * size.Height;
				var rendered = 0;
				for (var x = 0; x < size.Width; x++)
				{
					for (var y = 0; y < size.Height; y++)
					{
						channel.SetValue(x, y, Color4f.FromArgb(1.0f, 0.5f, 0.75f, 1.0f));
						rendered++;
						Thread.Sleep(1);
						RenderWindow.SetProgress("rendering...", rendered / max);
						if (Cancel) break;
					}
					if (Cancel) break;
				}
			}

			// must set progress to 1.0f to signal RenderWindow (and pipeline/rc)
			// that rendering is now done.
			RenderWindow.SetProgress("MockingBird Modal with ChangeQueue done", 1.0f);

			// and send completion signal
			RenderWindow.EndAsyncRender(RenderWindow.RenderSuccessCode.Completed);

			Done = true;
			RhinoApp.WriteLine("... done");
		}
	}
}
