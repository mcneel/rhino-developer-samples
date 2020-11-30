# Sample to get bounding box of remote 3dm file
from rhino3dm import *
import requests # pip install requests
req = requests.get("https://files.mcneel.com/TEST/Rhino Logo.3dm")
model = File3dm.FromByteArray(req.content)
for obj in model.Objects:
    geometry = obj.Geometry
    bbox = geometry.GetBoundingBox()
    print("{}, {}".format(bbox.Min, bbox.Max))
