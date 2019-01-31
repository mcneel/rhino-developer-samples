using System.Drawing;
using System.Windows.Forms;
using Rhino;
using Rhino.Display;
using Rhino.DocObjects;
using RhinoWindows.Forms;

namespace SampleCsDragDrop
{
  internal class SampleCsDropTarget : RhinoDropTarget
  {
    public SampleCsDropTarget()
    {
      Enable = true;
      AllowDropOnLayer = true;
      AllowDropOnObject = true;
      AllowDropOnRhinoView = true;
      AllowDropOnSubObject = true;
    }

    protected override bool SupportDataObject(DataObject data)
    {
      var str = DropString(data);
      RhinoApp.WriteLine($"SampleCsDropTarget.SupportDataObject({str})");
      return !string.IsNullOrEmpty(str);
    }

    protected override bool OnDropOnLayer(RhinoDoc doc, Layer layer, DataObject dataObject, DragDropEffects dropEffect, Point point)
    {
      RhinoApp.WriteLine($"SampleCsDropTarget.OnDropOnLayer({DropString(dataObject)})");
      return true;
    }

    protected override bool OnDropOnRhinoView(RhinoView rhinoView, DataObject dataObject, DragDropEffects dropEffect, Point point)
    {
      RhinoApp.WriteLine($"SampleCsDropTarget.OnDropOnRhinoView({DropString(dataObject)})");
      return true;
    }

    protected override bool OnDropOnObject(ObjRef objRef, RhinoView rhinoView, DataObject dataObject, DragDropEffects dropEffect, Point point)
    {
      RhinoApp.WriteLine($"SampleCsDropTarget.OnDropOnObject({DropString(dataObject)})");
      return true;
    }

    private string DropString(DataObject dataObject)
    {
      var control = dataObject?.GetData(typeof(SampleCsUserControl)) as SampleCsUserControl;
      return control?.DropString;
    }
  }
}
