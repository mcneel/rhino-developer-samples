import System.Collections.Generic;
import Rhino;
from Rhino.Commands import Result
from Rhino.Geometry import Point3d
from scriptcontext import doc

def RunCommand():
  points = [Point3d(0, 0, 0), Point3d(0, 0, 1), Point3d(0, 1, 0), 
    Point3d(0, 1, 1), Point3d(1, 0, 0), Point3d(1, 0, 1), 
    Point3d(1, 1, 0), Point3d(1, 1, 1)]

  print "Before sort ..."
  for point in points:
    print "point: {0}".format(point)

  sorted_points = Point3d.SortAndCullPointList(
      points, doc.ModelAbsoluteTolerance);

  print "After sort ..."
  for point in sorted_points:
    print "point: {0}".format(point)

  doc.Objects.AddPoints(sorted_points);
  doc.Views.Redraw();
  return Result.Success;

if __name__ == "__main__":
  RunCommand()