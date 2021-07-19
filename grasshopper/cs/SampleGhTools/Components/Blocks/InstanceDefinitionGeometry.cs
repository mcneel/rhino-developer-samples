using Grasshopper.Kernel;
using Rhino;
using Rhino.DocObjects;
using Rhino.Geometry;
using System;
using System.Collections.Generic;

namespace SampleGhTools.Components.Blocks
{
  /// <summary>
  /// Gets the geometry from an instance definition
  /// </summary>
  public class InstanceDefinitionGeometry : InstanceComponentBase
  {
    public InstanceDefinitionGeometry()
      : base("Instance Definition Geometry", "IDefGeom", "Gets the geometry from an instance definition.", "Sample", "Block")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
    {
      pManager.AddTextParameter("IDef", "ID", "Id of the instance definition.", GH_ParamAccess.item);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
    {
      pManager.AddGeometryParameter("Geometry", "G", "Instance defintion geometry.", GH_ParamAccess.list);
      pManager.AddTextParameter("Layers", "L", "Ids of instance defintion geometry layers.", GH_ParamAccess.list);
    }

    protected override void SolveInstance(IGH_DataAccess DA)
    {
      if (null == DA) throw new ArgumentNullException(nameof(DA));

      var str = "";
      if (!DA.GetData(0, ref str)) return;

      var guid_str = str.Trim();
      var idef = InstanceDefinitionFromIdString(guid_str, true);
      if (null != idef)
      {
        var xform = Transform.Identity;
        var out_geometry = new List<GeometryBase>();
        var out_layers = new List<string>();
        GetInstanceDefinitionGeometry(Document, idef, xform, out_geometry, out_layers);
        if (out_geometry.Count == out_layers.Count)
        {
          DA.SetDataList(0, out_geometry);
          DA.SetDataList(1, out_layers);
        }
      }
    }

    protected void GetInstanceDefinitionGeometry(RhinoDoc doc, InstanceDefinition idef, Transform xform, List<GeometryBase> out_geometry, List<string> out_layers)
    {
      if (null != doc || null != idef)
      {
        var bXform = xform.IsValid && !xform.Equals(Transform.Identity);

        var bScale = false;
        var xform_scale = Transform.Identity;
        if (idef.UnitSystem != Rhino.UnitSystem.None && idef.UnitSystem != doc.ModelUnitSystem)
        {
          var scale = Rhino.RhinoMath.UnitScale(idef.UnitSystem, doc.ModelUnitSystem);
          xform_scale = Transform.Scale(Plane.WorldXY, scale, scale, scale);
          bScale = xform_scale.IsValid;
        }

        foreach (var rh_obj in idef.GetObjects())
        {
          if (null == rh_obj)
            continue;

          if (rh_obj is InstanceObject iref_obj)
          {
            var xf_iref = xform * iref_obj.InstanceXform;
            GetInstanceDefinitionGeometry(doc, iref_obj.InstanceDefinition, xf_iref, out_geometry, out_layers);
            continue;
          }

          var geometry = rh_obj.Geometry.Duplicate();
          if (null == geometry)
            continue;

          if (TransformSimilarityType.NotSimilarity == xform.SimilarityType)
          {
            if (!geometry.MakeDeformable() && geometry.ObjectType == ObjectType.Curve)
            {
              if (geometry is Curve curve)
                geometry = curve.ToNurbsCurve();
            }
          }

          var xf = Transform.Identity;
          if (bXform && bScale)
            xf = xform * xform_scale;
          else if (bXform)
            xf = xform;
          else if (bScale)
            xf = xform_scale;
          if (xform.IsValid && !xform.Equals(Transform.Identity))
          { 
            if (!geometry.Transform(xf))
              continue;
          }

          if (TransformSimilarityType.OrientationReversing == xform.SimilarityType)
          {
            if (geometry.ObjectType == ObjectType.Brep)
            {
              var brep = geometry as Brep;
              brep?.Flip();
            }
            else if (geometry.ObjectType == ObjectType.Mesh)
            {
              var mesh = geometry as Mesh;
              mesh?.Flip(true, true, true);
            }
          }

          out_geometry.Add(geometry);
          out_layers.Add(doc.Layers[rh_obj.Attributes.LayerIndex].Id.ToString());
        }
      }
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.InstanceDefinitionGeometry_24x24;
    public override Guid ComponentGuid => new Guid("2fd9bc71-0c40-44f9-a65f-9279013e00c2");
  }
}