import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

// read model 
const model = '../../../models/unassignedmaterials.3dm'

let buffer = fs.readFileSync(model)
let arr = new Uint8Array(buffer)
let doc = rhino.File3dm.fromByteArray(arr)
let objects = doc.objects()
let materials = doc.materials()

console.log('Materials in file: ', materials.count)
console.log(`Render Content Count: ${doc.renderContent().count}`)

for ( let i = 0; i < doc.renderContent().count; i ++ ) {

    const rc = doc.renderContent().get(i)

    if(rc.kind === 'material') {
        const material = rc.toMaterial()
        console.log(material.name)
    }

}