#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSelAdjacentSubDFaces command
//

#pragma region SampleSelAdjacentSubDFaces command

class CCommandSampleSelAdjacentSubDFaces : public CRhinoCommand
{
public:
  CCommandSampleSelAdjacentSubDFaces() = default;
  UUID CommandUUID() override
  {
    // {5D7C9DB8-4E72-4409-B594-770734587978}
    static const GUID SampleSelAdjacentSubDFacesCommand_UUID =
    { 0x5D7C9DB8, 0x4E72, 0x4409, { 0xB5, 0x94, 0x77, 0x07, 0x34, 0x58, 0x79, 0x78 } };
    return SampleSelAdjacentSubDFacesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSelAdjacentSubDFaces"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSelAdjacentSubDFaces object
static class CCommandSampleSelAdjacentSubDFaces theSampleSelAdjacentSubDFacesCommand;

CRhinoCommand::result CCommandSampleSelAdjacentSubDFaces::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select SubD face");
  go.SetGeometryFilter(ON::meshface_filter);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& objref = go.Object(0);
  const ON_SubDComponentRef fref = objref.SubDFaceRef();
  if (!fref.IsValid())
    return CRhinoCommand::failure;

  const ON_SubDFace* f = fref.Face();
  if (nullptr == f)
    return CRhinoCommand::failure;

  const CRhinoSubDObject* subd_obj = CRhinoSubDObject::Cast(objref.Object());
  if (nullptr == subd_obj)
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_COMPONENT_INDEX> ci;
  ci.Append(f->ComponentIndex());

  const bool bStopAtCrease = false;

  const ON_SubDEdgePtr* eptr = f->m_edge4;
  for (unsigned short fei = 0; fei < f->m_edge_count; ++fei, ++eptr)
  {
    if (4 == fei)
    {
      eptr = f->m_edgex;
      if (nullptr == eptr)
        break;
    }
    const ON_SubDEdge* e = eptr->Edge();
    if (nullptr == e || 2 != e->m_face_count)
      continue;
    const ON_SubDFace* f1 = e->NeighborFace(f, bStopAtCrease);
    if (nullptr == f1)
      continue;
    ci.Append(f1->ComponentIndex());
  }

  for (int i = 0; i < ci.Count(); ++i)
    subd_obj->SelectSubObject(ci[i], true, true, true);

  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSelAdjacentSubDFaces command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
