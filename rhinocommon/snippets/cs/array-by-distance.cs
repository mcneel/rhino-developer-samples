partial class Examples
{
  static double m_distance = 1;
  static Rhino.Geometry.Point3d m_point_start;
  public static Rhino.Commands.Result ArrayByDistance(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    var rc = Rhino.Input.RhinoGet.GetOneObject("Select object", true, Rhino.DocObjects.ObjectType.AnyObject, out objref);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    rc = Rhino.Input.RhinoGet.GetPoint("Start point", false, out m_point_start);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    var obj = objref.Object();
    if (obj == null)
      return Rhino.Commands.Result.Failure;

    // create an instance of a GetPoint class and add a delegate
    // for the DynamicDraw event
    var gp = new Rhino.Input.Custom.GetPoint();
    gp.DrawLineFromPoint(m_point_start, true);
    var optdouble = new Rhino.Input.Custom.OptionDouble(m_distance);
    bool constrain = false;
    var optconstrain = new Rhino.Input.Custom.OptionToggle(constrain, "Off", "On");
    gp.AddOptionDouble("Distance", ref optdouble);
    gp.AddOptionToggle("Constrain", ref optconstrain);
    gp.DynamicDraw += ArrayByDistanceDraw;
    gp.Tag = obj;
    while (gp.Get() == Rhino.Input.GetResult.Option)
    {
      m_distance = optdouble.CurrentValue;
      if (constrain != optconstrain.CurrentValue)
      {
        constrain = optconstrain.CurrentValue;
        if (constrain)
        {
          var gp2 = new Rhino.Input.Custom.GetPoint();
          gp2.DrawLineFromPoint(m_point_start, true);
          gp2.SetCommandPrompt("Second point on constraint line");
          if (gp2.Get() == Rhino.Input.GetResult.Point)
            gp.Constrain(m_point_start, gp2.Point());
          else
          {
            gp.ClearCommandOptions();
            optconstrain.CurrentValue = false;
            constrain = false;
            gp.AddOptionDouble("Distance", ref optdouble);
            gp.AddOptionToggle("Constrain", ref optconstrain);
          }
        }
        else
        {
          gp.ClearConstraints();
        }
      }
    }

    if (gp.CommandResult() == Rhino.Commands.Result.Success)
    {
      m_distance = optdouble.CurrentValue;
      var pt = gp.Point();
      var vec = pt - m_point_start;
      double length = vec.Length;
      vec.Unitize();
      int count = (int)(length / m_distance);
      for (int i = 1; i < count; i++)
      {
        var translate = vec * (i * m_distance);
        var xf = Rhino.Geometry.Transform.Translation(translate);
        doc.Objects.Transform(obj, xf, false);
      }
      doc.Views.Redraw();
    }

    return gp.CommandResult();
  }

  // this function is called whenever the GetPoint's DynamicDraw
  // event occurs
  static void ArrayByDistanceDraw(object sender, Rhino.Input.Custom.GetPointDrawEventArgs e)
  {
    Rhino.DocObjects.RhinoObject rhobj = e.Source.Tag as Rhino.DocObjects.RhinoObject;
    if (rhobj == null)
      return;
    var vec = e.CurrentPoint - m_point_start;
    double length = vec.Length;
    vec.Unitize();
    int count = (int)(length / m_distance);
    for (int i = 1; i < count; i++)
    {
      var translate = vec * (i * m_distance);
      var xf = Rhino.Geometry.Transform.Translation(translate);
      e.Display.DrawObject(rhobj, xf);
    }
  }
}
