// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'

const model = 'hello_mesh.3dm'

// BOILERPLATE //
let scene, camera, renderer

init()
load()

function init () {

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

function load() {

    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.4.0/' )

    const material = new THREE.MeshNormalMaterial()
    loader.load( model, function ( object ) {

        // hide spinner
        document.getElementById('loader').style.display = 'none'

        console.log( object )

        object.traverse( child => {
            child.material = material
        })

        scene.add( object )
    
    } )

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