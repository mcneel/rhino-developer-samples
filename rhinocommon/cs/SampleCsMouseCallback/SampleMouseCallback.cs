using Rhino;
using Rhino.UI;

namespace SampleCsMouseCallback
{
	public class SampleMouseCallback : MouseCallback
	{
		protected override void OnMouseEnter(MouseCallbackEventArgs e)
		{
			RhinoApp.WriteLine($"Entering view {e.View?.ActiveViewport?.Name}");
		}

		protected override void OnMouseHover(MouseCallbackEventArgs e)
		{
			RhinoApp.WriteLine($"Hovering on view {e.View?.ActiveViewport?.Name}");
			base.OnMouseHover(e);
		}

		protected override void OnMouseLeave(MouseCallbackEventArgs e)
		{
			RhinoApp.WriteLine($"Leaving view {e.View?.ActiveViewport?.Name}");
			base.OnMouseLeave(e);
		}

		protected override void OnMouseMove(MouseCallbackEventArgs e)
		{
			RhinoApp.WriteLine($"Moving over view {e.View?.ActiveViewport?.Name}");
			base.OnMouseMove(e);
		}
	}
}
