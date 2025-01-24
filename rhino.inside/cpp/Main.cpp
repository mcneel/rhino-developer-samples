#include "pch.h"
#include "RhinoCore.h"

int wmain(int argc, wchar_t** argv)
{
  // Load Rhino
  CRhinoCore rhino_core(argc, argv);

  // Geet the Rhino version number, etc.
  int ver = RhinoApp().ExeVersion();
  int sr = RhinoApp().ExeServiceRelease();
  ON_wString date;
  RhinoApp().GetBuildDate(date);
  std::wstring str0(static_cast<const wchar_t*>(date));
  std::wcout << "Rhino " << ver << "." << sr << " (" << str0 << ") loaded." << std::endl;

  // Create a NURBS curve by interpolating points
  ON_3dPointArray points(16);
  points.Append(ON_3dPoint(0.0, 3.12494, 0.0));
  points.Append(ON_3dPoint(7.01306, 3.31419, 0.0));
  points.Append(ON_3dPoint(8.01888, 3.34416, 0.0));
  points.Append(ON_3dPoint(9.02578, 3.37375, 0.0));
  points.Append(ON_3dPoint(10.0338, 3.40260, 0.0));
  points.Append(ON_3dPoint(11.0430, 3.43034, 0.0));
  points.Append(ON_3dPoint(12.0533, 3.45659, 0.0));
  points.Append(ON_3dPoint(13.0648, 3.48098, 0.0));
  points.Append(ON_3dPoint(14.0776, 3.50313, 0.0));
  points.Append(ON_3dPoint(15.0916, 3.52267, 0.0));
  points.Append(ON_3dPoint(16.1068, 3.53923, 0.0));
  points.Append(ON_3dPoint(17.1233, 3.55249, 0.0));
  points.Append(ON_3dPoint(18.1410, 3.56222, 0.0));
  points.Append(ON_3dPoint(19.1587, 3.56829, 0.0));
  points.Append(ON_3dPoint(20.1758, 3.57091, 0.0));
  points.Append(ON_3dPoint(30.3156, 3.45748, 0.0));

  const int knot_style = 0; // uniform
  ON_NurbsCurve* pCurve = RhinoInterpCurve(3, points, nullptr, nullptr, knot_style, nullptr);
  if (pCurve)
  {
    double length = ON_UNSET_VALUE;
    if (pCurve->GetLength(&length))
      std::cout << "ON_NurbsCurve with " << length << " length created" << std::endl;

    CRhinoCreateDocumentOptions options;
    options.SetCreateHeadless(true);
    int doc_runtime_serial_number = CRhinoDoc::CreateDocument(nullptr, &options);

    CRhinoDoc* pDoc = CRhinoDoc::FromRuntimeSerialNumber(doc_runtime_serial_number);
    pDoc->AddCurveObject(*pCurve, nullptr, nullptr, 0);
    delete pCurve; // Don't leak

    ON_wString path;
    CRhinoFileUtilities::GetMyDocumentsFolder(path);
    path += L"\\RhinoInsideConsoleCxx.3dm";

    FILE* pFile = ON::OpenFile(static_cast<const wchar_t*>(path), L"wb");
    if (nullptr != pFile)
    {
      ON_BinaryFile archive(ON::archive_mode::write3dm, pFile);
      CRhinoFileWriteOptions fwo;
      pDoc->Write3dmFile(archive, fwo);
      ON::CloseFile(pFile);

      std::wstring str1(static_cast<const wchar_t*>(path));
      std::wcout << "Curve saved to " << str1 << std::endl;
    }
  }

  system("pause");
}
