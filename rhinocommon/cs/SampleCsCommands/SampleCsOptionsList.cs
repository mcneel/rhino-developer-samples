using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsOptionsList : Command
  {
    public override string EnglishName => "SampleCsOptionsList";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var fruit_list = new[] { "Apple", "Bannana", "Grape", "Mango", "Orange", "Pear" };
      var nut_list = new[] { "Almonds", "Cashews", "Hazelnuts", "Pecans", "Pistachios", "Walnuts" };
      var vegetable_list = new[] { "Asparagus", "Broccoli", "Carrot", "Corn", "Lettuce", "Onion" };

      // Get persistent settings
      var fruit_value = Settings.GetInteger("Fruit", 0);
      var nut_value = Settings.GetInteger("Nut", 0);
      var vegetable_value = Settings.GetInteger("Vegetable", 0);

      var gp = new GetPoint();
      gp.SetCommandPrompt("GetPoint with options");

      var rc = Result.Cancel;

      while (true)
      {
        gp.ClearCommandOptions();

        var fruit_index = gp.AddOptionList("Fruit", fruit_list, fruit_value);
        var nut_index = gp.AddOptionList("Nut", nut_list, nut_value);
        var vegetable_index = gp.AddOptionList("Vegetable", vegetable_list, vegetable_value);

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
            if (option.Index == fruit_index)
              fruit_value = option.CurrentListOptionIndex;
            else if (option.Index == nut_index)
              nut_value = option.CurrentListOptionIndex;
            else if (option.Index == vegetable_index)
              vegetable_value = option.CurrentListOptionIndex;
          }
          continue;
        }

        break;
      }

      if (rc == Result.Success)
      {
        // Set persistent settings
        Settings.SetInteger("Fruit", fruit_value);
        Settings.SetInteger("Nut", nut_value);
        Settings.SetInteger("Vegetable", vegetable_value);
      }

      return rc;
    }
  }
}