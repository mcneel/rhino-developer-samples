using System;
using System.Text;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [
    System.Runtime.InteropServices.Guid("b46acba3-4d22-4975-b994-35a408813336"),
    // Required to call RhinoApp.RunScript
    Rhino.Commands.CommandStyle(Rhino.Commands.Style.ScriptRunner) 
  ]
  public class SampleCsSmooth : Command
  {
    private double m_smooth_factor = 0.2;
    private bool m_smooth_x = true;
    private bool m_smooth_y = true;
    private bool m_smooth_z = true;
    private bool m_use_cplane = false;
    private bool m_fix_boundaries = true;

    public SampleCsSmooth()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSmooth"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Pick some objects to smooth
      Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
      go.SetCommandPrompt("Select objects to smooth");
      go.SubObjectSelect = false;
      go.ReferenceObjectSelect = false;
      go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve | Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.Mesh;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Prompt for some command line options. In this case,
      // we will clone the -Smooth command.
      Rhino.Input.Custom.GetOption gs = new Rhino.Input.Custom.GetOption();
      gs.SetCommandPrompt("Choose smooth option");
      gs.AcceptNothing(true);
      for (; ; )
      {
        gs.ClearCommandOptions();

        Rhino.Input.Custom.OptionDouble smooth_factor_option = new Rhino.Input.Custom.OptionDouble(m_smooth_factor);
        Rhino.Input.Custom.OptionToggle use_cplane_option = new Rhino.Input.Custom.OptionToggle(m_use_cplane, "World", "CPlane");
        Rhino.Input.Custom.OptionToggle smooth_x_option = new Rhino.Input.Custom.OptionToggle(m_smooth_x, "No", "Yes");
        Rhino.Input.Custom.OptionToggle smooth_y_option = new Rhino.Input.Custom.OptionToggle(m_smooth_y, "No", "Yes");
        Rhino.Input.Custom.OptionToggle smooth_z_option = new Rhino.Input.Custom.OptionToggle(m_smooth_z, "No", "Yes");
        Rhino.Input.Custom.OptionToggle fix_boundaries_option = new Rhino.Input.Custom.OptionToggle(m_fix_boundaries, "No", "Yes");

        int smooth_factor_index = gs.AddOptionDouble("SmoothFactor", ref smooth_factor_option);
        int use_cplane_index = gs.AddOptionToggle("CoordinateSystem", ref use_cplane_option);
        int smooth_x_index = gs.AddOptionToggle("X", ref smooth_x_option);
        int smooth_y_index = gs.AddOptionToggle("Y", ref smooth_y_option);
        int smooth_z_index = gs.AddOptionToggle("Z", ref smooth_z_option);
        int fix_boundaries_index = gs.AddOptionToggle("FixBoundaries", ref fix_boundaries_option);

        Rhino.Input.GetResult getrc = gs.Get();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        if (getrc == Rhino.Input.GetResult.Option)
        {
          int index = gs.OptionIndex();
          if (index == smooth_factor_index)
            m_smooth_factor = smooth_factor_option.CurrentValue;
          else if (index == use_cplane_index)
            m_use_cplane = use_cplane_option.CurrentValue;
          else if (index == smooth_x_index)
            m_smooth_x = smooth_x_option.CurrentValue;
          else if (index == smooth_y_index)
            m_smooth_y = smooth_y_option.CurrentValue;
          else if (index == smooth_z_index)
            m_smooth_z = smooth_z_option.CurrentValue;
          else if (index == fix_boundaries_index)
            m_fix_boundaries = fix_boundaries_option.CurrentValue;

          continue;
        }

        break;
      }

      // Build a command line macro that we can script
      StringBuilder sb = new StringBuilder();
      sb.Append("_-Smooth ");
      sb.Append(string.Format("_SmoothFactor={0} ", m_smooth_factor));
      sb.Append(string.Format("_CoordinateSystem={0} ", m_use_cplane ? "_CPlane" : "_World"));
      sb.Append(string.Format("_X={0} ", m_smooth_x ? "_Yes" : "_No"));
      sb.Append(string.Format("_Y={0} ", m_smooth_y ? "_Yes" : "_No"));
      sb.Append(string.Format("_Z={0} ", m_smooth_z ? "_Yes" : "_No"));
      sb.Append(string.Format("_FixBoundaries={0} ", m_fix_boundaries ? "_Yes" : "_No"));
      sb.Append("_Enter");

      string script = sb.ToString();

#if (!RELEASE)
      Rhino.RhinoApp.WriteLine(script);
#endif

      // Script the smooth command
      Rhino.RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}
