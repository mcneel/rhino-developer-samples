from scriptcontext import doc
import Rhino

def RunCommand():
    instanceDefinitions = doc.InstanceDefinitions
    instanceDefinitionCount = instanceDefinitions.Count

    if instanceDefinitionCount == 0:
        print "Document contains no instance definitions."
        return

    dump = Rhino.FileIO.TextLog()
    dump.IndentSize = 4

    for i in range(0, instanceDefinitionCount):
        DumpInstanceDefinition(instanceDefinitions[i], dump, True)

    print dump.ToString()

def DumpInstanceDefinition(instanceDefinition, dump, isRoot):
    if instanceDefinition != None and not instanceDefinition.IsDeleted:
        if isRoot:
            node = '-'
        else:
            node = '+'
        dump.Print(u"{0} Instance definition {1} = {2}\n".format(node, instanceDefinition.Index, instanceDefinition.Name))

        if instanceDefinition.ObjectCount  > 0:
            dump.PushIndent()
            for i in range(0, instanceDefinition.ObjectCount):
                obj = instanceDefinition.Object(i)
                if obj != None and type(obj) == Rhino.DocObjects.InstanceObject:
                    DumpInstanceDefinition(obj.InstanceDefinition, dump, False) # Recursive...
                else:
                    dump.Print(u"+ Object {0} = {1}\n".format(i, obj.ShortDescription(False)))
            dump.PopIndent()

if __name__ == "__main__":
    RunCommand()
