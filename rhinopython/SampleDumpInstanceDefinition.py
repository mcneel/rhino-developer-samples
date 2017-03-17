################################################################################
# SampleDumpInstanceDefinition.py -- December 2015
# Copyright (c) Robert McNeel & Associates
################################################################################
import Rhino
import System
import scriptcontext

# Recursive function to print the contents of an instance definition
def DumpInstanceDefinition(idef, indent):
    if None != idef and not idef.IsDeleted:
        line = u"\u2500"
        corner = u"\u2514"
        node = line if (0 == indent) else corner
        str = " " * indent * 2
        Rhino.RhinoApp.WriteLine(str.Format("{0}{1} Instance definition {2} = {3}", str, node, idef.Index, idef.Name))
        idef_object_count = idef.ObjectCount
        if idef_object_count > 0:
            indent += 1
            str = " " * indent * 2
            i = 0
            while i < idef_object_count:
                obj = idef.Object(i)
                if None != obj:
                    if type(obj) is Rhino.DocObjects.InstanceObject:
                        indent = DumpInstanceDefinition(obj.InstanceDefinition, indent)
                    else:
                        Rhino.RhinoApp.WriteLine(str.Format("{0}{1} Object {2} = {3}", str, corner, i, obj.ShortDescription(False)))
                i += 1
            indent -= 1
            return indent

# The 'main' function
def SampleDumpInstanceDefinition():
    idef_table = scriptcontext.doc.InstanceDefinitions;
    idef_count = idef_table.ActiveCount;
    if idef_count > 0:
        indent = 0;
        i = 0
        while i < idef_count:
            indent = DumpInstanceDefinition(idef_table[i], indent)
            i += 1

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleDumpInstanceDefinition() # Call the function defined above