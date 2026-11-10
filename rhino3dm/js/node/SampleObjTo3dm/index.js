// can use imports since in package.json we have set "type":"module"
import * as fs from 'fs'
import rhino3dm from 'rhino3dm'
import * as THREE from 'three'
import { OBJLoader } from 'three/examples/jsm/loaders/OBJLoader.js'
import { mergeGeometries } from 'three/examples/jsm/utils/BufferGeometryUtils.js'

const rhino = await rhino3dm()

console.log('Loaded rhino3dm.')

let file = fs.readFileSync('../../../models/Rhino_Logo.obj').toString()

const loader = new OBJLoader()
const obj = loader.parse(file)

let exportGeometry = null
if (obj.children) {
    obj.children.forEach((mesh) => {
        if (!exportGeometry) {
            exportGeometry = mesh.geometry
        } else {
            exportGeometry = mergeGeometries([exportGeometry, mesh.geometry], false)
        }
    })
}

// createFromThreejsBuffers takes the BufferGeometry's attribute arrays; true rotates from three.js's Y-up to Rhino's Z-up
const attributes = exportGeometry.attributes
const rhinoMesh = rhino.Mesh.createFromThreejsBuffers( {
    position: attributes.position.array,
    normal: attributes.normal?.array,
    uv: attributes.uv?.array,
    index: exportGeometry.index?.array
}, true )

const doc = new rhino.File3dm()
doc.objects.add(rhinoMesh, null)

let opts = new rhino.File3dmWriteOptions()
opts.version = 8
let buffer = doc.toByteArray(opts)
fs.writeFileSync('Rhino_Logo_Mesh.3dm', buffer)


