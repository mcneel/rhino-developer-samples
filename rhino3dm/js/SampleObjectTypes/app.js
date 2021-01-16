let rhino
rhino3dm().then(async m => {
    console.log('Loaded rhino3dm.')
    rhino = m // global
    init()
    create()
})

function create () {

    const loader = new THREE.BufferGeometryLoader();

    // -- POINTS / POINTCLOUDS -- //

    // POINTS

    let ptA = [0, 0, 0]

    const geometry = new THREE.BufferGeometry()
    const vertices = new Float32Array( ptA )
    geometry.setAttribute( 'position', new THREE.BufferAttribute( vertices, 3 ) )
    const pointsMaterial = new THREE.PointsMaterial( { color: 0xff0000 } )
    const threejsPoint = new THREE.Points( geometry, pointsMaterial )

    scene.add(threejsPoint)

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

    const pointCloud = new rhino.PointCloud()
    pointCloud.add( ptA )
    pointCloud.add( ptB )
    pointCloud.add( ptC )
    pointCloud.add( ptD )
    pointCloud.add( ptE )
    pointCloud.add( ptF )
    pointCloud.add( ptG )
    pointCloud.add( ptH )
    pointCloud.add( ptI )
    pointCloud.add( ptJ )
    
    const threejsPointsGeometry = loader.parse ( pointCloud.toThreejsJSON() )
    const threejsPoints = new THREE.Points( threejsPointsGeometry, pointsMaterial )

    scene.add( threejsPoints )

    // -- CURVES -- //

    // LINE //

    const line = new rhino.Line( ptA, ptE) 

    const lineGeometry = new THREE.BufferGeometry()
    const lineVertices = new Float32Array( line.from.concat( line.to ) )
    lineGeometry.setAttribute( 'position', new THREE.BufferAttribute( lineVertices, 3 ) )
    const lineMaterial = new THREE.LineBasicMaterial( { color: 0x0000ff } )
    const lineObject = new THREE.Line( lineGeometry, lineMaterial )
    scene.add( lineObject )

    // CIRCLE //

    const circle = new rhino.Circle( 10 )
    circle.center = ptC

    const circleGeometry = new THREE.RingBufferGeometry( circle.radius, circle.radius, 32 )
    const circleObject = new THREE.Line( circleGeometry, lineMaterial )
    circleObject.position.fromArray( circle.center )
    scene.add( circleObject )

    // ARC //

    const arc = new rhino.Arc( circle, Math.PI )
    const arcGeometry = new THREE.RingBufferGeometry( arc.radius, arc.radius, 32, 1, 0, arc.angleRadians )
    const arcObject = new THREE.Line( arcGeometry, lineMaterial )

    scene.add( arcObject )

    // ELLIPSE //

    // CURVE //

    const curvePoints = new rhino.Point3dList()
    curvePoints.add( ptF[0], ptF[1], ptF[2] )
    curvePoints.add( ptG[0], ptG[1], ptG[2] )
    curvePoints.add( ptH[0], ptH[1], ptH[2] )
    curvePoints.add( ptI[0], ptI[1], ptI[2] )
    curvePoints.add( ptJ[0], ptJ[1], ptJ[2] )

    const nurbsCurve = rhino.NurbsCurve.create( false, 3, curvePoints )

    let cPts = []
    let domain = nurbsCurve.domain
    let pointCount = 20
    let divisions = pointCount - 1.0

    for (let i = 0; i < 20; i ++ ) {

        let t = domain[ 0 ] + ( i / divisions ) * ( domain[ 1 ] - domain[ 0 ] );

        const tPt = nurbsCurve.pointAt( t )

        cPts.push( tPt[0] )
        cPts.push( tPt[1] )
        cPts.push( tPt[2] )

    }

    const curveGeometry = new THREE.BufferGeometry()
    const curveVertices = new Float32Array( cPts )
    curveGeometry.setAttribute( 'position', new THREE.BufferAttribute( curveVertices, 3 ) )
    const curveObject = new THREE.Line( curveGeometry, lineMaterial )

    scene.add( curveObject )

    // -- MESHES -- //

    // -- BREPS -- //

    // -- EXTRUSIONS -- //

    // -- SUBD -- //

    // hide spinner
    document.getElementById('loader').style.display = 'none'

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

  controls = new THREE.OrbitControls( camera, renderer.domElement  )

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