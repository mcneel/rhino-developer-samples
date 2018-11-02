################################################################################
# SampleSerialization.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import clr
import System
import System.Collections.Generic.IEnumerable as IEnumerable
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
    
    # Select baseeobject
    base_id = rs.GetObject('Select object')
    base_obj = rs.coercerhinoobject(base_id)
    
    # Select object to embed
    id = rs.GetObject('Select object to embed')
    geometry = rs.coercegeometry(id)
    print(geometry)
    
    # Convert geometry to bytes
    bytes = GeometryBaseToBytes(geometry)
    print(bytes)
    
    # Add bytes to base object's dictionary
    key = 'test'
    base_obj.Attributes.UserDictionary.Set.Overloads[str,IEnumerable[System.Byte]](key ,bytes)
    
    # Get bytes from base object
    new_bytes = base_obj.Attributes.UserDictionary.GetBytes(key)
    print(new_bytes)

    # Convert bytes to geometry
    new_geometry = BytesToGeometryBase(new_bytes)
    print(new_geometry)
    
    # Add geometry to document
    new_id = sc.doc.Objects.Add(new_geometry)
    print(new_id)
    
    # Remove bytes from dictionary
    base_obj.Attributes.UserDictionary.Remove(key)

################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    SampleSerialization()
