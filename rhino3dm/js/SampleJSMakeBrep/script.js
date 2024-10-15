// #region INFO //

//print rhino3dm CDN url to know version info
const imports = document.getElementById('imports')
const rhino3dmUrl = JSON.parse(imports.innerText).imports.rhino3dm
console.log(rhino3dmUrl)

let notes = "This example shows how to create simple breps and extrusions with rhino3dm.js \n\n"
notes += "While this has been possible before this version of rhino3dm, we can now set the render meshes for Breps and Extrusions, so the Rhino objects created in this example can be directly visualized in the three.js editor."
console.log(notes)

// #endregion INFO //

// #region IMPORTS //

import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

// #endregion IMPORTS //

// #region ELEMENTS //

const sphereButton = document.getElementById("sphereButton")
sphereButton.addEventListener('click', ()=>{
    createObject('sphere')
})
const boxButton = document.getElementById("boxButton")
boxButton.addEventListener('click', ()=>{
    createObject('box')
})
const cylinderButton = document.getElementById("cylinderButton")
cylinderButton.addEventListener('click', ()=>{
    createObject('cylinder')
})

const downloadButton = document.getElementById("downloadButton")
downloadButton.disabled = true
downloadButton.onclick = download

// #endregion ELEMENTS //

// #region OBJECTS //

let scene, camera, renderer
const doc = new rhino.File3dm()

//turn off the rhinoground plane
//doc.settings().renderSettings().groundPlane.enabled = false

// #endregion OBJECTS and VARS //

// #region MATERIALS //

const redMaterial = new rhino.Material()
redMaterial.toPhysicallyBased()
redMaterial.physicallyBased().baseColor = { r: 1, g: 0, b: 0, a: 0 }
redMaterial.physicallyBased().metallic = 0.7
redMaterial.physicallyBased().roughness = 0.5
doc.materials().add(redMaterial)

const redThreeMaterial = new THREE.MeshPhysicalMaterial( {color:0xff0000, metalness:0.7, roughness: 0.5} )

const magentaMaterial = new rhino.Material()
magentaMaterial.toPhysicallyBased()
magentaMaterial.physicallyBased().baseColor = { r: 1, g: 0, b: 1, a: 0 }
magentaMaterial.physicallyBased().metallic = 0.2
magentaMaterial.physicallyBased().roughness = 0.9
doc.materials().add(magentaMaterial)

const magentaThreeMaterial = new THREE.MeshPhysicalMaterial( {color:0xff00ff, metalness:0.2, roughness: 0.9} )

const greenMaterial = new rhino.Material()
greenMaterial.toPhysicallyBased()
greenMaterial.physicallyBased().baseColor = { r: 0, g: 1, b: 0, a: 0 }
greenMaterial.physicallyBased().metallic = 0.2
greenMaterial.physicallyBased().roughness = 0.9
doc.materials().add(greenMaterial)

const greenThreeMaterial = new THREE.MeshPhysicalMaterial( {color:0x00ff00, metalness:0.7, roughness: 0.9} )

console.log(doc.materials().count)

// #endregion MATERIALS //

// #region LAYERS //

const main_layer_index = doc.layers().addLayer( 'Objects', { r: 255, g: 255, b: 255, a: 255 } )
const main_layer = doc.layers().findIndex(main_layer_index)
const main_layer_id = main_layer.id

const brep_layer_index = doc.layers().addLayer( 'Spheres', { r: 255, g: 0, b: 0, a: 255 } )
const brep_layer = doc.layers().findIndex(brep_layer_index)
brep_layer.parentLayerId = main_layer_id
brep_layer.renderMaterialIndex = 0

const ext_layer_index = doc.layers().addLayer( 'Extrusions', { r: 255, g: 0, b: 255, a: 255 } )
const ext_layer = doc.layers().findIndex(ext_layer_index)
ext_layer.parentLayerId = main_layer_id
ext_layer.renderMaterialIndex = 1

const cyl_layer_index = doc.layers().addLayer( 'Cylinders', { r: 0, g: 255, b: 0, a: 255 } )
const cyl_layer = doc.layers().findIndex(cyl_layer_index)
cyl_layer.parentLayerId = main_layer_id
cyl_layer.renderMaterialIndex = 2

// #endregion LAYERS //

// #region CALL //

init()

// #endregion CALL //

// #region CREATE //

function createObject(objectType) {

    // hide spinner
    document.getElementById('msg').style.display = 'none'

    // enable download
    downloadButton.disabled = false

    let geometry
    let rhinoObject, rhinoMesh, result, xform
    const oa = new rhino.ObjectAttributes()
    oa.materialSource = rhino.ObjectMaterialSource.MaterialFromLayer

    // generate a random positon about the origin of the scnene
    const xDim = 100
    const yDim = 100
    const zDim = 100
    const dim = Math.random() * 20
    const halfDim = dim/2
    const x = Math.random() * xDim * (Math.random() > 0.5 ? 1 : - 1)
    const y = Math.random() * yDim * (Math.random() > 0.5 ? 1 : - 1)
    const z = Math.random() * zDim * (Math.random() > 0.5 ? 1 : - 1)

    console.log( [ x, y, z ] )

    let material = null

    switch (objectType) {
        case 'sphere':

            geometry = new THREE.SphereGeometry( dim, 32, 32 )
            geometry.translate( x, z, -y )

            rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: geometry } )

            const sphere = new rhino.Sphere( [ x, y, z ], dim )
            rhinoObject = rhino.Brep.createFromSphere( sphere )
            result = rhinoObject.faces().get(0).setMesh( rhinoMesh, rhino.MeshType.Render )

            oa.layerIndex = brep_layer_index

            material = redThreeMaterial

            break

        case 'box':

            geometry = new THREE.BoxGeometry( dim, dim, dim, 1, 1, 1 )
            geometry.translate( x, z, -y )

            const rhinoBox = new rhino.Box( new rhino.BoundingBox( [ -halfDim, -halfDim, -halfDim ], [ halfDim, halfDim, halfDim ] ) )
            rhinoObject = rhino.Extrusion.createBoxExtrusion( rhinoBox, true )

            xform = rhino.Transform.translationXYZ(x, y, z)
            rhinoObject.transform( xform )

            rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: geometry } )
            result = rhinoObject.setMesh( rhinoMesh, rhino.MeshType.Render )

            oa.layerIndex = ext_layer_index

            material = magentaThreeMaterial

            break

        case 'cylinder':

            geometry = new THREE.CylinderGeometry( dim, dim, dim, 32 )
            geometry.translate( x, z + halfDim, -y)

            const circle = new rhino.Circle([ x, y, z ], dim)
            const cylinder = new rhino.Cylinder(circle, dim)
            rhinoObject = rhino.Brep.createFromCylinder( cylinder, true, true )

            rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: geometry } )
            result = rhinoObject.faces().get(0).setMesh( rhinoMesh, rhino.MeshType.Render )

            oa.layerIndex = cyl_layer_index

            material = greenThreeMaterial

            break
    }

    
    
    const mesh = new THREE.Mesh(geometry, material)
    scene.add(mesh)

    doc.objects().add( rhinoObject, oa)


}

// #endregion CREATE //

// #region INIT //

/**
 * Initialize the three.js renderer, scene, camera, and controls
 */

function init() {

    THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)

    renderer = new THREE.WebGLRenderer({ antialias: true })
    renderer.setPixelRatio(window.devicePixelRatio)
    renderer.setSize(window.innerWidth, window.innerHeight)
    document.body.appendChild(renderer.domElement)

    scene = new THREE.Scene()
    scene.background = new THREE.Color(1, 1, 1)

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 1000)
    camera.position.z = 500

    const controls = new OrbitControls(camera, renderer.domElement)

    const light = new THREE.DirectionalLight()
    scene.add(light)

    const aLight = new THREE.AmbientLight()
    scene.add(aLight)

    window.addEventListener('resize', onWindowResize, false)

    animate()
}

/**
 * Continuously anumate the scene
 */
function animate() {
    requestAnimationFrame(animate)
    renderer.render(scene, camera)
}

/**
 * Handle window resize
 */
function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight
    camera.updateProjectionMatrix()
    renderer.setSize(window.innerWidth, window.innerHeight)
    animate()
}

// #endregion INIT //

// #region UTILITIES //

function download() {
    const options = new rhino.File3dmWriteOptions()
    options.version = 8
    let buffer = doc.toByteArrayOptions(options)
    saveByteArray('rhinoDocument_' + options.version + '.3dm', buffer)
}

function saveByteArray(fileName, byte) {
    let blob = new Blob([byte], { type: 'application/octect-stream' })
    let link = document.createElement('a')
    link.href = window.URL.createObjectURL(blob)
    link.download = fileName
    link.click()
}

// #endregion UTILITIES //