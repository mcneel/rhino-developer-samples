// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'

const file = 'hello_mesh.3dm'
let scene, camera, renderer

// wait until the rhino3dm library is loaded, then load the 3dm file
const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

let res = await fetch(file)
let buffer = await res.arrayBuffer()
let arr = new Uint8Array(buffer)
let doc = rhino.File3dm.fromByteArray(arr)

init()

let material = new THREE.MeshNormalMaterial()

let objects = doc.objects()
for (let i = 0; i < objects.count; i++) {
    let mesh = objects.get(i).geometry()
    if(mesh instanceof rhino.Mesh) {
        // convert all meshes in 3dm model into threejs objects
        let threeMesh = meshToThreejs(mesh, material)
        scene.add(threeMesh)
    }
}


// BOILERPLATE //

function init(){

    THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)

    scene = new THREE.Scene()
    scene.background = new THREE.Color(1,1,1)
    camera = new THREE.PerspectiveCamera( 45, window.innerWidth/window.innerHeight, 1, 1000 )
    camera.position.set(26,-40,5)

    renderer = new THREE.WebGLRenderer({antialias: true})
    renderer.setPixelRatio( window.devicePixelRatio )
    renderer.setSize( window.innerWidth, window.innerHeight )
    document.body.appendChild( renderer.domElement )

    const controls = new OrbitControls( camera, renderer.domElement )

    window.addEventListener( 'resize', onWindowResize, false )
    animate()
}

function animate () {
    requestAnimationFrame( animate )
    renderer.render( scene, camera )
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight
    camera.updateProjectionMatrix()
    renderer.setSize( window.innerWidth, window.innerHeight )
    animate()
}

function meshToThreejs(mesh, material) {
    const loader = new THREE.BufferGeometryLoader()
    const geometry = loader.parse(mesh.toThreejsJSON())
    return new THREE.Mesh(geometry, material)
}