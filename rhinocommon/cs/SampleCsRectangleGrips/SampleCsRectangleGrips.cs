using System;
using Rhino.DocObjects.Custom;
using Rhino.Geometry;

namespace SampleCsRectangleGrips
{
  public class SampleCsRectangleGrips : CustomObjectGrips
  {
    private readonly SampleCsRectangleGrip[] m_sample_cs_rectangle_grips;
    private Plane m_plane;
    private Point3d[] m_active_rectangle;
    private Point3d[] m_original_rectangle;
    private bool m_draw_rectangle;

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsRectangleGrips()
    {
      m_draw_rectangle = false;
      m_sample_cs_rectangle_grips = new SampleCsRectangleGrip[8];
      for (var i = 0; i < 8; i++)
        m_sample_cs_rectangle_grips[i] = new SampleCsRectangleGrip();
    }

    /// <summary>
    /// Creates grips
    /// </summary>
    public bool CreateGrips(PolylineCurve polylineCurve)
    {
      if (!SampleCsRectangleHelper.IsRectangle(polylineCurve))
        return false;

      if (GripCount > 0)
        return false;

      Plane plane;
      if (!polylineCurve.TryGetPlane(out plane))
        return false;
      m_plane = plane;

      m_active_rectangle = new Point3d[5];
      for (var i = 0; i < polylineCurve.PointCount; i++)
        m_active_rectangle[i] = polylineCurve.Point(i);

      m_original_rectangle = new Point3d[5];
      Array.Copy(m_active_rectangle, m_original_rectangle, 5);

      var line = new Line();
      for (var i = 0; i < 4; i++)
      {
        var gi = 2 * i;
        line.From = m_active_rectangle[i];
        line.To = m_active_rectangle[i + 1];
        m_sample_cs_rectangle_grips[gi].OriginalLocation = line.From;
        m_sample_cs_rectangle_grips[gi + 1].OriginalLocation = 0.5 * line.From + 0.5 * line.To;
        m_sample_cs_rectangle_grips[gi].Active = true;
        m_sample_cs_rectangle_grips[gi + 1].Active = true;
      }

      for (var i = 0; i < 8; i++)
        AddGrip(m_sample_cs_rectangle_grips[i]);

      return true;
    }

    /// <summary>
    /// Grips updater
    /// </summary>
    private void UpdateGrips()
    {
      if (NewLocation)
      {
        // If anything moved this time, the ones that didn't move will
        // be inactive for the rest of the drag
        for (var i = 0; i < 8; i++)
        {
          if (m_sample_cs_rectangle_grips[i].Active && m_sample_cs_rectangle_grips[i].Moved)
          {
            for (var j = 0; j < 8; j++)
            {
              if (!m_sample_cs_rectangle_grips[j].Moved)
                m_sample_cs_rectangle_grips[j].Active = false;
            }
            break;
          }
        }

        // First check corners
        for (var i = 0; i < 8; i += 2)
        {
          if (m_sample_cs_rectangle_grips[i].Active && m_sample_cs_rectangle_grips[i].Moved)
          {
            // if corner moves, middles are inactive for this drag
            m_sample_cs_rectangle_grips[(i + 1) % 8].Active = false;
            m_sample_cs_rectangle_grips[(i + 7) % 8].Active = false;
          }
        }

        // Second check middles
        for (var i = 1; i < 8; i += 2)
        {
          if (m_sample_cs_rectangle_grips[i].Active && m_sample_cs_rectangle_grips[i].Moved)
          {
            // if middle moves, corners are inactive for this drag
            m_sample_cs_rectangle_grips[(i + 1) % 8].Active = false;
            m_sample_cs_rectangle_grips[(i + 7) % 8].Active = false;
          }
        }

        // Set up transforms
        var world_to_plane = Transform.ChangeBasis(Plane.WorldXY, m_plane);
        var plane_to_world = Transform.ChangeBasis(m_plane, Plane.WorldXY);

        // Copy and transform active rectangle points
        var rectangle = new Point3d[5];
        Array.Copy(m_active_rectangle, rectangle, 5);
        for (var i = 0; i < 5; i++)
          rectangle[i].Transform(world_to_plane);

        // Copy and transform current grip locations
        var locations = new Point3d[8];
        for (var i = 0; i < 8; i++)
        {
          locations[i] = m_sample_cs_rectangle_grips[i].CurrentLocation;
          locations[i].Transform(world_to_plane);
        }

        // Figure out which x, y coordinates have been changed
        var x0 = rectangle[0].X;
        if (m_sample_cs_rectangle_grips[0].Active && m_sample_cs_rectangle_grips[0].Moved)
          x0 = locations[0].X;
        else if (m_sample_cs_rectangle_grips[6].Active && m_sample_cs_rectangle_grips[6].Moved)
          x0 = locations[6].X;
        else if (m_sample_cs_rectangle_grips[7].Active && m_sample_cs_rectangle_grips[7].Moved)
          x0 = locations[7].X;

        var x1 = rectangle[2].X;
        if (m_sample_cs_rectangle_grips[4].Active && m_sample_cs_rectangle_grips[4].Moved)
          x1 = locations[4].X;
        else if (m_sample_cs_rectangle_grips[2].Active && m_sample_cs_rectangle_grips[2].Moved)
          x1 = locations[2].X;
        else if (m_sample_cs_rectangle_grips[3].Active && m_sample_cs_rectangle_grips[3].Moved)
          x1 = locations[3].X;

        var y0 = rectangle[0].Y;
        if (m_sample_cs_rectangle_grips[0].Active && m_sample_cs_rectangle_grips[0].Moved)
          y0 = locations[0].Y;
        else if (m_sample_cs_rectangle_grips[2].Active && m_sample_cs_rectangle_grips[2].Moved)
          y0 = locations[2].Y;
        else if (m_sample_cs_rectangle_grips[1].Active && m_sample_cs_rectangle_grips[1].Moved)
          y0 = locations[1].Y;

        var y1 = rectangle[2].Y;
        if (m_sample_cs_rectangle_grips[4].Active && m_sample_cs_rectangle_grips[4].Moved)
          y1 = locations[4].Y;
        else if (m_sample_cs_rectangle_grips[6].Active && m_sample_cs_rectangle_grips[6].Moved)
          y1 = locations[6].Y;
        else if (m_sample_cs_rectangle_grips[5].Active && m_sample_cs_rectangle_grips[5].Moved)
          y1 = locations[5].Y;

        rectangle[0].X = rectangle[3].X = x0;
        rectangle[1].X = rectangle[2].X = x1;
        rectangle[0].Y = rectangle[1].Y = y0;
        rectangle[2].Y = rectangle[3].Y = y1;
        rectangle[4] = rectangle[0];

        // Copy and transform updated rectangle points
        Array.Copy(rectangle, m_active_rectangle, 5);
        for (var i = 0; i < 5; i++)
          m_active_rectangle[i].Transform(plane_to_world);

        // Apply rectangular constraints to grip locations
        var line = new Line();
        for (var i = 0; i < 4; i++)
        {
          var gi = 2 * i;
          line.From = m_active_rectangle[i];
          line.To = m_active_rectangle[i + 1];
          m_sample_cs_rectangle_grips[gi].Move(line.From);
          m_sample_cs_rectangle_grips[gi + 1].Move(0.5 * line.From + 0.5 * line.To);
        }

        m_draw_rectangle = true;
        NewLocation = false;
      }
    }

    /// <summary>
    /// CustomObjectGrips override
    /// </summary>
    protected override void OnReset()
    {
      m_draw_rectangle = false;
      Array.Copy(m_original_rectangle, m_active_rectangle, 5);
      base.OnReset();
    }

    /// <summary>
    /// CustomObjectGrips override
    /// </summary>
    protected override GeometryBase NewGeometry()
    {
      UpdateGrips();
      if (GripsMoved && m_draw_rectangle)
        return new PolylineCurve(m_active_rectangle);
      return null;
    }

    /// <summary>
    /// CustomObjectGrips override
    /// </summary>
    protected override void OnDraw(GripsDrawEventArgs args)
    {
      UpdateGrips();
      if (m_draw_rectangle && args.DrawDynamicStuff)
      {
        for (var i = 1; i < 5; i++)
        {
          var start = i - 1;
          var end = i;
          args.DrawControlPolygonLine(m_active_rectangle[start], m_active_rectangle[end], start, end);
        }
      }
      base.OnDraw(args);
    }
  }
}
