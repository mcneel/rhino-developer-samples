import Rhino;
import rhinoscriptsyntax as rs

def RunCommand():
  rc, corners = Rhino.Input.RhinoGet.GetRectangle()
  if rc <> Rhino.Commands.Result.Success:
      return rc

  plane = Rhino.Geometry.Plane(corners[0], corners[1], corners[2])
  u_dir = rs.Distance(corners[0], corners[1])
  v_dir = rs.Distance(corners[1], corners[2])
  rs.AddPlaneSurface(plane, u_dir, v_dir)

if __name__ == "__main__":
    RunCommand()
