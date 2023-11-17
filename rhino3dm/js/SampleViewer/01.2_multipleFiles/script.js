// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'

const select = document.getElementById( 'file-select' )
select.onchange = load

let model = 'hello_mesh.3dm'

const loader = new Rhino3dmLoader()
loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.0.1/' )

const material = new THREE.MeshNormalMaterial()

// BOILERPLATE //
let scene, camera, renderer, rhinoObject

init()
load()

function load() {

    // don't load what's already loaded
    if ( model === select.value  && rhinoObject !== undefined ) return;

    //handle case when the selection isn't related to a 3dm file
    if ( select.value.includes( '.3dm' ) ) {
        model = select.value
    }

    document.getElementById( 'loader' ).style.display = 'block'

    // load model
    loader.load( model, function ( object ) {

        // hide spinner
        document.getElementById( 'loader' ).style.display = 'none'

        console.log( object )

        // clear scene
        scene.traverse( child => {

            if ( child.userData.hasOwnProperty( 'objectType' ) && child.userData.objectType === 'File3dm' ) {

              scene.remove( child )

            }

          } )

        object.traverse( child => {

            child.material = material

        })
        scene.add( object )
    
    } )

}

function init() {

    THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)

    scene = new THREE.Scene()
    scene.background = new THREE.Color(1,1,1)
    camera = new THREE.PerspectiveCamera( 45, window.innerWidth/window.innerHeight, 1, 1000 )
    camera.position.set(26,-40,5)

    // add a directional light
    const directionalLight = new THREE.DirectionalLight(0xffffff)
    directionalLight.intensity = 2
    scene.add(directionalLight)

    const ambientLight = new THREE.AmbientLight()
    scene.add(ambientLight)

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