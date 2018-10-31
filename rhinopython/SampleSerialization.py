################################################################################
# SampleSerialization.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import clr
import System
import Rhino
import rhinoscriptsyntax as rs
import scriptcontext as sc

################################################################################
# Converts GeometryBase object to an array of bytes
################################################################################
def GeometryBaseToBytes(geometry):
    with System.IO.MemoryStream() as stream:
        formatter = System.Runtime.Serialization.Formatters.Binary.BinaryFormatter()
        formatter.Serialize(stream, geometry);
        return stream.ToArray()

################################################################################
# Converts an array of bytes to GeometryBase object
################################################################################
def BytesToGeometryBase(bytes):
    with System.IO.MemoryStream() as stream:
        formatter = System.Runtime.Serialization.Formatters.Binary.BinaryFormatter()
        stream.Write(bytes, 0, bytes.Length)
        stream.Seek(0, System.IO.SeekOrigin.Begin)
        obj = formatter.Deserialize(stream)
        geometry = clr.Convert(obj, Rhino.Geometry.GeometryBase)
        if geometry and geometry.IsValid:
            return geometry

################################################################################
# Main function
################################################################################
def SampleSerialization():
    id = rs.GetObject('Select object')
    geometry = rs.coercegeometry(id)
    print(geometry)
    
    bytes = GeometryBaseToBytes(geometry)
    print(bytes)
    
    new_geometry = BytesToGeometryBase(bytes)
    print(new_geometry)
    
    new_id = sc.doc.Objects.Add(new_geometry)
    print(new_id)

################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    SampleSerialization()
