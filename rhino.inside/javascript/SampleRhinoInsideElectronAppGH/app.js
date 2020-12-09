const edge = require('electron-edge-js')

// construct the path to the .NET dll
const namespace = 'SampleRhinoInsideJavascriptTaskManagerLib'
const baseAppPath = '../' + namespace + '/bin/Debug/'
const baseDll = baseAppPath + namespace + '.dll'

console.log(baseDll)

const rhinoTypeName = namespace + '.RhinoMethods'

const startGrasshopper = edge.func({
  assemblyFile: baseDll,
  typeName: rhinoTypeName,
  methodName: 'StartGrasshopper'
})

// wait for the rhino3dm web assembly to load asynchronously
let rhino
rhino3dm().then((m) => {
  rhino = m // global
  console.log('rhino3dm loaded.')
  init()

  var payload = {
    cb: function (data, callback) {
        callback(null, onGhObjectAdded(data))
    }
  }

  startGrasshopper(payload, function(error, result) {
    if (error) throw JSON.stringify(error)
    console.log(result)
    // hide spinner
    document.getElementById('loader').style.display = 'none'
  })

})

function onGhObjectAdded(data)
{

  scene.traverseVisible(function(child) {
    if (child.type !== 'Scene') {
       scene.remove(child);
    }
  });


  //convert this to object
  var obj = JSON.parse(data);
  var rhinoMesh = rhino.CommonObject.decode(obj);

  let material = new THREE.MeshNormalMaterial();
  var threeMesh = meshToThreejs(rhinoMesh, material);

  scene.add(threeMesh);
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

  camera.position.z = 50

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

