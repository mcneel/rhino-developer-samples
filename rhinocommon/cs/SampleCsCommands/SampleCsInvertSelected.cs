using System;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("df016c71-cd32-4ab8-8aa6-ce2e413cd409")]
  public class SampleCsInvertSelected : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsInverteSelected"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var filter = new ObjectEnumeratorSettings
      {
        NormalObjects = true,
        LockedObjects = false,
        HiddenObjects = false,
        ActiveObjects = true,
        ReferenceObjects = true
      };

      var rh_objects = doc.Objects.FindByFilter(filter);
      foreach (var rh_obj in rh_objects)
      {
        var select = 0 == rh_obj.IsSelected(false) && rh_obj.IsSelectable();
        rh_obj.Select(select);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
