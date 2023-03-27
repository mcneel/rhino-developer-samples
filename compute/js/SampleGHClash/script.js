// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import rhino3dm from 'rhino3dm'
import { RhinoCompute } from 'rhinocompute'

// reference the definition
const definitionName = 'SampleGHClash.gh'

// globals
let definition, doc
// declare variables to store scene, camera, and renderer
let scene, camera, renderer

const rhino = await rhino3dm()

console.log('Loaded rhino3dm.')

RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.

// source a .gh / .ghx file in the same directory
let url = definitionName
let res = await fetch(url)
let buffer = await res.arrayBuffer()
definition = new Uint8Array(buffer)

init()
compute()


async function compute() {

    // create meshes
    const mainSphere = new THREE.SphereGeometry( 10, 32, 32 )
    const material = new THREE.MeshBasicMaterial( { wireframe:true, color: 0x000000 } )
    const mainMesh = new THREE.Mesh( mainSphere, material )
    mainMesh.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.MathUtils.degToRad(-90));
    scene.add(mainMesh)

    // create 3dm sphere
    const rhinoMesh = rhino.Mesh.createFromThreejsJSON( { data: mainSphere } )
    const rhinoMeshData = JSON.stringify( rhinoMesh.encode() )

    // create collision spheres
    const rhinoCollisionMeshData = []
    const pointsGeometry = new THREE.BufferGeometry()
    const positionBuffer = new Float32Array( 3 * 200 )

    for ( let i = 0; i < 200; i++ ) {
        const x = Math.random() * (20 - -20) + -20
        const y = Math.random() * (20 - -20) + -20
        const z = Math.random() * (20 - -20) + -20

        //create 3js clash sphere
        const clashSphere = new THREE.SphereGeometry( 2, 10, 10 )
        clashSphere.translate(x, y, z)

        //create 3dm clash sphere
        const clashMesh = rhino.Mesh.createFromThreejsJSON( { data: clashSphere } )
        const clashMeshData = JSON.stringify( clashMesh.encode() )
        rhinoCollisionMeshData.push( clashMeshData )

        positionBuffer[i*3] = x
        positionBuffer[i*3+1] = y
        positionBuffer[i*3+2] = z

    }

    //vizualize the position of the clash spheres
    pointsGeometry.setAttribute( 'position', new THREE.BufferAttribute( positionBuffer, 3 ) )
    const points = new THREE.Points(pointsGeometry, new THREE.PointsMaterial( { color: 0xff0000, sizeAttenuation: false, size: 3 } ))
    points.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.MathUtils.degToRad(-90))
    scene.add(points)

    // format data
    let param1 = new RhinoCompute.Grasshopper.DataTree('mainMesh')
    param1.append([0], [rhinoMeshData] )
    let param2 = new RhinoCompute.Grasshopper.DataTree('collisionMeshes')
    param2.append([0], rhinoCollisionMeshData)

    // Add all params to an array
    let trees = []
    trees.push( param1 )
    trees.push( param2 )

    // Call RhinoCompute

    const res = await RhinoCompute.Grasshopper.evaluateDefinition(definition, trees)

    console.log(res) 

    collectResults(res)

}

/**
 * Parse response
 */
function collectResults(responseJson) {

    const values = responseJson.values

    // clear doc
    if( doc !== undefined)
        doc.delete()

    //console.log(values)
    doc = new rhino.File3dm()

    // for each output (RH_OUT:*)...
    for ( let i = 0; i < values.length; i ++ ) {
      // ...iterate through data tree structure...
      for (const path in values[i].InnerTree) {
        const branch = values[i].InnerTree[path]
        // ...and for each branch...
        for( let j = 0; j < branch.length; j ++) {
          // ...load rhino geometry into doc
          const rhinoObject = decodeItem(branch[j])
          if (rhinoObject !== null) {
            doc.objects().add(rhinoObject, null)
          }
        }
      }
    }

    if (doc.objects().count < 1) {
      console.error('No rhino objects to load!')
      showSpinner(false)
      return
    }

    // set up loader for converting the results to threejs
    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.0.0-beta/' )

    const resMaterial = new THREE.MeshBasicMaterial( {wireframe: true, color: 0x00ff00} )

    // load rhino doc into three.js scene
    const buffer = new Uint8Array(doc.toByteArray()).buffer
    loader.parse(buffer, function ( object ) 
    {

        // add material to resulting meshes
        object.traverse( child => {
            child.material = resMaterial
            child.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.MathUtils.degToRad(-90))
        } )

        // add object graph from rhino model to three.js scene
        scene.add( object )

        // hide spinner
        showSpinner(false)

    }, (error) => {
      console.error( error )
    })
}

/**
 * Attempt to decode data tree item to rhino geometry
 */
function decodeItem(item) {
    const data = JSON.parse(item.data)
    if (item.type === 'System.String') {
      // hack for draco meshes
      try {
          return rhino.DracoCompression.decompressBase64String(data)
      } catch {} // ignore errors (maybe the string was just a string...)
    } else if (typeof data === 'object') {
      return rhino.CommonObject.decode(data)
    }
    return null
}

function getAuth( key ) {
    let value = localStorage[key]
    if ( value === undefined ) {
        const prompt = key.includes('URL') ? 'Server URL' : 'Server API Key'
        value = window.prompt('RhinoCompute ' + prompt)
        if ( value !== null ) {
            localStorage.setItem( key, value )
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

// BOILERPLATE //

function init() {

    // Rhino models are z-up, so set this as the default
    THREE.Object3D.DefaultUp = new THREE.Vector3( 0, 0, 1 )

    // create a scene and a camera
    scene = new THREE.Scene()
    scene.background = new THREE.Color(1, 1, 1)
    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000)
    camera.position.y = 30
    camera.position.z = 30

    // create the renderer and add it to the html
    renderer = new THREE.WebGLRenderer( { antialias: true } )
    renderer.setPixelRatio( window.devicePixelRatio )
    renderer.setSize( window.innerWidth, window.innerHeight )
    document.body.appendChild(renderer.domElement)

    // add some controls to orbit the camera
    const controls = new OrbitControls(camera, renderer.domElement)

    // add a directional light
    const directionalLight = new THREE.DirectionalLight( 0xffffff )
    directionalLight.intensity = 2
    scene.add( directionalLight )

    const ambientLight = new THREE.AmbientLight()
    scene.add( ambientLight )

    // handle changes in the window size
    window.addEventListener( 'resize', onWindowResize, false )

    animate()

}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight
    camera.updateProjectionMatrix()
    renderer.setSize( window.innerWidth, window.innerHeight )
    animate()
}

// function to continuously render the scene
function animate() {

    requestAnimationFrame(animate)
    renderer.render(scene, camera)

}
