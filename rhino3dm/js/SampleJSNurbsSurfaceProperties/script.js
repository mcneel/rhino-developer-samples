import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'

const downloadButton = document.getElementById("downloadButton")
downloadButton.onclick = download

let doc
let scene, camera, renderer

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

init()
load()
//create()

async function load() {
  // load 3dm

  const file = await fetch( '../../models/nurbssurface.3dm' )
  const buffer = await file.arrayBuffer()
  const arr = new Uint8Array(buffer)
  const doc = rhino.File3dm.fromByteArray(arr)

  const objects = doc.objects

  for ( const object of objects ) {

    const objectType = object.geometry.objectType

    //console.log( objectType )

    switch ( objectType ) {

      case rhino.ObjectType.Brep:

        const brep = object.geometry

        console.log(`Brep Vertices Count: ${brep.vertices.count}`)
        console.log(`Brep Vertex: ${JSON.stringify(brep.vertices.get(3).location)}`)
        console.log(`Brep Vertex Index: ${brep.vertices.get(3).vertexIndex}`)
        console.log(`Brep Edge Count: ${brep.vertices.get(3).edgeIndices().length}`)
        console.log(`Brep Edge Indices: ${brep.vertices.get(3).edgeIndices()}`)
        console.log(`Brep Edge Index: ${brep.vertices.get(3).edgeIndices()[1]}`)

        const surface = brep.surfaces.get( 0 )

        console.log( surface.domain( 0 ) ) //domain in u
        console.log( surface.domain( 1 ) ) //domain in v

        surface.setDomain( 0, new rhino.Interval( 0, 1 ) )
        surface.setDomain( 1, new rhino.Interval( 0, 1 ) )

        console.log( surface.domain( 0 ) ) //domain in u
        console.log( surface.domain( 1 ) ) //domain in v

        const s_points = []

        for ( let u = 0; u < 1; u += 0.01 ) {

          for ( let v = 0; v < 1; v += 0.01 ) {

            const pt3d = surface.pointAt( u, v )

            const pt = new THREE.Vector3( pt3d.X, pt3d.Y, pt3d.Z )

            s_points.push( pt )

          }

        }

        const spt_geometry = new THREE.BufferGeometry().setFromPoints( s_points )
        const spt_material = new THREE.PointsMaterial( { color: 0x000000, size: 2, sizeAttenuation: false} )
        const spt = new THREE.Points( spt_geometry, spt_material )
        scene.add( spt )

        const nurbssurface = surface.toNurbsSurface()

        console.log( nurbssurface.orderU )
        console.log( nurbssurface.orderV )
        console.log( nurbssurface.points.countU )
        console.log( nurbssurface.points.countV )
        console.log( nurbssurface.points.countU * nurbssurface.points.countV ) // NurbsSurfacePointList has no count, as in RhinoCommon
        console.log( nurbssurface.makeRational() )
        console.log( nurbssurface.makeNonRational() )
        console.log( nurbssurface.points.getControlPoint( 0, 0 ) )
        console.log( nurbssurface.knotsU.get( 3 ) )

        const points = []

        const cnt_u = nurbssurface.points.countU
        const cnt_v = nurbssurface.points.countV

        for ( let u = 0; u < cnt_u; u ++ ) {

          for ( let v = 0; v < cnt_v; v ++ ) {

            const pt4d = nurbssurface.points.getControlPoint( u, v )
            const [ ok, pt3d ] = nurbssurface.points.getPoint( u, v )

            const pt = new THREE.Vector3( pt3d.X, pt3d.Y, pt3d.Z )
            points.push( pt )

          }

        }

        const pt_geometry = new THREE.BufferGeometry().setFromPoints( points )
        const pt_material = new THREE.PointsMaterial( { color: 0xff0000, size: 5, sizeAttenuation: false } )
        const pts = new THREE.Points( pt_geometry, pt_material )
        scene.add( pts )



        const buffers = object.geometry.faces.get( 0 ).getMesh( rhino.MeshType.Any ).toThreejsBuffers( false )

        const geometry = new THREE.BufferGeometry()
        geometry.setAttribute( 'position', new THREE.BufferAttribute( buffers.position, 3 ) )
        geometry.setAttribute( 'normal', new THREE.BufferAttribute( buffers.normal, 3 ) )
        geometry.setIndex( new THREE.BufferAttribute( buffers.index, 1 ) )

        const material = new THREE.MeshNormalMaterial( { side: THREE.DoubleSide } )

        const mesh = new THREE.Mesh( geometry, material )

        scene.add( mesh )
      
        break
      case rhino.ObjectType.Curve:

        const nurbscurve = object.geometry.toNurbsCurve()

        console.log( nurbscurve.points.get( 0 ) )

        break
    }

    

    // hide spinner
    document.getElementById('loader').style.display = 'none'



  }
}


function create() {

  doc = new rhino.File3dm()

  // -- POINTS / POINTCLOUDS -- //

  // POINTS

  let ptA = { X: 0, Y: 0, Z: 0 }

  const geometry = new THREE.BufferGeometry()
  const vertices = new Float32Array([ptA.X, ptA.Y, ptA.Z])
  geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3))
  const pointsMaterial = new THREE.PointsMaterial({ color: 0xff0000 })
  const threejsPoint = new THREE.Points(geometry, pointsMaterial)

  scene.add(threejsPoint)
  doc.objects.addPoint(ptA)

  // POINTCLOUD

  const ptB = { X: 10, Y: 0, Z: 0 }
  const ptC = { X: 10, Y: 10, Z: 0 }
  const ptD = { X: 0, Y: 10, Z: 0 }
  const ptE = { X: 20, Y: 20, Z: 20 }
  const ptF = { X: 30, Y: 0, Z: 0 }
  const ptG = { X: 35, Y: 5, Z: 0 }
  const ptH = { X: 40, Y: -5, Z: 0 }
  const ptI = { X: 45, Y: 5, Z: 0 }
  const ptJ = { X: 50, Y: 0, Z: 0 }

  const red = { r: 255, g: 0, b: 0, a: 0 }

  const pointCloud = new rhino.PointCloud()
  pointCloud.add(ptA, red)
  pointCloud.add(ptB, red)
  pointCloud.add(ptC, red)
  pointCloud.add(ptD, red)
  pointCloud.add(ptE, red)
  pointCloud.add(ptF, red)
  pointCloud.add(ptG, red)
  pointCloud.add(ptH, red)
  pointCloud.add(ptI, red)
  pointCloud.add(ptJ, red)

  const threejsPointsGeometry = new THREE.BufferGeometry()
  threejsPointsGeometry.setAttribute('position', new THREE.BufferAttribute(pointCloud.toThreejsBuffers(false).position, 3))
  const threejsPoints = new THREE.Points(threejsPointsGeometry, pointsMaterial)

  scene.add(threejsPoints)
  doc.objects.add(pointCloud, null)

  // -- CURVES -- //

  // LINE //

  const line = new rhino.LineCurve(ptA, ptE)

  const lineGeometry = new THREE.BufferGeometry()
  const lineVertices = new Float32Array([line.line.From.X, line.line.From.Y, line.line.From.Z, line.line.To.X, line.line.To.Y, line.line.To.Z])
  lineGeometry.setAttribute('position', new THREE.BufferAttribute(lineVertices, 3))
  const lineMaterial = new THREE.LineBasicMaterial({ color: 0x0000ff })
  const lineObject = new THREE.Line(lineGeometry, lineMaterial)
  scene.add(lineObject)
  doc.objects.add(line.toNurbsCurve(), null)

  // CIRCLE //

  const circle = new rhino.Circle(10)

  const circleGeometry = new THREE.RingGeometry(circle.Radius, circle.Radius, 32)
  const circleObject = new THREE.Line(circleGeometry, lineMaterial)
  const center = rhino.Circle.center(circle)
  circleObject.position.set(center.X, center.Y, center.Z)
  scene.add(circleObject)
  doc.objects.add(rhino.Circle.toNurbsCurve(circle), null)

  // ARC //

  const arc = new rhino.Arc(circle, Math.PI)
  const arcGeometry = new THREE.RingGeometry(arc.Radius, arc.Radius, 32, 1, 0, rhino.Arc.angle(arc))
  const arcObject = new THREE.Line(arcGeometry, lineMaterial)

  scene.add(arcObject)
  doc.objects.add(rhino.Arc.toNurbsCurve(arc), null)

  // ELLIPSE //

  // CURVE //

  const curvePoints = new rhino.Point3dList()
  curvePoints.add(ptF.X, ptF.Y, ptF.Z)
  curvePoints.add(ptG.X, ptG.Y, ptG.Z)
  curvePoints.add(ptH.X, ptH.Y, ptH.Z)
  curvePoints.add(ptI.X, ptI.Y, ptI.Z)
  curvePoints.add(ptJ.X, ptJ.Y, ptJ.Z)

  const nurbsCurve = rhino.NurbsCurve.create(false, 3, curvePoints)

  let cPts = []
  let domain = nurbsCurve.domain
  let pointCount = 20
  let divisions = pointCount - 1.0

  for (let i = 0; i < 20; i++) {

    let t = domain.T0 + (i / divisions) * (domain.T1 - domain.T0);

    const tPt = nurbsCurve.pointAt(t)

    cPts.push(tPt.X)
    cPts.push(tPt.Y)
    cPts.push(tPt.Z)

  }

  const curveGeometry = new THREE.BufferGeometry()
  const curveVertices = new Float32Array(cPts)
  curveGeometry.setAttribute('position', new THREE.BufferAttribute(curveVertices, 3))
  const curveObject = new THREE.Line(curveGeometry, lineMaterial)

  scene.add(curveObject)
  doc.objects.add(nurbsCurve, null)

  // -- MESHES -- //

  const mesh = new rhino.Mesh()
  mesh.vertices.add(ptA.X, ptA.Y, ptA.Z)
  mesh.vertices.add(ptB.X, ptB.Y, ptB.Z)
  mesh.vertices.add(ptC.X, ptC.Y, ptC.Z)

  mesh.faces.addFace(0, 1, 2)

  mesh.vertexColors.add(255, 0, 255)
  mesh.vertexColors.add(0, 255, 255)
  mesh.vertexColors.add(255, 255, 255)

  mesh.normals.computeNormals()

  const buffers = mesh.toThreejsBuffers(false)
  const meshGeometry = new THREE.BufferGeometry()
  meshGeometry.setAttribute('position', new THREE.BufferAttribute(buffers.position, 3))
  meshGeometry.setAttribute('normal', new THREE.BufferAttribute(buffers.normal, 3))
  meshGeometry.setAttribute('color', new THREE.BufferAttribute(buffers.color, 3))
  meshGeometry.setIndex(new THREE.BufferAttribute(buffers.index, 1))
  const threejsMesh = new THREE.Mesh(meshGeometry, new THREE.MeshStandardMaterial({ vertexColors: true }))
  scene.add(threejsMesh)
  doc.objects.add(mesh, null)

  // hide spinner
  document.getElementById('loader').style.display = 'none'

  // enable download button
  downloadButton.disabled = false

  console.log(scene)

}

// download button handler
function download() {
  const options = new rhino.File3dmWriteOptions()
  options.version = 7
  let buffer = doc.toByteArray(options)
  saveByteArray('rhinoObjectTypes' + options.version + '.3dm', buffer)
}

function saveByteArray(fileName, byte) {
  let blob = new Blob([byte], { type: 'application/octect-stream' })
  let link = document.createElement('a')
  link.href = window.URL.createObjectURL(blob)
  link.download = fileName
  link.click()
}

// BOILERPLATE //

function init() {

  // Rhino models are z-up, so set this as the default
  THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)

  scene = new THREE.Scene()
  scene.background = new THREE.Color(1, 1, 1)
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 1000)
  camera.position.z = 50

  renderer = new THREE.WebGLRenderer({ antialias: true })
  renderer.setPixelRatio(window.devicePixelRatio)
  renderer.setSize(window.innerWidth, window.innerHeight)
  document.body.appendChild(renderer.domElement)

  const controls = new OrbitControls(camera, renderer.domElement)

  const light = new THREE.DirectionalLight()
  scene.add(light)

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
  renderer.setSize(window.innerWidth, window.innerHeight)
  animate()
}