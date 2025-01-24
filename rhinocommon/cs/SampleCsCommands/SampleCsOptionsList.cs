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
      string[] fruit_list = new[] { "Apple", "Bannana", "Grape", "Mango", "Orange", "Pear" };
      string[] nut_list = new[] { "Almonds", "Cashews", "Hazelnuts", "Pecans", "Pistachios", "Walnuts" };
      string[] vegetable_list = new[] { "Asparagus", "Broccoli", "Carrot", "Corn", "Lettuce", "Onion" };

      // Get persistent settings
      int fruit_value = Settings.GetInteger("Fruit", 0);
      int nut_value = Settings.GetInteger("Nut", 0);
      int vegetable_value = Settings.GetInteger("Vegetable", 0);

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("GetPoint with options");

      Result rc = Result.Cancel;

      while (true)
      {
        gp.ClearCommandOptions();

        int fruit_index = gp.AddOptionList("Fruit", fruit_list, fruit_value);
        int nut_index = gp.AddOptionList("Nut", nut_list, nut_value);
        int vegetable_index = gp.AddOptionList("Vegetable", vegetable_list, vegetable_value);

        Rhino.Input.GetResult res = gp.Get();

        if (res == Rhino.Input.GetResult.Point)
        {
          doc.Objects.AddPoint(gp.Point());
          doc.Views.Redraw();
          rc = Result.Success;
        }
        else if (res == Rhino.Input.GetResult.Option)
        {
          CommandLineOption option = gp.Option();
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