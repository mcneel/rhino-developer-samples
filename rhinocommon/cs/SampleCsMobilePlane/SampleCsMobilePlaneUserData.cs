using System;
using Rhino.DocObjects;
using Rhino.DocObjects.Custom;
using Rhino.FileIO;
using Rhino.Geometry;

namespace SampleCsMobilePlane
{
  /// <summary>
  /// SampleCsMobilePlaneUserData
  /// </summary>
  [System.Runtime.InteropServices.Guid("D91F19A1-A0C3-4062-B540-F724B5D9D777")]
  public class SampleCsMobilePlaneUserData : UserData
  {
    #region Data members

    public Plane Plane { get; set; }
    public Guid ViewportId { get; set; }
    public bool Enabled { get; set; }

    #endregion

    #region Userdata overrides

    /// <summary>
    /// Called when the object is being duplicated.
    /// </summary>
    protected override void OnDuplicate(UserData source)
    {
      var src = source as SampleCsMobilePlaneUserData;
      if (null != src)
      {
        Plane = src.Plane;
        ViewportId = src.ViewportId;
        Enabled = src.Enabled;
      }
    }

    /// <summary>
    /// Descriptive name of the user data.
    /// </summary>
    public override string Description
    {
      get { return "Mobile Plane User Data"; }
    }

    /// <summary>
    /// Return true to serialize to 3dm files.
    /// </summary>
    public override bool ShouldWrite
    {
      get { return true; }
    }

    /// <summary>
    /// Reads the content of this data from a stream archive.
    /// </summary>
    protected override bool Read(BinaryArchiveReader archive)
    {
      int major, minor;
      archive.Read3dmChunkVersion(out major, out minor);
      if (1 == major && 0 == minor)
      {
        Plane = archive.ReadPlane();
        ViewportId = archive.ReadGuid();
        Enabled = archive.ReadBool();
      }
      return !archive.ReadErrorOccured;
    }

    /// <summary>
    /// Writes the content of this data to a stream archive.
    /// </summary>
    protected override bool Write(BinaryArchiveWriter archive)
    {
      archive.Write3dmChunkVersion(1, 0);
      archive.WritePlane(Plane);
      archive.WriteGuid(ViewportId);
      archive.WriteBool(Enabled);
      return !archive.WriteErrorOccured;
    }

    /// <summary>
    /// Called when the object associated with this data is transformed.
    /// </summary>
    protected override void OnTransform(Transform xform)
    {
      base.OnTransform(xform);

      var p = Plane;
      p.Transform(xform);
      Plane = p;

      if (Enabled)
        UpdateConstructionPlane();
    }

    #endregion

    #region Public methods

    /// <summary>
    /// Update the view's construction plane.
    /// </summary>
    public void UpdateConstructionPlane()
    {
      var doc = Rhino.RhinoDoc.ActiveDoc;
      if (null != doc)
      {
        var view = doc.Views.Find(ViewportId);
        if (null != view)
        {
          view.ActiveViewport.SetConstructionPlane(Plane);
          view.Redraw();
        }
      }
    }

    #endregion

    #region Static helper methods

    /// <summary>
    /// Verifies a mobile plane is attached to a Rhino object.
    /// </summary>
    public static bool IsAttached(RhinoObject obj)
    {
      var rc = false;
      if (null != obj)
      {
        var data = DataFromObject(obj);
        rc = null != data;
      }
      return rc;
    }

    /// <summary>
    /// Attaches a mobile plane to a Rhino object
    /// </summary>
    public static bool Attach(RhinoObject obj, Plane plane, Guid viewportId)
    {
      var rc = false;
      if (null != obj)
      {
        var data = DataFromObject(obj);
        if (null != data)
        {
          data.Plane = plane;
          data.ViewportId = viewportId;
          rc = true;
        }
        else
        {
          data = new SampleCsMobilePlaneUserData
          {
            Plane = plane,
            ViewportId = viewportId,
            Enabled = true
          };
          rc = obj.Geometry.UserData.Add(data);
        }
      }
      return rc;
    }

    /// <summary>
    /// Detaches a mobile plane from a Rhino object
    /// </summary>
    public static bool Detach(RhinoObject obj)
    {
      var rc = false;
      if (null != obj)
      {
        var data = DataFromObject(obj);
        if (null != data)
          rc = obj.Geometry.UserData.Remove(data);
      }
      return rc;
    }

    /// <summary>
    /// Returns the enabled state of a mobile plane.
    /// </summary>
    public static bool IsEnabled(RhinoObject obj)
    {
      var rc = false;
      if (null != obj)
      {
        var data = DataFromObject(obj);
        if (null != data)
          rc = data.Enabled;
      }
      return rc;
    }

    /// <summary>
    /// Enable mobile plane's dynamic construction plane update.
    /// </summary>
    public static bool Enable(RhinoObject obj, bool bEnable)
    {
      var rc = false;
      if (null != obj)
      {
        var data = DataFromObject(obj);
        if (null != data)
        {
          data.Enabled = bEnable;
          rc = true;
        }
      }
      return rc;
    }

    /// <summary>
    /// Refreshes, or updates, the construction plane tracked by the mobile plane.
    /// </summary>
    public static bool Refresh(RhinoObject obj, bool bIgnoreEnabled)
    {
      var rc = false;
      if (null != obj)
      {
        var data = DataFromObject(obj);
        if (null != data)
        {
          if (bIgnoreEnabled || data.Enabled)
            data.UpdateConstructionPlane();
          rc = true;
        }
      }
      return rc;
    }

    /// <summary>
    /// Gets the mobile plane user data from a Rhino object
    /// </summary>
    public static SampleCsMobilePlaneUserData DataFromObject(RhinoObject obj)
    {
      SampleCsMobilePlaneUserData rc = null;
      if (null != obj)
        rc = obj.Geometry.UserData.Find(typeof(SampleCsMobilePlaneUserData)) as SampleCsMobilePlaneUserData;
      return rc;
    }

    #endregion
  }
}
