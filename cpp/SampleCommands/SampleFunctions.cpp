#include "stdafx.h"

/// <summary>
/// Gets the sub-curves of a Rhino curve object.
/// </summary>
/// <param name="curve_obj">The curve object.</param>
/// <param name="bIncludeSingle">
/// If true and the curve does not have sub-curves, then
/// add a copy of the input curve to the output array.
/// If false and the curve does not have sub-curves, the
/// do nothing.
/// </param>
/// <param name="sub_curves">The output curve array.</param>
/// <returns>The number of curves added to the output curve array.</returns>
/// <remarks>
/// CRITICAL: Memory for the curves in the output array is allocated and
/// becomes the responsibility of the caller.
/// </remarks>
int RhinoGetSubCurves(
  const CRhinoCurveObject* curve_obj,
  bool bIncludeSingle,
  ON_SimpleArray<ON_Curve*>& sub_curves
)
{
  const int sub_curve_count = sub_curves.Count();
  if (nullptr != curve_obj)
  {
    ON_SimpleArray<CRhinoObject*> sub_objects;
    const int sub_object_count = curve_obj->GetSubObjects(sub_objects);
    if (sub_object_count > 0)
    {
      for (int i = 0; i < sub_object_count; i++)
      {
        CRhinoCurveObject* sub_curve_obj = CRhinoCurveObject::Cast(sub_objects[i]);
        if (nullptr == sub_curve_obj)
          continue;

        const ON_Curve* sub_curve = sub_curve_obj->Curve();
        if (nullptr != sub_curve)
        {
          ON_Curve* duplicate = sub_curve->DuplicateCurve();
          if (nullptr != duplicate)
            sub_curves.Append(duplicate);
        }

        delete sub_objects[i]; // Don't leak...
        sub_objects[i] = nullptr;
      }
    }
    else if (bIncludeSingle)
    {
      const ON_Curve* curve = curve_obj->Curve();
      if (nullptr != curve)
      {
        ON_Curve* duplicate = curve->DuplicateCurve();
        if (nullptr != duplicate)
          sub_curves.Append(duplicate);
      }
    }
  }

  // Return number added
  return sub_curves.Count() - sub_curve_count;
}

/// <summary>
/// Evaluate torsion of a curve at a parmeter.
/// </summary>
/// <param name="curve">Curve to evaluate.</param>
/// <param name="t">Evaluation parameter.</param>
/// <returns>The torsion if successful.</returns>
/// <remarks>See Barrett O'Neill, Elementary Differential Geometry, page 69.</remarks>
double ON_CurveTorsion(const ON_Curve& curve, double t)
{
  double tau = ON_UNSET_VALUE;
  double v[12] = {};
  if (curve.Evaluate(t, 3, 3, v))
  {
    tau = 0.0;
    ON_3dVector d1(&v[3]);
    ON_3dVector d2(&v[6]);
    ON_3dVector d3(&v[9]);
    ON_3dVector b = ON_CrossProduct(d1, d2);
    double len2 = b * b;
    if (len2 > 0.0)
      tau = b * d3 / len2;
  }
  return tau;
}

/// <summary>
/// Sets a polyline curve to use arc length parameterization
/// for higher quality geometry.
/// </summary>
/// <param name="curve">The polyline curve to modify.</param>
/// <param name="tolerance">Minimum distance tolerance.</param>
void ON_PolylineCurve_SetArcLengthParameterization(
  ON_PolylineCurve& curve,
  double tolerance
)
{
  ON_Polyline& polyline = curve.m_pline;
  double d, mind = tolerance;
  curve.m_t[0] = 0;
  const int count = polyline.Count();
  for (int i = 1; i < count; i++)
  {
    d = (polyline[i] - polyline[i - 1]).Length();
    if (d < mind)
      d = mind;
    if (d < fabs(curve.m_t[i - 1]) * 1e-5)
      d = fabs(curve.m_t[i - 1]) * 1e-5;
    curve.m_t[i] = curve.m_t[i - 1] + d;
  }
}

/// <summary>
/// Create a blend curve with G1 continuity between two existing curves.
/// </summary>
/// <param name="crvA">Curve to blend from (blending will occur at curve end point).</param>
/// <param name="crvB">Curve to blend to (blending will occur at curve start point).</param>
/// <param name="bulgeA">Bulge factor at curveA end of blend. Values near 1.0 work best.</param>
/// <param name="bulgeB">Bulge factor at curveB end of blend. Values near 1.0 work best.</param>
/// <returns>An ON_Curve representing the blend between A and B.</returns>
/// <remarks>
/// CRITICAL: Memory for the resulting curve is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Curve* ON_BlendG1Curve(
  const ON_Curve* crvA, 
  const ON_Curve* crvB, 
  double bulgeA, 
  double bulgeB
)
{
  ON_Curve* rc = nullptr;
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

    ON_NurbsCurve* nc = ON_NurbsCurve::New();
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

/// <summary>
/// Create a blend curve with G2 continuity between two existing curves.
/// </summary>
/// <param name="crvA">Curve to blend from (blending will occur at curve end point).</param>
/// <param name="crvB">Curve to blend to (blending will occur at curve start point).</param>
/// <param name="bulgeA">Bulge factor at curveA end of blend. Values near 1.0 work best.</param>
/// <param name="bulgeB">Bulge factor at curveB end of blend. Values near 1.0 work best.</param>
/// <returns>An ON_Curve representing the blend between A and B.</returns>
/// <remarks>
/// CRITICAL: Memory for the resulting curve is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Curve* ON_BlendG2Curve(
  const ON_Curve* crvA, 
  const ON_Curve* crvB, 
  double bulgeA, 
  double bulgeB
)
{
  ON_Curve* rc = nullptr;
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

    ON_NurbsCurve* nc = ON_NurbsCurve::New();
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

/// <summary>
/// Dolly the camera location and so that the view frustum contains
/// camcoord_bbox and the volume of camcoord_bbox fills the frustum.
/// If the projection is perspective, the camera angle is not changed.
/// </summary>
/// <param name="current_vp">Current projection, must be valid.</param>
/// <param name="camcoord_bbox">Valid bounding box in current_vp camera coordinates.</param>
/// <param name="zoomed_vp">Can be the same as current_vp projection.</param>
/// <returns>True if successful.</returns>
bool ON_DollyExtents(
  const ON_Viewport& current_vp,
  ON_BoundingBox camcoord_bbox,
  ON_Viewport& zoomed_vp
)
{
  if (&zoomed_vp != &current_vp)
    zoomed_vp = current_vp;

  if (!camcoord_bbox.IsValid() || !zoomed_vp.IsValid())
    return false;

  double aspect = 0.0;
  if (!current_vp.GetFrustumAspect(aspect))
    return false;

  if (!ON_IsValid(aspect) || 0.0 == aspect)
    return false;

  ON_3dVector scale(1.0, 1.0, 0.0);
  current_vp.GetViewScale(&scale.x, &scale.y);

  const double xmin = camcoord_bbox.m_min.x;
  const double xmax = camcoord_bbox.m_max.x;
  const double ymin = camcoord_bbox.m_min.y;
  const double ymax = camcoord_bbox.m_max.y;
  double dx = 0.5 * (xmax - xmin) * scale.x;
  double dy = 0.5 * (ymax - ymin) * scale.y;
  if (dx <= ON_SQRT_EPSILON && dy <= ON_SQRT_EPSILON)
    dx = dy = 0.5;

  if (dx < dy * aspect)
    dx = dy * aspect;
  else
    dy = dx / aspect;

  // Pad depths a bit so clippling plane are not coplanar with displayed geometry
  // zmax is on frustum near and zmin is on frustum far
  double zmin = camcoord_bbox.m_min.z;
  double zmax = camcoord_bbox.m_max.z;

  double dz = (zmax - zmin) * 0.00390625; // 0.00390625 = 1/256
  if (ON::perspective_view == current_vp.Projection())
  {
    if (dz <= 1.0e-6)
      dz = 1.0e-6;
  }
  else if (dz <= 0.125)
    dz = 0.125;
  zmax += dz;

  // It is ok to adjust zmin by more generous amount because it
  // does not effect the ability to zoom in on small objects a 
  // perspective view.
  if (dz <= 0.125)
    dz = 0.125;
  zmin -= dz;
  dz = zmax - zmin;

  double frus_near = 0.0;
  if (ON::parallel_view == current_vp.Projection())
  {
    frus_near = 0.125 * dz;
  }
  else if (ON::perspective_view == current_vp.Projection())
  {
    double ax, ay;
    if (current_vp.GetCameraAngle(NULL, &ay, &ax))
    {
      double zx = (ON_IsValid(ax) && ax > 0.0) ? dx / tan(ax) : 0.0;
      double zy = (ON_IsValid(ay) && ay > 0.0) ? dy / tan(ay) : 0.0;
      frus_near = (zx > zy) ? zx : zy;
    }
  }

  bool rc = false;
  if (!ON_IsValid(frus_near) || frus_near <= ON_SQRT_EPSILON)
  {
    frus_near = 1.0;
  }

  ON_3dPoint camloc = current_vp.CameraLocation();
  if (camloc.IsValid())
  {
    ON_3dVector dolly = 0.5*(xmax + xmin)*zoomed_vp.CameraX()
      + 0.5*(ymax + ymin)*zoomed_vp.CameraY()
      + (frus_near + zmax)*zoomed_vp.CameraZ();
    camloc += dolly;
    if (zoomed_vp.SetCameraLocation(camloc))
    {
      double frus_far = frus_near + dz;
      rc = zoomed_vp.SetFrustum(-dx, dx, -dy, dy, frus_near, frus_far);
    }
  }
  return rc;
}

/// <summary>
/// Gets the indices of all the Brep faces that use a specified Brep edge.
/// </summary>
/// <param name="pBrep">The Brep.</param>
/// <param name="edge_index">The edge index to query.</param>
/// <param name="face_indices">The indices of the faces that use the edge.</param>
/// <returns>The number indices in face_indices.</returns>
int ON_Brep_EdgeFaceIndices(
  const ON_Brep* pBrep,
  int edge_index,
  ON_SimpleArray<int>& face_indices
)
{
  int rc = 0;
  if (pBrep)
  {
    const ON_BrepEdge* pEdge = pBrep->Edge(edge_index);
    if (pEdge)
    {
      const int trim_count = pEdge->TrimCount();
      for (int i = 0; i < trim_count; i++)
      {
        const ON_BrepTrim* pTrim = pEdge->Trim(i);
        if (pTrim)
        {
          const ON_BrepFace* pFace = pTrim->Face();
          if (pFace)
            face_indices.Append(pFace->m_face_index);
        }
      }
    }
    rc = face_indices.Count();
  }
  return rc;
}

/// <summary>
/// Gets the indices of all the Brep edges that delineate a specified Brep face.
/// </summary>
/// <param name="pBrep">The Brep.</param>
/// <param name="face_index">The index of the Brep face to query.</param>
/// <param name="edge_indices">The indices of the edges that use the face.</param>
/// <returns>The number indices in edge_indices.</returns>
int ON_Brep_FaceEdgeIndices(
  const ON_Brep* pBrep,
  int face_index,
  ON_SimpleArray<int>& edge_indices
)
{
  int rc = 0;
  if (pBrep)
  {
    const ON_BrepFace* pFace = pBrep->Face(face_index);
    if (pFace)
    {
      int loop_count = pFace->LoopCount();
      for (int i = 0; i < loop_count; i++)
      {
        const ON_BrepLoop* pLoop = pFace->Loop(i);
        if (pLoop)
        {
          int trim_count = pLoop->TrimCount();
          for (int j = 0; j < trim_count; j++)
          {
            const ON_BrepTrim* pTrim = pLoop->Trim(j);
            if (pTrim)
            {
              const ON_BrepEdge* pEdge = pTrim->Edge();
              if (pEdge)
                edge_indices.Append(pEdge->m_edge_index);
            }
          }
        }
      }
    }
    rc = edge_indices.Count();
  }
  return rc;
}

/// <summary>
/// Get the BRep definition of a trimmed surface.
/// </summary>
/// <param name="srf">Surface that will be trimmed.</param>
/// <param name="crv2d">
/// Closed, 2d parameter space boundary curve that defines the
/// outer boundary of the trimmed surface.
/// </param>
/// <param name="tol">Tolerance for fitting 3d edge curves.</param>
/// <returns>
/// An ON_Brep representation of the trimmed surface with a single face.
/// </returns>
/// <remarks>
/// CRITICAL: Memory for the resulting BRep is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Brep* ON_BrepFromSurfaceAndBoundary(
  const ON_Surface& srf,
  const ON_Curve& crv2d,
  double tol
)
{
  if (!srf.IsValid() || !crv2d.IsValid() || !crv2d.IsClosed() || 2 != crv2d.Dimension())
    return nullptr;

  ON_PlaneSurface* s = new ON_PlaneSurface();
  s->m_plane = ON_Plane::World_xy;
  s->SetExtents(0, srf.Domain(0), true);
  s->SetExtents(1, srf.Domain(1), true);

  ON_Brep* brep = ON_Brep::New();
  const int si = brep->AddSurface(s);
  ON_BrepFace& face = brep->NewFace(si);
  face.DestroyRuntimeCache();

  ON_Curve* crv3d = crv2d.DuplicateCurve();
  crv3d->ChangeDimension(3);

  ON_SimpleArray<ON_Curve*> boundary;
  boundary.Append(crv3d);

  if (brep->NewPlanarFaceLoop(face.m_face_index, ON_BrepLoop::outer, boundary, false))
  {
    int si = brep->AddSurface(srf.DuplicateSurface());
    face.ChangeSurface(si);
    brep->RebuildEdges(face, tol, true, true);
    brep->SetTolerancesBoxesAndFlags();
    brep->Compact();
  }
  else
  {
    delete brep;
    brep = nullptr;
  }

  return brep;
}

/// <summary>
/// Returns the  bitmap preview image from a 3dm file.
/// </summary>
/// <param name="pszFilePath">The full path to the 3dm file.</param>
/// <returns>An HBITMAP if successful, or nullptr if the 3dm file does not 
/// contain a preview image or if there was an error reading the file.
/// </returns>
/// <remarks>
/// CRITICAL: Memory for the bitmap is allocated. It is the calling
/// functions responsibility to clean up the memory by calling the
/// Win32 DeleteObject() function.
/// </remarks>
HBITMAP ON_ReadBitmapPreviewImage(const wchar_t* pszFilePath)
{
  HBITMAP hBitmap = nullptr;

  if (nullptr == pszFilePath || 0 == pszFilePath[0])
    return hBitmap;

  FILE* archive_fp = ON::OpenFile(pszFilePath, L"rb");
  if (archive_fp)
  {
    ON_BinaryFile archive(ON::archive_mode::read3dm, archive_fp);
    int version = 0;
    ON_String comments;
    if (archive.Read3dmStartSection(&version, comments))
    {
      ON_3dmProperties properties;
      if (archive.Read3dmProperties(properties))
      {
        if (properties.m_PreviewImage.IsValid())
        {
          HDC hdc = ::GetDC(nullptr);
          hBitmap = ::CreateDIBitmap(
            hdc,                                           // handle to DC
            &properties.m_PreviewImage.m_bmi->bmiHeader,   // bitmap data
            CBM_INIT,                                      // initialization option
            (const void*)properties.m_PreviewImage.m_bits, // initialization data
            properties.m_PreviewImage.m_bmi,               // color-format data
            DIB_RGB_COLORS                                 // color-data usage
          );
          ::ReleaseDC(nullptr, hdc);
        }
      }
    }
    ON::CloseFile(archive_fp);
  }
  return hBitmap;
}

/// <summary>
/// CRhinoHatchPatternTable helpers for system hatch patterns
/// </summary>
class CRhinoHatchTableHelper
{
public:
  /// <summary>
  /// Returns the Solid hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Solid(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Hatch1 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Hatch1(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Hatch2 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Hatch2(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Hatch3 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Hatch3(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the HatchDash hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* HatchDash(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Grid hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Grid(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Grid60 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Grid60(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Plus hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Plus(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Squares hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Squares(CRhinoHatchPatternTable& table);

private:
  static const CRhinoHatchPattern* FindOrCreateHatchPattern(CRhinoHatchPatternTable& table, const ON_HatchPattern& hatch_pattern);
};

const CRhinoHatchPattern* CRhinoHatchTableHelper::Solid(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Solid);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Hatch1(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Hatch1);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Hatch2(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Hatch2);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Hatch3(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Hatch3);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::HatchDash(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::HatchDash);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Grid(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Grid);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Grid60(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Grid60);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Plus(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Plus);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::Squares(CRhinoHatchPatternTable& table)
{
  return FindOrCreateHatchPattern(table, CRhinoHatchPattern::Squares);
}

const CRhinoHatchPattern* CRhinoHatchTableHelper::FindOrCreateHatchPattern(CRhinoHatchPatternTable& table, const ON_HatchPattern& hatch_pattern)
{
  const CRhinoHatchPattern* rc = table.HatchPatternFromName(hatch_pattern.Name(), false);
  if (nullptr == rc)
  {
    const ON_ComponentManifestItem& manifest_item = table.CreateHatchPattern(hatch_pattern, 0, 0);
    rc = table.HatchPattern(manifest_item);
  }
  return rc;
}
