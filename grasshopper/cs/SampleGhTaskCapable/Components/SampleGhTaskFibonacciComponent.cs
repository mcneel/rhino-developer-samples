using System;
using System.Threading.Tasks;
using Grasshopper.Kernel;

namespace SampleGhTaskCapable.Components
{
  /// <summary>
  /// Task-capable components inherit from GH_TaskCapableComponent
  /// </summary>
  public class SampleGhTaskFibonacciComponent : GH_TaskCapableComponent<int?>
  {
    public SampleGhTaskFibonacciComponent()
      : base("Sample Task Fibonacci", "SampleTFib", "Task computes a Fibonacci number.", "Sample", "C#")
    {
    }

    protected override void RegisterInputParams(GH_InputParamManager input)
    {
      input.AddIntegerParameter("Steps", "S", "Number of steps to compute.", GH_ParamAccess.item);
    }

    protected override void RegisterOutputParams(GH_OutputParamManager output)
    {
      output.AddIntegerParameter("Fibonacci number", "F", "The Fibonacci number", GH_ParamAccess.item);
    }

    /// <summary>
    /// Simple and quick iterative way to generate the sequence of Fibonacci Numbers.
    /// </summary>
    static int? Fibonacci(int n)
    {
      int x = 0, y = 1, rc = 0;

      if (n == 0) return 0; // To return the first Fibonacci number   
      if (n == 1) return 1; // To return the second Fibonacci number   

      for (var i = 2; i <= n; i++)
      {
        rc = x + y;
        x = y;
        y = rc;
      }

      return rc;
    }

    protected override void SolveInstance(IGH_DataAccess data)
    {
      const int max_steps = 46;

      if (InPreSolve)
      {
        // First pass; collect data and construct tasks
        var steps = 0;
        data.GetData(0, ref steps);
        if (steps < 0)
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Steps must greater than or equal to 0.");
          return;
        }
        if (steps > max_steps) // Prevents overflow...
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, $"Steps must less than or equal to { (object)max_steps}.");
          return;
        }

        Task<int?> tsk = Task.Run(() => Fibonacci(steps), CancelToken);

        // Add a null task even if data collection fails. 
        // This keeps the list size in sync with the iterations.
        TaskList.Add(tsk);
        return;
      }

      if (!GetSolveResults(data, out var result))
      {
        // Compute right here, right now.
        // 1. Collect
        var steps = 0;
        data.GetData(0, ref steps);
        if (steps < 0)
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Steps must greater than or equal to 0.");
          return;
        }
        if (steps > max_steps) // Prevents overflow...
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, $"Steps must less than or equal to { (object)max_steps}.");
          return;
        }

        // 2. Compute
        result = Fibonacci(steps);
      }

      // 3. Set
      if (result != null)
      {
        data.SetData(0, result);
      }
    }

    public override GH_Exposure Exposure => GH_Exposure.primary | GH_Exposure.obscure;

    protected override System.Drawing.Bitmap Icon => Properties.Resources.SampleGhTaskFibonacciComponent_24x24;

    public override Guid ComponentGuid => new Guid("3e65f1dc-8178-4390-a16f-6b9df2914ef5");
  }
}
