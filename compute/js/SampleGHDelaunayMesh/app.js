let args = {
    algo : null, 
    pointer : null,
    values : []
};

let definition = null;

let param1 = new RhinoCompute.Grasshopper.DataTree('RH_IN:102:Points');

rhino3dm().then( async m => {
    console.log('Loaded rhino3dm.');
    rhino = m; // global

    RhinoCompute.url = "http://localhost:8081/" // Rhino.Compute server url
    RhinoCompute.apiKey = "" // your Rhino.Compute server api key

    // source a .ghx file in the same directory
    let url = 'DMesh.ghx';
    let res = await fetch(url);
    let buffer = await res.arrayBuffer();
    definition = new Uint8Array(buffer);

    init();
    compute();
});

function compute(){
    
    // generate random points

    let points = [];
    const cntPts = 100;
    const bndX = 100;
    const bndY = 100;
    const bndZ = 10;

    for(let i = 0; i < cntPts; i++) {
        let x = Math.random()*(bndX - -bndX) + -bndX;
        let y = Math.random()*(bndY - -bndY) + -bndY;
        let z = Math.random()*(bndZ - -bndZ) + -bndZ;

        let pt = "{\"X\":"+x+",\"Y\":"+y+",\"Z\":"+z+"}";

        points.push(pt);

        //viz in three
        let geo = new THREE.SphereGeometry( 1, 5, 5 );
        geo.translate(x, y, z);
        let mat = new THREE.MeshBasicMaterial( {color: 0xff0000, wireframe: true} );
        let sph = new THREE.Mesh( geo, mat );
        scene.add( sph );
    }

    param1.append([0], points);

    // clear values
    let trees = [];
    trees.push(param1);
    //console.log(param1);

    RhinoCompute.Grasshopper.evaluateDefinition(definition, trees).then(result => {
        console.log(result);

        // remove spinner
        document.getElementById('loader').remove();

        let data = JSON.parse(result.values[0].InnerTree['{ 0; }'][0].data);
        let mesh = rhino.CommonObject.decode(data);

        let material = new THREE.MeshBasicMaterial( {wireframe: true, color: 0x00ff00 } );
        let threeMesh = meshToThreejs(mesh, material);

        scene.add(threeMesh);

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

    camera.position.z = 300;

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
    var geometry = new THREE.BufferGeometry();
    var vertices = mesh.vertices();
    var vertexbuffer = new Float32Array(3 * vertices.count);
    for( var i=0; i<vertices.count; i++) {
      pt = vertices.get(i);
      vertexbuffer[i*3] = pt[0];
      vertexbuffer[i*3+1] = pt[1];
      vertexbuffer[i*3+2] = pt[2];
    }
    // itemSize = 3 because there are 3 values (components) per vertex
    geometry.setAttribute( 'position', new THREE.BufferAttribute( vertexbuffer, 3 ) );
  
    indices = [];
    var faces = mesh.faces();
    for( var i=0; i<faces.count; i++) {
      face = faces.get(i);
      indices.push(face[0], face[1], face[2]);
      if( face[2] != face[3] ) {
        indices.push(face[2], face[3], face[0]);
      }
    }
    geometry.setIndex(indices);
  
    var normals = mesh.normals();
    var normalBuffer = new Float32Array(3*normals.count);
    for( var i=0; i<normals.count; i++) {
      pt = normals.get(i);
      normalBuffer[i*3] = pt[0];
      normalBuffer[i*3+1] = pt[1];
      normalBuffer[i*3+2] = pt[1];
    }
    geometry.setAttribute( 'normal', new THREE.BufferAttribute( normalBuffer, 3 ) );
    return new THREE.Mesh( geometry, material );
}