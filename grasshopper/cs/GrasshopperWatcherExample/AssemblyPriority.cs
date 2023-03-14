using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper;

namespace GrasshopperWatcherExample
{
  public class AssemblyPriority : Grasshopper.Kernel.GH_AssemblyPriority
  {
    public override Grasshopper.Kernel.GH_LoadingInstruction PriorityLoad()
    {
      // https://developer.rhino3d.com/api/grasshopper/html/M_Grasshopper_Kernel_GH_AssemblyPriority_PriorityLoad.htm

      Instances.CanvasCreated += Instances_CanvasCreated;

      return Grasshopper.Kernel.GH_LoadingInstruction.Proceed;
    }

    private void Instances_CanvasCreated(Grasshopper.GUI.Canvas.GH_Canvas canvas)
    {
      Instances.ActiveCanvas.Document_ObjectsAdded += ActiveCanvas_Document_ObjectsAdded;

      Instances.CanvasCreated -= Instances_CanvasCreated;
    }

    private void ActiveCanvas_Document_ObjectsAdded(Grasshopper.Kernel.GH_Document sender, Grasshopper.Kernel.GH_DocObjectEventArgs e)
    {
      foreach (var obj in e.Objects)
      {
        var msg = $"{obj.InstanceGuid}: {obj.Category} / {obj.SubCategory} / {obj.Name} ({obj.ComponentGuid})";
        Rhino.RhinoApp.WriteLine(msg);
      }
    }
  }
}
