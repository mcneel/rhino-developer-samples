import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import rhino3dm from 'rhino3dm'

const downloadButton = document.getElementById("downloadButton")
downloadButton.onclick = download

let doc
let scene, camera, renderer

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

init()
create()

function create () {

    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.0.1/' )

    doc = new rhino.File3dm()

    // Create layers

    //parent layer for Point Objects and Point Clouds
    const pt_index = doc.layers().addLayer( 'Points', { r: 255, g: 0, b: 0, a: 255 } )
    const pts_layer_id = doc.layers().findName( 'Points', '').id

    //because we want to nest this layer, we need to set the parent layer id
    const layer_pointObjects = new rhino.Layer()
    layer_pointObjects.name = 'Point Objects'
    layer_pointObjects.color = { r: 255, g: 0, b: 0, a: 255 }
    layer_pointObjects.parentLayerId = pts_layer_id
    const ptObject_layer_index = doc.layers().add( layer_pointObjects )

    //because we want to nest this layer, we need to set the parent layer id
    const layer_pointCloud = new rhino.Layer()
    layer_pointCloud.name = 'Point Clouds'
    layer_pointCloud.color = { r: 255, g: 0, b: 0, a: 255 }
    layer_pointCloud.parentLayerId = pts_layer_id
    const ptCloud_layer_index = doc.layers().add( layer_pointCloud )

    const curves_layer_index = doc.layers().addLayer( 'Curves',  { r: 0, g: 0, b: 0, a: 255 })

    const meshes_layer_index = doc.layers().addLayer( 'Meshes', { r: 255, g: 255, b: 0, a: 255 } )

    const brep_layer_index = doc.layers().addLayer( 'Breps', { r: 255, g: 255, b: 255, a: 255 } )

    const extrusion_layer_index = doc.layers().addLayer( 'Extrusions', { r: 0, g: 255, b: 255, a: 255 } )

    const textdot_layer_index = doc.layers().addLayer( 'TextDots', { r: 0, g: 0, b: 255, a: 255 } )


    // Create two groups

    doc.groups().add(new rhino.Group())
    doc.groups().add(new rhino.Group())
    console.log(`nº of groups ${doc.groups().count}`)

    const group1 = doc.groups().get(0)
    console.log(`group id: ${group1.id}`)
    console.log(`group index: ${group1.index}`)

    const group2 = doc.groups().get(1)
    console.log(`group id: ${group2.id}`)
    console.log(`group index: ${group2.index}`)

    // -- POINTS / POINTCLOUDS -- //

    const oa_points = new rhino.ObjectAttributes()
    oa_points.layerIndex = ptObject_layer_index

    const oa_pointClouds = new rhino.ObjectAttributes()
    oa_pointClouds.layerIndex = ptCloud_layer_index

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
    const ptK = [ 50, 0, 0 ]
    const ptL = [ 50, 0, 0 ]
    const ptM = [ 50, 0, 0 ]

    const red = { r: 255, g: 0, b: 0, a: 0 }
    const blue = { r: 0, g: 0, b: 255, a: 0 }

    const pointCloud = new rhino.PointCloud()
    pointCloud.add( ptA )
    pointCloud.addPointColor( ptC, blue )
    pointCloud.addPointNormalColor( ptD, [0,0,0], red )
    pointCloud.addPointNormalColor( ptE, [0,0,0], blue )
    pointCloud.addPointNormalColor( ptF, [0,0,0], red )
    pointCloud.addPointNormalColor( ptG, [0,0,0], blue )
    pointCloud.addPointNormalColor( ptH, [0,0,0], red )
    pointCloud.addPointNormalColor( ptI, [0,0,0], red )
    pointCloud.addPointNormalColor( ptJ, [0,0,0], red )

    doc.objects().add( pointCloud, oa_pointClouds )

    // -- CURVES -- //

    const oa_curves = new rhino.ObjectAttributes()
    oa_curves.layerIndex = curves_layer_index

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

    // PolyCurve

    const polyCurve = new rhino.PolyCurve()

    let xform = rhino.Transform.translationXYZ(20, 0, 20)
    circle.transform(xform)
    const arc2 = new rhino.Arc( circle, Math.PI )
    const p1 = arc2.startPoint
    const p2 = [ p1[0] + 10, p1[1], p1[2] + 10 ]
    const line2 = new rhino.Line( p1, p2 )

    let r1 = polyCurve.appendArc(arc2)
    let r2 = polyCurve.appendLine(line2)
    doc.objects().add( polyCurve, oa_curves )

    // -- MESHES -- //

    const oa_meshes = new rhino.ObjectAttributes()
    oa_meshes.layerIndex = meshes_layer_index

    const mesh = new rhino.Mesh()
    mesh.vertices().add( ptA[0], ptA[1], ptA[2] )
    mesh.vertices().add( ptB[0], ptB[1], ptB[2] )
    mesh.vertices().add( ptC[0], ptC[1], ptC[2] )

    mesh.faces().addTriFace( 0, 1, 2 )

    mesh.vertexColors().add( 255, 0, 255 )
    mesh.vertexColors().add( 0, 255, 255 )
    mesh.vertexColors().add( 255, 255, 255 )

    mesh.normals().computeNormals()

    xform = rhino.Transform.translationVector( [1,10,20] )
    //const xform = rhino.Transform.rotation( [10, 10, 10], [3, 3, 3], [0,0,0] )
    //xform = rhino.Transform.mirror([0,0,0])
    mesh.transform(xform)

    doc.objects().add( mesh, oa_meshes )

    // -- BREPS -- //

    const oa_breps = new rhino.ObjectAttributes()
    oa_breps.layerIndex = brep_layer_index

    // Rhino3dmLoader will warn you that there is no mesh geometry
    const sphere = new rhino.Sphere( ptA, 2.5 )
    const brep = rhino.Brep.createFromSphere( sphere )

    doc.objects().add( brep, oa_breps )

    // -- EXTRUSIONS -- //

    const oa_extrusions = new rhino.ObjectAttributes()
    oa_extrusions.layerIndex = extrusion_layer_index

    const extrusion = rhino.Extrusion.create( nurbsCurveD4, 10, false )

    // Rhino3dmLoader will warn you that there is no mesh geometry
    doc.objects().add( extrusion, oa_extrusions )

    // -- SUBD -- //

    // -- TEXT DOT -- //

    const oa_dots = new rhino.ObjectAttributes()
    oa_dots.layerIndex = textdot_layer_index

    const dot = new rhino.TextDot( 'Hello!', ptD )
    doc.objects().add( dot, oa_dots )

    // create a copy of the doc.toByteArray data to get an ArrayBuffer
    let arr = new Uint8Array( doc.toByteArray() ).buffer

    loader.parse( arr, function ( object ) {

      // hide spinner
      document.getElementById('loader').style.display = 'none'
      console.log( object )

      object.traverse(child => {
        if (child.isPoints) {
          child.material.size = 10
        }
      }, false)
      console.log(object)
      scene.add( object )

      // enable download button
      downloadButton.disabled = false
  
    }, ( error ) => { console.error } )

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

function init () {

  // Rhino models are z-up, so set this as the default
  THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)

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
  light.position.set(0,0,1)
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
