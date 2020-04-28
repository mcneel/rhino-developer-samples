using System;
using Rhino;
using Rhino.Collections;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  #region GetSetPointTransform class
  /// <summary>
  /// GetSetPointTransform
  /// </summary>
  internal class GetSetPointTransform : GetTransform
  {
    public GetSetPointTransform(bool bSetX, bool bSetY, bool bSetZ)
    {
      SetX = bSetX;
      SetY = bSetY;
      SetZ = bSetZ;
    }

    public bool SetX { get; set; }
    public bool SetY { get; set; }
    public bool SetZ { get; set; }

    public override Transform CalculateTransform(RhinoViewport viewport, Point3d point)
    {
      Transform xform = new Transform(0);

      if (SetX)
        xform[0, 3] = point.X;
      else
        xform[0, 0] = 1.0;

      if (SetY)
        xform[1, 3] = point.Y;
      else
        xform[1, 1] = 1.0;

      if (SetZ)
        xform[2, 3] = point.Z;
      else
        xform[2, 2] = 1.0;

      xform[3, 3] = 1.0;

      return xform;
    }
  }
  #endregion


  #region SampleCsSetPoint command
  /// <summary>
  /// SampleCsSetPoint
  /// </summary>
  public class SampleCsSetPoint : TransformCommand
  {
    public override string EnglishName => "SampleCsSetPoint";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Result rc = Result.Nothing;

      // Select objects to rotate
      TransformObjectList list = new TransformObjectList();
      rc = SelectObjects("Select objects to transform", list);
      if (rc != Result.Success)
        return rc;

      OptionToggle opt_xset = new OptionToggle(true, "No", "Yes");
      OptionToggle opt_yset = new OptionToggle(true, "No", "Yes");
      OptionToggle opt_zset = new OptionToggle(true, "No", "Yes");

      // Second reference point
      GetSetPointTransform gx = new GetSetPointTransform(true, true, true);
      gx.SetCommandPrompt("Location for points");
      gx.AddTransformObjects(list);
      for (;;)
      {
        gx.ClearCommandOptions();
        gx.AddOptionToggle("XSet", ref opt_xset);
        gx.AddOptionToggle("YSet", ref opt_yset);
        gx.AddOptionToggle("ZSet", ref opt_zset);

        var res = gx.GetXform();

        if (res == GetResult.Point)
        {
          RhinoView view = gx.View();
          rc = (null != view) ? Result.Success : Result.Failure;
          if (rc == Result.Success)
          {
            Transform xform = gx.CalculateTransform(view.ActiveViewport, gx.Point());
            rc = (xform.IsValid) ? Result.Success : Result.Failure;
            if (rc == Result.Success)
            {
              TransformObjects(list, xform, false, false);
              doc.Views.Redraw();
            }
          }
        }
        else if (res == GetResult.Option)
        {
          gx.SetX = opt_xset.CurrentValue;
          gx.SetY = opt_yset.CurrentValue;
          gx.SetZ = opt_zset.CurrentValue;
          continue;
        }
        else
        {
          rc = Result.Cancel;
        }

        break;
      }

      return rc;
    }
  }
  #endregion
}
