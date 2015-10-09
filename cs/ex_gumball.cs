using Rhino;
using Rhino.Geometry;


namespace examples_cs
{
  public class GumballExampleCommand : Rhino.Commands.TransformCommand
  {
    public override string EnglishName  { get { return "csExampleGumball"; } }

    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      // Select objects to scale
      var list = new Rhino.Collections.TransformObjectList();
      var rc = SelectObjects("Select objects to gumball", list);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      var bbox = list.GetBoundingBox(true, true);
      if (!bbox.IsValid)
        return Rhino.Commands.Result.Failure;

      Rhino.Commands.Result cmdrc;

      var base_gumball = new Rhino.UI.Gumball.GumballObject();
      base_gumball.SetFromBoundingBox(bbox);
      var dc = new Rhino.UI.Gumball.GumballDisplayConduit();
      var appearance = new Rhino.UI.Gumball.GumballAppearanceSettings();

      // turn off some of the scale appearance settings to have a slightly different gumball
      appearance.ScaleXEnabled = false;
      appearance.ScaleYEnabled = false;
      appearance.ScaleZEnabled = false;

      bool bCopy = false;
      while (true)
      {
        dc.SetBaseGumball(base_gumball, appearance);
        dc.Enabled = true;
        doc.Views.Redraw();

        GetGumballXform gp = new GetGumballXform(dc);
        int copy_optindx = gp.AddOption("Copy");
        if (dc.PreTransform == Transform.Identity)
          gp.SetCommandPrompt("Drag gumball");
        else
        {
          gp.AcceptNothing(true);
          gp.SetCommandPrompt("Drag gumball. Press Enter when done");
        }
        gp.AddTransformObjects(list);
        gp.MoveGumball();
        dc.Enabled = false;
        cmdrc = gp.CommandResult();
        if (cmdrc != Rhino.Commands.Result.Success)
          break;

        var getpoint_result = gp.Result();
        if (getpoint_result == Rhino.Input.GetResult.Point)
        {
          if (!dc.InRelocate)
          {
            Transform xform = dc.TotalTransform;
            dc.PreTransform = xform;
          }
          // update location of base gumball
          var gbframe = dc.Gumball.Frame;
          var baseFrame = base_gumball.Frame;
          baseFrame.Plane = gbframe.Plane;
          baseFrame.ScaleGripDistance = gbframe.ScaleGripDistance;
          base_gumball.Frame = baseFrame;
          continue;
        }
        if (getpoint_result == Rhino.Input.GetResult.Option)
        {
          if (gp.OptionIndex() == copy_optindx)
            bCopy = true;
          continue;
        }

        break;
      }

      dc.Enabled = false;
      if (dc.PreTransform != Transform.Identity)
      {
        Transform xform = dc.PreTransform;
        TransformObjects(list, xform, bCopy, bCopy);
      }
      doc.Views.Redraw();
      return cmdrc;
    }
  }


  class GetGumballXform : Rhino.Input.Custom.GetTransform
  {
    readonly Rhino.UI.Gumball.GumballDisplayConduit m_dc;
    public GetGumballXform(Rhino.UI.Gumball.GumballDisplayConduit dc)
    {
      m_dc = dc;
    }

    public override Transform CalculateTransform(Rhino.Display.RhinoViewport viewport, Point3d point)
    {
      if (m_dc.InRelocate)
      {
        // don't move objects while relocating gumball
        return m_dc.PreTransform;
      }

      return m_dc.TotalTransform;
    }

    protected override void OnMouseDown(Rhino.Input.Custom.GetPointMouseEventArgs e)
    {
      if (m_dc.PickResult.Mode != Rhino.UI.Gumball.GumballMode.None)
        return;
      m_dc.PickResult.SetToDefault();

      Rhino.Input.Custom.PickContext pick_context = new Rhino.Input.Custom.PickContext();
      pick_context.View = e.Viewport.ParentView;
      pick_context.PickStyle = Rhino.Input.Custom.PickStyle.PointPick;
      var xform = e.Viewport.GetPickTransform(e.WindowPoint);
      pick_context.SetPickTransform(xform);
      Rhino.Geometry.Line pick_line;
      e.Viewport.GetFrustumLine(e.WindowPoint.X, e.WindowPoint.Y, out pick_line);
      pick_context.PickLine = pick_line;
      pick_context.UpdateClippingPlanes();
      // pick gumball and, if hit, set getpoint dragging constraints.
      m_dc.PickGumball(pick_context, this);
    }

    protected override void OnMouseMove(Rhino.Input.Custom.GetPointMouseEventArgs e)
    {
      if( m_dc.PickResult.Mode == Rhino.UI.Gumball.GumballMode.None )
        return;

      m_dc.CheckShiftAndControlKeys();
      Rhino.Geometry.Line world_line;
      if (!e.Viewport.GetFrustumLine(e.WindowPoint.X, e.WindowPoint.Y, out world_line))
        world_line = Rhino.Geometry.Line.Unset;

      bool rc = m_dc.UpdateGumball(e.Point, world_line);
      if( rc )
        base.OnMouseMove(e);
    }
    
    protected override void OnDynamicDraw(Rhino.Input.Custom.GetPointDrawEventArgs e)
    {
      // Disable default GetTransform drawing by not calling the base class
      // implementation. All aspects of gumball display are handled by 
      // GumballDisplayConduit
      //base.OnDynamicDraw(e);
    }
    
    // lets user drag m_gumball around.
    public Rhino.Input.GetResult MoveGumball()
    {
      // Get point on a MouseUp event
      if( m_dc.PreTransform != Transform.Identity )
      {
        HaveTransform = true;
        Transform = m_dc.PreTransform;
      }
      SetBasePoint(m_dc.BaseGumball.Frame.Plane.Origin, false);

      // V5 uses a display conduit to provide display feedback
      // so shaded objects move
      ObjectList.DisplayFeedbackEnabled = true;
      if( Transform != Transform.Identity )
        ObjectList.UpdateDisplayFeedbackTransform(Transform);

      // Call Get with mouseUp set to true
      var rc = this.Get(true);

      // V5 uses a display conduit to provide display feedback
      // so shaded objects move
      ObjectList.DisplayFeedbackEnabled = false;
      return rc;
    }
  }
}

