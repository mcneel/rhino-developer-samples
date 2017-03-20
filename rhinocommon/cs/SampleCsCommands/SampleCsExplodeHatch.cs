using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("17b2d51e-5bd6-457d-a43c-1c2ec3529f7c")]
  public class SampleCsExplodeHatch : Command
  {
    public SampleCsExplodeHatch()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsExplodeHatch"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjectType filter = Rhino.DocObjects.ObjectType.Hatch;
      Rhino.DocObjects.ObjRef objref = null;
      Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select hatch to explode", false, filter, out objref);
      if (rc != Rhino.Commands.Result.Success || objref == null)
        return rc;

      Rhino.Geometry.Hatch hatch = (Rhino.Geometry.Hatch)objref.Geometry();
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
                  Rhino.Geometry.Point point = (Rhino.Geometry.Point)geom;
                  if (null != point)
                    doc.Objects.AddPoint(point.Location);
                }
                break;
              case Rhino.DocObjects.ObjectType.Curve:
                {
                  Rhino.Geometry.Curve curve = (Rhino.Geometry.Curve)geom;
                  if (null != curve)
                    doc.Objects.AddCurve(curve);
                }
                break;
              case Rhino.DocObjects.ObjectType.Brep:
                {
                  Rhino.Geometry.Brep brep = (Rhino.Geometry.Brep)geom;
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
}
