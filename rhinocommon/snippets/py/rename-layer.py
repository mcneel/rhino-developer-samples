import rhinoscriptsyntax as rs
from scriptcontext import doc

def rename():
    layerName = rs.GetString("Name of layer to rename")

    matchingLayers = [layer for layer in doc.Layers if layer.Name == layerName]

    layerToRename = None
    if len(matchingLayers) == 0:
        print "Layer \"{0}\" does not exist.".format(layerName)
        return
    if len(matchingLayers) == 1:
        layerToRename = matchingLayers[0]
    elif len(matchingLayers) > 1:
        i = 0;
        for layer in matchingLayers:
            print "({0}) {1}".format(
                i+1, matchingLayers[i].FullPath.replace("::", "->"))
            i += 1

        selectedLayer = rs.GetInteger(
            "which layer?", -1, 1, len(matchingLayers))
        if selectedLayer == None:
            return
        layerToRename = matchingLayers[selectedLayer - 1]

    layerName = rs.GetString("New layer name")
    layerToRename.Name = layerName
    layerToRename.CommitChanges()
    return

if __name__ == "__main__":
    rename()
