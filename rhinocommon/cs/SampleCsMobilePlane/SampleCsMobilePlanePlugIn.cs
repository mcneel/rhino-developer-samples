using Rhino;
using Rhino.DocObjects;
using Rhino.PlugIns;

namespace SampleCsMobilePlane
{
  public class SampleCsMobilePlanePlugIn : PlugIn
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsMobilePlanePlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only instance of the SampleCsMobilePlanePlugIn plug-in.
    /// </summary>
    public static SampleCsMobilePlanePlugIn Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// Called by Rhino when loading this plug-in.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      RhinoDoc.UndeleteRhinoObject += OnUndeleteRhinoObject;
      return LoadReturnCode.Success;
    }

    /// <summary>
    /// Called if an object is undeleted (e.g. Undo)
    /// </summary>
    void OnUndeleteRhinoObject(object sender, RhinoObjectEventArgs e)
    {
      SampleCsMobilePlaneUserData.Refresh(e.TheObject, false);
    }
  }
}