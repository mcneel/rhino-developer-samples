let args = {
    algo : null, 
    pointer : null,
    values : []
};

let definition = null;

// get slider values
let count = document.getElementById('count').value;
let radius = document.getElementById('radius').value;
let length = document.getElementById('length').value;

let param1 = new RhinoCompute.Grasshopper.DataTree('RH_IN:201:Length');
param1.append([0], [length]);

let param2 = new RhinoCompute.Grasshopper.DataTree('RH_IN:201:Radius');
param2.append([0], [radius]);

let param3 = new RhinoCompute.Grasshopper.DataTree('RH_IN:201:Count');
param3.append([0], [count]);

rhino3dm().then(async m => {
    console.log('Loaded rhino3dm.');
    rhino = m; // global

    RhinoCompute.url = "http://localhost:8081/" // Rhino.Compute server url
    RhinoCompute.apiKey = "" // your Rhino.Compute server api key

    // load a grasshopper file!
    let url = 'BranchNodeRnd.gh';
    let res = await fetch(url);
    let buffer = await res.arrayBuffer();
    let arr = new Uint8Array(buffer);
    definition = arr;

    init();
    compute();
});

function compute(){

    // clear values
    let trees = [];

    trees.push(param1);
    trees.push(param2);
    trees.push(param3);

    RhinoCompute.Grasshopper.evaluateDefinition(definition, trees).then(result => {
        // RhinoCompute.computeFetch("grasshopper", args).then(result => {
        console.log(result);

        // hide spinner
        document.getElementById('loader').style.display = 'none';

        let data = JSON.parse(result.values[0].InnerTree['{ 0; }'][0].data);
        let mesh = rhino.CommonObject.decode(data);

        let material = new THREE.MeshNormalMaterial();
        let threeMesh = meshToThreejs(mesh, material);

        // clear meshes from scene
        scene.traverse(child => {
            if(child.type === 'Mesh'){
                scene.remove(child);
            }
        });

        scene.add(threeMesh);
    });
}

function onSliderChange(){

    // show spinner
    document.getElementById('loader').style.display = 'block';

    // get slider values
    count = document.getElementById('count').value;
    radius = document.getElementById('radius').value;
    length = document.getElementById('length').value;

    param1 = new RhinoCompute.Grasshopper.DataTree('RH_IN:201:Length');
    param1.append([0], [length]);

    param2 = new RhinoCompute.Grasshopper.DataTree('RH_IN:201:Radius');
    param2.append([0], [radius]);

    param3 = new RhinoCompute.Grasshopper.DataTree('RH_IN:201:Count');
    param3.append([0], [count]);

    compute();
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
    var canvas = document.getElementById('canvas');
    canvas.appendChild( renderer.domElement );

    controls = new THREE.OrbitControls( camera, renderer.domElement  );

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

function meshToThreejs(mesh, material) {
    let loader = new THREE.BufferGeometryLoader();
    var geometry = loader.parse(mesh.toThreejsJSON());
    return new THREE.Mesh(geometry, material);
}