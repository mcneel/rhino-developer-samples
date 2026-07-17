using Rhino;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.PlugIns;
using Rhino.Render;
using Rhino.Render.CustomRenderMeshes;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace SampleCsRdk
{
  /// <summary>
  /// Make this class public to test.  If it is public and the plug-in is
  /// loaded then any object in a viewport that is in rendered mode will display
  /// a sphere centered on the object's bounding box with a radius of 1/4 the
  /// bounding box diagonal length.
  ///
  /// Ported to the Rhino 9 Rhino.Render.CustomRenderMeshes.RenderMeshProvider
  /// API (replacing the obsolete CustomRenderMeshProvider2 / RenderPrimitiveList).
  /// The new API is hash based: instead of caching meshes ourselves and
  /// invalidating them on object replace, we fold the source geometry into the
  /// running hash so the RDK rebuilds the meshes whenever the object changes.
  /// </summary>
  [Guid("4959B079-F6C7-4006-BE15-0CB3255CD77D")]
  public class TestCustomMeshProvider : RenderMeshProvider
  {
    public override string Name
    {
      get { return "Test Custom Mesh Provider"; }
    }

    public override bool HasCustomRenderMeshes(MeshType mt, ViewportInfo vp, RhinoDoc doc, Guid objectId, ref Flags flags, PlugIn plugin, DisplayPipelineAttributes attrs)
    {
      return true;
    }

    public override RenderMeshes RenderMeshes(MeshType mt, ViewportInfo vp, RhinoDoc doc, Guid objectId, List<InstanceObject> ancestry, ref Flags flags, RenderMeshes previousPrimitives, PlugIn plugin, DisplayPipelineAttributes attrs)
    {
      if (!HasCustomRenderMeshes(mt, vp, doc, objectId, ref flags, plugin, attrs))
        return null;

      RhinoObject obj = doc?.Objects.FindId(objectId);

      Sphere sphere = obj == null
        ? new Sphere(new Point3d(0.0, 0.0, 0.0), 10.0)
        : SphereFromObject(obj);

      // Honor the ReturnNullForStandardMaterial flag. When the caller sets it
      // (e.g. shaded display), return a null material to indicate that the
      // object's standard material should be used - the display resolves it.
      // Only supply our own material when the flag is clear (e.g. production
      // rendering). Requires the RH-96417 fix, present in Rhino 9.
      RenderMaterial material = null;
      if (!flags.HasFlag(Flags.ReturnNullForStandardMaterial))
        material = obj?.RenderMaterial ?? RenderMaterial.CreateBasicMaterial(Material.DefaultMaterial, doc);

      // Fold the incoming hash together with this sphere so the cached meshes
      // are rebuilt when an upstream provider or the source geometry changes.
      uint incomingHash = previousPrimitives?.Hash ?? 0;
      uint modificationHash = (uint)(sphere.Center.GetHashCode() ^ sphere.Radius.GetHashCode());

      RenderMeshes renderMeshes = new RenderMeshes(doc, objectId, ProviderId, incomingHash + modificationHash, (uint)Flags.None);

      renderMeshes.AddInstance(new Instance
      {
        Mesh = Mesh.CreateFromSphere(sphere, 100, 100),
        Material = material
      });

      return renderMeshes;
    }

    static Sphere SphereFromObject(RhinoObject obj)
    {
      BoundingBox bbox = obj.Geometry.GetBoundingBox(false);
      double radius = bbox.Diagonal.Length * 0.25;
      radius = radius < 0.1 ? 0.1 : radius;
      Sphere sphere = new Sphere(bbox.Center, radius);
      return sphere;
    }
  }
}
