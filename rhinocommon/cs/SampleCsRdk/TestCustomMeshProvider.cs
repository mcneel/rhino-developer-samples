using System;

using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Render;
using Rhino;
using Rhino.Display;

namespace SampleCsRdk
{
	/// <summary>
	/// Make this class public to test.  If it is public and the plug-in is
	/// loaded then any viewports that are in rendered mode will display a
	/// sphere with a center at the center of the objects bounding box and a
	/// radius of 1/4 the bounding box diagonal vector length.
	/// </summary>
	public class TestCustomMeshProvider : CustomRenderMeshProvider2
	{
		Guid PluginId { get; set; }
		public TestCustomMeshProvider()
		{
			PluginId = rdktest_csPlugIn.IdFromName("C# Test Render Plug-In (RDK)");
			Rhino.RhinoDoc.ReplaceRhinoObject += RhinoDocReplaceRhinoObject;
		}

		void RhinoDocReplaceRhinoObject(object sender, RhinoReplaceObjectEventArgs e)
		{
			ObjectChanged(e.Document, e.NewRhinoObject);
		}

		public override string Name
		{
			get { return "Test Custom Mesh Provider"; }
		}

		public override BoundingBox BoundingBox(ViewportInfo vp, RhinoObject obj, RhinoDoc doc, Guid requestingPlugIn, bool preview)
		{
			var bbox = base.BoundingBox(vp, obj, doc, requestingPlugIn, preview);

			if (obj == null)
			{
				var sphere = new Sphere(new Point3d(0.0, 0.0, 0.0), 10.0);
				bbox.Union(sphere.BoundingBox);
			}
			else
			{
				var sphere = SphereFromObject(obj);
				bbox.Union(sphere.BoundingBox);
			}

			return bbox;
		}

		Sphere SphereFromObject(RhinoObject obj)
		{
			var bbox = obj.Geometry.GetBoundingBox(false);
			var radius = bbox.Diagonal.Length * 0.25;
			radius = radius < 0.1 ? 0.1 : radius;
			var sphere = new Sphere(bbox.Center, radius);
			return sphere;
		}

		public override bool WillBuildCustomMeshes(ViewportInfo vp, RhinoObject obj, RhinoDoc doc, Guid requestingPlugIn, DisplayPipelineAttributes attrs)
		{
			return true;
		}

		public override bool BuildCustomMeshes(ViewportInfo vp, RhinoDoc doc, RenderPrimitiveList objMeshes, Guid requestingPlugIn, DisplayPipelineAttributes attrs)
		{
			if (!WillBuildCustomMeshes(vp, objMeshes.RhinoObject, doc, requestingPlugIn, attrs))
				return false;

			var obj = objMeshes.RhinoObject;

			if (obj == null)
			{
				Sphere sphere = new Sphere(new Point3d(0.0, 0.0, 0.0), 10.0);
				objMeshes.Add(Rhino.Geometry.Mesh.CreateFromSphere(sphere, 100, 100), RenderMaterial.CreateBasicMaterial(Rhino.DocObjects.Material.DefaultMaterial));
			}
			else
			{
				var sphere = SphereFromObject(obj);
				objMeshes.Add(Mesh.CreateFromSphere(sphere, 100, 100), obj.RenderMaterial);
			}

			return true;
		}
	}
}

