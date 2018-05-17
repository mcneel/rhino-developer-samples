using System;
using Grasshopper.Kernel;

namespace SampleGhTaskCapable.Components
{
  /// <summary>
  /// Standard components inherit from GH_Component
  /// </summary>
  public class SampleGhFibonacciComponent : GH_Component
  {
    public SampleGhFibonacciComponent()
      : base("Sample Fibonacci", "SampleFib", "Computes a Fibonacci number.", "Sample", "C#")
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
    static int Fibonacci(int n)
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
      var result = Fibonacci(steps);

      // 3. Set
      data.SetData(0, result);
    }

    public override GH_Exposure Exposure => GH_Exposure.primary | GH_Exposure.obscure;

    protected override System.Drawing.Bitmap Icon => Properties.Resources.SampleGhFibonacciComponent_24x24;

    public override Guid ComponentGuid => new Guid("b74840f7-51dd-43cf-9e62-d06589851e70");
  }
}
