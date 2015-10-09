from Rhino.Geometry import NurbsSurface, Point3d
from scriptcontext import doc

surface = NurbsSurface.CreateFromCorners(
  Point3d(5, 0, 0),
  Point3d(5, 5, 5),
  Point3d(0, 5, 0),
  Point3d(0, 0, 0));

doc.Objects.AddSurface(surface);
doc.Views.Redraw();