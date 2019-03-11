using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsExplodeBlock : Command
  {
  public override string EnglishName => "SampleCsExplodeBlock";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select block to explode");
      go.GeometryFilter = ObjectType.InstanceReference;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      if (!(go.Object(0).Object() is InstanceObject iref))
        return Result.Failure;

      var xform = Transform.Identity;
      var rc = ExplodeBlockHelper(doc, iref, xform);
      if (rc)
      {
        doc.Objects.Delete(go.Object(0), false);
        doc.Views.Redraw();
      }

      return Result.Success;
    }

    protected bool ExplodeBlockHelper(RhinoDoc doc, InstanceObject iref, Transform xf)
    {
      var idef = iref?.InstanceDefinition;
      if (idef == null)
        return false;

      var xform = xf * iref.InstanceXform;
      var do_xform = (xform.IsValid && !xform.Equals(Transform.Identity));

      var atts = iref.Attributes.Duplicate();
      atts.ObjectId = Guid.Empty;

      var objects = idef.GetObjects();
      foreach (var obj in objects)
      {
        if (null == obj)
          continue;

        if (obj is InstanceObject iref_obj)
        {
          // Explode any nested instances...
          ExplodeBlockHelper(doc, iref_obj, xform);
          continue;
        }

        var geom = obj.DuplicateGeometry();
        if (do_xform)
        {
          // Check for non-uniform scaling
          if (TransformSimilarityType.NotSimilarity == xform.SimilarityType)
          {
            if (!geom.MakeDeformable() && geom.ObjectType == ObjectType.Curve)
            {
              if (geom is Curve crv)
                geom = crv.ToNurbsCurve();
            }
          }

          if (!geom.Transform(xform))
            continue;

          if (TransformSimilarityType.OrientationReversing == xform.SimilarityType)
          {
            if (geom.ObjectType == ObjectType.Brep)
            {
              var brep = geom as Brep;
              brep?.Flip();
            }
            else if (geom.ObjectType == ObjectType.Mesh)
            {
              var mesh = geom as Mesh;
              mesh?.Flip(true, true, true);
            }
          }
        }

        doc.Objects.Add(geom, atts);
      }

      return true;
    }
  }
}
