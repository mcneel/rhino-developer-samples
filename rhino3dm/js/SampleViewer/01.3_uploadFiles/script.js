// #region IMPORTS //

import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { Rhino3dmLoader } from 'three/examples/jsm/loaders/3DMLoader'

// #endregion IMPORTS //

// #region GLOBALS //

const loader = new Rhino3dmLoader()
loader.setLibraryPath( 'https://unpkg.com/rhino3dm@8.4.0/' )

const upload = document.getElementById("file-upload")

let renderer, scene, camera, controls
const material = new THREE.MeshNormalMaterial()

// #endregion GLOBALS //

// #region EVENTS //

upload.addEventListener('change', function () {
    if (this.files && this.files[0]) {
        showSpinner(true)
        const file = this.files[0]

        const reader = new FileReader()

        reader.readAsArrayBuffer(file)

        reader.addEventListener('load', function (e) {
            const arr = new Uint8Array(e.target.result).buffer

            loader.parse(arr, (object) => {

                // hide spinner
                showSpinner(false)
                console.log(object)

           

                //clear objects from scene
                scene.traverse(child => {
                    if (child.userData.hasOwnProperty( 'objectType' ) && child.userData.objectType === 'File3dm' ) {
                        scene.remove(child)
                    }
                })

                //add doc to scene
                scene.add(object)

                // zoom to extents
                zoomCameraToSelection(camera, controls, [object])

                //animate()

            }, (error) => {
                console.log( error )
            } )
        })

    }
})

// #endregion EVENTS //

// #region INIT //

init()
animate()

function init() {

    // Rhino models are z-up, so set this as the default
    THREE.Object3D.DEFAULT_UP = new THREE.Vector3(0, 0, 1)

    //renderer

    renderer = new THREE.WebGLRenderer({ antialias: true })
    renderer.setPixelRatio(window.devicePixelRatio)
    renderer.setSize(window.innerWidth, window.innerHeight)
    document.body.appendChild(renderer.domElement)

    //scene

    // create a scene and a camera
    scene = new THREE.Scene()
    scene.background = new THREE.Color(0.8, 0.8, 0.8)


    //camera

    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000)
    camera.position.set(0,0,5)

    //controls
    controls = new OrbitControls(camera, renderer.domElement)

    // add a directional light
    const directionalLight = new THREE.DirectionalLight(0xffffff)
    directionalLight.intensity = 2
    scene.add(directionalLight)

    const ambientLight = new THREE.AmbientLight()
    scene.add(ambientLight)

    const sphereGeo = new THREE.SphereGeometry(4, 20, 20)
    const sphere = new THREE.Mesh(sphereGeo, new THREE.MeshNormalMaterial)
    //scene.add( sphere )

    // handle changes in the window size
    window.addEventListener('resize', onWindowResize, false)

}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight
    camera.updateProjectionMatrix()
    renderer.setSize(window.innerWidth, window.innerHeight)
}

// function to continuously render the scene
function animate() {

    requestAnimationFrame(animate)
    renderer.render(scene, camera)

}

// #endregion INIT //

// #region UTILITY //

/**
 * Shows or hides the loading spinner
 */
function showSpinner(enable) {
    if (enable)
        document.getElementById('loader').style.display = 'block'
    else
        document.getElementById('loader').style.display = 'none'
}


/**
 * Helper function that behaves like rhino's "zoom to selection", but for three.js!
 */
function zoomCameraToSelection(camera, controls, selection, fitOffset = 1.1) {

    const box = new THREE.Box3();

    for (const object of selection) {
        if (object.isLight) continue
        box.expandByObject(object);
    }

    const size = box.getSize(new THREE.Vector3());
    const center = box.getCenter(new THREE.Vector3());

    const maxSize = Math.max(size.x, size.y, size.z);
    const fitHeightDistance = maxSize / (2 * Math.atan(Math.PI * camera.fov / 360));
    const fitWidthDistance = fitHeightDistance / camera.aspect;
    const distance = fitOffset * Math.max(fitHeightDistance, fitWidthDistance);

    const direction = controls.target.clone()
        .sub(camera.position)
        .normalize()
        .multiplyScalar(distance);
    controls.maxDistance = distance * 10;
    controls.target.copy(center);

    camera.near = distance / 100;
    camera.far = distance * 100;
    camera.updateProjectionMatrix();
    camera.position.copy(controls.target).sub(direction);

    controls.update();

}

// #endregion UTILITY //