using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsDir command
  /// </summary>
  public class SampleCsDir : Command
  {
    public override string EnglishName => "SampleCsDir";

    /// <summary>
    /// Command.RunCommand override
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select surface for direction display");
      go.GeometryFilter = ObjectType.Surface;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      ObjRef objRef = go.Object(0);
      RhinoObject rhinoObject = objRef.Object();
      Brep brep = objRef.Brep();
      if (null == rhinoObject || null == brep || 1 != brep.Faces.Count)
        return Result.Failure;

      bool objectWasPreselected = go.ObjectsWerePreselected;
      rhinoObject.Select(false);

      bool ureverse = false;
      bool vreverse = false;
      bool swapuv = false;
      bool flip = false;
      bool rc = ShowSurfaceDir(brep, rhinoObject.Attributes.WireDensity, ref ureverse, ref vreverse, ref swapuv, ref flip);
      if (rc && (ureverse || vreverse || swapuv || flip) && !rhinoObject.IsReference)
      {
        Brep brepCopy = brep.DuplicateBrep();
        if (null != brepCopy)
        {
          BrepFace faceCopy = brepCopy.Faces[0];
          if (ureverse)
            faceCopy.Reverse(0);
          if (vreverse)
            faceCopy.Reverse(1);
          if (swapuv)
            faceCopy.Transpose();
          if (flip)
            brepCopy.Flip();

          Guid objectId = objRef.ObjectId;
          doc.Objects.Replace(objectId, brepCopy);
          if (objectWasPreselected)
          {
            rhinoObject = doc.Objects.FindId(objectId);
            if (null != rhinoObject)
              rhinoObject.Select(true);
          }
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    private bool ShowSurfaceDir(Brep brep, int wireDensity, ref bool ureverse, ref bool vreverse, ref bool swapuv, ref bool flip)
    {
      bool rc = false;

      if (null == brep)
        return rc;

      bool isSolid = brep.IsSolid;

      SurfaceDirPoint gp = new SurfaceDirPoint(brep, wireDensity);
      gp.SetCommandPrompt("Press Enter when done");
      gp.AcceptNothing(true);

      while (true)
      {
        gp.ClearCommandOptions();
        int ureverseIndex = gp.AddOption("UReverse");
        int vreverseIndex = gp.AddOption("VReverse");
        int swapuvIndex = gp.AddOption("SwapUV");
        int flipIndex = (!isSolid) ? gp.AddOption("Flip") : -1;

        gp.UReverse = ureverse;
        gp.VReverse = vreverse;
        gp.SwapUV = swapuv;
        gp.Flip = flip;

        GetResult result = gp.Get();

        if (result == GetResult.Cancel)
        {
          rc = false;
          break;
        }

        if (result == GetResult.Point)
        {
          if (!isSolid)
            flip = !flip;
          continue;
        }

        if (result == GetResult.Option)
        {
          int index = gp.Option().Index;
          if (index == ureverseIndex)
          {
            if (!swapuv)
              ureverse = !ureverse;
            else
              vreverse = !vreverse;
          }
          else if (index == vreverseIndex)
          {
            if (!swapuv)
              vreverse = !vreverse;
            else
              ureverse = !ureverse;
          }
          else if (index == swapuvIndex)
          {
            swapuv = !swapuv;
          }
          else if (index == flipIndex && !isSolid)
          {
            flip = !flip;
          }
          continue;
        }

        rc = true;
        break;
      }

      return rc;
    }
  }

  /// <summary>
  /// SurfaceDirPoint class
  /// </summary>
  internal class SurfaceDirPoint : GetPoint
  {
    private readonly BrepFace m_face;
    private Vector3d m_udir;
    private Vector3d m_vdir;
    private Vector3d m_normal;

    /// <summary>
    /// Constructor
    /// </summary>
    public SurfaceDirPoint(Brep brep, int wireDensity)
    {
      m_face = brep.Faces[0];
      Constrain(brep, wireDensity, -1, false);
    }

    public bool UReverse { get; set; }
    public bool VReverse { get; set; }
    public bool SwapUV { get; set; }
    public bool Flip { get; set; }

    /// <summary>
    /// GetPoint.OnMouseMove override
    /// </summary>
    protected override void OnMouseMove(GetPointMouseEventArgs args)
    {
      base.OnMouseMove(args);

      m_udir = Vector3d.Zero;
      m_vdir = Vector3d.Zero;
      m_normal = Vector3d.Zero;

      if (m_face.ClosestPoint(args.Point, out double u, out double v))
      {
        // BrepFace.NormalAt takes BrepFace.OrientationIsReversed into account
        m_normal = m_face.NormalAt(u, v);

        if (Flip)
          m_normal = -m_normal;

        if (m_face.Evaluate(u, v, 1, out Point3d point, out Vector3d[] derivatives) && 2 == derivatives.Length)
        {
          m_udir = derivatives[0];
          m_udir.Unitize();

          m_vdir = derivatives[1];
          m_vdir.Unitize();

          if (UReverse)
            m_udir = -m_udir;

          if (VReverse)
            m_vdir = -m_vdir;

          if (SwapUV)
            (m_vdir, m_udir) = (m_udir, m_vdir);

          double slopeAngle = RhinoMath.ToDegrees(Math.Asin(m_normal * args.Viewport.ConstructionPlane().ZAxis));
          Rhino.UI.StatusBar.SetNumberPane(slopeAngle);
        }
      }
    }

    /// <summary>
    /// GetPoint.OnDynamicDraw override
    /// </summary>
    protected override void OnDynamicDraw(GetPointDrawEventArgs args)
    {
      base.OnDynamicDraw(args);

      if (!m_normal.IsTiny())
        args.Display.DrawDirectionArrow(args.CurrentPoint, m_normal, Rhino.ApplicationSettings.AppearanceSettings.TrackingColor);

      if (!m_udir.IsTiny())
        args.Display.DrawDirectionArrow(args.CurrentPoint, m_udir, System.Drawing.Color.FromArgb(211, 0, 0));

      if (!m_vdir.IsTiny())
        args.Display.DrawDirectionArrow(args.CurrentPoint, m_vdir, System.Drawing.Color.FromArgb(29, 175, 29));
    }
  }
}