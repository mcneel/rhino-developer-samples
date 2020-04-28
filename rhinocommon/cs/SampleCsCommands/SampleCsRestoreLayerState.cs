using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsRestoreLayerState : Command
  {
    public override string EnglishName => "SampleCsRestoreLayerState";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Guid plugin_id = Rhino.PlugIns.PlugIn.IdFromName("Rhino Bonus Tools");
      if (plugin_id == Guid.Empty)
        return Result.Failure;

      // The object returned by Rhino.RhinoApp.GetPlugInObject is a COM object, not a .NET object. 
      // C# 4 introduced the new dynamic keyword, which was designed with COM Interop in mind. 
      // The intent was to make calling properties and methods in C# as easy as it was in VBScript, etc.

      dynamic plugin_obj = Rhino.RhinoApp.GetPlugInObject(plugin_id);
      if (null == plugin_obj)
        return Result.Failure;

      object[] layer_states = plugin_obj.LayerStateNames();
      foreach (object layer_state in layer_states)
      {
        string state_name = layer_state.ToString();
        if (state_name.Equals("MyLayerState", StringComparison.OrdinalIgnoreCase))
          plugin_obj.RestoreLayerState(layer_state, 0);
      }

      return Result.Success;
    }
  }
}
