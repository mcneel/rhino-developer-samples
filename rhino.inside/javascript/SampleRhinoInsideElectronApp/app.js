const edge = require('electron-edge-js')

// construct the path to the .NET dll
const namespace = 'SampleRhinoInsideJavascriptMethodsLib'
const baseAppPath = '../' + namespace + '/bin/Debug/'
const baseDll = baseAppPath + namespace + '.dll'

// console.log(baseDll)

const rhinoTypeName = namespace + '.RhinoMethods'

const startRhino = edge.func({
    assemblyFile: baseDll,
    typeName: rhinoTypeName,
    methodName: 'StartRhino'
})

const makeSphere = edge.func({
    assemblyFile: baseDll,
    typeName: rhinoTypeName,
    methodName: 'MakeSphere'
})

// wait for the rhino3dm web assembly to load asynchronously
let rhino
rhino3dm().then((m) => {
  rhino = m // global
  console.log('rhino3dm loaded.')
  init()
  run()
})

function run () {

  startRhino('', function(error, result) {
    if (error) throw error
    console.log(result)

    let params = {radius: 2.00, countU: 20, countV:10}

    makeSphere(params, function(error, result) {
      if (error) throw error

      // hide spinner
      document.getElementById('loader').style.display = 'none'

      //convert this to object
      const obj = JSON.parse(result)
      const rhinoMesh = rhino.CommonObject.decode(obj)

      let material = new THREE.MeshBasicMaterial( {wireframe: true, color: 0x00ff00 } )
      const threeMesh = meshToThreejs(rhinoMesh, material)

      console.log(`Mesh has ${rhinoMesh.vertices().count} vertices and ${rhinoMesh.faces().count} faces.`)
      rhinoMesh.delete()

      scene.add(threeMesh)

    })

  })

}

function meshToThreejs (mesh, material) {
  let loader = new THREE.BufferGeometryLoader()
  let geometry = loader.parse(mesh.toThreejsJSON())
  return new THREE.Mesh(geometry, material)
}

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

  camera.position.z = 15

  window.addEventListener( 'resize', onWindowResize, false )

  animate()
}

const animate = function () {
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

