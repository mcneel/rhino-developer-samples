partial class Examples
{
  public static Rhino.Commands.Result CommandLineOptions(Rhino.RhinoDoc doc)
  {
    // For this example we will use a GetPoint class, but all of the custom
    // "Get" classes support command line options.
    Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
    gp.SetCommandPrompt("GetPoint with options");

    // set up the options
    Rhino.Input.Custom.OptionInteger intOption = new Rhino.Input.Custom.OptionInteger(1, 1, 99);
    Rhino.Input.Custom.OptionDouble dblOption = new Rhino.Input.Custom.OptionDouble(2.2, 0, 99.9);
    Rhino.Input.Custom.OptionToggle boolOption = new Rhino.Input.Custom.OptionToggle(true, "Off", "On");
    string[] listValues = new string[] { "Item0", "Item1", "Item2", "Item3", "Item4" };

    gp.AddOptionInteger("Integer", ref intOption);
    gp.AddOptionDouble("Double", ref dblOption);
    gp.AddOptionToggle("Boolean", ref boolOption);
    int listIndex = 3;
    int opList = gp.AddOptionList("List", listValues, listIndex);

    while (true)
    {
      // perform the get operation. This will prompt the user to input a point, but also
      // allow for command line options defined above
      Rhino.Input.GetResult get_rc = gp.Get();
      if (gp.CommandResult() != Rhino.Commands.Result.Success)
        return gp.CommandResult();

      if (get_rc == Rhino.Input.GetResult.Point)
      {
        doc.Objects.AddPoint(gp.Point());
        doc.Views.Redraw();
        Rhino.RhinoApp.WriteLine("Command line option values are");
        Rhino.RhinoApp.WriteLine(" Integer = {0}", intOption.CurrentValue);
        Rhino.RhinoApp.WriteLine(" Double = {0}", dblOption.CurrentValue);
        Rhino.RhinoApp.WriteLine(" Boolean = {0}", boolOption.CurrentValue);
        Rhino.RhinoApp.WriteLine(" List = {0}", listValues[listIndex]);
      }
      else if (get_rc == Rhino.Input.GetResult.Option)
      {
        if (gp.OptionIndex() == opList)
          listIndex = gp.Option().CurrentListOptionIndex;
        continue;
      }
      break;
    }
    return Rhino.Commands.Result.Success;
  }
}
