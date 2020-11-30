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

rhino3dm().then( async m => {
    console.log('Loaded rhino3dm.');
    rhino = m; // global

    /* 
    *  Variables for Rhino.Compute
    */

    // if calling McNeel's public Rhino.Compute Server, get an auth token:
    RhinoCompute.authToken = RhinoCompute.getAuthToken();

    // if calling your own compute server, comment the RhinoCompute.authToken line
    // and uncomment the following two lines:

    //RhinoCompute.url = 'http://your-compute-server-address/'
    //RhinoCompute.apiKey = 'your-compute-apikey'

    init();
    compute();

});

function compute() {

    console.log('Creating main sphere');

    // create and render 3js sphere
    let mainSphere = new THREE.SphereBufferGeometry(model.sphereRadius, 32, 32);
    let material = new THREE.MeshStandardMaterial({wireframe:true});
    let mainMesh = new THREE.Mesh(mainSphere, material);
    mainMesh.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.Math.degToRad(-90));
    scene.add(mainMesh);

    // create 3dm sphere
    let mainRhinoSphere = rhino.Mesh.createFromThreejsJSON( { data: mainSphere } )
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
        let clashSphere = new THREE.SphereBufferGeometry( model.spheresRadius, 10, 10 );
        clashSphere.translate(x, y, z);

        //create 3dm clash sphere
        let rhinoClashSphere = rhino.Mesh.createFromThreejsJSON( { data: clashSphere } );
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
     
                const loader = new THREE.BufferGeometryLoader();
                const geometry = loader.parse((m.toThreejsJSON()));
                let threemesh = new THREE.Mesh(geometry, material);
                
                //need to rotate these
                threemesh.rotateOnWorldAxis(new THREE.Vector3(1,0,0), THREE.Math.degToRad(-90));
                scene.add(threemesh);

                m.delete();
            }

    });
}

// BOILERPLATE //

var scene, camera, renderer, controls;

function init(){
    scene = new THREE.Scene();
    scene.background = new THREE.Color(1,1,1);
    camera = new THREE.PerspectiveCamera( 45, window.innerWidth/window.innerHeight, 1, 1000 );

    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
    document.body.appendChild( renderer.domElement );

    controls = new THREE.OrbitControls( camera, renderer.domElement );

    camera.position.z = 50;

    window.addEventListener( 'resize', onWindowResize, false );

    animate();
}

var animate = function () {
    requestAnimationFrame( animate );
    controls.update();
    renderer.render( scene, camera );
};
  
function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( window.innerWidth, window.innerHeight );
    animate();
}
