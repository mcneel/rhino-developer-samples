#include "stdafx.h"
#include <Psapi.h>

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
/// Create a blend curve with G0 continuity between two existing curves.
/// </summary>
/// <param name="crvA">Curve to blend from (blending will occur at curve end point).</param>
/// <param name="crvB">Curve to blend to (blending will occur at curve start point).</param>
/// <returns>An ON_Curve representing the blend between A and B.</returns>
/// <remarks>
/// CRITICAL: Memory for the resulting curve is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Curve* ON_BlendG0Curve(
  const ON_Curve* crvA,
  const ON_Curve* crvB
  )
{
  ON_Curve* rc = nullptr;
  if (crvA && !crvA->IsClosed() && crvB && !crvB->IsClosed())
  {
    double ta = crvA->Domain().Max();
    double tb = crvB->Domain().Min();

    ON_3dPoint A = crvA->PointAt(ta);
    ON_3dPoint B = crvB->PointAt(tb);

    rc = new ON_LineCurve(A, B);
  }
  return rc;
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
  if (crvA && !crvA->IsClosed() && crvB && !crvB->IsClosed())
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
  if (crvA && !crvA->IsClosed() && crvB && !crvB->IsClosed())
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


/// <summary>
/// Returns true if Rhino was started as a standalone executable.
/// Returns false if Rhino was started by some other application or process.
/// </summary>
bool IsRhinoRunningAsExe()
{
  bool rc = false;
  DWORD dwProcessId = ::GetCurrentProcessId();
  HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
  if (hProcess)
  {
    wchar_t szPath[MAX_PATH];
    DWORD dwLength = ::GetModuleFileNameEx(hProcess, 0, szPath, MAX_PATH);
    ::CloseHandle(hProcess);
    if (dwLength > 0)
    {
      ON_wString name;
      ON_FileSystemPath::SplitPath(szPath, nullptr, nullptr, &name, nullptr);
      rc = name.EqualOrdinal(L"Rhino", true);
    }
  }
  return rc;
}

/// <summary>
/// Returns true if Rhino has input focus.
/// </summary>
bool RhinoHasFocus()
{
  // Retrieves a handle to the foreground window
  HWND hWnd = ::GetForegroundWindow();
  if (hWnd != NULL)
  {
    // Retrieves the identifier of the thread that created the specified window
    DWORD dwProcessId = 0;
    ::GetWindowThreadProcessId(hWnd, &dwProcessId);
    // Retrieves the process identifier of the calling process (a.k.a. Rhino)
    DWORD dwRhinoProcessId = ::GetCurrentProcessId();
    // Compare and return
    return (dwRhinoProcessId == dwProcessId);
  }
  return false;
}
