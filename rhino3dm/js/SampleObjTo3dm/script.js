import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.126.0/build/three.module.js'
import { OrbitControls } from 'https://cdn.jsdelivr.net/npm/three@0.126.0/examples/jsm/controls/OrbitControls.js'
import { OBJLoader } from 'https://cdn.jsdelivr.net/npm/three@0.126.0/examples/jsm/loaders/OBJLoader.js';
import rhino3dm from 'https://cdn.jsdelivr.net/npm/rhino3dm@0.15.0-beta/rhino3dm.module.js'

const downloadButton = document.getElementById("downloadButton")
downloadButton.onclick = download

let rhino, doc
rhino3dm().then(async m => {
    console.log('Loaded rhino3dm.')
    rhino = m // global
    init()
    convert()
})

function convert () {

    doc = new rhino.File3dm()
    
    const loader = new OBJLoader( )
    loader.load( 'Rhino_Logo.obj', function ( obj ) {

      scene.add( obj )

      const mesh = obj.children[ 0 ]
      const rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: mesh.geometry } )
      doc.objects().add( rhinoMesh, null )

    }, function ( xhr ) {

      if ( xhr.lengthComputable ) {

        const percentComplete = xhr.loaded / xhr.total * 100;
        console.log( 'model ' + Math.round( percentComplete, 2 ) + '% downloaded' );

      }

    }, function () {
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
  let buffer = doc.toByteArray()
  saveByteArray( 'rhinoObjects.3dm', buffer )
}

function saveByteArray ( fileName, byte ) {
  let blob = new Blob( [ byte ], {type: 'application/octect-stream'} )
  let link = document.createElement( 'a' )
  link.href = window.URL.createObjectURL( blob )
  link.download = fileName
  link.click()
}

// BOILERPLATE //

let scene, camera, renderer

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