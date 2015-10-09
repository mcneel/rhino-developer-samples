from scriptcontext import doc

def RunCommand():
  instanceDefinitionNames = [instanceDefinition.Name for instanceDefinition in doc.InstanceDefinitions 
                             if instanceDefinition != None and not instanceDefinition.IsDeleted]

  for n in instanceDefinitionNames:
    print "instance definition = {0}".format(n)

if __name__ == "__main__":
  RunCommand()
