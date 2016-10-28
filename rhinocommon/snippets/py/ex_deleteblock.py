import rhinoscriptsyntax as rs
from scriptcontext import doc

def Delete():
    blockName = rs.GetString("block to delete")
    instanceDefinition = doc.InstanceDefinitions.Find(blockName, True)
    if not instanceDefinition:
        print "{0} block does not exist".format(blockName)
        return
        
    rs.DeleteBlock(blockName)
    
if __name__ == "__main__":
    Delete()