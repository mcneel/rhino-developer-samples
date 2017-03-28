using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("e327e3b0-c6b6-4537-95cd-430fc56e76b6")]
  public class SampleCsExplodeBlock : Command
  {
    public SampleCsExplodeBlock()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsExplodeBlock"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new Rhino.Input.Custom.GetObject();
      go.SetCommandPrompt("Select block to explode");
      go.GeometryFilter = ObjectType.InstanceReference;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var iref = go.Object(0).Object() as Rhino.DocObjects.InstanceObject;
      if (null == iref)
        return Result.Failure;

      var xform = Transform.Identity;
      bool rc = ExplodeBlockHelper(doc, iref, xform);
      if (rc)
      {
        doc.Objects.Delete(go.Object(0), false);
        doc.Views.Redraw();
      }

      return Result.Success;
    }

    protected bool ExplodeBlockHelper(RhinoDoc doc, InstanceObject iref, Transform xf)
    {
      if (null == iref)
        return false;

      var idef = iref.InstanceDefinition;
      if (null == idef)
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

        var iref_obj = obj as InstanceObject;
        if (null != iref_obj)
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
              var crv = geom as Curve;
              if (null != crv)
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
              if (null != brep)
                brep.Flip();
            }
            else if (geom.ObjectType == ObjectType.Mesh)
            {
              var mesh = geom as Mesh;
              if (null != mesh)
                mesh.Flip(true, true, true);
            }
          }
        }

        doc.Objects.Add(geom, atts);
      }

      return true;
    }
  }
}
