using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Grasshopper.Kernel;
using Rhino;
using Rhino.Geometry;

// In order to load the result of this wizard, you will also need to
// add the output bin/ folder of this project to the list of loaded
// folder in Grasshopper.
// You can use the _GrasshopperDeveloperSettings Rhino command for that.

namespace SampleGhTaskCapable
{
  public class SampleGhTaskCapableComponent : GH_TaskCapableComponent<SampleGhTaskCapableComponent.SolveResults>
  {
    /// <summary>
    /// Each implementation of GH_Component must provide a public 
    /// constructor without any arguments.
    /// Category represents the Tab in which the component will appear, 
    /// Subcategory the panel. If you use non-existing tab or panel names, 
    /// new tabs/panels will automatically be created.
    /// </summary>
    public SampleGhTaskCapableComponent()
      : base("Sample Dash Pattern", "SampleDash", "Convert a curve to a dash pattern.", "Sample", "C#")
    {
    }

    /// <summary>
    /// Registers all the input parameters for this component.
    /// </summary>
    protected override void RegisterInputParams(GH_InputParamManager input)
    {
      input.AddCurveParameter("Curve", "C", "Curve to dash", GH_ParamAccess.item);
      input.AddNumberParameter("Pattern", "Pt", "An collection of dash and gap lengths.", GH_ParamAccess.list);
    }

    /// <summary>
    /// Registers all the output parameters for this component.
    /// </summary>
    protected override void RegisterOutputParams(GH_OutputParamManager output)
    {
      output.AddCurveParameter("Dashes", "D", "Dash segments", GH_ParamAccess.list);
      output.AddCurveParameter("Gaps", "G", "Gap segments", GH_ParamAccess.list);
      output.HideParameter(1);
    }

    public class SolveResults
    {
      public List<Curve> Dashes { get; set; }
      public List<Curve> Gaps { get; set; }
    }

    SolveResults Compute(Curve crv, List<double> pattern)
    {
      var rc = new SolveResults();
      if (pattern.Count == 0)
        return null;

      var pattern_length = 0.0;
      for (var i = 0; i < pattern.Count; i++)
      {
        if (pattern[i] < 0.0)
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Warning, "Dash patterns cannot have negative length segments");
          pattern[i] = 0.0;
        }
        pattern_length += pattern[i];
      }

      if (pattern_length <= 1e-12)
      {
        AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Total pattern length is too short.");
        return rc;
      }

      crv = crv.DuplicateCurve();

      var dashes = new List<Curve>();
      var gaps = new List<Curve>();

      var index = -1;
      var dash = false;

      while (true)
      {
        //toggle gap/dash flag
        dash = !dash;

        //increment
        index++;

        //limit
        if (index >= pattern.Count) { index = 0; }

        //get pattern length
        if (Math.Abs(pattern[index]) < 1e-32)
        {
          if (dash) { dashes.Add(null); }
          else { gaps.Add(null); }
        }
        else
        {
          //measure length
          double length = crv.GetLength();
          if (!RhinoMath.IsValidDouble(length)) { break; }

          //cut short if we're at the end of the curve
          if (length <= pattern[index])
          {
            if (dash) { dashes.Add(crv); }
            else { gaps.Add(crv); }
            break;
          }

          //compute normalised arc-length parameter.
          if (!crv.LengthParameter(pattern[index], out var t)) { break; }

          var segment = crv.Trim(crv.Domain.Min, t);
          if (segment != null && segment.IsValid)
          {
            if (dash) { dashes.Add(segment); }
            else { gaps.Add(segment); }
          }

          crv = crv.Trim(t, crv.Domain.Max);
          if (crv == null) break;
        }
      }

      rc.Dashes = dashes;
      rc.Gaps = gaps;

      return rc;
    }

    /// <summary>
    /// This is the method that actually does the work.
    /// </summary>
    /// <param name="data">The DA object can be used to retrieve data from input parameters and 
    /// to store data in output parameters.</param>
    protected override void SolveInstance(IGH_DataAccess data)
    {
      if (InPreSolve)
      {
        // First pass; collect data and construct tasks
        Curve crv = null;
        var pattern = new List<double>();

        Task<SolveResults> tsk = null;
        if (data.GetData(0, ref crv) && data.GetDataList(1, pattern))
        {
          tsk = Task.Run(() => Compute(crv, pattern), CancelToken);
        }
        // Add a null task even if data collection fails. This keeps the
        // list size in sync with the iterations
        TaskList.Add(tsk);
        return;
      }

      if (!GetSolveResults(data, out var results))
      {
        // Compute right here, right now.
        // 1. Collect
        Curve crv = null;
        var pattern = new List<double>();

        if (!data.GetData(0, ref crv)) { return; }
        if (!data.GetDataList(1, pattern)) { return; }

        // 2. Compute
        results = Compute(crv, pattern);
      }

      // 3. Set
      if (results != null)
      {
        if (results.Dashes != null || results.Gaps != null)
        {
          data.SetDataList(0, results.Dashes);
          data.SetDataList(1, results.Gaps);
        }
      }
    }

    public override GH_Exposure Exposure => GH_Exposure.primary | GH_Exposure.obscure;

    /// <summary>
    /// Provides an Icon for every component that will be visible in the User Interface.
    /// Icons need to be 24x24 pixels.
    /// </summary>
    protected override System.Drawing.Bitmap Icon => Properties.Resources.SampleGhTaskCapableComponent_24x24;

    /// <summary>
    /// Each component must have a unique Guid to identify it. 
    /// It is vital this Guid doesn't change otherwise old ghx files 
    /// that use the old ID will partially fail during loading.
    /// </summary>
    public override Guid ComponentGuid => new Guid("be2b27e6-0a39-4ba3-82c9-40ab5b3f4fce");
  }
}
