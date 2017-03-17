################################################################################
# SampleImportNamedViews.py -- May 2014
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import scriptcontext
import rhinoscriptsyntax as rs
import Rhino

# Demonstrates how to import named views from a 3dm file.
def SampleImportNamedViews():
    filename = rs.OpenFileName("Open", "Rhino 3D Models (*.3dm)|*.3dm||")
    if filename:
        f = Rhino.FileIO.File3dm.Read(filename)
        if (f.NamedViews.Count > 0):
            for vi in f.NamedViews:
                scriptcontext.doc.NamedViews.Add(vi)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleImportNamedViews()