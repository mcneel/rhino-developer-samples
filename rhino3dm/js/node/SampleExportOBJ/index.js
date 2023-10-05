// can use imports since in package.json we have set "type":"module"
import * as fs from 'fs'
import rhino3dm from 'rhino3dm'
import * as THREE from 'three'
import { OBJExporter } from 'three/examples/jsm/exporters/OBJExporter.js'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

// read model 
const model = '../../../models/Rhino_Logo_Mesh7.3dm'

let buffer = fs.readFileSync(model)
let arr = new Uint8Array(buffer)
let doc = rhino.File3dm.fromByteArray(arr)
let objects = doc.objects()

console.log(`objects: ${objects.count}`)

const loader = new THREE.BufferGeometryLoader()
const exporter = new OBJExporter()

const scene = new THREE.Scene()
const material = new THREE.MeshBasicMaterial()

// go through objects and add the meshes to the scene
// finally, export the scne as an obj
for ( let i = 0; i < objects.count; i ++ ) {

    const rgeo = objects.get( i ).geometry()

    if( rgeo.objectType === rhino.ObjectType.Mesh) {

        const geometry = loader.parse( rgeo.toThreejsJSON() )
        const mesh = new THREE.Mesh( geometry, material )
        scene.add( mesh )
        
		const result = exporter.parse( scene )
        fs.writeFileSync('Rhino_Logo_Mesh.obj', result)

    }

}