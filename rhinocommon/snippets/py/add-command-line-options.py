import Rhino
import scriptcontext

def CommandLineOptions():
    # For this example we will use a GetPoint class, but all of the custom
    # "Get" classes support command line options.
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("GetPoint with options")

    # set up the options
    intOption = Rhino.Input.Custom.OptionInteger(1, 1, 99)
    dblOption = Rhino.Input.Custom.OptionDouble(2.2, 0, 99.9)
    boolOption = Rhino.Input.Custom.OptionToggle(True, "Off", "On")
    listValues = "Item0", "Item1", "Item2", "Item3", "Item4"

    gp.AddOptionInteger("Integer", intOption)
    gp.AddOptionDouble("Double", dblOption)
    gp.AddOptionToggle("Boolean", boolOption)
    listIndex = 3
    opList = gp.AddOptionList("List", listValues, listIndex)
    while True:
        # perform the get operation. This will prompt the user to
        # input a point, but also allow for command line options
        # defined above
        get_rc = gp.Get()
        if gp.CommandResult()!=Rhino.Commands.Result.Success:
            return gp.CommandResult()
        if get_rc==Rhino.Input.GetResult.Point:
            point = gp.Point()
            scriptcontext.doc.Objects.AddPoint(point)
            scriptcontext.doc.Views.Redraw()
            print "Command line option values are"
            print " Integer =", intOption.CurrentValue
            print " Double =", dblOption.CurrentValue
            print " Boolean =", boolOption.CurrentValue
            print " List =", listValues[listIndex]
        elif get_rc==Rhino.Input.GetResult.Option:
            if gp.OptionIndex()==opList:
              listIndex = gp.Option().CurrentListOptionIndex
            continue
        break
    return Rhino.Commands.Result.Success


if __name__ == "__main__":
    CommandLineOptions()
