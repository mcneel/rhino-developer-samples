################################################################################
# SampleDescribeObject.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax as rs

# Reports detailed technical information about the data structure
# of the selected objects.
def SampleDumpGeometry():
    obj_id = rs.GetObject("Select object to list")
    if obj_id:
        geom = rs.coercegeometry(obj_id)
        dump = Rhino.Runtime.HostUtils.DebugDumpToString(geom)
        print dump
        
# Reports object properties details
def SampleDescribeObject():
    obj_id = rs.GetObject("Select object for displaying detailed object properties")
    if obj_id:
        obj = rs.coercerhinoobject(obj_id)
        log = Rhino.FileIO.TextLog()
        obj.Description(log)
        print log

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleDescribeObject()