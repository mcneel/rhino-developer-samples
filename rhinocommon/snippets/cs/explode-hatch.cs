partial class Examples
{
  public static Rhino.Commands.Result ExplodeHatch(Rhino.RhinoDoc doc)
  {
    const ObjectType filter = Rhino.DocObjects.ObjectType.Hatch;
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select hatch to explode", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Hatch hatch = objref.Geometry() as Rhino.Geometry.Hatch;
    if (null == hatch)
      return Rhino.Commands.Result.Failure;

    Rhino.Geometry.GeometryBase[] hatch_geom = hatch.Explode();
    if (null != hatch_geom)
    {
      for (int i = 0; i < hatch_geom.Length; i++)
      {
        Rhino.Geometry.GeometryBase geom = hatch_geom[i];
        if (null != geom)
        {
          switch (geom.ObjectType)
          {
            case Rhino.DocObjects.ObjectType.Point:
              {
                Rhino.Geometry.Point point = geom as Rhino.Geometry.Point;
                if (null != point)
                  doc.Objects.AddPoint(point.Location);
              }
              break;
            case Rhino.DocObjects.ObjectType.Curve:
              {
                Rhino.Geometry.Curve curve = geom as Rhino.Geometry.Curve;
                if (null != curve)
                  doc.Objects.AddCurve(curve);
              }
              break;
            case Rhino.DocObjects.ObjectType.Brep:
              {
                Rhino.Geometry.Brep brep = geom as Rhino.Geometry.Brep;
                if (null != brep)
                  doc.Objects.AddBrep(brep);
              }
              break;
          }
        }
      }
    }

    return Rhino.Commands.Result.Success;
  }
}
