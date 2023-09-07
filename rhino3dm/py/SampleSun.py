# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm

model = rhino3dm.File3dm()

sun = model.Settings.RenderSettings.Sun

print('Sun maxYear: ' + str(sun.MaxYear ) )
print('Sun vector: ' + str(sun.Vector ) )
print('Sun enableAllowed: ' + str(sun.EnableAllowed ) )
print('Sun enableOn: ' + str(sun.EnableOn ) )
print('Sun manualControlAllowed: ' + str(sun.ManualControlAllowed ) )
print('Sun manualControlOn: ' + str(sun.ManualControlOn ) )
print('Sun north: ' + str(sun.North ) )
print('Sun azimuth: ' + str(sun.Azimuth ) )
print('Sun altitude: ' + str(sun.Altitude ) )
print('Sun latitude: ' + str(sun.Latitude ) )
print('Sun longitude: ' + str(sun.Longitude ) )
print('Sun timeZone: ' + str(sun.TimeZone ) )
print('Sun daylightSavingOn: ' + str(sun.DaylightSavingOn ) )
print('Sun daylightSavingMinutes: ' + str(sun.DaylightSavingMinutes ) )
print('Sun year: ' + str(sun.Year ) )
print('Sun month: ' + str(sun.Month ) )
print('Sun day: ' + str(sun.Day ) )
print('Sun hours: ' + str(sun.Hours ) )
print('Sun intensity: ' + str(sun.Intensity ) )
print('Sun isValid: ' + str(sun.IsValid ) )
print('Sun light location: ' + str(sun.Light.Location ) )
print()
color = rhino3dm.Sun.SunColorFromAltitude(5.00)
print( 'Sun sunColorFromAltitude rgba: ' + str(color))
print()

sun.EnableAllowed = False
sun.EnableOn = True
sun.ManualControlAllowed = False
sun.ManualControlOn = True
sun.North = 45
sun.Azimuth = 123.456
sun.Altitude = 34.567
sun.Latitude = 41.0
sun.Longitude = 3.45
sun.TimeZone = 2
sun.DaylightSavingOn = True
sun.DaylightSavingMinutes = 30
sun.Year = 2023
sun.Month = 7
sun.Day = 26
sun.Hours = 14
sun.Intensity = 2

print('Sun maxYear: ' + str(sun.MaxYear ) )
print('Sun vector: ' + str(sun.Vector ) )
print('Sun enableAllowed: ' + str(sun.EnableAllowed ) )
print('Sun enableOn: ' + str(sun.EnableOn ) )
print('Sun manualControlAllowed: ' + str(sun.ManualControlAllowed ) )
print('Sun manualControlOn: ' + str(sun.ManualControlOn ) )
print('Sun north: ' + str(sun.North ) )
print('Sun azimuth: ' + str(sun.Azimuth ) )
print('Sun altitude: ' + str(sun.Altitude ) )
print('Sun latitude: ' + str(sun.Latitude ) )
print('Sun longitude: ' + str(sun.Longitude ) )
print('Sun timeZone: ' + str(sun.TimeZone ) )
print('Sun daylightSavingOn: ' + str(sun.DaylightSavingOn ) )
print('Sun daylightSavingMinutes: ' + str(sun.DaylightSavingMinutes ) )
print('Sun year: ' + str(sun.Year ) )
print('Sun month: ' + str(sun.Month ) )
print('Sun day: ' + str(sun.Day ) )
print('Sun hours: ' + str(sun.Hours ) )
print('Sun intensity: ' + str(sun.Intensity ) )
print('Sun isValid: ' + str(sun.IsValid ) )
print('Sun light location: ' + str(sun.Light.Location ) )


