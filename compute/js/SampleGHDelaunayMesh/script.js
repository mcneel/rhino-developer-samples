// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'
import { RhinoCompute } from 'rhinocompute'

// reference the definition
const definitionName = 'delaunay.gh'

let definition
let scene, camera, renderer, controls 

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.

// source a .gh/.ghx file in the same directory
let url = definitionName
let res = await fetch(url)
let buffer = await res.arrayBuffer()
definition = new Uint8Array(buffer)

init()
compute()

async function compute() {

  // generate random points

  let points = []
  const cntPts = 100
  const bndX = 100
  const bndY = 100
  const bndZ = 10

  for (let i = 0; i < cntPts; i++) {
    let x = Math.random() * (bndX - -bndX) + -bndX
    let y = Math.random() * (bndY - -bndY) + -bndY
    let z = Math.random() * (bndZ - -bndZ) + -bndZ

    let pt = "{\"X\":" + x + ",\"Y\":" + y + ",\"Z\":" + z + "}"

    points.push(pt)

    //viz in three
    let geo = new THREE.SphereGeometry(1, 5, 5)
    geo.translate(x, y, z)
    let mat = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true })
    let sph = new THREE.Mesh(geo, mat)
    scene.add(sph)
  }

  const param1 = new RhinoCompute.Grasshopper.DataTree( 'points' )
  param1.append( [ 0 ], points )

  // clear values
  const trees = []
  trees.push( param1 )
  //console.log(param1)

  // Call RhinoCompute
  const res = await RhinoCompute.Grasshopper.evaluateDefinition(definition, trees)

  console.log(res)

  // remove spinner
  document.getElementById('loader').remove()

  const data = JSON.parse( res.values[0].InnerTree['{0}'][0].data )
  const mesh = rhino.CommonObject.decode( data )

  const material = new THREE.MeshBasicMaterial( { wireframe: true, color: 0x00ff00 } )
  const threeMesh = meshToThreejs( mesh, material )

  scene.add( threeMesh )

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

// BOILERPLATE //

function init() {

  // Rhino models are z-up, so set this as the default
  THREE.Object3D.DefaultUp = new THREE.Vector3(0, 0, 1)

  scene = new THREE.Scene()
  scene.background = new THREE.Color(1, 1, 1)
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 1000)
  camera.position.z = 300

  renderer = new THREE.WebGLRenderer({ antialias: true })
  renderer.setPixelRatio(window.devicePixelRatio)
  renderer.setSize(window.innerWidth, window.innerHeight)
  document.body.appendChild(renderer.domElement)

  controls = new OrbitControls(camera, renderer.domElement)

  window.addEventListener('resize', onWindowResize, false)

  animate()
}

function animate() {
  requestAnimationFrame(animate)
  renderer.render(scene, camera)
}

function onWindowResize() {
  camera.aspect = window.innerWidth / window.innerHeight
  camera.updateProjectionMatrix()
  renderer.setSize( window.innerWidth, window.innerHeight )
  animate()
}

function meshToThreejs(mesh, material) {
  const loader = new THREE.BufferGeometryLoader()
  const geometry = loader.parse( mesh.toThreejsJSON() )
  return new THREE.Mesh( geometry, material )
}