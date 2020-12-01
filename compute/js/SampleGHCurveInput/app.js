import * as THREE from 'https://unpkg.com/three@0.122.0/build/three.module.js'
import { OrbitControls } from 'https://unpkg.com/three@0.122.0/examples/jsm/controls/OrbitControls.js'

let args = {
    algo : null, 
    pointer : null,
    values : []
}

let definition = null
let param1 = new RhinoCompute.Grasshopper.DataTree('RH_IN:curve')
let rhino

rhino3dm().then( async m => {
    console.log('Loaded rhino3dm.')
    rhino = m // global

    RhinoCompute.url = "http://localhost:8081/" // Rhino.Compute server url
    RhinoCompute.apiKey = "" // your Rhino.Compute server api key

    // source a .ghx file in the same directory
    let url = 'worm.gh'
    let res = await fetch(url)
    let buffer = await res.arrayBuffer()
    definition = new Uint8Array(buffer)

    init()
    compute()
})

async function compute(){
    
    const crvPoints = new rhino.Point3dList()
    crvPoints.add( 0, 0, 0,)
    crvPoints.add( 10, 10, 0,)
    crvPoints.add( 20, -10, 0,)
    crvPoints.add( 30, 10, 20,)
    crvPoints.add( 40, -10, -20,)
    crvPoints.add( 50, 0, 0,)

    const nCrv = rhino.NurbsCurve.create( false, 3, crvPoints)

    const crvData = JSON.stringify( nCrv.encode() )

    param1.append([0], [ crvData ] )

    // clear values
    let trees = []
    trees.push(param1)
    console.log(param1)

    RhinoCompute.Grasshopper.evaluateDefinition(definition, trees).then(result => {
        console.log(result)

        // remove spinner
        document.getElementById('loader').remove()

        let data = JSON.parse(result.values[0].InnerTree['{ 0; }'][0].data)
        let mesh = rhino.CommonObject.decode(data)

        let material = new THREE.MeshBasicMaterial( { vertexColors: true, side:2 } )
        let threeMesh = meshToThreejs(mesh, material)

        scene.add(threeMesh)

    })
}

// BOILERPLATE //

var scene, camera, renderer, controls

function init () {
  scene = new THREE.Scene()
  scene.background = new THREE.Color(1,1,1)
  camera = new THREE.PerspectiveCamera( 45, window.innerWidth/window.innerHeight, 1, 1000 )

  renderer = new THREE.WebGLRenderer({antialias: true})
  renderer.setPixelRatio( window.devicePixelRatio )
  renderer.setSize( window.innerWidth, window.innerHeight )
  let canvas = document.getElementById('canvas')
  canvas.appendChild( renderer.domElement )

  controls = new OrbitControls( camera, renderer.domElement  )

  camera.position.z = 50

  window.addEventListener( 'resize', onWindowResize, false )

  animate()
}

var animate = function () {
  requestAnimationFrame( animate )
  controls.update()
  renderer.render( scene, camera )
}
  
function onWindowResize() {
  camera.aspect = window.innerWidth / window.innerHeight
  camera.updateProjectionMatrix()
  renderer.setSize( window.innerWidth, window.innerHeight )
  animate()
}

function meshToThreejs (mesh, material) {
  let loader = new THREE.BufferGeometryLoader()
  var geometry = loader.parse(mesh.toThreejsJSON())
  return new THREE.Mesh(geometry, material)
}