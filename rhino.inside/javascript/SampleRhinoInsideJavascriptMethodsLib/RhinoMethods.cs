using System;
using System.Threading.Tasks;
using Rhino.Runtime.InProcess;

namespace SampleRhinoInsideJavascriptMethodsLib
{
  public class RhinoMethods
  {
    static RhinoCore rhinoCore;

    public RhinoMethods()
    {
      RhinoInside.Resolver.Initialize();
    }
    public async Task<object> StartRhino(dynamic input)
    {
      return await Task.Run(() =>
      {
        try
        {
          rhinoCore = new RhinoCore(new string[] { "/NOSPLASH" }, WindowStyle.NoWindow);
          return "Rhino has started.";
        }
        catch (Exception ex)
        {
          return ex.Message;
        }
      });
    }
    public async Task<object> MakeSphere(dynamic input)
    {
      return await Task.Run(() =>
      {
        var sphere = new Rhino.Geometry.Sphere(Rhino.Geometry.Point3d.Origin, input.radius);
        var sphereMesh = Rhino.Geometry.Mesh.CreateFromSphere(sphere, input.countU, input.countV);
        return Newtonsoft.Json.JsonConvert.SerializeObject(sphereMesh);
      });
    }
  }
}
