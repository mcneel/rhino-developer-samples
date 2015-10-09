using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;
using System.Linq;

namespace examples_cs
{
  public class SetActiveViewCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csSetActiveView"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // view and view names
      var active_view_name = doc.Views.ActiveView.ActiveViewport.Name;

      var non_active_views = 
        doc.Views
        .Where(v => v.ActiveViewport.Name != active_view_name)
        .ToDictionary(v => v.ActiveViewport.Name, v => v);

      // get name of view to set active
      var gs = new GetString();
      gs.SetCommandPrompt("Name of view to set active");
      gs.AcceptNothing(true);
      gs.SetDefaultString(active_view_name);
      foreach (var view_name in non_active_views.Keys)
        gs.AddOption(view_name);
      var result = gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var selected_view_name = 
        result == GetResult.Option ? gs.Option().EnglishName : gs.StringResult();

      if (selected_view_name != active_view_name)
        if (non_active_views.ContainsKey(selected_view_name))
          doc.Views.ActiveView = non_active_views[selected_view_name];
        else
          RhinoApp.WriteLine("\"{0}\" is not a view name", selected_view_name);

      return Rhino.Commands.Result.Success;
    }
  }
}