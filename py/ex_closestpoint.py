import Rhino
import rhinoscriptsyntax as rs

# data passed to the RTree's SearchCallback function that
# we can use for recording what is going on
class SearchData:
    def __init__(self, mesh, point):
        self.HitCount = 0
        self.Mesh = mesh
        self.Point = point
        self.Index = -1
        self.Distance = 0
        

def SearchCallback(sender, e):
    data = e.Tag
    data.HitCount += 1
    vertex = data.Mesh.Vertices[e.Id]
    distance = data.Point.DistanceTo(vertex)
    if data.Index == -1 or data.Distance > distance:
        # shrink the sphere to help improve the test
        e.SearchSphere = Rhino.Geometry.Sphere(data.Point, distance)
        data.Index = e.Id
        data.Distance = distance

def RunSearch():
    id = rs.GetObject("select mesh", rs.filter.mesh)
    mesh = rs.coercemesh(id)
    if mesh:
        rs.UnselectObject(id)
        tree = Rhino.Geometry.RTree()
        # I can add a RhinoCommon function that just builds an rtree from the
        # vertices in one quick shot, but for now...
        for i,vertex in enumerate(mesh.Vertices): tree.Insert(vertex, i)
        
        while(True):
            point = rs.GetPoint("test point")
            if not point: break
            
            data = SearchData(mesh, point)
            # Use the first vertex in the mesh to define a start sphere
            distance = point.DistanceTo(mesh.Vertices[0])
            sphere = Rhino.Geometry.Sphere(point, distance * 1.1)
            if tree.Search(sphere, SearchCallback, data):
                rs.AddPoint(mesh.Vertices[data.Index])
                print "Found point in {0} tests".format(data.HitCount)

if __name__=="__main__":
    RunSearch()
