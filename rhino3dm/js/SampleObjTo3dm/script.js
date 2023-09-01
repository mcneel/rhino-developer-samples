import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { OBJLoader } from 'three/examples/jsm/loaders/OBJLoader'
import rhino3dm from 'rhino3dm'

const downloadButton = document.getElementById("downloadButton")
downloadButton.onclick = download

let doc
let scene, camera, renderer

const rhino = await rhino3dm()

console.log('Loaded rhino3dm.')

init()
convert()

function convert () {

    doc = new rhino.File3dm()
    
    const loader = new OBJLoader( )

    loader.load( 'Rhino_Logo.obj', 
    
    //onLoad
    ( obj ) => {

      scene.add( obj )

      const mesh = obj.children[ 0 ]
      const rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: mesh.geometry } )
      doc.objects().add( rhinoMesh, null )

    }, 
    
    //onProgress
    ( xhr ) => {

      if ( xhr.lengthComputable ) {

        const percentComplete = xhr.loaded / xhr.total * 100;
        console.log( 'model ' + Math.round( percentComplete, 2 ) + '% downloaded' );

      }

    }, 
    
    //onError
    () => {
      console.log( 'Error' )
    })

    // hide spinner
    document.getElementById('loader').style.display = 'none'

    // enable download button
    downloadButton.disabled = false

    console.log(scene)

}

// download button handler
function download () {
  const options = new rhino.File3dmWriteOptions()
  options.version = 7
  let buffer = doc.toByteArrayOptions(options)
  saveByteArray( 'rhinoObjects'+options.version+'.3dm', buffer )
}

function saveByteArray ( fileName, byte ) {
  let blob = new Blob( [ byte ], {type: 'application/octect-stream'} )
  let link = document.createElement( 'a' )
  link.href = window.URL.createObjectURL( blob )
  link.download = fileName
  link.click()
}

// BOILERPLATE //

function init () {

  // Rhino models are z-up, so set this as the default
  THREE.Object3D.DefaultUp = new THREE.Vector3( 0, 0, 1 )

  scene = new THREE.Scene()
  scene.background = new THREE.Color(1,1,1)
  camera = new THREE.PerspectiveCamera( 45, window.innerWidth/window.innerHeight, 1, 1000 )
  camera.position.z = 50

  renderer = new THREE.WebGLRenderer({antialias: true})
  renderer.setPixelRatio( window.devicePixelRatio )
  renderer.setSize( window.innerWidth, window.innerHeight )
  document.body.appendChild( renderer.domElement )

  const controls = new OrbitControls( camera, renderer.domElement  )

  const light = new THREE.DirectionalLight()
  scene.add( light )

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