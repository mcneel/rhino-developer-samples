from Rhino import *
from Rhino.Input.Custom import *
from Rhino.Commands import *
from scriptcontext import doc
import rhinoscriptsyntax as rs

def RunCommand():
    distance_display_precision = rs.GetInteger("Display precision",
        doc.ModelDistanceDisplayPrecision, 0, 7)
    if distance_display_precision != None: return Result.Nothing

    if distance_display_precision != doc.ModelDistanceDisplayPrecision:
        doc.ModelDistanceDisplayPrecision = distance_display_precision

    return Result.Success

if __name__ ==  "__main__":
    RunCommand()
