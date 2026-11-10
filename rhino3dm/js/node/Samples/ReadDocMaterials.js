import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

// read model 
const model = '../../../models/unassignedmaterials.3dm'

let buffer = fs.readFileSync(model)
let arr = new Uint8Array(buffer)
let doc = rhino.File3dm.fromByteArray(arr)
let objects = doc.objects
let materials = doc.materials

// render content comes in one collection per kind: materials, environments and textures
const renderContentCount = [...doc.renderMaterials].length + [...doc.renderEnvironments].length + [...doc.renderTextures].length

console.log('Materials in file: ', materials.count)
console.log(`Render Content Count: ${renderContentCount}`)

for ( const rc of doc.renderMaterials ) {

    const material = rc.toMaterial()
    console.log(material.name)

}