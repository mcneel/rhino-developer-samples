using System;
using System.Threading.Tasks;
using Grasshopper.Kernel;

namespace SampleGhTaskCapable.Components
{
  /// <summary>
  /// Task-capable components inherit from GH_TaskCapableComponent
  /// </summary>
  public class SampleGhTaskFibonacciComponent : GH_TaskCapableComponent<SampleGhTaskFibonacciComponent.SolveResults>
  {
    public SampleGhTaskFibonacciComponent()
      : base("Task Fibonacci", "TFib", "Task computes a Fibonacci number.", "Sample", "Task")
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

    public class SolveResults
    {
      public int Value { get; set; }
    }

    private static SolveResults ComputeFibonacci(int n)
    {
      SolveResults result = new SolveResults();
      if (n == 0)
        result.Value = 0;
      else if (n == 1)
        result.Value = 1;
      else
      {
        int x = 0, y = 1, rc = 0;
        for (int i = 2; i <= n; i++)
        {
          rc = x + y;
          x = y;
          y = rc;
        }
        result.Value = rc;
      }
      return result;
    }

    protected override void SolveInstance(IGH_DataAccess data)
    {
      const int max_steps = 46;

      if (InPreSolve)
      {
        // First pass; collect data and construct tasks
        int steps = 0;
        data.GetData(0, ref steps);
        if (steps < 0)
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Steps must be >= 0.");
          return;
        }
        if (steps > max_steps) // Prevent overflow...
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, $"Steps must be <= {max_steps}.");
          return;
        }

        // Run the task
        Task<SolveResults> task = Task.Run(() => ComputeFibonacci(steps), CancelToken);
        TaskList.Add(task);
        return;
      }

      if (!GetSolveResults(data, out SolveResults result))
      {
        // Compute right here, right now.
        // 1. Collect
        int steps = 0;
        data.GetData(0, ref steps);
        if (steps < 0)
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Steps must be >= 0.");
          return;
        }
        if (steps > max_steps) // Prevent overflow...
        {
          AddRuntimeMessage(GH_RuntimeMessageLevel.Error, $"Steps must be <= {max_steps}.");
          return;
        }

        // 2. Compute
        result = ComputeFibonacci(steps);
      }

      // 3. Set
      if (result != null)
      {
        data.SetData(0, result.Value);
      }
    }

    public override GH_Exposure Exposure => GH_Exposure.primary | GH_Exposure.obscure;

    protected override System.Drawing.Bitmap Icon => Properties.Resources.SampleGhTaskFibonacciComponent_24x24;

    public override Guid ComponentGuid => new Guid("3e65f1dc-8178-4390-a16f-6b9df2914ef5");
  }
}
