using Rhino;
using Rhino.Commands;
using Rhino.PlugIns;
using Rhino.Render;

namespace MockingBirdRenderPlugIn
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
public class MockingRenderPlugIn : RenderPlugIn
	{
		public MockingRenderPlugIn()
		{
			if (Instance == null) Instance = this;
		}

		public static MockingRenderPlugIn Instance { get; private set; }

		protected override LoadReturnCode OnLoad(ref string errorMessage)
		{
			return LoadReturnCode.Success;
		}

		protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
		{
			RhinoApp.WriteLine("_Render command support not yet implemented");
			return Result.Nothing;
		}

		protected override void CreatePreview(CreatePreviewEventArgs args)
		{
			foreach (var o in args.Objects)
			{
				RhinoApp.WriteLine($"{o.Material.Name}: {o.Material.TypeName} | {o.Material.TypeId} | {o.Material.TypeDescription}");
			}
			args.PreviewImage = null;
		}
	}
}