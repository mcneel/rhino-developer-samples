import Rhino
import scriptcontext
import System.Guid

def AddLinearDimension2():
    origin = Rhino.Geometry.Point3d(1,1,0)
    offset = Rhino.Geometry.Point3d(11,1,0)
    pt = Rhino.Geometry.Point3d((offset.X-origin.X)/2.0,3,0)
    plane = Rhino.Geometry.Plane.WorldXY
    plane.Origin = origin

    rc, u, v = plane.ClosestParameter(origin)
    ext1 = Rhino.Geometry.Point2d(u,v)
    rc, u, v = plane.ClosestParameter(offset)
    ext2 = Rhino.Geometry.Point2d(u,v)
    rc, u, v = plane.ClosestParameter(pt)
    linePt = Rhino.Geometry.Point2d(u,v)

    dimension = Rhino.Geometry.LinearDimension(plane, ext1, ext2, linePt)
    if scriptcontext.doc.Objects.AddLinearDimension(dimension)!=System.Guid.Empty:
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure

if __name__=="__main__":
    AddLinearDimension2()
