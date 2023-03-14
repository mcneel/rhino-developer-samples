const fs = require('fs')
const rhino3dm = require('rhino3dm')
const THREE = require('three')

global.THREE = THREE
require('three/examples/js/loaders/OBJLoader')

let rhino
rhino3dm().then(async m => {

    console.log('Loaded rhino3dm.')
    rhino = m // global

    let file = fs.readFileSync('Rhino_Logo.obj').toString()

    const loader = new THREE.OBJLoader();
    const obj = loader.parse(file)

    //const mesh = obj.children[ 0 ]
    //const rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: mesh.geometry } )
    
    let exportGeometry = null
    if (obj.children) {
        obj.children.forEach((mesh) => {
            if (!exportGeometry) {
                exportGeometry = mesh.geometry
            } else {
                exportGeometry = THREE.BufferGeometryUtils.mergeBufferGeometries([exportGeometry, mesh.geometry], false)
            }
        })
    }

    const rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: exportGeometry } )
    
    const doc = new rhino.File3dm()
    doc.objects().add(rhinoMesh, null)

    let opts = new rhino.File3dmWriteOptions();
    opts.version = 8;
    let buffer = doc.toByteArray(opts)
    fs.writeFileSync('Rhino_Logo_Mesh_'+opts.version+'.3dm', buffer)


})

