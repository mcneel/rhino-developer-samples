import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const downloadButton = document.getElementById("downloadButton")
downloadButton.onclick = download

let renderer, scene, camera
let doc

init()
create()
animate()

// create //

function create () 
{
    // create three polyline

    //create a blue LineBasicMaterial
    const material = new THREE.LineBasicMaterial( { color: 0x0000ff } )

    const points = [];
    points.push( new THREE.Vector3( - 10, 0, 10 ) )
    points.push( new THREE.Vector3( -5, 0, -10 ) )
    points.push( new THREE.Vector3(  0, 0, 10 ) )
    points.push( new THREE.Vector3(  5, 0, -10 ) )
    points.push( new THREE.Vector3(  10, 0, 10 ) )

    const geometry = new THREE.BufferGeometry().setFromPoints( points )

    const polyline = new THREE.Line( geometry, material )

    scene.add( polyline )

    // convert polyline buffer geo to rhino polyline

    doc = new rhino.File3dm()

    const count = geometry.attributes.position.count
    const rhinoPolyline = new rhino.Polyline()

    for (let i = 0; i < count; i++) {

        const x = geometry.attributes.position.getX(i)
        const y = geometry.attributes.position.getY(i)
        const z = geometry.attributes.position.getZ(i)

        rhinoPolyline.add( x, y, z )

    }

    doc.objects().addPolyline( rhinoPolyline, null )

     // hide spinner
    document.getElementById('loader').style.display = 'none'

    // enable download button
    downloadButton.disabled = false

}

// BOILERPLATE //

// download button handler
function download() {
    const options = new rhino.File3dmWriteOptions()
    options.version = 8
    let buffer = doc.toByteArrayOptions(options)
    saveByteArray('rhinoObjectTypes' + options.version + '.3dm', buffer)
  }
  
  function saveByteArray(fileName, byte) {
    let blob = new Blob([byte], { type: 'application/octect-stream' })
    let link = document.createElement('a')
    link.href = window.URL.createObjectURL(blob)
    link.download = fileName
    link.click()
  }

function init () {

    // Rhino models are z-up, so set this as the default
    THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)
  
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