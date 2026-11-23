// Import libraries
import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'
import { RhinoCompute } from 'rhinocompute'

let model = {
    // main sphere
    sphere: null,
    sphereRadius: 10,
    // spheres array to clash with main sphere
    spheres: [],
    spheresRadius: 2,
    // number of spheres to create
    num: 200,
  };

let scene, camera, renderer

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.');
RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.

init();
compute();

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

function compute() {

    console.log('Creating main sphere');

    // create and render 3js sphere
    const mainSphere = new THREE.SphereGeometry(model.sphereRadius, 32, 32);
    const material = new THREE.MeshStandardMaterial({wireframe:true});
    const mainMesh = new THREE.Mesh(mainSphere, material);
    mainMesh.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.MathUtils.degToRad(-90));
    scene.add(mainMesh);

    // create 3dm sphere
    let mainRhinoSphere = meshFromThreejs( mainSphere )
    model.sphere = mainRhinoSphere;

    createClashSpheres();
    doMeshClash();

}

function createClashSpheres() {

    console.log('Creating clash spheres');

    let pointsGeometry = new THREE.BufferGeometry();
    let positionBuffer = new Float32Array(3 * model.num);

    for (let i = 0; i < model.num; i++) {
        let x = Math.random() * (20 - -20) + -20;
        let y = Math.random() * (20 - -20) + -20;
        let z = Math.random() * (20 - -20) + -20;

        //create 3js clash sphere
        let clashSphere = new THREE.SphereGeometry( model.spheresRadius, 10, 10 );
        clashSphere.translate(x, y, z);

        //create 3dm clash sphere
        let rhinoClashSphere = meshFromThreejs( clashSphere );
        model.spheres.push(rhinoClashSphere);

        positionBuffer[i*3] = x;
        positionBuffer[i*3+1] = y;
        positionBuffer[i*3+2] = z;

    }

    //vizualize the position of the clash spheres
    pointsGeometry.setAttribute( 'position', new THREE.BufferAttribute( positionBuffer, 3 ) );
    let points = new THREE.Points(pointsGeometry, new THREE.PointsMaterial( { color: 0xff0000, sizeAttenuation: false, size: 3 } ));
    scene.add(points);

} 

function doMeshClash() {

    console.log('Running mesh clash');

    RhinoCompute.computeFetch('rhino/geometry/intersect/meshclash/search', [model.sphere, model.spheres, 0.1, 5])
        .then(function (result) {

            console.log(result);

            // remove spinner
            document.getElementById('loader').remove();

            //add objects to scene
            for (var i = 0; i < result.length; i++) {

                let m = rhino.CommonObject.decode(result[i].MeshB);
                let material = new THREE.MeshBasicMaterial({ wireframe: true, color: 0x00ff00 });
     
                let threemesh = meshToThreejs(m, material);
                
                //need to rotate these
                threemesh.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.MathUtils.degToRad(-90));
                scene.add(threemesh);

                m.delete();
            }

    });
}

// BOILERPLATE //



function init(){

    scene = new THREE.Scene();
    scene.background = new THREE.Color(1,1,1);
    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000)
    camera.position.y = 30
    camera.position.z = 30

    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
    document.body.appendChild( renderer.domElement );

    const controls = new OrbitControls( camera, renderer.domElement );

    window.addEventListener( 'resize', onWindowResize, false );

    animate();
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

// three.js BufferGeometry -> rhino3dm Mesh (three.js is Y-up, Rhino is Z-up)
function meshFromThreejs(geometry) {
  return rhino.Mesh.createFromThreejsBuffers( {
    position: geometry.attributes.position.array,
    normal: geometry.attributes.normal?.array,
    uv: geometry.attributes.uv?.array,
    index: geometry.index?.array
  }, true )
}

// rhino3dm Mesh -> three.js mesh
function meshToThreejs(mesh, material) {
  const buffers = mesh.toThreejsBuffers(false)
  const geometry = new THREE.BufferGeometry()
  geometry.setAttribute('position', new THREE.BufferAttribute(buffers.position, 3))
  geometry.setAttribute('normal', new THREE.BufferAttribute(buffers.normal, 3))
  geometry.setIndex(new THREE.BufferAttribute(buffers.index, 1))
  return new THREE.Mesh(geometry, material)
}
