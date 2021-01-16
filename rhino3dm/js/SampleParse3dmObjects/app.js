import * as THREE from 'https://unpkg.com/three@0.124.0/build/three.module.js'
import { OrbitControls } from 'https://unpkg.com/three@0.124.0/examples/jsm/controls/OrbitControls.js'
import { Rhino3dmLoader } from 'https://unpkg.com/three@0.124.0/examples/jsm/loaders/3DMLoader.js'

let rhino
rhino3dm().then(async m => {
    console.log('Loaded rhino3dm.')
    rhino = m // global
    init()
    create()
})

function create () {

    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/three@0.124.0/examples/jsm/libs/rhino3dm/' )

    let doc = new rhino.File3dm()

    // -- POINTS / POINTCLOUDS -- //

    // POINTS

    let ptA = [0, 0, 0]

    doc.objects().addPoint(ptA)

    // POINTCLOUD

    const ptB = [10, 0, 0]
    const ptC = [10, 10, 0]
    const ptD = [0, 10, 0]
    const ptE = [ 20, 20, 20]
    const ptF = [30, 0, 0]
    const ptG = [35, 5, 0]
    const ptH = [40, -5, 0]
    const ptI = [45, 5, 0]
    const ptJ = [50, 0, 0]

    let red = { r: 255, g: 0, b: 0, a: 0}

    const pointCloud = new rhino.PointCloud()
    pointCloud.add( ptA, red )
    pointCloud.add( ptB, red )
    pointCloud.add( ptC, red )
    pointCloud.add( ptD, red )
    pointCloud.add( ptE, red )
    pointCloud.add( ptF, red )
    pointCloud.add( ptG, red )
    pointCloud.add( ptH, red )
    pointCloud.add( ptI, red )
    pointCloud.add( ptJ, red )

    doc.objects().add(pointCloud, null)

    // -- CURVES -- //

    // LINE //

    const line = new rhino.LineCurve( ptA, ptE )
    doc.objects().add(line, null)

    // CIRCLE //

    const circle = new rhino.Circle( 10 )
    doc.objects().add(circle.toNurbsCurve(), null)

    // ARC //

    const arc = new rhino.Arc( circle, Math.PI )
    doc.objects().add(arc.toNurbsCurve(), null)

    // ELLIPSE //

    // CURVE //

    const curvePoints = new rhino.Point3dList()
    curvePoints.add( ptF[0], ptF[1], ptF[2] )
    curvePoints.add( ptG[0], ptG[1], ptG[2] )
    curvePoints.add( ptH[0], ptH[1], ptH[2] )
    curvePoints.add( ptI[0], ptI[1], ptI[2] )
    curvePoints.add( ptJ[0], ptJ[1], ptJ[2] )

    const nurbsCurveD1 = rhino.NurbsCurve.create( false, 1, curvePoints )
    const nurbsCurveD2 = rhino.NurbsCurve.create( false, 2, curvePoints )
    const nurbsCurveD3 = rhino.NurbsCurve.create( false, 3, curvePoints )
    const nurbsCurveD4 = rhino.NurbsCurve.create( false, 4, curvePoints )

    doc.objects().add(nurbsCurveD1, null)
    doc.objects().add(nurbsCurveD2, null)
    doc.objects().add(nurbsCurveD3, null)
    doc.objects().add(nurbsCurveD4, null)

    // -- MESHES -- //

    // -- BREPS -- //

    // -- EXTRUSIONS -- //

    // -- SUBD -- //

    // create a copy of the doc.toByteArray data to get an ArrayBuffer
    let arr = new Uint8Array( doc.toByteArray() ).buffer

    loader.parse( arr, function ( object ) {

      // hide spinner
      document.getElementById('loader').remove()
      console.log(object)
      scene.add(object)
  
    } )

}

// BOILERPLATE //

let scene, camera, renderer, controls

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

let animate = function () {
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