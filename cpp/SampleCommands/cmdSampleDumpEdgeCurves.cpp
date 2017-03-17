#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDumpEdgeCurves command
//

class CCommandSampleDumpEdgeCurvesTextOut : public CRhinoTextOut
{
public:
  void PrintText(ON_TextLog& dump, LPVOID pData)
  {
    UNREFERENCED_PARAMETER(pData);

    const ON_BrepFace* face = static_cast<const ON_BrepFace*>(Data());
    if (0 == face)
      return;

    const ON_Brep* brep = face->Brep();
    if (0 == brep)
      return;

    dump.Print(L"ON_BrepFace[%d]:\n", face->m_face_index);
    dump.PushIndent();

    for (int i = 0; i < face->m_li.Count(); i++)
    {
      const ON_BrepLoop& loop = brep->m_L[face->m_li[i]];
      for (int j = 0; j < loop.m_ti.Count(); j++)
      {
        const ON_BrepTrim& trim = brep->m_T[loop.m_ti[j]];
        if (trim.m_type == ON_BrepTrim::seam)
          continue;
        if (trim.m_ei < 0)
          continue;

        const ON_BrepEdge& edge = brep->m_E[trim.m_ei];
        const ON_Curve* crv = edge.EdgeCurveOf();
        if (0 == crv)
          continue;

        dump.Print(L"\n");
        edge.Dump(dump);
        dump.Print(L"\n");

        dump.PushIndent();
        crv->Dump(dump);
        dump.PopIndent();
      }
    }
  }
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDumpEdgeCurves : public CRhinoCommand
{
public:
  CCommandSampleDumpEdgeCurves() {}
  ~CCommandSampleDumpEdgeCurves() {}
  UUID CommandUUID()
  {
    // {CBC593E5-CBB3-4D5F-BE38-17A447AC9132}
    static const GUID SampleDumpEdgeCurvesCommand_UUID =
    { 0xCBC593E5, 0xCBB3, 0x4D5F, { 0xBE, 0x38, 0x17, 0xA4, 0x47, 0xAC, 0x91, 0x32 } };
    return SampleDumpEdgeCurvesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDumpEdgeCurves"; }
  const wchar_t* LocalCommandName() { return L"SampleDumpEdgeCurves"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDumpEdgeCurves object
static class CCommandSampleDumpEdgeCurves theSampleDumpEdgeCurvesCommand;

CRhinoCommand::result CCommandSampleDumpEdgeCurves::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select face");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(TRUE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& object_ref = go.Object(0);
  const ON_BrepFace* face = object_ref.Face();
  if (0 == face)
    return CRhinoCommand::failure;

  CCommandSampleDumpEdgeCurvesTextOut out;
  return out.DisplayText(context.IsInteractive() ? true : false, L"Edge Curves", (LPVOID)face);

  return CRhinoCommand::success;
}

//
// END SampleDumpEdgeCurves command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
