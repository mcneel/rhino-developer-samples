using System;
using System.Threading.Tasks;
using Rhino.Geometry;
using Grasshopper.Kernel;
using Grasshopper.Kernel.Types;

namespace SampleGhTaskCapable.Components
{
  public class SampleGhTaskVolumeComponent : GH_TaskCapableComponent<SampleGhTaskVolumeComponent.SolveResults>
  {
    public SampleGhTaskVolumeComponent()
      : base("Task Volume", "TVol", "Task compute the volume of closed breps and meshes.", "Sample", "Task")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager input)
    {
      input.AddGeometryParameter("Geometry", "G", "Closed brep or mesh for volume computation", GH_ParamAccess.item);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager output)
    {
      output.AddNumberParameter("Volume", "V", "Volume of geometry", GH_ParamAccess.item);
    }

    public class SolveResults
    {
      public double Volume { get; set; }
    }

    private SolveResults ComputeVolume(IGH_GeometricGoo geometry)
    {
      if (null == geometry)
        return null;

      bool rc = false;
      Brep brep = null;
      Mesh mesh = null;

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

      if (rc)
      {
        VolumeMassProperties mp = null;
        if (null != brep)
        {
          if (!brep.IsSolid)
            AddRuntimeMessage(GH_RuntimeMessageLevel.Warning, "Volume cannot be reliably computed for an open brep.");
          mp = VolumeMassProperties.Compute(brep, true, false, false, false);
        }
        else if (null != mesh)
        {
          if (!mesh.IsClosed)
            AddRuntimeMessage(GH_RuntimeMessageLevel.Warning, "Volume cannot be reliably computed for an open mesh.");
          mp = VolumeMassProperties.Compute(mesh, true, false, false, false);
        }
        if (null != mp)
          return new SolveResults { Volume = mp.Volume };
      }

      return null;
    }

    protected override void SolveInstance(IGH_DataAccess data)
    {
      if (InPreSolve)
      {
        IGH_GeometricGoo geometry = null;
        if (!data.GetData(0, ref geometry))
          return;

        Task<SolveResults> task = Task.Run(() => ComputeVolume(geometry), CancelToken);
        TaskList.Add(task);
        return;
      }

      if (!GetSolveResults(data, out SolveResults result))
      {
        IGH_GeometricGoo geometry = null;
        if (!data.GetData(0, ref geometry))
          return;

        result = ComputeVolume(geometry);
      }

      if (null != result)
      {
        data.SetData(0, result.Volume);
      }
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.SampleGhTaskVolumeComponent_24x24;

    public override Guid ComponentGuid => new Guid("828ccaf1-440d-4712-921e-0fc82962c1b4");
  }
}