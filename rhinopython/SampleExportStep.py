################################################################################
# ExportStep.py 
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import rhinoscriptsyntax as rs

# Demonstrates how to export a selected surface or polysurface
# to a STEP file.
def SampleExportStep():
    RH_SURFACE = 8
    RH_POLYSURFACE = 16
    filter = RH_SURFACE + RH_POLYSURFACE
    # Pick a surface or poysurface to export
    objectId = rs.GetObject('Select surface or polysurface to export', filter, True, True)
    if objectId:
        # Prompt for name of file to save
        filename = rs.SaveFileName('Export STEP', 'STEP Files (*.stp)|*.stp||')
        if filename:
            # Surround string with double-quotes (in case it contains spaces)
            filename = chr(34)+ filename + chr(34)
            script = '_-Export ' + filename + ' _Enter'
            # Script the Export command
            rs.Command(script, False)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleExportStep()