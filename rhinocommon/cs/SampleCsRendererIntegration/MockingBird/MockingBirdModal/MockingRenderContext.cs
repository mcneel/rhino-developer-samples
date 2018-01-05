using System.Threading;
using Rhino;
using Rhino.Display;
using Rhino.Render;

namespace MockingBirdModal
{
	/// <summary>
	/// The render context essentially hosts our render engine. It'll contain the
	/// main render entry function that gets called by the RenderPipeline
	/// mechanism.
	/// </summary>
	public class MockingRenderContext : AsyncRenderContext
	{
		public bool Done { get; private set; }

		private bool Paused { get; set; }


		// our main rendering function.
		public void Renderer()
		{
			RhinoApp.WriteLine("Starting modal MockingBird");

			Paused = false;

			Done = false;
			using (var channel = RenderWindow.OpenChannel(RenderWindow.StandardChannels.RGBA))
			{
				var size = RenderWindow.Size();
				var max = (float)size.Width*size.Height;
				var rendered = 0;
				for (var x = 0; x < size.Width; x++)
				{
					for (var y = 0; y < size.Height; y++)
					{
						channel.SetValue(x, y, Color4f.FromArgb(1.0f, 1.0f, 0.75f, 0.5f));
						rendered++;
						if (Cancel) break;
						RenderWindow.SetProgress("rendering...", rendered/max);
					}
					do
					{
						Thread.Sleep(10);
					} while (Paused && !Cancel);
					if (Cancel) break;
				}
			}

			// must set progress to 1.0f to signal RenderWindow (and pipeline/rc)
			// that rendering is now done.
			RenderWindow.SetProgress("MockingBird Modal done", 1.0f);

			// and send completion signal
			RenderWindow.EndAsyncRender(RenderWindow.RenderSuccessCode.Completed);

			Done = true;
			RhinoApp.WriteLine("... done");
		}

		public bool SupportsPause()
		{
			return true;
		}

		public void PauseRendering()
		{
			Paused = true;
		}

		public void ResumeRendering()
		{
			Paused = false;
		}
	}
}
