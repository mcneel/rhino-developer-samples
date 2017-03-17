#include "stdafx.h"

/*
Description:
  Create a Blend curve with G1 continuity between two existing curves.
Parameters:
  curveA [in] - Curve to blend from (blending will occur at curve end point).
  curveB [in] - Curve to blend to (blending will occur at curve start point).
  bulgeA [in] - Bulge factor at curveA end of blend. Values near 1.0 work best.
  bulgeB [in] Bulge factor at curveB end of blend. Values near 1.0 work best.
Returns:
  A curve representing the blend between A and B or NULL on failure.
*/
ON_Curve* RhinoBlendG1Curve(const ON_Curve* crvA, const ON_Curve* crvB, double bulgeA, double bulgeB)
{
  ON_Curve* rc = NULL;
  if (crvA && crvB)
  {
    double ta = crvA->Domain().Max();
    double tb = crvB->Domain().Min();

    ON_3dPoint A = crvA->PointAt(ta);
    ON_3dPoint B = crvB->PointAt(tb);
    ON_3dVector TA = crvA->TangentAt(ta);
    ON_3dVector TB = crvB->TangentAt(tb);

    ON_3dPoint P = A + TA * bulgeA * A.DistanceTo(B);
    ON_3dPoint Q = B - TB * bulgeB * A.DistanceTo(B);

    ON_BezierCurve bezier(3, false, 4);
    bezier.SetCV(0, A);
    bezier.SetCV(1, P);
    bezier.SetCV(2, Q);
    bezier.SetCV(3, B);

    ON_NurbsCurve* nc = new ON_NurbsCurve();
    if (!bezier.GetNurbForm(*nc))
    {
      delete nc;
    }
    else
    {
      double length;
      nc->GetLength(&length);
      nc->SetDomain(0.0, length);
      rc = nc;
    }
  }
  return rc;
}

/*
Description:
  Create a Blend curve with G2 continuity between two existing curves.
Parameters:
  curveA [in] - Curve to blend from (blending will occur at curve end point).
  curveB [in] - Curve to blend to (blending will occur at curve start point).
  bulgeA [in] - Bulge factor at curveA end of blend. Values near 1.0 work best.
  bulgeB [in] Bulge factor at curveB end of blend. Values near 1.0 work best.
Returns:
  A curve representing the blend between A and B or NULL on failure.
*/
ON_Curve* RhinoBlendG2Curve(const ON_Curve* crvA, const ON_Curve* crvB, double bulgeA, double bulgeB)
{
  ON_Curve* rc = NULL;
  if (crvA && crvB)
  {
    double ta = crvA->Domain().Max();
    double tb = crvB->Domain().Min();

    ON_3dPoint A = crvA->PointAt(ta);
    ON_3dPoint B = crvB->PointAt(tb);
    ON_3dVector TA = crvA->TangentAt(ta);
    ON_3dVector TB = crvB->TangentAt(tb);
    ON_3dVector KA = crvA->CurvatureAt(ta);
    ON_3dVector KB = crvB->CurvatureAt(tb);

    ON_3dPoint P = A + TA * A.DistanceTo(B);
    ON_3dPoint Q = B - TB * A.DistanceTo(B);

    ON_3dPoint R = ((3 - 2 * bulgeA) / 3) * A + (2 * bulgeA / 3) * P;
    ON_3dPoint S = ((3 - 2 * bulgeB) / 3) * B + (2 * bulgeB / 3) * Q;

    ON_BezierCurve bezier(3, false, 4);
    bezier.SetCV(0, A);
    bezier.SetCV(1, R);
    bezier.SetCV(2, S);
    bezier.SetCV(3, B);

    ON_3dVector D1A = bezier.DerivativeAt(0.0);
    ON_3dVector D1B = bezier.DerivativeAt(1.0);

    ON_3dVector D2A = ON_2ndDerFromCurvature(D1A, KA);
    ON_3dVector D2B = ON_2ndDerFromCurvature(D1B, KB);

    bezier.IncreaseDegree(5);
    ON_SetCVFromDerivative(0, 2, &D2A.x, bezier);
    ON_SetCVFromDerivative(1, 2, &D2B.x, bezier);

    ON_NurbsCurve* nc = new ON_NurbsCurve();
    if (!bezier.GetNurbForm(*nc))
    {
      delete nc;
    }
    else
    {
      double length;
      nc->GetLength(&length);
      nc->SetDomain(0.0, length);
      rc = nc;
    }
  }
  return rc;
}
