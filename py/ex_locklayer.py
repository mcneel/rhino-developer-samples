import rhinoscriptsyntax as rs
from scriptcontext import doc

def lock():
    layerName = rs.GetString("Name of layer to lock")
    
    matchingLayers = [layer for layer in doc.Layers if layer.Name == layerName]
    
    layerToLock = None
    if len(matchingLayers) == 0:
        print "Layer \"{0}\" does not exist.".format(layerName)
    elif len(matchingLayers) == 1:
        layerToLock = matchingLayers[0]
    elif len(matchingLayers) > 1:
        i = 0;
        for layer in matchingLayers:
            print "({0}) {1}".format(i+1, matchingLayers[i].FullPath.replace("::", "->"))
            i += 1
            
        selectedLayer = rs.GetInteger("which layer?", -1, 1, len(matchingLayers))
        if selectedLayer == None:
            return
        layerToLock = matchingLayers[selectedLayer - 1]
        
    if layerToLock.IsLocked:
        print "layer {0} is already locked.".format(layerToLock.FullPath)
    else:
        layerToLock.IsLocked = True
        layerToLock.CommitChanges()
          
if __name__ == "__main__":
    lock()
        