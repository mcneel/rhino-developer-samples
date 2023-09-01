import rhino3dm from 'rhino3dm'
const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const file3dm = new rhino.File3dm()

console.log( `Flatshade Enabled: ${ file3dm.settings().renderSettings().flatShade }` ) // false
file3dm.settings().renderSettings().flatShade = true
console.log( `Flatshade Enabled: ${ file3dm.settings().renderSettings().flatShade }` ) // true

const renderSettings = file3dm.settings().renderSettings()

const gp = file3dm.settings().renderSettings().groundPlane

console.log( `Ground Plane Enabled: ${ gp.enabled }` )
console.log( `Ground Plane Altitude: ${ gp.altitude }` )
console.log()

gp.enabled = false
gp.altitude = 1.23

console.log( `Ground Plane Enabled: ${ gp.enabled }` )
console.log( `Ground Plane Altitude: ${ gp.altitude }` )

