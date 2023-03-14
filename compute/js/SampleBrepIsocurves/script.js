import * as THREE from 'three'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import rhino3dm from 'rhino3dm'
import { RhinoCompute } from 'rhinocompute'

// holds top level variables
var RHINOLOGO = {};

const btnLoad = document.getElementById('btn-load')
btnLoad.addEventListener('click', (event) => {

    getRhinoLogoMeshes()

})

const inputWires = document.getElementById('input-wires')
inputWires.addEventListener('click', (event) => {

    RHINOLOGO.wireMaterial.visible = inputWires.checked

})

const inputMeshes = document.getElementById('input-meshes')
inputMeshes.addEventListener('click', (event) => {

    RHINOLOGO.meshMaterial.visible = inputMeshes.checked

})



// load rhino3dm
var Module;
rhino3dm().then((m) => {
  Module = m;
  console.log("rhino3dm loaded!");
  init();
  animate();
});



/**
 * Set up empty GL canvas with a gradient background. The geomtry will
 * be loaded into the scene through a separate routine.
 */
function init() {

  RhinoCompute.url = getAuth('RHINO_COMPUTE_URL') // RhinoCompute server url. Use http://localhost:8081/ if debugging locally.
  RhinoCompute.apiKey = getAuth('RHINO_COMPUTE_KEY')  // RhinoCompute server api key. Leave blank if debugging locally.


  RHINOLOGO.scene = new THREE.Scene();

  RHINOLOGO.camera = new THREE.PerspectiveCamera( 45, window.innerWidth / window.innerHeight, 1, 1000 );
  RHINOLOGO.camera.position.z = 40;
  RHINOLOGO.wireMaterial = new THREE.LineBasicMaterial({color: 0x000000});
  RHINOLOGO.meshMaterial = new THREE.MeshPhongMaterial({color: 0xffffff});


  // create a gradient background
  var canvas = document.createElement( 'canvas' );
  canvas.width = 128;
  canvas.height = 128;
  var context = canvas.getContext( '2d' );
  var gradient = context.createLinearGradient( 0, 0, 0, canvas.height);
  gradient.addColorStop( 0.1, 'rgba(0,210,210,1)' );
  gradient.addColorStop( 1, 'rgba(255,255,255,1)' );
  context.fillStyle = gradient;
  context.fillRect( 0, 0, canvas.width, canvas.height );
  RHINOLOGO.scene.background = new THREE.CanvasTexture( canvas );

  //  add a couple lights
  var light = new THREE.DirectionalLight( 0xffffff );
  light.position.set( 0, 0, 1 );
  RHINOLOGO.scene.add( light );
  var light2 = new THREE.DirectionalLight( 0x666666 );
  light2.position.set( 0.2, 0.2, -1 );
  RHINOLOGO.scene.add( light2 );

  RHINOLOGO.renderer = new THREE.WebGLRenderer( { antialias: true } );

  // orbit controls help with mouse/trackpad interaction
  controls = new OrbitControls( RHINOLOGO.camera, RHINOLOGO.renderer.domElement );
  controls.enableDamping = true; // an animation loop is required when either damping or auto-rotation are enabled
  controls.dampingFactor = 0.25;
  controls.screenSpacePanning = false;
  controls.minDistance = 10;
  controls.maxDistance = 500;

  RHINOLOGO.renderer.setPixelRatio( window.devicePixelRatio );
  RHINOLOGO.renderer.setSize( window.innerWidth, window.innerHeight );
  var glcanvas = document.getElementById( 'glcanvas' );
  glcanvas.appendChild( RHINOLOGO.renderer.domElement );

  window.addEventListener( 'resize', onWindowResize, false );
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

function onWindowResize() {
  RHINOLOGO.camera.aspect = window.innerWidth / window.innerHeight;
  RHINOLOGO.camera.updateProjectionMatrix();
  RHINOLOGO.renderer.setSize( window.innerWidth, window.innerHeight );
}

function animate() {
  requestAnimationFrame( animate );
  controls.update();
  RHINOLOGO.renderer.render( RHINOLOGO.scene, RHINOLOGO.camera );
}

/**
 * Loads the 'Rhino Logo.3dm' model and computes meshes for each of
 * breps in the model.
 */
function getRhinoLogoMeshes() {
  if( RHINOLOGO.doc!=null ) {
    alert("Model already loaded");
    return;
  }
  const req = new XMLHttpRequest();
  req.open("GET", "https://files.mcneel.com/rhino3dm/models/RhinoLogo.3dm");
  req.responseType = "arraybuffer";
  req.addEventListener("loadend", loadEnd);
  req.send(null);

  function loadEnd(e) {
    const longInt8View = new Uint8Array(req.response);
    var model = Module.File3dm.fromByteArray(longInt8View);
    var doc = new RhinoLogoDoc(model);
    onModelLoaded(doc);
    doc.computeBrepWires(onWiresComputed);
    doc.computeBrepMeshes(onMeshesComputed);
  }
}


/**
 * Create a THREE.LineSegments class representing the face edges of a
 * rhino3dm.Mesh class. This is not the best way to get the edges as the
 * topology edges are not exposed in rhino3dm yet.
 *
 * @param {rhino3dm.Mesh} mesh - the mesh geometry to extract wires from
 * @param {THREE.Material} material - material to associate with the wires
 *   for drawing.
 * @return {THREE.LineSegments} line segments geometry for a scene
 */
function meshToLineSegments(mesh, material) {

  function tovec3(pt) {  return new THREE.Vector3(pt[0], pt[1], pt[2]); }

  var verts = mesh.vertices();
  var faces = mesh.faces();

  var geometry = new THREE.Geometry();
  for ( var i = 0; i < faces.count; i++ ) {
    var face = faces.get(i);
    var pts = [verts.get(face[0]), verts.get(face[1]), verts.get(face[2]), verts.get(face[3])];
    geometry.vertices.push(
      tovec3(pts[0]), tovec3(pts[1]),
      tovec3(pts[1]), tovec3(pts[2]),
      tovec3(pts[2]), tovec3(pts[3])
    );
    if( face[2]!=face[3] ){
      geometry.vertices.push( tovec3(pts[3]), tovec3(pts[0]) );
    }
  }
  return new THREE.LineSegments( geometry, material );
}

function curveToLineSegments(curve, material) {

  const points = []
  var domain = curve.domain;
  var start = domain[0];
  var range = domain[1]-domain[0];
  var interval = range / 50.0;
  for ( var i = 0; i < 51; i++ ) {
    let t = start + i*interval;
    var pt = curve.pointAt(t);
    points.push( new THREE.Vector3( pt[0], pt[1], pt[2] ) )
    //geometry.vertices.push(new THREE.Vector3(pt[0], pt[1], pt[2]));
  }

  const geometry = new THREE.BufferGeometry().setFromPoints( points );

  return new THREE.Line( geometry, material );

}

/**
 * Create a THREE.Mesh class representing a rhino3dm.Mesh class.
 *
 * @param {rhino3dm.Mesh} mesh - the mesh geometry to extract triangles from
 * @param {THREE.Material} material - material to associate with the mesh
 *   for drawing.
 * @return {THREE.Mesh} mesh geometry for a scene
 */
function meshToThreejs(mesh, material) {
  var geometry = new THREE.BufferGeometry();
  var vertices = mesh.vertices();
  var vertexbuffer = new Float32Array(3 * vertices.count);
  for( var i=0; i<vertices.count; i++) {
    let pt = vertices.get(i);
    vertexbuffer[i*3] = pt[0];
    vertexbuffer[i*3+1] = pt[1];
    vertexbuffer[i*3+2] = pt[2];
  }
  // itemSize = 3 because there are 3 values (components) per vertex
  geometry.setAttribute( 'position', new THREE.BufferAttribute( vertexbuffer, 3 ) );

  let indices = [];
  var faces = mesh.faces();
  for( var i=0; i<faces.count; i++) {
    let face = faces.get(i);
    indices.push(face[0], face[1], face[2]);
    if( face[2] != face[3] ) {
      indices.push(face[2], face[3], face[0]);
    }
  }
  geometry.setIndex(indices);

  var normals = mesh.normals();
  var normalBuffer = new Float32Array(3*normals.count);
  for( var i=0; i<normals.count; i++) {
    let pt = normals.get(i);
    normalBuffer[i*3] = pt[0];
    normalBuffer[i*3+1] = pt[1];
    normalBuffer[i*3+2] = pt[1];
  }
  geometry.setAttribute( 'normal', new THREE.BufferAttribute( normalBuffer, 3 ) );
  return new THREE.Mesh( geometry, material );
}

/**
 * Called every time compute.rhino3d.com returns a set of meshes for a brep
 * This is a good time to convert the rhino3dm.Mesh geometry into geometry
 * used in three.js
 *
 * @param(SampleDoc) model - the model being computed
 */
function onMeshesComputed(model) {
  for( let brepindex=0; brepindex<model.breps.length; brepindex++) {
    var brep = model.breps[brepindex];
    if( brep["meshes"].length > 0 && brep["threejs"]==null){
      var meshes = brep["meshes"];
      brep["threejs"] = [];
      var lineMaterial = new THREE.LineBasicMaterial({color: 0x003333});

      for( let meshindex=0; meshindex<meshes.length; meshindex++) {
        let mesh = meshes[meshindex]
        var lines = null;// meshToLineSegments(mesh, lineMaterial);
        //RHINOLOGO.scene.add( lines );
        var threemesh = meshToThreejs(mesh, RHINOLOGO.meshMaterial);
        RHINOLOGO.scene.add( threemesh );
        brep["threejs"].push([lines, threemesh]);
      }
    }
  }
}

function onWiresComputed(model) {
  for( let brepindex=0; brepindex<model.breps.length; brepindex++) {
    var brep = model.breps[brepindex];
    if( brep["wires"].length > 0 && brep["threejswires"]==null){
      var curves = brep["wires"];
      brep["threejswires"] = [];

      for( let curveindex=0; curveindex<curves.length; curveindex++) {
        let curve = curves[curveindex]
        var lines = curveToLineSegments(curve, RHINOLOGO.wireMaterial);
        RHINOLOGO.scene.add( lines );
        brep["threejswires"].push(lines);
      }
    }
  }
}

/**
 * called once the "Rhino Logo.3dm" file has been downloaded and converted
 * into an instance of a rhino3dm.File3dm class.
 *
 * @param(SampleDoc) model - the model just loaded
 */
function onModelLoaded(doc) {
  RHINOLOGO.doc = doc;
  // the Rhino Logo works better when rotated on to the XY plane
  var objects = doc.model.objects();
  for( var i=0; i<objects.count; i++ ) {
    var geometry = objects.get(i).geometry();
    geometry.rotate(-1.571, [1,0,0], [0,0,0]);
  }
}



class RhinoLogoDoc {
  constructor(model) {
    this.model = model;
    this.breps = [];
    var objecttable = model.objects();
    for(var i=0; i<objecttable.count; i++) {
      var modelobject = objecttable.get(i);
      this.breps.push({"geometry":modelobject.geometry(), "meshes":[], "wires":[], "threejs":null, "threejswires":null});
    }
  }

  computeBrepMeshes(callback) {
    for(var i=0; i<this.breps.length; i++) {
      var brep = this.breps[i]["geometry"];

      const fetchFunc = (m, index) => {
        RhinoCompute.Mesh.createFromBrep(brep)
        .then(result=>{
          var meshes = result.map(r=>Module.CommonObject.decode(r));
          m.breps[index]["meshes"] = meshes;
          callback(this);
        });
      };
      fetchFunc(this, i);
    }
  }

  computeBrepWires(callback) {
    for(var i=0; i<this.breps.length; i++) {
      var brep = this.breps[i]["geometry"];

      const fetchFunc = (m, index, brep) => {
        RhinoCompute.Brep.getWireframe(brep, 1)
        .then(result=>{
          var curves = result.map(r=>Module.CommonObject.decode(r));
          m.breps[index]["wires"] = curves;
          callback(this);
        });
      };
      fetchFunc(this, i, brep);
    }
  }
};