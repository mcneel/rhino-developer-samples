# requires rhino3dm 9.2.0b0
# python3 -m pip install rhino3dm==9.2.0b0

import rhino3dm
import datetime

model = rhino3dm.File3dm()

sun = model.Settings.RenderSettings.Sun

print('Sun maxYear: ' + str(sun.MaxYear ) )
print('Sun vector: ' + str(sun.Vector ) )
print('Sun enableAllowed: ' + str(sun.EnableAllowed ) )
print('Sun enableOn: ' + str(sun.Enabled ) )
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
date = sun.GetDateTime(rhino3dm.DateTimeKind.Local)
print('Sun year: ' + str(date.year ) )
print('Sun month: ' + str(date.month ) )
print('Sun day: ' + str(date.day ) )
print('Sun hours: ' + str(date.hour + date.minute / 60 + date.second / 3600 ) )
print('Sun intensity: ' + str(sun.Intensity ) )
print('Sun isValid: ' + str(sun.IsValid ) )
print('Sun light location: ' + str(sun.Light.Location ) )
print()
color = rhino3dm.Sun.ColorFromAltitude(5.00)
print( 'Sun sunColorFromAltitude rgba: ' + str(color))
print()

sun.EnableAllowed = False
sun.Enabled = True
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
sun.SetDateTime(datetime.datetime(2023, 7, 26, 14), rhino3dm.DateTimeKind.Local)
sun.Intensity = 2

print('Sun maxYear: ' + str(sun.MaxYear ) )
print('Sun vector: ' + str(sun.Vector ) )
print('Sun enableAllowed: ' + str(sun.EnableAllowed ) )
print('Sun enableOn: ' + str(sun.Enabled ) )
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
date = sun.GetDateTime(rhino3dm.DateTimeKind.Local)
print('Sun year: ' + str(date.year ) )
print('Sun month: ' + str(date.month ) )
print('Sun day: ' + str(date.day ) )
print('Sun hours: ' + str(date.hour + date.minute / 60 + date.second / 3600 ) )
print('Sun intensity: ' + str(sun.Intensity ) )
print('Sun isValid: ' + str(sun.IsValid ) )
print('Sun light location: ' + str(sun.Light.Location ) )


