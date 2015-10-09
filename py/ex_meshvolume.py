from Rhino.Input.Custom import *
from Rhino.DocObjects import ObjectType
from Rhino.Geometry import *
from Rhino.Commands import Result

def RunCommand():
  gm = GetObject()
  gm.SetCommandPrompt("Select solid meshes for volume calculation")
  gm.GeometryFilter = ObjectType.Mesh
  gm.GeometryAttributeFilter = GeometryAttributeFilter.ClosedMesh
  gm.SubObjectSelect = False
  gm.GroupSelect = True
  gm.GetMultiple(1, 0)
  if gm.CommandResult() != Result.Success:
    return gm.CommandResult()

  volume = 0.0
  volume_error = 0.0
  for obj_ref in gm.Objects():
    if obj_ref.Mesh() != None:
      mass_properties = VolumeMassProperties.Compute(obj_ref.Mesh())
      if mass_properties != None:
        volume += mass_properties.Volume
        volume_error += mass_properties.VolumeError

  print "Total volume = {0:f} (+/- {1:f})".format(volume, volume_error)
  return Result.Success

if __name__ == "__main__":
  RunCommand()