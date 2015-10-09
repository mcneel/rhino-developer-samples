import Rhino
import scriptcontext

def dynamic_array():
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select object", True, Rhino.DocObjects.ObjectType.AnyObject)
    if rc!=Rhino.Commands.Result.Success: return
    
    rc, pt_start = Rhino.Input.RhinoGet.GetPoint("Start point", False)
    if rc!=Rhino.Commands.Result.Success: return
    
    obj = objref.Object()
    if not obj: return
    
    dist = 1
    if scriptcontext.sticky.has_key("dynamic_array_distance"):
        dist = scriptcontext.sticky["dynamic_array_distance"]
    # This is a function that is called whenever the GetPoint's
    # DynamicDraw event occurs
    def ArrayByDistanceDraw( sender, args ):
        rhobj = args.Source.Tag
        if not rhobj: return
        vec = args.CurrentPoint - pt_start
        length = vec.Length
        vec.Unitize()
        count = int(length / dist)
        for i in range(1,count):
            translate = vec * (i*dist)
            xf = Rhino.Geometry.Transform.Translation(translate)
            args.Display.DrawObject(rhobj, xf)

    # Create an instance of a GetPoint class and add a delegate
    # for the DynamicDraw event
    gp = Rhino.Input.Custom.GetPoint()
    gp.DrawLineFromPoint(pt_start, True)
    optdouble = Rhino.Input.Custom.OptionDouble(dist)
    constrain = False
    optconstrain = Rhino.Input.Custom.OptionToggle(constrain,"Off", "On")
    gp.AddOptionDouble("Distance", optdouble)
    gp.AddOptionToggle("Constrain", optconstrain)
    gp.DynamicDraw += ArrayByDistanceDraw
    gp.Tag = obj
    while gp.Get()==Rhino.Input.GetResult.Option:
        dist = optdouble.CurrentValue
        if constrain!=optconstrain.CurrentValue:
            constrain = optconstrain.CurrentValue
            if constrain:
                gp2 = Rhino.Input.Custom.GetPoint()
                gp2.DrawLineFromPoint(pt_start, True)
                gp2.SetCommandPrompt("Second point on constraint line")
                if gp2.Get()==Rhino.Input.GetResult.Point:
                    gp.Constrain(pt_start, gp2.Point())
                else:
                    gp.ClearCommandOptions()
                    optconstrain.CurrentValue = False
                    constrain = False
                    gp.AddOptionDouble("Distance", optdouble)
                    gp.AddOptionToggle("Constrain", optconstrain)
            else:
                gp.ClearConstraints()
        continue
    if gp.CommandResult()==Rhino.Commands.Result.Success:
        scriptcontext.sticky["dynamic_array_distance"] = dist
        pt = gp.Point()
        vec = pt - pt_start
        length = vec.Length
        vec.Unitize()
        count = int(length / dist)
        for i in range(1, count):
            translate = vec * (i*dist)
            xf = Rhino.Geometry.Transform.Translation(translate)
            scriptcontext.doc.Objects.Transform(obj,xf,False)
        scriptcontext.doc.Views.Redraw()


if( __name__ == "__main__" ):
    dynamic_array()