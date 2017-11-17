################################################################################
# SampleGetLiteralString.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino

# Returns the string, that can contain spaces, entered by the user.
def SampleGetLiteralString():
    gs = Rhino.Input.Custom.GetString()
    gs.SetCommandPrompt("Enter some string")
    gs.GetLiteralString()
    if (gs.CommandResult() == Rhino.Commands.Result.Success):
        print gs.StringResult()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleGetLiteralString()