################################################################################
# SampleParseTextFields.py
# Copyright (c) 2020 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext


def SampleParseTextFields():
    
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select object")
    go.Get()
    if(go.CommandResult() != Rhino.Commands.Result.Success):
        return go.CommandResult()

    rhino_object = go.Object(0).Object()
    rhino_object_id = str(rhino_object.Id)

#Add some user text to the objects attributes with the Area() TextField
    rhino_object.Attributes.SetUserString("Area","%<Area(\"" + rhino_object_id+ "\")>%")
    rhino_object.Attributes.SetUserString("ObjectName","%<ObjectName(\"" + rhino_object_id + "\")>%")
    rhino_object.Attributes.SetUserString("ObjectLayer","%<ObjectLayer(\"" + rhino_object_id + "\")>%")

    attributes = rhino_object.Attributes;
    kvps = attributes.GetUserStrings();

#Grab all of the user text key values from the object that we just assigned and parse them through 
#Rhino.RhinoApp.ParseTextField()
    for i in range(len(kvps)):
       value = kvps[i]
       parsed = Rhino.RhinoApp.ParseTextField(value,rhino_object,None)
       Rhino.RhinoApp.WriteLine(kvps[i] + " " + parsed)

#Call the textfield Area() directly and use the unit system overload method to return the results in Millimeter
#Rhino.Runtime.TextFields contains all of the textfield functions
    area_value = Rhino.Runtime.TextFields.Area(rhino_object_id,str(Rhino.UnitSystem.Millimeters))
    Rhino.RhinoApp.WriteLine("Area direct call: " + str(area_value))
    
    object_name = Rhino.Runtime.TextFields.ObjectName(rhino_object_id)
    Rhino.RhinoApp.WriteLine("ObjectName direct call: " + str(area_value))
    
    object_layer = Rhino.Runtime.TextFields.ObjectLayer(rhino_object_id)
    Rhino.RhinoApp.WriteLine("ObjectLayer direct call: " + str(area_value))
    
    
    return Rhino.Commands.Result.Success

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == '__main__' ):
    SampleParseTextFields()