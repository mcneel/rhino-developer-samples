void TestNurbsCurve(CRhinoDoc& doc)
{
  const int degree = 3;
  const int cv_count = 6;
  const int knot_count = cv_count + degree - 1;
  const int order = degree + 1;

  ON_3dPoint cvs[cv_count];
  cvs[0] = ON_3dPoint(0.0, 0.0, 0.0);
  cvs[1] = ON_3dPoint(5.0, 10.0, 0.0);
  cvs[2] = ON_3dPoint(10.0, 0.0, 0.0);
  cvs[3] = ON_3dPoint(15.0, 10.0, 0.0);
  cvs[4] = ON_3dPoint(20.0, 0.0, 0.0);
  cvs[5] = ON_3dPoint(25.0, 10.0, 0.0);

  double knots[knot_count];
  knots[0] = 0.0;
  knots[1] = 0.0;
  knots[2] = 0.0;
  knots[3] = 1.0;
  knots[4] = 2.0;
  knots[5] = 3.0;
  knots[6] = 3.0;
  knots[7] = 3.0;

  ON_NurbsCurve curve(3, FALSE, order, cv_count);

  for (int i = 0; i < cv_count; i++)
    curve.SetCV(i, cvs[i]);

  for (int i = 0; i < knot_count; i++)
    curve.m_knot[i] = knots[i];

  if( curve.IsValid() )
  {
    doc.AddCurveObject(curve);
    doc.Redraw();
  }
}
