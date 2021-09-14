using System;
using System.Threading.Tasks;
using Rhino.Geometry;
using Grasshopper.Kernel;
using Grasshopper.Kernel.Types;

namespace SampleGhTaskCapable.Components
{
  public class SampleGhTaskAreaComponent : GH_TaskCapableComponent<SampleGhTaskAreaComponent.SolveResults>
  {
    public SampleGhTaskAreaComponent()
      : base("Task Area", "TArea", "Task compute the area breps, meshes and planar closed curves.", "Sample", "Task")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager input)
    {
      input.AddGeometryParameter("Geometry", "G", "Brep, mesh or planar closed curve for area computation", GH_ParamAccess.item);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager output)
    {
      output.AddNumberParameter("Area", "A", "Area of geometry", GH_ParamAccess.item);
    }

    public class SolveResults
    {
      public double Area { get; set; }
    }

    private SolveResults ComputeArea(IGH_GeometricGoo geometry)
    {
      if (null == geometry)
        return null;

      bool rc = false;

      Brep brep = null;
      Mesh mesh = null;
      Curve curve = null;

      if (!rc)
      {
        var gh_brep = geometry as GH_Brep;
        rc = (null != gh_brep && null != gh_brep.Value);
        if (rc)
          brep = gh_brep.Value;
      }

      if (!rc)
      {
        var gh_surface = geometry as GH_Surface;
        rc = (null != gh_surface && null != gh_surface.Value);
        if (rc)
          brep = gh_surface.Value;
      }

      if (!rc)
      {
        var gh_mesh = geometry as GH_Mesh;
        rc = (null != gh_mesh && null != gh_mesh.Value);
        if (rc)
          mesh = gh_mesh.Value;
      }

      if (!rc)
      {
        var gh_curve = geometry as GH_Curve;
        rc = (null != gh_curve && null != gh_curve.Value);
        if (rc)
          curve = gh_curve.Value;
      }

      if (!rc)
      {
        var gh_circle = geometry as GH_Circle;
        if (null != gh_circle)
        {
          var circle = gh_circle.Value;
          return new SolveResults { Area = Math.PI * circle.Radius * circle.Radius };
        }
      }

      if (!rc)
      {
        var gh_rectangle = geometry as GH_Rectangle;
        if (null != gh_rectangle)
        {
          var rectangle = gh_rectangle.Value;
          return new SolveResults { Area = Math.Abs(rectangle.Width * rectangle.Height) };
        }
      }

      // Indirect casts
      if (!rc)
        rc = GH_Convert.ToBrep(geometry, ref brep, GH_Conversion.Both);
      if (!rc)
        rc = GH_Convert.ToCurve(geometry, ref curve, GH_Conversion.Both);
      if (!rc)
        rc = GH_Convert.ToMesh(geometry, ref mesh, GH_Conversion.Both);

      if (rc)
      {
        AreaMassProperties mp = null;
        
        if (null != brep)
        {
          mp = AreaMassProperties.Compute(brep, true, false, false, false);
        }
        else if (null != mesh)
        {
          mp = AreaMassProperties.Compute(mesh, true, false, false, false);
        }
        else if (null != curve)
        {
          if (!curve.IsClosed)
          {
            AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Curve must be closed for Area calculation");
            return null;
          }
          if (!curve.IsPlanar(DocumentTolerance()))
          {
            AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Curve must be planar for Area calculation");
            return null;
          }
          var geom = new GeometryBase[] { curve };
          mp = AreaMassProperties.Compute(geom, true, false, false, false);
        }

        if (null != mp)
          return new SolveResults { Area = mp.Area };
        else
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Area could not be computed.");
      }

      return null;
    }

    protected override void SolveInstance(IGH_DataAccess data)
    {
      if (InPreSolve)
      {
        IGH_GeometricGoo geometry = null;
        data.GetData(0, ref geometry);
        TaskList.Add(Task.Run(() => ComputeArea(geometry), CancelToken));
        return;
      }

      if (!GetSolveResults(data, out SolveResults result))
      {
        IGH_GeometricGoo geometry = null;
        if (!data.GetData(0, ref geometry))
          return;
        result = ComputeArea(geometry);
      }

      if (null != result)
      {
        data.SetData(0, result.Area);
      }
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.SampleGhTaskAreaComponent_24x24;

    public override Guid ComponentGuid => new Guid("689C4320-BA81-4D6A-92D5-053CFA2F7CB4");
  }
}