// global variables
let _rhino3dm = null;
let _model = {
  // saved nurbs curves
  curves: [],
  // new nurbs curve
  points: null,
  // viewport for canvas
  viewport: null,
};

// wait for the rhino3dm web assembly to load asynchronously
rhino3dm().then(function(m) {
  _rhino3dm = m; // global
  run();
});

// initialize canvas and model
function run() {
  let canvas = getCanvas();
  canvas.addEventListener('mousedown', onMouseDown);
  canvas.addEventListener('mousemove', onMouseMove);
  window.addEventListener('keyup', onKeyUp);
  _model.points = new _rhino3dm.Point3dList();
  _model.viewport = new _rhino3dm.ViewportInfo();
  _model.viewport.screenPort = [0, 0, canvas.clientWidth, canvas.clientHeight];
  _model.viewport.setFrustum(-30,30,-30,30,1,1000);
  draw();
}

function download3dm() {
  if(_model.curves.length<1){
    console.log('no curves');
    return;
  }

  let doc = new _rhino3dm.File3dm();
  for(let i=0; i<_model.curves.length;i++) {
    doc.objects().add(_model.curves[i], null);
  }

  let options = new _rhino3dm.File3dmWriteOptions();
  options.version = 6;
  let buffer = doc.toByteArray(options);
  saveByteArray("sketch2d.3dm", buffer);
  doc.delete();
}

function saveByteArray(fileName, byte) {
  let blob = new Blob([byte], {type: "application/octect-stream"});
  let link = document.createElement('a');
  link.href = window.URL.createObjectURL(blob);
  link.download = fileName;
  link.click();
};

/* * * * * * * * * * * * * * * *  interaction   * * * * * * * * * * * * * * * */

// handles mouse down events
// adds a new control point at the location of the mouse
function onMouseDown(event) {
  // get the location of the mouse on the canvas
  let [x,y] = getXY(event);

  // if this is a brand new curve, add the first control point
  if (_model.points.count === 0) {
    _model.points.add(x, y, 0);
  }

  // add a new control point that will be saved on the next mouse click
  // (the location of the previous control point is now frozen)
  _model.points.add(x, y, 0);
  draw();
}

// handles mouse move events
// the last control point in the list follows the mouse
function onMouseMove(event) {
  let index = _model.points.count - 1;
  if (index >= 0) {
    let [x,y] = getXY(event);
    _model.points.set(index, [x, y, 0]);
    draw();
  }
}

// handles key up events
function onKeyUp(event) {
  switch (event.key) {
    // when the enter key is pressed, save the new nurbs curve
    case "Enter":
      if (_model.points.count < 4) { // 3 pts (min.) + next pt
        console.error('Not enough points!');
      } else {
        // remove the last point in the list (a.k.a. next)
        let index = _model.points.count - 1;
        _model.points.removeAt(index);

        // construct a curve from the points list
        let degree = _model.points.count - 1;
        if (degree > 3)
          degree = 3;

        // construct a nurbs curve
        // (first arg == true to create a closed periodic uniform curve)
        _model.curves.push(_rhino3dm.NurbsCurve.create(true, degree, _model.points));
      }

      // clear points list
      _model.points.clear();
      break;
  }
  draw();
}

/* * * * * * * * * * * * * * * * *  helpers   * * * * * * * * * * * * * * * * */

// gets the canvas
function getCanvas() {
  return document.getElementById('canvas');
}

// gets the [x, y] location of the mouse in world coordinates
function getXY(evt) {
  let canvas = getCanvas();
  let rect = canvas.getBoundingClientRect();
  let x = evt.clientX - rect.left;
  let y = evt.clientY - rect.top;
  let s2w = _model.viewport.getXform(_rhino3dm.CoordinateSystem.Screen, _rhino3dm.CoordinateSystem.World)
  let world_point = _rhino3dm.Point3d.transform([x,y,0], s2w);
  s2w.delete();
  return [world_point[0],world_point[1]];
}

/* * * * * * * * * * * * * * * * *  drawing   * * * * * * * * * * * * * * * * */

// clears the canvas and draws the model
function draw() {
  // get canvas' 2d context
  let canvas = getCanvas();
  let ctx = canvas.getContext('2d');
  let w2s = _model.viewport.getXform(_rhino3dm.CoordinateSystem.World, _rhino3dm.CoordinateSystem.Screen);

  // clear and draw a grid
  ctx.beginPath();
  ctx.lineWidth = 0.5;
  ctx.strokeStyle = 'rgb(130,130,130)';
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  for(let i=0; i<50; i+=1){
    [x,y,_] = _rhino3dm.Point3d.transform([i,-50,0], w2s);
    [x1,y1,_] = _rhino3dm.Point3d.transform([i,50,0], w2s);
    ctx.moveTo(x,y);
    ctx.lineTo(x1,y1);
    [x,y,_] = _rhino3dm.Point3d.transform([-i,-50,0], w2s);
    [x1,y1,_] = _rhino3dm.Point3d.transform([-i,50,0], w2s);
    ctx.moveTo(x,y);
    ctx.lineTo(x1,y1);

    [x,y,_] = _rhino3dm.Point3d.transform([-50, i, 0], w2s);
    [x1,y1,_] = _rhino3dm.Point3d.transform([50, i, 0], w2s);
    ctx.moveTo(x,y);
    ctx.lineTo(x1,y1);
    [x,y,_] = _rhino3dm.Point3d.transform([-50, -i, 0], w2s);
    [x1,y1,_] = _rhino3dm.Point3d.transform([50, -i, 0], w2s);
    ctx.moveTo(x,y);
    ctx.lineTo(x1,y1);
  }
  ctx.stroke();

  ctx.lineWidth = 2;
  ctx.strokeStyle = 'rgb(150,75,75)';
  [x,y,_] = _rhino3dm.Point3d.transform([0,0,0], w2s);
  [x1,y1,_] = _rhino3dm.Point3d.transform([50,0,0], w2s);
  ctx.beginPath();
  ctx.moveTo(x,y);
  ctx.lineTo(x1,y1);
  ctx.stroke();
  ctx.beginPath();
  ctx.strokeStyle = 'rgb(75,150,75)';
  [x1,y1,_] = _rhino3dm.Point3d.transform([0,50,0], w2s);
  ctx.moveTo(x,y);
  ctx.lineTo(x1,y1);
  ctx.stroke();

  // draw saved nurbs curves
  for (let i=0; i<_model.curves.length; i++)
    drawNurbsCurve(ctx, _model.curves[i], w2s);

  // create a temporary curve from the points and draw it
  if (_model.points !== null && _model.points.count > 0) {
    let degree = _model.points.count - 1;
    if (degree > 3)
      degree = 3;
    let curve = _rhino3dm.NurbsCurve.create(true, degree, _model.points);
    drawNurbsCurve(ctx, curve, w2s);

    // draw control polygon from the temp curve's control points
    //drawControlPolygon(ctx, curve.points());
    drawControlPolygon(ctx, _model.points);

    // delete the temp curve when we're done using it
    // (webassembly memory management isn't great)
    curve.delete();
  }

  w2s.delete();
}

// draws a nurbs curve
function drawNurbsCurve(ctx, curve, w2s) {
  ctx.lineWidth = 1;
  ctx.strokeStyle = 'black';

  const divisions = 200; // TODO: dynamic
  ctx.beginPath();

  let [t0,t1] = curve.domain;
  let world_point = curve.pointAt(t0);
  let screen_point = _rhino3dm.Point3d.transform(world_point, w2s);
  ctx.moveTo(screen_point[0],screen_point[1]);
  for (let j=1; j<=divisions; j++) {
    let t = t0 + j / divisions * (t1-t0);
    world_point = curve.pointAt(t);
    let screen_point = _rhino3dm.Point3d.transform(world_point, w2s);
    ctx.lineTo(screen_point[0],screen_point[1]);
  }
  ctx.stroke();
}

// draws a control polygon
function drawControlPolygon(ctx, points) {
  // draw dashed lines between control points
  ctx.strokestyle = 'darkgray';
  ctx.setLineDash([4,4]);
  ctx.beginPath();

  let w2s = _model.viewport.getXform(_rhino3dm.CoordinateSystem.World, _rhino3dm.CoordinateSystem.Screen)
  for (let i=0; i<points.count; i++) {
    let world_point = points.get(i);
    let screen_point = _rhino3dm.Point3d.transform(world_point, w2s);
    if (0 === i)
      ctx.moveTo(screen_point[0], screen_point[1]);
    else
      ctx.lineTo(screen_point[0], screen_point[1]);
  }
  if( points.count > 2 ){
    let world_point = points.get(0);
    let screen_point = _rhino3dm.Point3d.transform(world_point, w2s);
    ctx.lineTo(screen_point[0], screen_point[1]);
  }

  ctx.stroke();

  // draw control points
  ctx.setLineDash([]);
  ctx.fillStyle = 'white';
  ctx.strokeStyle = 'black';
  for (let i=0; i<points.count; i++) {
    let world_point = points.get(i);
    let screen_point = _rhino3dm.Point3d.transform(world_point, w2s);
    let [x,y,z] = screen_point;
    ctx.fillRect(x-1,y-1, 3, 3);
    ctx.strokeRect(x-2, y-2, 5, 5);
  }
  w2s.delete();
}
