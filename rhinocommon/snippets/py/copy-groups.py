import Rhino
from Rhino.Commands import *
from scriptcontext import doc

def RhinoUpdateObjectGroups(obj, group_map):
    if obj == None: return

    attrib_group_count = obj.Attributes.GroupCount
    if attrib_group_count == 0: return

    doc = obj.Document
    if doc == None: return

    groups = doc.Groups

    group_count = groups.Count
    if group_count == 0: return

    if group_map.Count == 0:
        for i in range(0, group_count):
            group_map.append(-1)

    attributes = obj.Attributes
    group_list = attributes.GetGroupList()
    if group_list == None: return
    attrib_group_count = group_list.Length

    for i in range(0, attrib_group_count):
        old_group_index = group_list[i]
        new_group_index = group_map[old_group_index]
        if new_group_index == -1:
            new_group_index = doc.Groups.Add()
            group_map[old_group_index] = new_group_index
        group_list[i] = new_group_index

    attributes.RemoveFromAllGroups()
    for i in range(0, attrib_group_count):
        attributes.AddToGroup(group_list[i])

    obj.CommitChanges()

def RunCommand():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects to copy in place")
    go.GroupSelect = True
    go.SubObjectSelect = False
    go.GetMultiple(1, 0)
    if go.CommandResult() != Result.Success:
        return go.CommandResult()

    xform = Rhino.Geometry.Transform.Identity
    group_map = []

    for obj_ref in go.Objects():
        if obj_ref != None:
            obj = obj_ref.Object()
            duplicate = doc.Objects.Transform(obj_ref.ObjectId, xform, False)
            RhinoUpdateObjectGroups(obj, group_map)
    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
