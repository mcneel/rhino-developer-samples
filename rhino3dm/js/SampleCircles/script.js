var rhino = null;

// Wait for the rhino3dm web assembly to load asynchronously
rhino3dm().then(function(m) {
  rhino = m; // global
  run();
});

function pointToString(pt) {
  var str = pt[0] + ',';
  str = str + pt[1] + ',';
  str = str + pt[1] + ',';
  str = str + pt[1];
  return str;
}

function dumpNurbsCurve(nc) {
  console.log('dimension: ' + nc.dimension);
  console.log('order: ' + nc.order);
  console.log('rational: ' + nc.isRational);
  var cv_count = nc.points().count;
  console.log('cv_count: ' + cv_count);
  for (let i = 0; i < cv_count; i++) {
    var pt = nc.points().get(i);
    var str = pointToString(pt)
    console.log('cv[' + i + '] = ' + str);
  }
  var knot_count = nc.knots().count;
  console.log('knot_count: ' + knot_count);
  for (let i = 0; i < knot_count; i++) {
    var k = nc.knots().get(i);
    console.log('k[' + i + '] = ' + k);
  }
  console.log('\n');
}

function run() {

  const zero_tolerance = 2.3283064365387E-10;

  // Define a circle
  const circle = new rhino.Circle(1.0);

  // Get it's NURB form
  const curve0 = circle.toNurbsCurve();

  // Dump
  console.log('CURVE0:');
  console.log('isCircle: ' + curve0.isCircle(zero_tolerance));
  dumpNurbsCurve(curve0);

  // Create new nurb2S curve and copy values from `curve0`
  const dimension = curve0.dimension; // 3
  const order = curve0.order; // order = degree + 1
  const rational = curve0.isRational;
  const cv_count = curve0.points().count
  const knot_count = curve0.knots().count;

  const curve1 = new rhino.NurbsCurve(dimension, rational, order, cv_count)
  for (let i = 0; i < cv_count; i++) {
    curve1.points().set(i, curve0.points().get(i));
  }
  for (let i = 0; i < knot_count; i++) {
    curve1.knots().set(i, curve0.knots().get(i));
  }

  // Dump
  console.log('CURVE1:');
  console.log('isCircle: ' + curve1.isCircle(zero_tolerance));
  dumpNurbsCurve(curve1);

  // Create a new NURBS curve, that evaluates to a circle, from scratch
  const curve2 = new rhino.NurbsCurve(3, true, 3, 9)
  curve2.points().set(0, [1.0, 0.0, 0.0, 1.0]);
  curve2.points().set(1, [0.7071067811865475, 0.7071067811865475, 0.0, 0.7071067811865475]);
  curve2.points().set(2, [0.0, 1.0, 0.0, 1.0]);
  curve2.points().set(3, [-0.7071067811865475, 0.7071067811865475, 0.0, 0.7071067811865475]);
  curve2.points().set(4, [-1.0, 0.0, 0.0, 1.0]);
  curve2.points().set(5, [-0.7071067811865475, -0.7071067811865475, 0.0, 0.7071067811865475]);
  curve2.points().set(6, [0.0, -1.0, 0.0, 1.0]);
  curve2.points().set(7, [0.7071067811865475, -0.7071067811865475, 0.0, 0.7071067811865475]);
  curve2.points().set(8, [1.0, 0.0, 0.0, 1.0]);
  curve2.knots().set(0, 0.0);
  curve2.knots().set(1, 0.0);
  curve2.knots().set(2, 0.5 * Math.PI);
  curve2.knots().set(3, 0.5 * Math.PI);
  curve2.knots().set(4, Math.PI);
  curve2.knots().set(5, Math.PI);
  curve2.knots().set(6, 1.5 * Math.PI);
  curve2.knots().set(7, 1.5 * Math.PI);
  curve2.knots().set(8, 2.0 * Math.PI);
  curve2.knots().set(9, 2.0 * Math.PI);
   
  // Dump
   console.log('CURVE2:');
   console.log('isCircle: ' + curve2.isCircle(zero_tolerance));
   dumpNurbsCurve(curve2);
}