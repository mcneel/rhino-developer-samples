using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCommandLineOptions : Command
  {
    public override string EnglishName => "SampleCsCommandLineOptions";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType geometry_filter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Mesh;
      var integer1 = 300;
      var integer2 = 300;

      var option_integer1 = new OptionInteger(integer1, 200, 900);
      var option_integer2 = new OptionInteger(integer2, 200, 900);

      var go = new GetObject();
      go.SetCommandPrompt("Select surfaces, polysurfaces, or meshes");
      go.GeometryFilter = geometry_filter;
      go.AddOptionInteger("Option1", ref option_integer1);
      go.AddOptionInteger("Option2", ref option_integer2);
      go.GroupSelect = true;
      go.SubObjectSelect = false;

      var have_preselected_objects = false;

      while (true)
      {
        var res = go.GetMultiple(1, 0);

        if (res == GetResult.Option)
        {
          go.EnablePreSelect(false, true);
          go.AlreadySelectedObjectSelect = true;
          go.EnableClearObjectsOnEntry(false);
          go.DeselectAllBeforePostSelect = false;
          go.EnableUnselectObjectsOnExit(false);
          continue;
        }

        if (res != GetResult.Object)
          return Result.Cancel;

        if (go.ObjectsWerePreselected)
        {
          have_preselected_objects = true;
          go.EnablePreSelect(false, true);
          go.AlreadySelectedObjectSelect = true;
          go.EnableClearObjectsOnEntry(false);
          go.DeselectAllBeforePostSelect = false;
          go.EnableUnselectObjectsOnExit(false);
          continue;
        }

        break;
      }

      if (have_preselected_objects)
      {
        // Normally, pre-selected objects will remain selected, when a
        // command finishes, and post-selected objects will be unselected.
        // This this way of picking, it is possible to have a combination
        // of pre-selected and post-selected. So, to make sure everything
        // "looks the same", lets unselect everything before finishing
        // the command.
        for (var i = 0; i < go.ObjectCount; i++)
        {
          var rhino_object = go.Object(i).Object();
          rhino_object?.Select(false);
        }
        doc.Views.Redraw();
      }

      var object_count = go.ObjectCount;
      integer1 = option_integer1.CurrentValue;
      integer2 = option_integer2.CurrentValue;

      RhinoApp.WriteLine("Select object count: {0}", object_count);
      RhinoApp.WriteLine("Value of integer1: {0}", integer1);
      RhinoApp.WriteLine("Value of integer2: {0}", integer2);

      return Result.Success;
    }
  }
}
