import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import rhino3dm from 'rhino3dm'

const downloadButton = document.getElementById("downloadButton")
downloadButton.onclick = download

let rhino, doc
rhino3dm().then(async m => {
    console.log('Loaded rhino3dm.')
    rhino = m // global
    init()
    create()
})

function create () {

    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@7.15.0/' )

    doc = new rhino.File3dm()

    // Create layers
    const layer_points = new rhino.Layer()
    layer_points.name = 'Points'
    layer_points.color = { r: 255, g: 0, b: 0, a: 255 }
    doc.layers().add( layer_points )

    const layer_curves = new rhino.Layer()
    layer_curves.name = 'Curves'
    layer_curves.color = { r: 0, g: 0, b: 0, a: 255 }
    doc.layers().add( layer_curves )

    const layer_meshes = new rhino.Layer()
    layer_meshes.name = 'Meshes'
    layer_meshes.color = { r: 255, g: 255, b: 0, a: 255 }
    doc.layers().add( layer_meshes )

    const layer_breps = new rhino.Layer()
    layer_breps.name = 'Breps'
    layer_breps.color = { r: 255, g: 255, b: 255, a: 255 }
    doc.layers().add( layer_breps )

    const layer_extrusions = new rhino.Layer()
    layer_extrusions.name = 'Extrusions'
    layer_extrusions.color = { r: 0, g: 255, b: 255, a: 255 }
    doc.layers().add( layer_extrusions )

    const layer_dots = new rhino.Layer()
    layer_dots.name = 'TextDots'
    layer_dots.color = { r: 0, g: 0, b: 255, a: 255 }
    doc.layers().add( layer_dots )

    // -- POINTS / POINTCLOUDS -- //

    const oa_points = new rhino.ObjectAttributes()
    oa_points.layerIndex = 0

    // POINTS

    const ptA = [0, 0, 0]
    const point = new rhino.Point( ptA )

    doc.objects().add( point, oa_points )

    // POINTCLOUD

    const ptB = [ 10, 0, 0 ]
    const ptC = [ 10, 10, 0 ]
    const ptD = [ 0, 10, 0 ]
    const ptE = [ 20, 20, 20 ]
    const ptF = [ 30, 0, 0 ]
    const ptG = [ 35, 5, 0 ]
    const ptH = [ 40, -5, 0 ]
    const ptI = [ 45, 5, 0 ]
    const ptJ = [ 50, 0, 0 ]

    const red = { r: 255, g: 0, b: 0, a: 0 }

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

    doc.objects().add( pointCloud, oa_points )

    // -- CURVES -- //

    const oa_curves = new rhino.ObjectAttributes()
    oa_curves.layerIndex = 1

    // LINE //

    const line = new rhino.LineCurve( ptA, ptE )
    doc.objects().add( line, oa_curves )

    // CIRCLE //

    const circle = new rhino.Circle( 10 )
    doc.objects().add( circle.toNurbsCurve(), oa_curves )

    // ARC //

    const arc = new rhino.Arc( circle, Math.PI )
    doc.objects().add( arc.toNurbsCurve(), oa_curves )

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

    doc.objects().add( nurbsCurveD1, oa_curves )
    doc.objects().add( nurbsCurveD2, oa_curves )
    doc.objects().add( nurbsCurveD3, oa_curves )
    doc.objects().add( nurbsCurveD4, oa_curves )

    // -- MESHES -- //

    const oa_meshes = new rhino.ObjectAttributes()
    oa_meshes.layerIndex = 2

    const mesh = new rhino.Mesh()
    mesh.vertices().add( ptA[0], ptA[1], ptA[2] )
    mesh.vertices().add( ptB[0], ptB[1], ptB[2] )
    mesh.vertices().add( ptC[0], ptC[1], ptC[2] )

    mesh.faces().addFace( 0, 1, 2 )

    mesh.vertexColors().add( 255, 0, 255 )
    mesh.vertexColors().add( 0, 255, 255 )
    mesh.vertexColors().add( 255, 255, 255 )

    mesh.normals().computeNormals()

    doc.objects().add( mesh, oa_meshes )

    // -- BREPS -- //

    const oa_breps = new rhino.ObjectAttributes()
    oa_breps.layerIndex = 3

    // Rhino3dmLoader will warn you that there is no mesh geometry
    const sphere = new rhino.Sphere( ptA, 2.5 )
    const brep = rhino.Brep.createFromSphere( sphere )

    doc.objects().add( brep, oa_breps )

    // -- EXTRUSIONS -- //

    const oa_extrusions = new rhino.ObjectAttributes()
    oa_extrusions.layerIndex = 4

    const extrusion = rhino.Extrusion.create( nurbsCurveD4, 10, false )
    // Rhino3dmLoader currently throws an error since 
    // there is no mesh geometry associated with this extrusion
    //doc.objects().add( extrusion, oa_extrusions )

    // -- SUBD -- //

    // -- TEXT DOT -- //

    const oa_dots = new rhino.ObjectAttributes()
    oa_dots.layerIndex = 5

    const dot = new rhino.TextDot( 'Hello!', ptD )
    doc.objects().add( dot, oa_dots )

    // create a copy of the doc.toByteArray data to get an ArrayBuffer
    let arr = new Uint8Array( doc.toByteArray() ).buffer

    loader.parse( arr, function ( object ) {

      // hide spinner
      document.getElementById('loader').style.display = 'none'
      console.log( object )
      scene.add( object )

      // enable download button
      downloadButton.disabled = false
  
    } )

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
  scene.background = new THREE.Color( 1,1,1 )
  camera = new THREE.PerspectiveCamera( 45, window.innerWidth/window.innerHeight, 1, 1000 )
  camera.position.z = 50

  renderer = new THREE.WebGLRenderer( { antialias: true } )
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