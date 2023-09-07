# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm

model = rhino3dm.File3dm()

sl = model.Settings.RenderSettings.Skylight
print(sl.Enabled)
print(sl.ShadowIntensity)
sl.ShadowIntensity = 1.4
sl.Enabled = False
print(sl.Enabled)
print(sl.ShadowIntensity)