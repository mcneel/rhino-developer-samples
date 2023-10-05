import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import { GUI } from 'dat.gui'

const model = 'hello_mesh.3dm'
let scene, camera, renderer, material, light

init()
initGui()
load()

function load() {

    material = new THREE.MeshPhysicalMaterial()

    loadPBRMaterial(material, 'chipped-paint-metal')
    material.metalness = 0.75
    material.roughness = 0.15
    material.normalScale.x = 1.0
    material.normalScale.y = 1.0
    material.envMap = scene.background

    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.0.0-beta2/' )

    // load 3dm file into three.js scene
    loader.load( model, function ( object ) {

        object.traverse( function (child) {
            // rotate each child object to match three.js default coordinate system (y = up)
            // normally we'd update the camera but this messes with the cubemaps
            child.rotateX( - Math.PI / 4 )

            // apply initial pbr material
            child.material = material
        })
        scene.add( object )
        console.log( object )

        // hide spinner
        document.getElementById('loader').style.display = 'none'

    } )

}

function loadPBRMaterial(material, name) {
    let tl = new THREE.TextureLoader()

    tl.setPath('materials/PBR/' + name + '/')
    material.map          = tl.load(name + '_base.png')
    material.aoMmap       = tl.load(name + '_ao.png')
    material.normalMap    = tl.load(name + '_normal.png')
    material.metalnessMap = tl.load(name + '_metallic.png')
}

function loadCubeMap(name, prefix, format) {
    const ctl = new THREE.CubeTextureLoader()

    ctl.setPath('textures/cube/' + name + '/')
    return ctl.load([prefix + 'px.' + format, prefix + 'nx.' + format, prefix + 'py.' + format,
                        prefix + 'ny.' + format, prefix + 'pz.' + format, prefix + 'nz.' + format])
}

function init() {

    scene = new THREE.Scene()
    scene.background = loadCubeMap('Park2', '', 'jpg')
    camera = new THREE.PerspectiveCamera( 35, window.innerWidth/window.innerHeight, 1, 10000 )
    camera.position.z = 50

    light = new THREE.DirectionalLight(0xffffff, 1.150)
    light.position.set(-10, 10, 0)
    camera.add( light )
    scene.add(camera)

    renderer = new THREE.WebGLRenderer({antialias: true})
    renderer.setPixelRatio( window.devicePixelRatio )
    renderer.setSize( window.innerWidth, window.innerHeight )
    renderer.shadowMap.enabled = true
    document.body.appendChild( renderer.domElement )

    const controls = new OrbitControls( camera, renderer.domElement  )

    window.addEventListener('resize', onWindowResize, false)
    animate()
}

function initGui() {
    var UserInterfaceControls = function() {
        this.lightColor = [light.color.r*255, light.color.g*255, light.color.b*255]
        this.cubemap = 'Park2'
        this.metalness = 0.75
        this.roughness = 0.15
        this.textures = 'chipped-paint-metal'
    }
    var ui_callbacks = new UserInterfaceControls()
    var gui = new GUI()
    gui.addColor(ui_callbacks, 'lightColor').onChange(function(value) {
        light.color.r = value[0]/255.0
        light.color.g = value[1]/255.0
        light.color.b = value[2]/255.0
        })
    gui.add(ui_callbacks, 'cubemap',[ 'Bridge2', 'MilkyWay', 'Park2', 'Park3Med', 'pisa', 'skyboxsun25deg', 'SwedishRoyalCastle' ]).onChange(function(value){
        const format = value === 'pisa' ? 'png' : 'jpg'
        scene.background = loadCubeMap(value, '', format)
        material.envMap = scene.background
    })
    let matFolder = gui.addFolder('Material')
    matFolder.add(ui_callbacks, 'metalness', 0.0, 1.0).onChange(function(value) {
        material.metalness = value
    })
    matFolder.add(ui_callbacks, 'roughness', 0.0, 1.0).onChange(function(value) {
        material.roughness = value
    })
    matFolder.add(ui_callbacks, 'textures',
        [ 'carbon-fiber', 'chipped-paint-metal', 'copper-rock1', 'grimy-metal',
        'metalgrid2', 'ornate-brass2', 'rust-coated', 'rust-panel', 'scuffed-plastic',
        'streaked-metal1' ]).onChange(function(value){
            loadPBRMaterial(material, value)
    })

    matFolder.open()

}

function animate() {
    requestAnimationFrame( animate )
    renderer.render( scene, camera )
}

function onWindowResize(){
    camera.aspect = window.innerWidth / window.innerHeight
    camera.updateProjectionMatrix()
    renderer.setSize( window.innerWidth, window.innerHeight )
    animate()
}