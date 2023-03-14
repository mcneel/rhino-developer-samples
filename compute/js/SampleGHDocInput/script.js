// #region IMPORTS //
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import rhino3dm from 'rhino3dm'
import { RhinoCompute } from 'rhinocompute'

// #endregion IMPORTS //

// #region VARS //

RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.


const data = {}
data.definitionName = 'SampleGHDocInput.gh'
data.fileName = 'input.3dm'
data.inputs = {
  'encodedFile':null,
  'extension':null
}

// #endregion VARS //

// #region DATA //

// load 3dm
const file3dm = await fetch( data.fileName )
const buffer3dm = await file3dm.arrayBuffer()
const arr = new Uint8Array( buffer3dm )
const b64ba = base64ByteArray( arr )

// console.log( b64ba ) 

data.inputs.encodedFile = b64ba
data.inputs.extension = '3dm'

// source a .gh / .ghx file in the same directory
const fileGH = await fetch( data.definitionName )
const bufferGH = await fileGH.arrayBuffer()
const definition = new Uint8Array(bufferGH)

// #endregion DATA //

// #region RUN //

let rhino, doc
rhino3dm().then(async m => {
    console.log('Loaded rhino3dm.')
    rhino = m // global
    init()
    compute()
})

// #endregion RUN //

// #region COMPUTE // 

async function compute() {

    console.log( 'compute' )
  
    console.log(data)
  
    // format data
    let param1 = new RhinoCompute.Grasshopper.DataTree( 'encodedFile' )
    param1.append( [ 0 ], [ data.inputs.encodedFile ] )
    let param2 = new RhinoCompute.Grasshopper.DataTree( 'extension' )
    param2.append( [ 0 ], [ data.inputs.extension ] )
  
    // Add all params to an array
    let trees = []
    trees.push( param1 )
    trees.push( param2 )
  
    // Call RhinoCompute
    const res = await RhinoCompute.Grasshopper.evaluateDefinition( definition, trees )
  
    console.log( res ) 
  
    collectResults(res)
  
  }


// #endregion COMPUTE //

// #region PARSE RESULT //

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
  
    // load rhino doc into three.js scene
    const buffer = new Uint8Array(doc.toByteArray()).buffer
  
    // set up loader for converting the results to threejs
    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@7.15.0/' )
  
    loader.parse( buffer, ( object )  =>
    {
  ///////////////////////////////////////////////////////////////////////////
        // change mesh material
        object.traverse(child => {
          if (child.isMesh) {
            child.material = new THREE.MeshNormalMaterial({ wireframe: true})
          }
        }, false)
  ///////////////////////////////////////////////////////////////////////////
  
        // clear objects from scene. do this here to avoid blink
        scene.traverse(child => {
            if (!child.isLight) {
                scene.remove(child)
            }
        })
  
        // add object graph from rhino model to three.js scene
        scene.add( object )
  
        // hide spinner and enable download button
        showSpinner(false)
        //downloadButton.disabled = false
  
        // zoom to extents
        zoomCameraToSelection(camera, controls, scene.children)
    }, (error) => {
        console.error(error)
    })
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

// #endregion PARSE RESULT //

// #region INIT //

// BOILERPLATE //
// declare variables to store scene, camera, and renderer
let scene, camera, renderer, controls
function init() {

  // Rhino models are z-up, so set this as the default
  THREE.Object3D.DefaultUp = new THREE.Vector3(0, 0, 1)

  // create a scene and a camera
  scene = new THREE.Scene()
  scene.background = new THREE.Color(1, 1, 1)
  camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000)
  camera.position.y = -30
  camera.position.z = 30

  // create the renderer and add it to the html
  renderer = new THREE.WebGLRenderer({ antialias: true })
  renderer.setPixelRatio(window.devicePixelRatio)
  renderer.setSize(window.innerWidth, window.innerHeight)
  document.body.appendChild(renderer.domElement)

  // add some controls to orbit the camera
  controls = new OrbitControls(camera, renderer.domElement)

  // add a directional light
  const directionalLight = new THREE.DirectionalLight(0xffffff)
  directionalLight.intensity = 2
  scene.add(directionalLight)

  const ambientLight = new THREE.AmbientLight()
  scene.add(ambientLight)

  // handle changes in the window size
  window.addEventListener('resize', onWindowResize, false)

  animate()

}

function onWindowResize() {
  camera.aspect = window.innerWidth / window.innerHeight
  camera.updateProjectionMatrix()
  renderer.setSize(window.innerWidth, window.innerHeight)
  animate()
}

// function to continuously render the scene
function animate() {

  requestAnimationFrame(animate)
  renderer.render(scene, camera)

}

function getAuth(key) {
  let value = localStorage[key]
  if (value === undefined) {
    const prompt = key.includes('URL') ? 'Server URL' : 'Server API Key'
    value = window.prompt('RhinoCompute ' + prompt)
    if (value !== null) {
      localStorage.setItem(key, value)
    }
  }
  return value
}

// #endregion INIT //


// #region UTILITIES //

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
 * Helper function that behaves like rhino's "zoom to selection", but for three.js!
 */
 function zoomCameraToSelection(camera, controls, selection, fitOffset = 1.1) {

    const box = new THREE.Box3();
  
    for (const object of selection) {
      if (object.isLight) continue
      box.expandByObject(object);
    }
  
    const size = box.getSize(new THREE.Vector3());
    const center = box.getCenter(new THREE.Vector3());
  
    const maxSize = Math.max(size.x, size.y, size.z);
    const fitHeightDistance = maxSize / (2 * Math.atan(Math.PI * camera.fov / 360));
    const fitWidthDistance = fitHeightDistance / camera.aspect;
    const distance = fitOffset * Math.max(fitHeightDistance, fitWidthDistance);
  
    const direction = controls.target.clone()
      .sub(camera.position)
      .normalize()
      .multiplyScalar(distance);
    controls.maxDistance = distance * 10;
    controls.target.copy(center);
  
    camera.near = distance / 100;
    camera.far = distance * 100;
    camera.updateProjectionMatrix();
    camera.position.copy(controls.target).sub(direction);
  
    controls.update();
  
  }

// https://gist.github.com/jonleighton/958841
/*
MIT LICENSE
Copyright 2011 Jon Leighton
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
function base64ByteArray(bytes) {
    var base64    = ''
    var encodings = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
  
    // var bytes         = new Uint8Array(arrayBuffer)
  
    // strip bom
    if (bytes[0] === 239 && bytes[1] === 187 && bytes[2] === 191)
      bytes = bytes.slice(3)
  
    var byteLength    = bytes.byteLength
    var byteRemainder = byteLength % 3
    var mainLength    = byteLength - byteRemainder
  
    var a, b, c, d
    var chunk
  
    // Main loop deals with bytes in chunks of 3
    for (var i = 0; i < mainLength; i = i + 3) {
      // Combine the three bytes into a single integer
      chunk = (bytes[i] << 16) | (bytes[i + 1] << 8) | bytes[i + 2]
  
      // Use bitmasks to extract 6-bit segments from the triplet
      a = (chunk & 16515072) >> 18 // 16515072 = (2^6 - 1) << 18
      b = (chunk & 258048)   >> 12 // 258048   = (2^6 - 1) << 12
      c = (chunk & 4032)     >>  6 // 4032     = (2^6 - 1) << 6
      d = chunk & 63               // 63       = 2^6 - 1
  
      // Convert the raw binary segments to the appropriate ASCII encoding
      base64 += encodings[a] + encodings[b] + encodings[c] + encodings[d]
    }
  
    // Deal with the remaining bytes and padding
    if (byteRemainder == 1) {
      chunk = bytes[mainLength]
  
      a = (chunk & 252) >> 2 // 252 = (2^6 - 1) << 2
  
      // Set the 4 least significant bits to zero
      b = (chunk & 3)   << 4 // 3   = 2^2 - 1
  
      base64 += encodings[a] + encodings[b] + '=='
    } else if (byteRemainder == 2) {
      chunk = (bytes[mainLength] << 8) | bytes[mainLength + 1]
  
      a = (chunk & 64512) >> 10 // 64512 = (2^6 - 1) << 10
      b = (chunk & 1008)  >>  4 // 1008  = (2^6 - 1) << 4
  
      // Set the 2 least significant bits to zero
      c = (chunk & 15)    <<  2 // 15    = 2^4 - 1
  
      base64 += encodings[a] + encodings[b] + encodings[c] + '='
    }
  
    return base64
  }
  

// #endregion UTILITIES //
