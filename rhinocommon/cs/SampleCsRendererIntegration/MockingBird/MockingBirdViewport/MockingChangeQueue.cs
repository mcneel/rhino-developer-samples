using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Rhino;
using Rhino.Render.ChangeQueue;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Render;
using Light = Rhino.Render.ChangeQueue.Light;
using Material = Rhino.Render.ChangeQueue.Material;
using Mesh = Rhino.Render.ChangeQueue.Mesh;

namespace MockingBirdViewport
{
	public class MockingChangeQueue : ChangeQueue
	{
		// for a regular rhino document (i.e. currently
		// active)
		// The constructor can look like you want, as long as the plugin ID,
		// document serial number and view info are given, needed for
		// the base class
		public MockingChangeQueue(Guid pluginId, uint docRuntimeSerialNumber, ViewInfo viewinfo)
			: base(pluginId, docRuntimeSerialNumber, viewinfo, null, true, true)
		{
		}
		
		/// <summary>
		/// The camera information.
		/// </summary>
		/// <param name="viewInfo">new viewport info</param>
		protected override void ApplyViewChange(ViewInfo viewInfo)
		{
			var vp = viewInfo.Viewport;
			int near, far;
			var screenport = vp.GetScreenPort(out near, out far);
			RhinoApp.WriteLine($"Camera @ {vp.CameraLocation}, direction {vp.CameraDirection}");
			RhinoApp.WriteLine($"\twith near {near} and far {far}");
			RhinoApp.WriteLine($"\tand {screenport}");
		}

		protected override void ApplyEnvironmentChanges(RenderEnvironment.Usage usage)
		{
			// background - when camera ray doesn't hit any geometry
			// skylight - image-based lighting
			// reflection - what is seen in reflections
			var env = EnvironmentForid(EnvironmentIdForUsage(usage));
			RhinoApp.WriteLine(env != null ? $"{usage} {env.Name}" : $"No env for {usage}");
			// retrieving textures is with RenderMaterial, refer to HandleRenderMaterial()
		}

		/// <summary>
		/// Lights in the scene, including any automatic lighting
		/// (will be CameraDirectional)
		/// </summary>
		/// <param name="lightChanges">List of <code>Light</code>s</param>
		protected override void ApplyLightChanges(List<Light> lightChanges)
		{
			foreach (var light in lightChanges)
			{
				RhinoApp.WriteLine($"A {light.ChangeType} light. {light.Data.Name}, {light.Data.LightStyle}");
				if (light.Data.LightStyle == LightStyle.CameraDirectional)
				{
					RhinoApp.WriteLine("Use ChangeQueue.ConvertCameraBasedLightToWorld() to convert light transform to world");
					RhinoApp.WriteLine($"\told location {light.Data.Location}, direction {light.Data.Direction}");
					ConvertCameraBasedLightToWorld(this, light, GetQueueView());
					RhinoApp.WriteLine($"\tnew location {light.Data.Location}, direction {light.Data.Direction}");
				}
			}
		}

		/// <summary>
		/// Get all geometry data.
		/// </summary>
		/// <param name="deleted">List of Mesh instance IDs</param>
		/// <param name="added">List of <code>Mesh</code>es to add</param>
		protected override void ApplyMeshChanges(Guid[] deleted, List<Mesh> added)
		{
			RhinoApp.WriteLine($"Received {added.Count} new meshes, {deleted.Length} for deletion");
			foreach (var m in added)
			{
				var totalVerts = 0;
				var totalFaces = 0;
				var totalQuads = 0;
				var meshIndex = 0;
				RhinoApp.WriteLine($"\t{m.Id()} with {m.GetMeshes().Length} submeshes");
				foreach (var sm in m.GetMeshes())
				{
					RhinoApp.WriteLine($"\t\tmesh index {meshIndex} mesh with {sm.Vertices.Count} verts, {sm.Faces.Count} faces ({sm.Faces.QuadCount} quads).");
					totalVerts += sm.Vertices.Count;
					totalFaces += sm.Faces.Count;
					totalQuads += sm.Faces.QuadCount;
					RhinoApp.WriteLine($"\t\tFor material we remember ({m.Id()},{meshIndex}) as identifier. Connect dots in ApplyMeshInstanceChanged");
					meshIndex++;
				}
				RhinoApp.WriteLine($"\t{totalVerts} verts, {totalFaces} faces (of which {totalQuads} quads)");
			}
		}

		/// <summary>
		/// Mesh instances added or deleted. Mesh instances here really means the
		/// objects in a scene. More than one object can reference the same geometry.
		/// For a single-shot render (production render) this is also where
		/// materials for the scene are provided.
		/// </summary>
		/// <param name="deleted">Objects to delete, a list of unsigned ints</param>
		/// <param name="addedOrChanged">List of MeshInstances (objects)</param>
		protected override void ApplyMeshInstanceChanges(List<uint> deleted, List<MeshInstance> addedOrChanged)
		{
			RhinoApp.WriteLine($"Received {addedOrChanged.Count} mesh instances to be either added or changed");
			foreach (var mi in addedOrChanged)
			{
				var mat = MaterialFromId(mi.MaterialId);
				RhinoApp.WriteLine($"\tAdd or change object {mi.InstanceId} uses mesh <{mi.MeshId}, {mi.MeshIndex}>, and material {mi.MaterialId}, named {mat.Name})");
				HandleRenderMaterial(mat);

			}
			// For single-shot rendering there won't be deletions.
		}

		private void HandleRenderMaterial(RenderMaterial material)
		{
			RhinoApp.WriteLine($"\t\tMaterial {material.Name} is a {material.TypeName} ({material.TypeDescription})");
			
			var diffchan = material.TextureChildSlotName(RenderMaterial.StandardChildSlots.Diffuse);
			var difftex = material.FindChild(diffchan) as RenderTexture;
			if (difftex != null)
			{
				RhinoApp.WriteLine($"\t\t\ta diffuse texture was found {difftex.Name}, hash {difftex.RenderHashWithoutLocalMapping}");
				RhinoApp.WriteLine($"\t\t\tprojection {difftex.GetProjectionMode()}, env mapping {difftex.GetInternalEnvironmentMappingMode()}");
				RhinoApp.WriteLine($"\t\t\tlocal mapping xform {difftex.LocalMappingTransform}");
				var texeval = difftex.CreateEvaluator(RenderTexture.TextureEvaluatorFlags.DisableLocalMapping);
				int u, v, w;
				difftex.PixelSize(out u, out v, out w);
				// for procedural textures there's no u/v/w, so check for that and set
				// to some acceptable defaults.
				if (u == 0) u = 1024;
				if (v == 0) v = 1024;
				if (w == 0) w = 1;
				RhinoApp.WriteLine($"\t\t\tTexture size {u}x{v}x{w}");
			}

		}

		public event EventHandler ChangesReady;
		protected override void NotifyEndUpdates()
		{
			ChangesReady?.Invoke(this, EventArgs.Empty);
		}
	}
}
