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

static HWND GetRealParent(HWND hWnd)
{
  // To obtain a window's owner window, instead of using GetParent,
  // use GetWindow with the GW_OWNER flag.
  HWND hWndOwner = ::GetWindow(hWnd, GW_OWNER);
  if (NULL != hWndOwner)
    return hWndOwner;

  // Obtain the parent window and not the owner
  return GetAncestor(hWnd, GA_PARENT);
}

/// <summary>
/// Returns true if the Rhino main window has been re-parented to some other application window.
/// Returns true if the Rhino main window parent is the Windows Desktop.
/// </summary>
bool IsRhinoReparented()
{
  HWND hParent = GetRealParent(RhinoApp().MainWnd());
  HWND hDesktop = ::GetDesktopWindow();
  return hParent != hDesktop;
}

/// <summary>
/// Returns module handle where "this" function is running in: EXE or DLL.
/// </summary>
HMODULE FancyGetModuleHandle()
{
  HMODULE hModule = NULL;
  ::GetModuleHandleEx(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    (LPCTSTR)FancyGetModuleHandle,
    &hModule
  );
  return hModule;
}


/// <summary>
/// Begin a CRhinoDoc undo record.
/// </summary>
/// <param name="doc">The active document.</param>
/// <param name="pszDescription">The undo description.</param>
CRhinoDocUndoRecordHelper::CRhinoDocUndoRecordHelper(CRhinoDoc& doc, const wchar_t* pszDescription)
{
  m_docRuntimeSerialNumber = doc.RuntimeSerialNumber();
  m_undoRecordSerialNumber = doc.BeginUndoRecord(pszDescription);
}

/// <summary>
/// Begin a CRhinoDoc undo record.
/// </summary>
/// <param name="docRuntimeSerialNumber">The active document's runtime serial number.</param>
/// <param name="pszDescription">The undo description.</param>
CRhinoDocUndoRecordHelper::CRhinoDocUndoRecordHelper(unsigned int docRuntimeSerialNumber, const wchar_t* pszDescription)
{
  CRhinoDoc* pDoc = CRhinoDoc::FromRuntimeSerialNumber(docRuntimeSerialNumber);
  if (pDoc)
  {
    m_docRuntimeSerialNumber = pDoc->RuntimeSerialNumber();
    m_undoRecordSerialNumber = pDoc->BeginUndoRecord(pszDescription);
  }
}

/// <summary>
/// Class destructor, ends the undo record.
/// </summary>
CRhinoDocUndoRecordHelper::~CRhinoDocUndoRecordHelper()
{
  EndUndoRecord();
}

/// <summary>
/// Ends the undo record.
/// </summary>
/// <returns>true if the undo record was ended.</returns>
bool CRhinoDocUndoRecordHelper::EndUndoRecord()
{
  bool rc = (m_docRuntimeSerialNumber > 0 && m_undoRecordSerialNumber > 0);
  if (rc)
  {
    CRhinoDoc* pDoc = CRhinoDoc::FromRuntimeSerialNumber(m_docRuntimeSerialNumber);
    if (pDoc)
      rc = pDoc->EndUndoRecord(m_undoRecordSerialNumber);
    m_docRuntimeSerialNumber = m_undoRecordSerialNumber = 0;
  }
  return rc;
}

ON_KnownColor ON_KnownColorFromUnsigned(unsigned int known_color_as_unsigned)
{
  switch (known_color_as_unsigned)
  {
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Unset);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::AliceBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::AntiqueWhite);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Aqua);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Aquamarine);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Azure);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Beige);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Bisque);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Black);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::BlanchedAlmond);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Blue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::BlueViolet);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Brown);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::BurlyWood);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::CadetBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Chartreuse);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Chocolate);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Coral);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::CornflowerBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Cornsilk);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Crimson);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Cyan);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkCyan);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkGoldenrod);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkGray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkKhaki);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkMagenta);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkOliveGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkOrange);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkOrchid);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkRed);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkSalmon);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkSeaGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkSlateBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkSlateGray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkTurquoise);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DarkViolet);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DeepPink);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DeepSkyBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DimGray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::DodgerBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Firebrick);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::FloralWhite);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::ForestGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Fuchsia);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Gainsboro);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::GhostWhite);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Gold);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Goldenrod);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Gray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Green);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::GreenYellow);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Honeydew);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::HotPink);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::IndianRed);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Indigo);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Ivory);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Khaki);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Lavender);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LavenderBlush);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LawnGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LemonChiffon);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightCoral);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightCyan);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightGoldenrodYellow);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightGray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightPink);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightSalmon);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightSeaGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightSkyBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightSlateGray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightSteelBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LightYellow);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Lime);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::LimeGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Linen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Magenta);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Maroon);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumAquamarine);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumOrchid);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumPurple);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumSeaGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumSlateBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumSpringGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumTurquoise);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MediumVioletRed);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MidnightBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MintCream);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::MistyRose);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Moccasin);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::NavajoWhite);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Navy);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::OldLace);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Olive);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::OliveDrab);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Orange);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::OrangeRed);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Orchid);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PaleGoldenrod);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PaleGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PaleTurquoise);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PaleVioletRed);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PapayaWhip);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PeachPuff);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Peru);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Pink);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Plum);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::PowderBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Purple);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::RebeccaPurple);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Red);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::RosyBrown);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::RoyalBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SaddleBrown);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Salmon);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SandyBrown);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SeaGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SeaShell);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Sienna);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Silver);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SkyBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SlateBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SlateGray);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Snow);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SpringGreen);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::SteelBlue);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Tan);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Teal);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Thistle);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Tomato);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Turquoise);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Violet);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Wheat);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::White);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::WhiteSmoke);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::Yellow);
    ON_ENUM_FROM_UNSIGNED_CASE(ON_KnownColor::YellowGreen);
  }
  ON_ERROR("Invalid known_color_as_unsigned value");
  return (ON_KnownColor::Unset);
}

ON_Color ON_ColorFromKnownColor(ON_KnownColor knownColor)
{
  unsigned int rgb = ON_UNSET_COLOR;

  switch (knownColor)
  {
  case ON_KnownColor::Unset:
    break;
  case ON_KnownColor::AliceBlue:
    rgb = 15792383;
    break;
  case ON_KnownColor::AntiqueWhite:
    rgb = 16444375;
    break;
  case ON_KnownColor::Aqua:
    rgb = 65535;
    break;
  case ON_KnownColor::Aquamarine:
    rgb = 8388564;
    break;
  case ON_KnownColor::Azure:
    rgb = 15794175;
    break;
  case ON_KnownColor::Beige:
    rgb = 16119260;
    break;
  case ON_KnownColor::Bisque:
    rgb = 16770244;
    break;
  case ON_KnownColor::Black:
    rgb = 0;
    break;
  case ON_KnownColor::BlanchedAlmond:
    rgb = 16772045;
    break;
  case ON_KnownColor::Blue:
    rgb = 255;
    break;
  case ON_KnownColor::BlueViolet:
    rgb = 9055202;
    break;
  case ON_KnownColor::Brown:
    rgb = 10824234;
    break;
  case ON_KnownColor::BurlyWood:
    rgb = 14596231;
    break;
  case ON_KnownColor::CadetBlue:
    rgb = 6266528;
    break;
  case ON_KnownColor::Chartreuse:
    rgb = 8388352;
    break;
  case ON_KnownColor::Chocolate:
    rgb = 13789470;
    break;
  case ON_KnownColor::Coral:
    rgb = 16744272;
    break;
  case ON_KnownColor::CornflowerBlue:
    rgb = 6591981;
    break;
  case ON_KnownColor::Cornsilk:
    rgb = 16775388;
    break;
  case ON_KnownColor::Crimson:
    rgb = 14423100;
    break;
  case ON_KnownColor::Cyan:
    rgb = 65535;
    break;
  case ON_KnownColor::DarkBlue:
    rgb = 139;
    break;
  case ON_KnownColor::DarkCyan:
    rgb = 35723;
    break;
  case ON_KnownColor::DarkGoldenrod:
    rgb = 12092939;
    break;
  case ON_KnownColor::DarkGray:
    rgb = 11119017;
    break;
  case ON_KnownColor::DarkGreen:
    rgb = 25600;
    break;
  case ON_KnownColor::DarkKhaki:
    rgb = 12433259;
    break;
  case ON_KnownColor::DarkMagenta:
    rgb = 9109643;
    break;
  case ON_KnownColor::DarkOliveGreen:
    rgb = 5597999;
    break;
  case ON_KnownColor::DarkOrange:
    rgb = 16747520;
    break;
  case ON_KnownColor::DarkOrchid:
    rgb = 10040012;
    break;
  case ON_KnownColor::DarkRed:
    rgb = 9109504;
    break;
  case ON_KnownColor::DarkSalmon:
    rgb = 15308410;
    break;
  case ON_KnownColor::DarkSeaGreen:
    rgb = 9419919;
    break;
  case ON_KnownColor::DarkSlateBlue:
    rgb = 4734347;
    break;
  case ON_KnownColor::DarkSlateGray:
    rgb = 3100495;
    break;
  case ON_KnownColor::DarkTurquoise:
    rgb = 52945;
    break;
  case ON_KnownColor::DarkViolet:
    rgb = 9699539;
    break;
  case ON_KnownColor::DeepPink:
    rgb = 16716947;
    break;
  case ON_KnownColor::DeepSkyBlue:
    rgb = 49151;
    break;
  case ON_KnownColor::DimGray:
    rgb = 6908265;
    break;
  case ON_KnownColor::DodgerBlue:
    rgb = 2003199;
    break;
  case ON_KnownColor::Firebrick:
    rgb = 11674146;
    break;
  case ON_KnownColor::FloralWhite:
    rgb = 16775920;
    break;
  case ON_KnownColor::ForestGreen:
    rgb = 2263842;
    break;
  case ON_KnownColor::Fuchsia:
    rgb = 16711935;
    break;
  case ON_KnownColor::Gainsboro:
    rgb = 14474460;
    break;
  case ON_KnownColor::GhostWhite:
    rgb = 16316671;
    break;
  case ON_KnownColor::Gold:
    rgb = 16766720;
    break;
  case ON_KnownColor::Goldenrod:
    rgb = 14329120;
    break;
  case ON_KnownColor::Gray:
    rgb = 8421504;
    break;
  case ON_KnownColor::Green:
    rgb = 32768;
    break;
  case ON_KnownColor::GreenYellow:
    rgb = 11403055;
    break;
  case ON_KnownColor::Honeydew:
    rgb = 15794160;
    break;
  case ON_KnownColor::HotPink:
    rgb = 16738740;
    break;
  case ON_KnownColor::IndianRed:
    rgb = 13458524;
    break;
  case ON_KnownColor::Indigo:
    rgb = 4915330;
    break;
  case ON_KnownColor::Ivory:
    rgb = 16777200;
    break;
  case ON_KnownColor::Khaki:
    rgb = 15787660;
    break;
  case ON_KnownColor::Lavender:
    rgb = 15132410;
    break;
  case ON_KnownColor::LavenderBlush:
    rgb = 16773365;
    break;
  case ON_KnownColor::LawnGreen:
    rgb = 8190976;
    break;
  case ON_KnownColor::LemonChiffon:
    rgb = 16775885;
    break;
  case ON_KnownColor::LightBlue:
    rgb = 11393254;
    break;
  case ON_KnownColor::LightCoral:
    rgb = 15761536;
    break;
  case ON_KnownColor::LightCyan:
    rgb = 14745599;
    break;
  case ON_KnownColor::LightGoldenrodYellow:
    rgb = 16448210;
    break;
  case ON_KnownColor::LightGray:
    rgb = 13882323;
    break;
  case ON_KnownColor::LightGreen:
    rgb = 9498256;
    break;
  case ON_KnownColor::LightPink:
    rgb = 16758465;
    break;
  case ON_KnownColor::LightSalmon:
    rgb = 16752762;
    break;
  case ON_KnownColor::LightSeaGreen:
    rgb = 2142890;
    break;
  case ON_KnownColor::LightSkyBlue:
    rgb = 8900346;
    break;
  case ON_KnownColor::LightSlateGray:
    rgb = 7833753;
    break;
  case ON_KnownColor::LightSteelBlue:
    rgb = 11584734;
    break;
  case ON_KnownColor::LightYellow:
    rgb = 16777184;
    break;
  case ON_KnownColor::Lime:
    rgb = 65280;
    break;
  case ON_KnownColor::LimeGreen:
    rgb = 3329330;
    break;
  case ON_KnownColor::Linen:
    rgb = 16445670;
    break;
  case ON_KnownColor::Magenta:
    rgb = 16711935;
    break;
  case ON_KnownColor::Maroon:
    rgb = 8388608;
    break;
  case ON_KnownColor::MediumAquamarine:
    rgb = 6737322;
    break;
  case ON_KnownColor::MediumBlue:
    rgb = 205;
    break;
  case ON_KnownColor::MediumOrchid:
    rgb = 12211667;
    break;
  case ON_KnownColor::MediumPurple:
    rgb = 9662683;
    break;
  case ON_KnownColor::MediumSeaGreen:
    rgb = 3978097;
    break;
  case ON_KnownColor::MediumSlateBlue:
    rgb = 8087790;
    break;
  case ON_KnownColor::MediumSpringGreen:
    rgb = 64154;
    break;
  case ON_KnownColor::MediumTurquoise:
    rgb = 4772300;
    break;
  case ON_KnownColor::MediumVioletRed:
    rgb = 13047173;
    break;
  case ON_KnownColor::MidnightBlue:
    rgb = 1644912;
    break;
  case ON_KnownColor::MintCream:
    rgb = 16121850;
    break;
  case ON_KnownColor::MistyRose:
    rgb = 16770273;
    break;
  case ON_KnownColor::Moccasin:
    rgb = 16770229;
    break;
  case ON_KnownColor::NavajoWhite:
    rgb = 16768685;
    break;
  case ON_KnownColor::Navy:
    rgb = 128;
    break;
  case ON_KnownColor::OldLace:
    rgb = 16643558;
    break;
  case ON_KnownColor::Olive:
    rgb = 8421376;
    break;
  case ON_KnownColor::OliveDrab:
    rgb = 7048739;
    break;
  case ON_KnownColor::Orange:
    rgb = 16753920;
    break;
  case ON_KnownColor::OrangeRed:
    rgb = 16729344;
    break;
  case ON_KnownColor::Orchid:
    rgb = 14315734;
    break;
  case ON_KnownColor::PaleGoldenrod:
    rgb = 15657130;
    break;
  case ON_KnownColor::PaleGreen:
    rgb = 10025880;
    break;
  case ON_KnownColor::PaleTurquoise:
    rgb = 11529966;
    break;
  case ON_KnownColor::PaleVioletRed:
    rgb = 14381203;
    break;
  case ON_KnownColor::PapayaWhip:
    rgb = 16773077;
    break;
  case ON_KnownColor::PeachPuff:
    rgb = 16767673;
    break;
  case ON_KnownColor::Peru:
    rgb = 13468991;
    break;
  case ON_KnownColor::Pink:
    rgb = 16761035;
    break;
  case ON_KnownColor::Plum:
    rgb = 14524637;
    break;
  case ON_KnownColor::PowderBlue:
    rgb = 11591910;
    break;
  case ON_KnownColor::Purple:
    rgb = 8388736;
    break;
  case ON_KnownColor::RebeccaPurple:
    rgb = 6697881;
    break;
  case ON_KnownColor::Red:
    rgb = 16711680;
    break;
  case ON_KnownColor::RosyBrown:
    rgb = 12357519;
    break;
  case ON_KnownColor::RoyalBlue:
    rgb = 4286945;
    break;
  case ON_KnownColor::SaddleBrown:
    rgb = 9127187;
    break;
  case ON_KnownColor::Salmon:
    rgb = 16416882;
    break;
  case ON_KnownColor::SandyBrown:
    rgb = 16032864;
    break;
  case ON_KnownColor::SeaGreen:
    rgb = 3050327;
    break;
  case ON_KnownColor::SeaShell:
    rgb = 16774638;
    break;
  case ON_KnownColor::Sienna:
    rgb = 10506797;
    break;
  case ON_KnownColor::Silver:
    rgb = 12632256;
    break;
  case ON_KnownColor::SkyBlue:
    rgb = 8900331;
    break;
  case ON_KnownColor::SlateBlue:
    rgb = 6970061;
    break;
  case ON_KnownColor::SlateGray:
    rgb = 7372944;
    break;
  case ON_KnownColor::Snow:
    rgb = 16775930;
    break;
  case ON_KnownColor::SpringGreen:
    rgb = 65407;
    break;
  case ON_KnownColor::SteelBlue:
    rgb = 4620980;
    break;
  case ON_KnownColor::Tan:
    rgb = 13808780;
    break;
  case ON_KnownColor::Teal:
    rgb = 32896;
    break;
  case ON_KnownColor::Thistle:
    rgb = 14204888;
    break;
  case ON_KnownColor::Tomato:
    rgb = 16737095;
    break;
  case ON_KnownColor::Turquoise:
    rgb = 4251856;
    break;
  case ON_KnownColor::Violet:
    rgb = 15631086;
    break;
  case ON_KnownColor::Wheat:
    rgb = 16113331;
    break;
  case ON_KnownColor::White:
    rgb = 16777215;
    break;
  case ON_KnownColor::WhiteSmoke:
    rgb = 16119285;
    break;
  case ON_KnownColor::Yellow:
    rgb = 16776960;
    break;
  case ON_KnownColor::YellowGreen:
    rgb = 10145074;
    break;
  default:
    break;
  }

  if (rgb != ON_UNSET_COLOR)
    return ON_Color(rgb);
  return ON_Color::UnsetColor;
}

ON_Color ON_ColorFromRandomKnownColor()
{
  ON_RandomNumberGenerator rg;
  rg.Seed();

  unsigned int known_color_as_unsigned = rg.RandomUnsignedInteger(
    (unsigned int)ON_KnownColor::AliceBlue, 
    (unsigned int)ON_KnownColor::YellowGreen
  );
  
  ON_KnownColor knownColor = ON_KnownColorFromUnsigned(known_color_as_unsigned);
  return ON_ColorFromKnownColor(knownColor);
}
