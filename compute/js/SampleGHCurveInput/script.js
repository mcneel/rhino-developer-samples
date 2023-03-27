// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import rhino3dm from 'rhino3dm'
import { RhinoCompute } from 'rhinocompute'

// reference the definition
const definitionName = 'worm.gh'

// globals
let definition, doc
let scene, camera, renderer

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.

// source a .gh / .ghx file in the same directory
let url = definitionName
let res = await fetch(url)
let buffer = await res.arrayBuffer()
definition = new Uint8Array(buffer)

init()
compute()

async function compute(){
    
    const crvPoints = new rhino.Point3dList()
    crvPoints.add( 0, 0, 0,)
    crvPoints.add( Math.random() * 10, Math.random() * 10, 0,)
    crvPoints.add( Math.random() * 20, Math.random() * -10, 0,)
    crvPoints.add( Math.random() * 30, Math.random() * 10, Math.random() * 20,)
    crvPoints.add( Math.random() * 40, Math.random() * -10, Math.random() * -20,)
    crvPoints.add( 50, 0, 0,)

    const nCrv = rhino.NurbsCurve.create( false, 3, crvPoints)

    const crvData = JSON.stringify( nCrv.encode() )

    const param1 = new RhinoCompute.Grasshopper.DataTree('curve')
    param1.append([0], [ crvData ] )

    // clear values
    let trees = []
    trees.push(param1)

    // Call RhinoCompute

    const res = await RhinoCompute.Grasshopper.evaluateDefinition(definition, trees)

    console.log(res) 

    collectResults(res)
}

/**
 * Parse response
 */
function collectResults(responseJson) {

  const values = responseJson.values

  // clear doc
  if( doc !== undefined)
      doc.delete()

  //console.log(values)
  doc = new rhino.File3dm()

  // for each output (RH_OUT:*)...
  for ( let i = 0; i < values.length; i ++ ) {
    // ...iterate through data tree structure...
    for (const path in values[i].InnerTree) {
      const branch = values[i].InnerTree[path]
      // ...and for each branch...
      for( let j = 0; j < branch.length; j ++) {
        // ...load rhino geometry into doc
        const rhinoObject = decodeItem(branch[j])
        if (rhinoObject !== null) {
          doc.objects().add(rhinoObject, null)
        }
      }
    }
  }

  if (doc.objects().count < 1) {
    console.error('No rhino objects to load!')
    showSpinner(false)
    return
  }

  // set up loader for converting the results to threejs
  const loader = new Rhino3dmLoader()
  loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.0.0-beta/' )

  const resMaterial = new THREE.MeshBasicMaterial( {vertexColors: true, wireframe: true} )
  // load rhino doc into three.js scene
  const buffer = new Uint8Array(doc.toByteArray()).buffer
  loader.parse( buffer, function ( object ) 
  {

      // add material to resulting meshes
      object.traverse( child => {
          child.material = resMaterial
      } )

      // add object graph from rhino model to three.js scene
      scene.add( object )

      // hide spinner
      showSpinner(false)

  })
}

/**
 * Shows or hides the loading spinner
 */
function showSpinner(enable) {
    if (enable)
      document.getElementById('loader').style.display = 'block'
    else
      document.getElementById('loader').style.display = 'none'
}

/**
* Attempt to decode data tree item to rhino geometry
*/
function decodeItem(item) {
  const data = JSON.parse(item.data)
  if (item.type === 'System.String') {
    // hack for draco meshes
    try {
        return rhino.DracoCompression.decompressBase64String(data)
    } catch {} // ignore errors (maybe the string was just a string...)
  } else if (typeof data === 'object') {
    return rhino.CommonObject.decode(data)
  }
  return null
}

function getAuth( key ) {
  let value = localStorage[key]
  if ( value === undefined ) {
      const prompt = key.includes('URL') ? 'Server URL' : 'Server API Key'
      value = window.prompt('RhinoCompute ' + prompt)
      if ( value !== null ) {
          localStorage.setItem( key, value )
      }
  }
  return value
}

// BOILERPLATE //

function init () {
  // Rhino models are z-up, so set this as the default
  THREE.Object3D.DefaultUp = new THREE.Vector3( 0, 0, 1 )
  
  scene = new THREE.Scene()
  scene.background = new THREE.Color(1,1,1)
  camera = new THREE.PerspectiveCamera( 75, window.innerWidth/window.innerHeight, 0.1, 1000 )

  camera.position.x = 100
  camera.position.y = 50
  camera.position.z = 25

  renderer = new THREE.WebGLRenderer({antialias: true})
  renderer.setPixelRatio( window.devicePixelRatio )
  renderer.setSize( window.innerWidth, window.innerHeight )
  document.body.appendChild(renderer.domElement)

  // add some controls to orbit the camera
  const controls = new OrbitControls(camera, renderer.domElement)

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