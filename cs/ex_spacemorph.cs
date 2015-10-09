using Rhino.DocObjects;

partial class Examples
{
  static Rhino.DocObjects.ObjectType SpaceMorphObjectFilter()
  {
    Rhino.DocObjects.ObjectType filter =
      Rhino.DocObjects.ObjectType.Point |
      Rhino.DocObjects.ObjectType.PointSet |
      Rhino.DocObjects.ObjectType.Curve |
      Rhino.DocObjects.ObjectType.Surface |
      Rhino.DocObjects.ObjectType.PolysrfFilter |
      Rhino.DocObjects.ObjectType.Mesh |
      Rhino.DocObjects.ObjectType.Grip |
      Rhino.DocObjects.ObjectType.Cage;
    return filter;
  }

  public static Rhino.Commands.Result Twist(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to twist", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Line axis;
    rc = Rhino.Input.RhinoGet.GetLine(out axis);
    if (rc != Rhino.Commands.Result.Success || axis == null)
      return rc;

    double angle = Rhino.RhinoMath.UnsetValue;
    rc = Rhino.Input.RhinoGet.GetNumber("Twist angle in degrees", false, ref angle);
    if (rc != Rhino.Commands.Result.Success || !Rhino.RhinoMath.IsValidDouble(angle))
      return rc;

    Rhino.Geometry.Morphs.TwistSpaceMorph morph = new Rhino.Geometry.Morphs.TwistSpaceMorph();
    morph.TwistAxis = axis;
    morph.TwistAngleRadians = Rhino.RhinoMath.ToRadians(angle);

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Bend(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to bend", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Line axis;
    rc = Rhino.Input.RhinoGet.GetLine(out axis);
    if (rc != Rhino.Commands.Result.Success || axis == null)
      return rc;

    Rhino.Geometry.Point3d point;
    rc = Rhino.Input.RhinoGet.GetPoint("Point to bend through", false, out point);
    if (rc != Rhino.Commands.Result.Success || !point.IsValid)
      return rc;

    Rhino.Geometry.Morphs.BendSpaceMorph morph = new Rhino.Geometry.Morphs.BendSpaceMorph(axis.From, axis.To, point, true, false);

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Taper(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to taper", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Line axis;
    rc = Rhino.Input.RhinoGet.GetLine(out axis);
    if (rc != Rhino.Commands.Result.Success || axis == null)
      return rc;

    double radius0 = Rhino.RhinoMath.UnsetValue;
    rc = Rhino.Input.RhinoGet.GetNumber("Starting radius", false, ref radius0);
    if (rc != Rhino.Commands.Result.Success || !Rhino.RhinoMath.IsValidDouble(radius0))
      return rc;

    double radius1 = Rhino.RhinoMath.UnsetValue;
    rc = Rhino.Input.RhinoGet.GetNumber("Ending radius", false, ref radius1);
    if (rc != Rhino.Commands.Result.Success || !Rhino.RhinoMath.IsValidDouble(radius1))
      return rc;

    Rhino.Geometry.Morphs.TaperSpaceMorph morph = new Rhino.Geometry.Morphs.TaperSpaceMorph(axis.From, axis.To, radius0, radius1, false, false);

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Maelstrom(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to maelstrom", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Plane plane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane();

    double radius0 = Rhino.RhinoMath.UnsetValue;
    rc = Rhino.Input.RhinoGet.GetNumber("Starting radius", false, ref radius0);
    if (rc != Rhino.Commands.Result.Success || !Rhino.RhinoMath.IsValidDouble(radius0))
      return rc;

    double radius1 = Rhino.RhinoMath.UnsetValue;
    rc = Rhino.Input.RhinoGet.GetNumber("Ending radius", false, ref radius1);
    if (rc != Rhino.Commands.Result.Success || !Rhino.RhinoMath.IsValidDouble(radius1))
      return rc;

    double angle = Rhino.RhinoMath.UnsetValue;
    rc = Rhino.Input.RhinoGet.GetNumber("Twist angle in degrees", false, ref angle);
    if (rc != Rhino.Commands.Result.Success || !Rhino.RhinoMath.IsValidDouble(angle))
      return rc;

    Rhino.Geometry.Morphs.MaelstromSpaceMorph morph = new Rhino.Geometry.Morphs.MaelstromSpaceMorph(plane, radius0, radius1, Rhino.RhinoMath.ToRadians(angle));

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Stretch(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to stretch", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Plane plane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane();

    Rhino.Geometry.Line axis;
    rc = Rhino.Input.RhinoGet.GetLine(out axis);
    if (rc != Rhino.Commands.Result.Success || axis == null)
      return rc;

    Rhino.Geometry.Morphs.StretchSpaceMorph morph = new Rhino.Geometry.Morphs.StretchSpaceMorph(axis.From, axis.To, axis.Length * 1.5);

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Sporph(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to sporph", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Input.Custom.GetObject go0 = new Rhino.Input.Custom.GetObject();
    go0.SetCommandPrompt("Source surface");
    go0.GeometryFilter = Rhino.DocObjects.ObjectType.Surface;
    go0.SubObjectSelect = false;
    go0.EnablePreSelect(false, true);
    go0.DeselectAllBeforePostSelect = false;
    go0.Get();
    if (go0.CommandResult() != Rhino.Commands.Result.Success)
      return go0.CommandResult();

    Rhino.DocObjects.ObjRef srf0_ref = go0.Object(0);

    Rhino.Input.Custom.GetObject go1 = new Rhino.Input.Custom.GetObject();
    go1.SetCommandPrompt("Source surface");
    go1.GeometryFilter = Rhino.DocObjects.ObjectType.Surface;
    go1.SubObjectSelect = false;
    go1.EnablePreSelect(false, true);
    go1.DeselectAllBeforePostSelect = false;
    go1.Get();
    if (go1.CommandResult() != Rhino.Commands.Result.Success)
      return go1.CommandResult();

    Rhino.DocObjects.ObjRef srf1_ref = go1.Object(0);

    Rhino.Geometry.Morphs.SporphSpaceMorph morph = new Rhino.Geometry.Morphs.SporphSpaceMorph(srf0_ref.Surface(), srf1_ref.Surface());

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Flow(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to flow", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Input.Custom.GetObject go0 = new Rhino.Input.Custom.GetObject();
    go0.SetCommandPrompt("Source curve");
    go0.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go0.SubObjectSelect = false;
    go0.EnablePreSelect(false, true);
    go0.DeselectAllBeforePostSelect = false;
    go0.Get();
    if (go0.CommandResult() != Rhino.Commands.Result.Success)
      return go0.CommandResult();

    Rhino.DocObjects.ObjRef crv0_ref = go0.Object(0);

    Rhino.Input.Custom.GetObject go1 = new Rhino.Input.Custom.GetObject();
    go1.SetCommandPrompt("Source curve");
    go1.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go1.SubObjectSelect = false;
    go1.EnablePreSelect(false, true);
    go1.DeselectAllBeforePostSelect = false;
    go1.Get();
    if (go1.CommandResult() != Rhino.Commands.Result.Success)
      return go1.CommandResult();

    Rhino.DocObjects.ObjRef crv1_ref = go1.Object(0);

    Rhino.Geometry.Morphs.FlowSpaceMorph morph = new Rhino.Geometry.Morphs.FlowSpaceMorph(crv0_ref.Curve(), crv1_ref.Curve(), false);

    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Commands.Result Splop(Rhino.RhinoDoc doc)
  {
    ObjectType filter = SpaceMorphObjectFilter();
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to splop", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Plane plane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane();

    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Surface to splop on");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Surface;
    go.SubObjectSelect = false;
    go.EnablePreSelect(false, true);
    go.DeselectAllBeforePostSelect = false;
    go.Get();
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();

    Rhino.DocObjects.ObjRef srfref = go.Object(0);

    double u, v;
    srfref.SurfaceParameter(out u, out v);

    Rhino.Geometry.Point2d uv = new Rhino.Geometry.Point2d(u,v);

    Rhino.Geometry.Morphs.SplopSpaceMorph morph = new Rhino.Geometry.Morphs.SplopSpaceMorph(plane, srfref.Surface(), uv);
    Rhino.Geometry.GeometryBase geom = objref.Geometry().Duplicate();
    if (morph.Morph(geom))
    {
      doc.Objects.Add(geom);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }

}
