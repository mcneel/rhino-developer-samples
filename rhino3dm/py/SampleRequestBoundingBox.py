# Sample to get bounding box of remote 3dm file
import rhino3dm
import requests # pip install requests
req = requests.get("https://files.mcneel.com/rhino3dm/models/RhinoLogo.3dm")
model = rhino3dm.File3dm.FromByteArray(req.content)
for obj in model.Objects:
    geometry = obj.Geometry
    bbox = geometry.GetBoundingBox()
    print("{}, {}".format(bbox.Min, bbox.Max))
