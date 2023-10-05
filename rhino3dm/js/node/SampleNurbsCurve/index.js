import * as fs from 'fs' //only if running in node.js
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const buffer = fs.readFileSync('../../../models/nurbscurve.3dm')
const arr = new Uint8Array(buffer)
const file3dm = rhino.File3dm.fromByteArray(arr)

const objects = file3dm.objects()
const cnt = objects.count

console.log(`Number of Objects: ${cnt}`)

for ( let i = 0; i < cnt; i ++ ) {
    const object = objects.get( i )
    const geometry = object.geometry()

    console.log(`NurbsCurve Order ${geometry.order}`)
    console.log(`NurbsCurve IsRational ${geometry.isRational}`)

    const knots = geometry.knots()
    console.log(knots.get(5))
    const knotList = knots.toList()
    console.log(knotList)
    
}
