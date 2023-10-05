import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'
import rhino3dm from 'rhino3dm'

const model = 'RectifiedArchimedeanSolids.3dm'
let rotation = 0.01
const selectMaterial = new THREE.MeshStandardMaterial( { metalness: 1, roughness: 0.7, color: 0xffff00 } )
const material = new THREE.MeshStandardMaterial( { metalness: 1, roughness: 0.7, color: 0xadd8e6 } )

window.addEventListener( 'click', handleInteraction, false)
window.addEventListener( 'mousemove', handleInteraction, false)
window.addEventListener( 'touchstart', handleInteraction, false)

// globals
let doc, meshes, parsedDoc
// declare variables to store scene, camera, and renderer
let scene, camera, renderer, mouse, raycaster

const rhino = await rhino3dm()

init()
load()


async function load() {

    const res = await fetch(model)
    const buffer = await res.arrayBuffer()
    const arr = new Uint8Array(buffer)
    doc = rhino.File3dm.fromByteArray(arr)

    const objs = doc.objects()
    const cnt = objs.count

    // Get User Strings from Attributes and Object straight from the Rhino file:

    console.group( 'User Strings from .3dm' )

    for ( var i = 0; i < cnt; i ++ ) {

        const rhinoObject = objs.get( i )

        if ( rhinoObject.geometry().objectType === rhino.ObjectType.Curve ) continue
        
        if ( rhinoObject.attributes().userStringCount > 0 ) {
            // get them
            const a_userStrings = rhinoObject.attributes().getUserStrings()

            console.log( a_userStrings )
        }

        if ( rhinoObject.geometry().userStringCount > 0 ) {
            // get them
            const g_userStrings = rhinoObject.geometry().getUserStrings()
            console.log( g_userStrings )
        }
    }

    console.groupEnd()

    // load and pass to threejs
    const loader = new Rhino3dmLoader()
    loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.0.0-beta2/' )

    loader.load( model, function ( object ) {

        // uncomment to hide spinner when model loads
        document.getElementById('loader').remove()
        object.traverse( child => {
            if (child.isMesh) {
                child.material = material
            }
        })
        scene.add( object )
        parsedDoc = object

    } , ( progress ) => { 
        console.log( ( progress.loaded / progress.total ) * 100 ) 
    }, ( error ) => { console.error( error ) } )


}

// function to setup the scene, camera, renderer, and load 3d model
function init () {

    // Rhino models are z-up, so set this as the default
    THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0,0,1)

    // create a scene and a camera
    scene = new THREE.Scene()
    scene.background = new THREE.Color(1,1,1)
    camera = new THREE.PerspectiveCamera( 65, window.innerWidth / window.innerHeight, 0.1, 1000 )
    camera.position.y = 10
    camera.position.z = 5

    // create the renderer and add it to the html
    renderer = new THREE.WebGLRenderer( { antialias: true } )
    renderer.setPixelRatio( window.devicePixelRatio )
    renderer.setSize( window.innerWidth, window.innerHeight )
    document.body.appendChild( renderer.domElement )

    // add some controls to orbit the camera
    const controls = new OrbitControls( camera, renderer.domElement )

    // add a directional light
    const directionalLight = new THREE.DirectionalLight( 0xffffff )
    directionalLight.intensity = 5
    scene.add( directionalLight )

    // add a directional light
    const ambientLight = new THREE.AmbientLight( 0xadd8e6 )
    ambientLight.intensity = 2
    scene.add( ambientLight )

    raycaster = new THREE.Raycaster()
    mouse = new THREE.Vector2()

    // handle changes in the window size
    window.addEventListener( 'resize', onWindowResize, false )

    animate()

}

function handleInteraction( event ) {

    // console.log( event )

    let coordinates = null
    if ( event instanceof MouseEvent ) {
        if ( event.type === 'click' ) {
            coordinates =  { x: event.clientX, y: event.clientY }
        }
        else if ( event.type === 'mousemove' ) {
            rotation = Math.abs( 1 - ( event.clientX / ( window.innerWidth / 2 ) ) ) / 10
            return
        }
    } else if ( event instanceof TouchEvent ) {
        if ( event.type === 'touchstart' ) {
            coordinates =  { x: event.changedTouches[0].clientX, y: event.changedTouches[0].clientY }
            rotation = Math.abs( 1 - ( event.changedTouches[0].clientX / ( window.innerWidth / 2 ) ) ) / 10
        }
    }
    onClick( coordinates )
}

function onClick( coo ) {

    console.log( `click! (${coo.x}, ${coo.y})`)

	// calculate mouse position in normalized device coordinates
    // (-1 to +1) for both components

	mouse.x = ( coo.x / window.innerWidth ) * 2 - 1
    mouse.y = - ( coo.y / window.innerHeight ) * 2 + 1
    
    raycaster.setFromCamera( mouse, camera )

    meshes = []
    // reset object colours
    scene.traverse((child, i) => {
        if (child.isMesh) {
            child.material = material
            meshes.push(child)
        }
    })

	// calculate objects intersecting the picking ray
    const intersects = raycaster.intersectObjects( meshes, true )

    let container = document.getElementById( 'container' )
    if (container) container.remove()

    if (intersects.length > 0) {

        console.group( 'User Strings from threejs (parsed 3dm)' )

        // get closest object
        const object = intersects[0].object
        console.log(object) // debug

        object.material = selectMaterial

        // get user strings
        let data, count, geometryData
        console.log(object.userData)
        if (object.userData.attributes !== undefined) {
            data = object.userData.attributes.userStrings
            geometryData = object.userData.attributes.geometry.userStrings
        }

        // do nothing if no user strings
        if ( data === undefined && geometryData === undefined ) return

        console.log( data )
        console.log( geometryData )

        console.groupEnd()
        
        // create container div with table inside
        container = document.createElement( 'div' )
        container.id = 'container'
        
        const table = document.createElement( 'table' )
        container.appendChild( table )

        for ( let i = 0; i < data.length; i ++ ) {

            const row = document.createElement( 'tr' )
            row.innerHTML = `<td>${data[ i ][ 0 ]}</td><td>${data[ i ][ 1 ]}</td>`
            table.appendChild( row )
        }

        const row = document.createElement( 'tr' )
        row.innerHTML = `<td>Name</td><td>${object.name}</td>`
        table.appendChild( row )

        document.body.appendChild( container )

        for ( let i = 0; i < geometryData.length; i ++ ) {

            const row = document.createElement( 'tr' )
            row.innerHTML = `<td>${geometryData[ i ][ 0 ]}</td><td>${geometryData[ i ][ 1 ]}</td>`
            table.appendChild( row )
        }
    }

}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight
    camera.updateProjectionMatrix()
    renderer.setSize( window.innerWidth, window.innerHeight )
    animate()
}

// function to continuously render the scene
function animate() {
    requestAnimationFrame( animate )
    if (parsedDoc !== undefined)
        parsedDoc.rotation.z += rotation
    renderer.render( scene, camera )
}