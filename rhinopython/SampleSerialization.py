################################################################################
# SampleSerialization.py
# Copyright (c) 2013-2026, Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
# ! python3
import System
from System.Collections.Generic import IEnumerable
import Rhino
import rhinoscriptsyntax as rs
import scriptcontext as sc

################################################################################
# Converts a GeometryBase object to an array of bytes.
# BinaryFormatter is unsupported on modern .NET (Rhino 9), so serialize the
# geometry to JSON and return its UTF-8 bytes.
################################################################################
def GeometryBaseToBytes(geometry):
    options = Rhino.FileIO.SerializationOptions()
    json = geometry.ToJSON(options)
    return System.Text.Encoding.UTF8.GetBytes(json)

################################################################################
# Converts an array of bytes to a GeometryBase object
################################################################################
def BytesToGeometryBase(bytes):
    json = System.Text.Encoding.UTF8.GetString(bytes)
    geometry = Rhino.Geometry.GeometryBase.FromJSON(json)
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
    base_obj.Attributes.UserDictionary.Set.Overloads[System.String, IEnumerable[System.Byte]](key, bytes)
    
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
