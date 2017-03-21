using System.Reflection;
using Rhino;
using Rhino.PlugIns;

namespace SampleCsRhino
{
  /// <summary>
  /// SampleCsRhinoPlugIn plug-in 
  /// </summary>
  public class SampleCsRhinoPlugIn : PlugIn
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsRhinoPlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the one and only instance of the SampleCsRhinoPlugIn plug-in.
    /// </summary>
    public static SampleCsRhinoPlugIn Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// Called by Rhino when loading this plug-in.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      var app_name = Assembly.GetExecutingAssembly().GetName().Name;
      var app_version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
      RhinoApp.WriteLine("{0} {1} loaded.", app_name, app_version);
      return LoadReturnCode.Success;
    }


    /// <summary>
    /// Override this function if you want to return a COM visible object to
    /// RhinoScript or an external application that is automating Rhino.
    /// </summary>
    public override object GetPlugInObject()
    {
      var rhino_obj = new SampleCsRhinoObject();
      return rhino_obj;
    }
  }
}