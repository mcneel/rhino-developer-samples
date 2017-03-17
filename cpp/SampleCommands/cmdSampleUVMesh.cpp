#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleUVMesh command
//

class CCommandSampleUVMesh : public CRhinoCommand
{
public:
  CCommandSampleUVMesh() { m_U = 10; m_V = 10; }
  ~CCommandSampleUVMesh() {}
  UUID CommandUUID()
  {
    // {B2FCE15E-E9E4-41DD-942F-A75D4AF87AD8}
    static const GUID SampleUVMeshCommand_UUID =
    { 0xB2FCE15E, 0xE9E4, 0x41DD, { 0x94, 0x2F, 0xA7, 0x5D, 0x4A, 0xF8, 0x7A, 0xD8 } };
    return SampleUVMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleUVMesh"; }
  const wchar_t* LocalCommandName() { return L"SampleUVMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

private:
  int m_U;
  int m_V;
};

// The one and only CCommandSampleUVMesh object
static class CCommandSampleUVMesh theSampleUVMeshCommand;

void CCommandSampleUVMesh::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  int i = m_U;
  if (pc.LoadProfileInt(lpszSection, L"U", &i))
    m_U = i;
  if (pc.LoadProfileInt(lpszSection, L"V", &i))
    m_V = i;
}

void CCommandSampleUVMesh::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  pc.SaveProfileInt(lpszSection, L"U", m_U);
  pc.SaveProfileInt(lpszSection, L"V", m_V);
}

CRhinoCommand::result CCommandSampleUVMesh::RunCommand(const CRhinoCommandContext& context)
{
  bool bDeleteInput = false;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select untrimmed surface");
  go.AddCommandOptionInteger(RHCMDOPTNAME(L"U"), &m_U, 0, 2);
  go.AddCommandOptionInteger(RHCMDOPTNAME(L"V"), &m_V, 0, 2);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"DeleteInput"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bDeleteInput, &bDeleteInput);
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::untrimmed_surface);

  CRhinoGet::result rs = CRhinoGet::no_result;
  while (CRhinoGet::cancel != rs && CRhinoGet::nothing != rs && CRhinoGet::object != rs)
    rs = go.GetObjects(1, 1);

  if (CRhinoGet::cancel == rs)
    return CRhinoCommand::cancel;
  if (0 == go.ObjectCount())
    return CRhinoCommand::failure;

  const CRhinoObject* pObj = go.Object(0).Object();
  if (0 == pObj)
    return CRhinoCommand::failure;

  const ON_BrepFace* pBrepFace = go.Object(0).Face();
  if (0 == pBrepFace)
    return CRhinoCommand::failure;

  const ON_Surface* pSurf = pBrepFace->SurfaceOf();
  if (0 == pSurf)
    return CRhinoCommand::failure;

  ON_SimpleArray<double> UArray(m_U + 1);
  ON_SimpleArray<double> VArray(m_V + 1);

  double UDist = (pSurf->Domain(0).m_t[1] - pSurf->Domain(0).m_t[0]) / m_U;
  double VDist = (pSurf->Domain(1).m_t[1] - pSurf->Domain(1).m_t[0]) / m_V;

  int i;
  for (i = 0; i <= m_U; i++)
    UArray.Append(pSurf->Domain(0).m_t[0] + i * UDist);
  for (i = 0; i <= m_V; i++)
    VArray.Append(pSurf->Domain(1).m_t[0] + i * VDist);

  // If m_U or m_V are large then there can be a slight difference between
  // pSurf->Domain(0).m_t[0] + (m_U-1) * UDist and pSurf->Domain(0).m_t[1]
  // ON_MeshSurface requires it to be less than or equal to pSurf->Domain(0).m_t[1]

  double* d = UArray.Last();
  if (pSurf->Domain(0).m_t[1] < *d)
    *d = pSurf->Domain(0).m_t[1];
  d = VArray.Last();
  if (pSurf->Domain(1).m_t[1] < *d)
    *d = pSurf->Domain(1).m_t[1];

  ON_Mesh* pMeshOut = ON_MeshSurface(*pSurf, m_U + 1, UArray.Array(), m_V + 1, VArray.Array());
  if (0 == pMeshOut)
    return CRhinoCommand::failure;

  CRhinoMeshObject* pMeshObj = new CRhinoMeshObject(pObj->Attributes());
  if (0 == pMeshObj)
  {
    delete pMeshOut;
    return CRhinoCommand::failure;
  }
  pMeshObj->SetMesh(pMeshOut);

  if (true == bDeleteInput)
    context.m_doc.ReplaceObject(pObj, pMeshObj);
  else
    context.m_doc.AddObject(pMeshObj);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleUVMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
