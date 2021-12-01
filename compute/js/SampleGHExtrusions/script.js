// Import libraries
import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.126.0/build/three.module.js'
import { OrbitControls } from 'https://cdn.jsdelivr.net/npm/three@0.126.0/examples/jsm/controls/OrbitControls.js'
import rhino3dm from 'https://cdn.jsdelivr.net/npm/rhino3dm@0.15.0-beta/rhino3dm.module.js'
import { RhinoCompute } from 'https://cdn.jsdelivr.net/npm/compute-rhino3d@0.13.0-beta/compute.rhino3d.module.js'

const definitionName = 'BranchNodeRnd.gh'

// Set up sliders
const radius_slider = document.getElementById('radius')
radius_slider.addEventListener('mouseup', onSliderChange, false)
radius_slider.addEventListener('touchend', onSliderChange, false)

const length_slider = document.getElementById('length')
length_slider.addEventListener('mouseup', onSliderChange, false)
length_slider.addEventListener('touchend', onSliderChange, false)

const count_slider = document.getElementById('count')
count_slider.addEventListener('mouseup', onSliderChange, false)
count_slider.addEventListener('touchend', onSliderChange, false)

let rhino, definition
rhino3dm().then( async m => {
    console.log( 'Loaded rhino3dm.' )
    rhino = m // global

    RhinoCompute.url = getAuth( 'RHINO_COMPUTE_URL' ) // RhinoCompute server url. Use http://localhost:8081 if debugging locally.
    RhinoCompute.apiKey = getAuth( 'RHINO_COMPUTE_KEY' )  // RhinoCompute server api key. Leave blank if debugging locally.

    // load a grasshopper file!
    const url = definitionName
    const res = await fetch(url)
    const buffer = await res.arrayBuffer()
    const arr = new Uint8Array(buffer)
    definition = arr

    init()
    compute()
})

async function compute() {

    const param1 = new RhinoCompute.Grasshopper.DataTree('Length')
    param1.append([0], [length_slider.valueAsNumber])

    const param2 = new RhinoCompute.Grasshopper.DataTree('Radius')
    param2.append([0], [radius_slider.valueAsNumber])

    const param3 = new RhinoCompute.Grasshopper.DataTree('Count')
    param3.append([0], [count_slider.valueAsNumber])

    // clear values
    const trees = []
    trees.push(param1)
    trees.push(param2)
    trees.push(param3)

    const res = await RhinoCompute.Grasshopper.evaluateDefinition(definition, trees)

    console.log(res)

    // hide spinner
    document.getElementById('loader').style.display = 'none'

    const data = JSON.parse(res.values[0].InnerTree['{0}'][0].data)
    const mesh = rhino.DracoCompression.decompressBase64String(data)

    const material = new THREE.MeshNormalMaterial()
    const threeMesh = meshToThreejs(mesh, material)

    // clear the scene
    scene.traverse(child => {
        if (child.isMesh) {
            scene.remove(child)
        }
    })

    scene.add(threeMesh)
}

function onSliderChange() {
    // show spinner
    document.getElementById('loader').style.display = 'block'
    compute()
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

let scene, camera, renderer, controls

function init() {

    // Rhino models are z-up, so set this as the default
    THREE.Object3D.DefaultUp = new THREE.Vector3(0, 0, 1)

    scene = new THREE.Scene()
    scene.background = new THREE.Color(1, 1, 1)
    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 1000)
    camera.position.z = 50

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
    renderer.setSize(window.innerWidth, window.innerHeight)
    animate()
}

function meshToThreejs(mesh, material) {
    const loader = new THREE.BufferGeometryLoader()
    const geometry = loader.parse(mesh.toThreejsJSON())
    return new THREE.Mesh(geometry, material)
}