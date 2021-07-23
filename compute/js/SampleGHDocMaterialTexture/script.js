// Import libraries
import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.126.0/build/three.module.js'
import { OrbitControls } from 'https://cdn.jsdelivr.net/npm/three@0.126.0/examples/jsm/controls/OrbitControls.js'
import { Rhino3dmLoader } from 'https://cdn.jsdelivr.net/npm/three@0.126.0/examples/jsm/loaders/3DMLoader.js'
import rhino3dm from 'https://cdn.jsdelivr.net/npm/rhino3dm@0.16.1/rhino3dm.module.js'
import { RhinoCompute } from 'https://cdn.jsdelivr.net/npm/compute-rhino3d@0.13.0-beta/compute.rhino3d.module.js'

// reference the definition
const definitionName = 'SampleGHDocMaterialTexture.gh'

const downloadButton = document.getElementById('downloadButton')
downloadButton.onclick = download

const image_input = document.getElementById('img_input')
image_input.onchange = updateImage

// setup input
const radius_input = 10.00
let dataURI_input = data // this is a base64 encoded image file in the dataURI format defined in data.js just to have an image to start with


// globals
let rhino, definition, doc
rhino3dm().then(async m => {
  console.log('Loaded rhino3dm.')
  rhino = m // global

  RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
  RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.

  // source a .gh / .ghx file in the same directory
  let url = definitionName
  let res = await fetch(url)
  let buffer = await res.arrayBuffer()
  definition = new Uint8Array(buffer)

  init()
  compute()
})

async function compute() {

  // hide spinner
  showSpinner(true)

  // format data
  let param1 = new RhinoCompute.Grasshopper.DataTree('radius')
  param1.append([0], [radius_input])
  let param2 = new RhinoCompute.Grasshopper.DataTree('RH_IN:dataURI')
  param2.append([0], [dataURI_input])

  // Add all params to an array
  let trees = []
  trees.push(param1)
  trees.push(param2)

  // Call RhinoCompute
  const res = await RhinoCompute.Grasshopper.evaluateDefinition(definition, trees)

  //console.log(res) 

  collectResults(res)

}

// from https://stackoverflow.com/a/21797381
function _base64ToArrayBuffer(base64) {
  var binary_string = window.atob(base64);
  var len = binary_string.length;
  var bytes = new Uint8Array(len);
  for (var i = 0; i < len; i++) {
    bytes[i] = binary_string.charCodeAt(i);
  }
  return bytes.buffer;
}

/**
 * Parse response
 */
function collectResults(responseJson) {

  // clear doc
  if (doc !== undefined)
    doc.delete()

  const values = responseJson.values
  console.log(responseJson)

  const str = values[0].InnerTree['{0}'][0].data
  const data = JSON.parse(str)
  const arr = _base64ToArrayBuffer(data)
  doc = rhino.File3dm.fromByteArray(arr)

  if (doc.objects().count < 1) {
    console.error('No rhino objects to load!')
    showSpinner(false)
    return
  }

  // set up loader for converting the results to threejs
  const loader = new Rhino3dmLoader()
  loader.setLibraryPath('https://cdn.jsdelivr.net/npm/rhino3dm@0.16.1/')

  // load rhino doc into three.js scene
  loader.parse(arr, function (object) {
    console.log(object)

    scene.traverse(child => {
      if (child.userData.hasOwnProperty('objectType') && child.userData.objectType === 'File3dm') {
        scene.remove(child)
      }
    })

    // zoom to extents
    zoomCameraToSelection(camera, controls, object.children)

    // add object graph from rhino model to three.js scene
    scene.add(object)

    // hide spinner
    showSpinner(false)

    // enable buttons
    downloadButton.disabled = false
    image_input.disabled = false

  })
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

/**
 * Shows or hides the loading spinner
 */
function showSpinner(enable) {
  if (enable)
    document.getElementById('loader').style.display = 'block'
  else
    document.getElementById('loader').style.display = 'none'
}

function updateImage() {

  const file = image_input.files[0];
  const reader = new FileReader();

  reader.addEventListener("load", function () {
    // convert image file to base64 string
    dataURI_input = reader.result 
    compute()
  }, false);

  if (file) {
    reader.readAsDataURL(file);
  }

}

// download button handler
function download() {
  let buffer = doc.toByteArray()
  saveByteArray('rhinoObjects.3dm', buffer)
}

function saveByteArray(fileName, byte) {
  let blob = new Blob([byte], { type: 'application/octect-stream' })
  let link = document.createElement('a')
  link.href = window.URL.createObjectURL(blob)
  link.download = fileName
  link.click()
}

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

