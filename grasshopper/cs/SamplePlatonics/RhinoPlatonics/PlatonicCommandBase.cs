using Rhino.Geometry;

namespace RhinoPlatonics
{
  /// <summary>
  /// The base class for all commands
  /// </summary>
  public abstract class PlatonicCommandBase : Rhino.Commands.Command
  {
    PlatonicsCommon.PlatonicBase m_platonic;
    double m_length;
    bool m_create_solid;
    bool m_group_faces;

    /// <summary>
    /// Public constructor
    /// </summary>
    protected PlatonicCommandBase()
    {
      m_length = 1.0;
      m_create_solid = true;
      m_group_faces = true;
    }

    /// <summary>
    /// Returns a new platonic-inherited object
    /// </summary>
    protected abstract PlatonicsCommon.PlatonicBase NewPlatonic();

    /// <summary>
    /// Returns the command's platonic-inherited object
    /// </summary>
    public PlatonicsCommon.PlatonicBase Platonic
    {
      get { return m_platonic ?? (m_platonic = NewPlatonic()); }
    }

    /// <summary>
    /// The English command name, which cooresponds to the name of the platonic
    /// </summary>
    public override string EnglishName
    {
      get { return Platonic.Name; }
    }

    /// <summary>
    /// Executes the platonic command
    /// </summary>
    protected override Rhino.Commands.Result RunCommand(Rhino.RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      // Get the platonic geometry
      Rhino.Geometry.Brep[] faces = Platonic.Faces();
      if (null == faces)
        return Rhino.Commands.Result.Failure;

      // Prompt for center point
      string prompt = string.Format("Center of {0}", Platonic.DisplayName.ToLower());
      Rhino.Geometry.Point3d origin;
      Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetPoint(prompt, false, out origin);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      double length = m_length;
      bool create_solid = m_create_solid;
      bool group_faces = m_group_faces;

      // Prompt for edge length
      PlatonicGetPoint get = new PlatonicGetPoint(faces, origin);
      get.SetCommandPrompt(Platonic.Prompt);
      get.SetDefaultNumber(length);
      get.SetBasePoint(origin, true);
      get.DrawLineFromPoint(origin, true);
      get.AcceptNumber(true, false);

      Rhino.Geometry.Transform xform = new Rhino.Geometry.Transform(1.0);
      rc = Rhino.Commands.Result.Cancel;

      for (; ; )
      {
        get.ClearCommandOptions();

        Rhino.Input.Custom.OptionToggle output_toggle = null;
        Rhino.Input.Custom.OptionToggle group_toggle = null;

        output_toggle = new Rhino.Input.Custom.OptionToggle(create_solid, "Faces", "Solid");
        get.AddOptionToggle("Output", ref output_toggle);

        if (!create_solid)
        {
          group_toggle = new Rhino.Input.Custom.OptionToggle(group_faces, "No", "Yes");
          get.AddOptionToggle("Group", ref group_toggle);
        }

        Rhino.Input.GetResult res = get.Get();

        if (res == Rhino.Input.GetResult.Point)
        {
          if (get.CalculateTransform(get.View().ActiveViewport, get.Point(), ref xform))
          {
            m_length = get.Length;
            rc = Rhino.Commands.Result.Success;
            break;
          }
        }
        else if (res == Rhino.Input.GetResult.Number)
        {
          length = System.Math.Abs(get.Number());
          if (get.CalculateTransform(get.View().ActiveViewport, length, ref xform))
          {
            m_length = get.Length;
            rc = Rhino.Commands.Result.Success;
            break;
          }
        }
        else if (res == Rhino.Input.GetResult.Option)
        {
          if (null != output_toggle && create_solid != output_toggle.CurrentValue)
            create_solid = output_toggle.CurrentValue;

          if (null != group_toggle && group_faces != group_toggle.CurrentValue)
            group_faces = group_toggle.CurrentValue;

          continue;
        }
        else
        {
          break;
        }
      }

      // If we got here, then we have the center point and edge length,
      // any any command options. Time to add the geomtry to the document.
      if (rc == Rhino.Commands.Result.Success && xform.IsValid)
      {
        m_create_solid = create_solid;
        m_group_faces = group_faces;

        Rhino.DocObjects.ObjectAttributes attribs = doc.CreateDefaultAttributes();
        attribs.Name = Platonic.Name;
        attribs.WireDensity = 0; // solid looks "nicer" with wires turned off...

        if (m_create_solid)
        {
          Rhino.Geometry.Brep[] breps = Rhino.Geometry.Brep.JoinBreps(faces, 2.1 * doc.ModelAbsoluteTolerance);
          foreach (Brep t in breps)
          {
            if (null == t) continue;
            t.Transform(xform);
            doc.Objects.AddBrep(t, attribs, null, false, false);
          }
        }
        else
        {
          if (m_group_faces)
            attribs.AddToGroup(doc.Groups.Add());

          foreach (Brep t in faces)
          {
            if (null == t) continue;
            t.Transform(xform);
            doc.Objects.AddBrep(t, attribs, null, false, false);
          }
        }

        doc.Views.Redraw();
      }

      return Rhino.Commands.Result.Success;
    }
  }
}
