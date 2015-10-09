import Rhino
import scriptcontext

def CreateBlock():
    # Select objects to define block
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt( "Select objects to define block" )
    go.ReferenceObjectSelect = False
    go.SubObjectSelect = False
    go.GroupSelect = True

    # Phantoms, grips, lights, etc., cannot be in blocks.
    forbidden_geometry_filter = Rhino.DocObjects.ObjectType.Light | Rhino.DocObjects.ObjectType.Grip | Rhino.DocObjects.ObjectType.Phantom
    geometry_filter = forbidden_geometry_filter ^ Rhino.DocObjects.ObjectType.AnyObject
    go.GeometryFilter = geometry_filter
    go.GetMultiple(1, 0)
    if go.CommandResult() != Rhino.Commands.Result.Success:
        return go.CommandResult()

    # Block base point
    rc, base_point = Rhino.Input.RhinoGet.GetPoint("Block base point", False)
    if rc != Rhino.Commands.Result.Success: return rc

    # Block definition name
    rc, idef_name = Rhino.Input.RhinoGet.GetString("Block definition name", False, "")
    if rc != Rhino.Commands.Result.Success: return rc
    # Validate block name
    idef_name = idef_name.strip()
    if not idef_name: return Rhino.Commands.Result.Nothing

    # See if block name already exists
    existing_idef = scriptcontext.doc.InstanceDefinitions.Find(idef_name, True)
    if existing_idef:
        print "Block definition", idef_name, "already exists"
        return Rhino.Commands.Result.Nothing

    # Gather all of the selected objects
    objrefs = go.Objects()
    geometry = [item.Object().Geometry for item in objrefs]
    attributes = [item.Object().Attributes for item in objrefs]

    # Add the instance definition
    idef_index = scriptcontext.doc.InstanceDefinitions.Add(idef_name, "", base_point, geometry, attributes)

    if idef_index<0:
        print "Unable to create block definition", idef_name
        return Rhino.Commands.Result.Failure
    return Rhino.Commands.Result.Failure


if __name__=="__main__":
    CreateBlock()
