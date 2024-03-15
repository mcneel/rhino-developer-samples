import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

// read model 
const model = '../../../models/textEntities_r8.3dm'

const buffer = fs.readFileSync(model)
const arr = new Uint8Array(buffer)
const doc = rhino.File3dm.fromByteArray(arr)
const objects = doc.objects()

for ( let i = 0; i < objects.count; i ++ ) {
    const ro = objects.get(i)
    const geometry = ro.geometry()
    console.log('-------------------')
    console.log(geometry.plainText)
    console.log(geometry.plainTextWithFields)
    console.log(geometry.richText)

}