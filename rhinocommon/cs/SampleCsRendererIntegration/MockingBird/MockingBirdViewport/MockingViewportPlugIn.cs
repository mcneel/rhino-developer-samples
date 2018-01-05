using Rhino.PlugIns;
using Rhino.Render;

namespace MockingBirdViewport
{
	/// <summary>
	/// This plug-in is created here as a general utility plug-in to show
	/// that the realtime viewport integration can be done outside of a
	/// Rhino.PlugIns.RenderPlugIn.
	///
	/// For this integration to work properly implementations of a
	/// Rhino.Render.RealtimeDisplayMode and a
	/// Rhino.Render.RealtimeDisplayModeClassInfo are needed. When those are
	/// present automatic registration will occur.
	/// </summary>
public class MockingViewportPlugIn : Rhino.PlugIns.PlugIn
{
	public MockingViewportPlugIn()
	{
		if (Instance == null) Instance = this;
	}

	public static MockingViewportPlugIn Instance { get; private set; }

	protected override LoadReturnCode OnLoad(ref string errorMessage)
	{
		return LoadReturnCode.Success;
	}
}
}