using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleRhinoInsideJavascriptTaskManagerLib
{
  public class RhinoMethods
  {
    RhinoTaskManager rtm;

    public  RhinoMethods()
    {
      rtm = new RhinoTaskManager();
    }

    public async Task<object> StartGrasshopper(dynamic input)
    {
      return await rtm.StartGrasshopperTask(input);
    }

  }
}
