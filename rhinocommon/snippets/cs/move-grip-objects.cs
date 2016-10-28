partial class Examples
{
  public static Rhino.Commands.Result MoveGripObjects(Rhino.RhinoDoc doc)
  {      
    // The following example demonstrates how to move a surface's grip objects.
    // In this sample, all grips will be moved a fixed distance of 0.5 units 
    // in the normal direction of the surface at that grip location.

    Rhino.DocObjects.ObjRef objRef;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select surface for control point editing", false, Rhino.DocObjects.ObjectType.Surface, out objRef);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    Rhino.DocObjects.RhinoObject obj = objRef.Object();
    if (null == obj)
      return Rhino.Commands.Result.Failure;

    Rhino.Geometry.Surface srf = objRef.Surface();
    if (null == srf)
      return Rhino.Commands.Result.Failure;

    // Make sure the object's grips are enabled
    obj.GripsOn = true;
    doc.Views.Redraw();

    Rhino.DocObjects.GripObject[] grips = obj.GetGrips();
    for (int i = 0; i < grips.Length; i++)
    {
      Rhino.DocObjects.GripObject grip = grips[i];

      // Calculate the point on the surface closest to our test point,
      // which is the grip's 3-D location (for this example).
      double u, v;
      if (srf.ClosestPoint(grip.CurrentLocation, out u, out v))
      {
        // Compute the surface normal at a point
        Rhino.Geometry.Vector3d dir = srf.NormalAt(u, v);
        dir.Unitize();

        // Scale by our fixed distance
        dir *= 0.5;

        // Move the grip to a new location
        grip.Move(dir);
      }
    }

    // Altered grip positions on a RhinoObject are used to calculate an updated
    // object that is added to the document.
    doc.Objects.GripUpdate(obj, false);
    doc.Views.Redraw();

    return Rhino.Commands.Result.Success;
  }

}
