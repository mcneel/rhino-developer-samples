# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm

model = rhino3dm.File3dm()

sf = model.Settings.RenderSettings.SafeFrame

print('Safe Frame Enabled: ' + str( sf.Enabled ) )
print('Safe Frame perspectiveOnly: ' + str( sf.PerspectiveOnly ) )
print('Safe Frame fieldGridOn: ' + str( sf.FieldGridOn ) )
print('Safe Frame liveFrameOn: ' + str( sf.LiveFrameOn ) )
print('Safe Frame actionFrameOn: ' + str( sf.ActionFrameOn ) )
print('Safe Frame actionFrameLinked: ' + str( sf.ActionFrameLinked ) )
print('Safe Frame ActionFrameXScale: ' + str( sf.ActionFrameXScale ) )
print('Safe Frame actionFrameYScale: ' + str( sf.ActionFrameYScale ) )
print('Safe Frame titleFrameOn: ' + str( sf.TitleFrameOn ) )
print('Safe Frame titleFrameLinked: ' + str( sf.TitleFrameLinked ) )
print('Safe Frame titleFrameXScale: ' + str( sf.TitleFrameXScale ) )
print('Safe Frame titleFrameYScale: ' + str( sf.TitleFrameYScale ) )
print()

sf.Enabled = True
sf.PerspectiveOnly = False
sf.FieldGridOn = True
sf.LiveFrameOn = False
sf.ActionFrameOn = False
sf.ActionFrameLinked = False
sf.ActionFrameXScale = 0.45
sf.ActionFrameYScale = 0.45
sf.TitleFrameOn = False
sf.TitleFrameLinked = False
sf.TitleFrameXScale = 0.45
sf.TitleFrameYScale = 0.45

print('Safe Frame Enabled: ' + str( sf.Enabled ) )
print('Safe Frame perspectiveOnly: ' + str( sf.PerspectiveOnly ) )
print('Safe Frame fieldGridOn: ' + str( sf.FieldGridOn ) )
print('Safe Frame liveFrameOn: ' + str( sf.LiveFrameOn ) )
print('Safe Frame actionFrameOn: ' + str( sf.ActionFrameOn ) )
print('Safe Frame actionFrameLinked: ' + str( sf.ActionFrameLinked ) )
print('Safe Frame ActionFrameXScale: ' + str( sf.ActionFrameXScale ) )
print('Safe Frame actionFrameYScale: ' + str( sf.ActionFrameYScale ) )
print('Safe Frame titleFrameOn: ' + str( sf.TitleFrameOn ) )
print('Safe Frame titleFrameLinked: ' + str( sf.TitleFrameLinked ) )
print('Safe Frame titleFrameXScale: ' + str( sf.TitleFrameXScale ) )
print('Safe Frame titleFrameYScale: ' + str( sf.TitleFrameYScale ) )
print()