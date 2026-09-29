
using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.DocObjects;
using Rhino.UI;
using SampleCommandLineView;

public class SampleCommandLineViewCommand : Command
{
  public override string EnglishName => "SampleCommandLineView";

  protected override Result RunCommand(RhinoDoc doc, RunMode mode)
  {
    // create a viewmodel that can be used in a UI dialog in interactive mode
    SampleModel model = SampleModel.ReadFrom(Settings);
    SampleViewModel vm = new(doc, model);
    Result res = Result.Success;
    if (mode == RunMode.Interactive)
    {
      // create a dialog, and let the user run the command interactively
      SampleDialog dialog = new(vm);
      res = dialog.ShowSemiModal(doc, RhinoEtoApp.MainWindow);

      doc.Views.Redraw();
      vm.Report();
    }
    else if (mode == RunMode.Scripted)
    {
      // the commandline view plays the role of 'view' in the MVVM architecture
      CommandLineView clv = new(vm);

      GetOption getOpt = new();
      getOpt.SetCommandPrompt("First demo with GetOption: Give options");

      // call Run with GetOption to have the user update the options on the view model
      res = clv.Run(getOpt);
      if (res == Result.Cancel)
        return res;

      doc.Views.Redraw();
      vm.Report();

      GetObject getObj = new();
      getObj.GeometryFilter = ObjectType.Curve;
      getObj.SetCommandPrompt("Second demo with GetObject: Select curves");

      // call Run with GetObject to have the user update the options on the view model
      // while selecting objects.
      res = clv.Run(getObj, g => g.GetMultiple(1, 0), out var cRefs);
      if (res == Result.Cancel)
        return res;

      RhinoApp.WriteLine($"Selected {cRefs.Count} curves.");
      vm.Report();
      doc.Objects.UnselectAll();
      doc.Views.Redraw();

      // instead of calling Run, it is also possible to build a get-object-with-options loop like below.
      // this allows more complex business logic to be handled inside the while loop.
      while (true)
      {
        cRefs.Clear();
        clv.BuildCommandLine(getObj);
        getObj.SetCommandPrompt("Third demo with GetObject: Select at least three curves.");
        GetResult gr = getObj.GetMultiple(3, 0);
        if (gr == GetResult.Cancel)
          return Result.Cancel;
        if (gr == GetResult.Option)
        {
          // after GetObject has completed, the view model state must
          // be updated by calling UpdateViewModel
          clv.UpdateViewModel(getObj);
          continue;
        }
        if (gr == GetResult.Object)
        {
          for (int j = 0; j < getObj.ObjectCount; ++j)
          {
            cRefs.Add(getObj.Object(j));
          }
          break;
        }
      }

      RhinoApp.WriteLine($"Selected {cRefs.Count} curves.");
      vm.Report();
      doc.Objects.UnselectAll();
      doc.Views.Redraw();
    }

    // save the model to the command settings.
    model.WriteTo(Settings);
    return res;
  }
}
