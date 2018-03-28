using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsPersistentSettings : Command
  {
    private const bool BOOL_DEFAULT = true;
    private const int INT_DEFAULT = 1;
    private const double DBL_DEFAULT = 3.14;
    private const int LIST_DEFAULT = 3;

    /// <summary>
    /// EnglishName override
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsPersistentSettings"; }
    }

    /// <summary>
    /// RunCommand override
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Get persistent settings (from Registry)
      var bool_value = Settings.GetBool("BoolValue", BOOL_DEFAULT);
      var int_value = Settings.GetInteger("IntValue", INT_DEFAULT);
      var dbl_value = Settings.GetDouble("DblValue", DBL_DEFAULT);
      var list_value = Settings.GetInteger("ListValue", LIST_DEFAULT);

      var gp = new GetPoint();
      gp.SetCommandPrompt("GetPoint with options");

      var rc = Result.Cancel;

      while (true)
      {
        gp.ClearCommandOptions();

        var bool_option = new OptionToggle(bool_value, "Off", "On");
        var int_option = new OptionInteger(int_value, 1, 99);
        var dbl_option = new OptionDouble(dbl_value, 0, 99.9);
        var list_items = new[] { "Item0", "Item1", "Item2", "Item3", "Item4" };

        var bool_index = gp.AddOptionToggle("Boolean", ref bool_option);
        var int_index = gp.AddOptionInteger("Integer", ref int_option);
        var dbl_index = gp.AddOptionDouble("Double", ref dbl_option);
        var list_index = gp.AddOptionList("List", list_items, list_value);
        var reset_index = gp.AddOption("Reset");

        var res = gp.Get();

        if (res == Rhino.Input.GetResult.Point)
        {
          doc.Objects.AddPoint(gp.Point());
          doc.Views.Redraw();
          rc = Result.Success;
        }
        else if (res == Rhino.Input.GetResult.Option)
        {
          var option = gp.Option();
          if (null != option)
          {
            if (option.Index == bool_index)
              bool_value = bool_option.CurrentValue;
            else if (option.Index == int_index)
              int_value = int_option.CurrentValue;
            else if (option.Index == dbl_index)
              dbl_value = dbl_option.CurrentValue;
            else if (option.Index == list_index)
              list_value = option.CurrentListOptionIndex;
            else if (option.Index == reset_index)
            {
              bool_value = BOOL_DEFAULT;
              int_value = INT_DEFAULT;
              dbl_value = DBL_DEFAULT;
              list_value = LIST_DEFAULT;
            }
          }
          continue;
        }

        break;
      }

      if (rc == Result.Success)
      {
        // Set persistent settings (to Registry)
        Settings.SetBool("BoolValue", bool_value);
        Settings.SetInteger("IntValue", int_value);
        Settings.SetDouble("DblValue", dbl_value);
        Settings.SetInteger("ListValue", list_value);
      }

      return rc;
    }
  }
}
