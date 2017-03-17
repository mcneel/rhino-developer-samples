#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleUnrollSurface command
//

#pragma region SampleUnrollSurface command

class CCommandSampleUnrollSurface : public CRhinoCommand
{
public:
  CCommandSampleUnrollSurface() {}
  ~CCommandSampleUnrollSurface() {}
  UUID CommandUUID()
  {
    // {CB7091AA-BDAB-4D26-91D6-FD455981B636}
    static const GUID SampleUnrollSurfaceCommand_UUID =
    { 0xCB7091AA, 0xBDAB, 0x4D26, { 0x91, 0xD6, 0xFD, 0x45, 0x59, 0x81, 0xB6, 0x36 } };
    return SampleUnrollSurfaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleUnrollSurface"; }
  const wchar_t* LocalCommandName() const { return L"SampleUnrollSurface"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool ReverseVReversedSurfaces(ON_Brep* pBrep);
};

// The one and only CCommandSampleUnrollSurface object
static class CCommandSampleUnrollSurface theSampleUnrollSurfaceCommand;

CRhinoCommand::result CCommandSampleUnrollSurface::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface to unroll");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* pObject = go.Object(0).Object();
  const ON_Brep* pBrep = go.Object(0).Brep();
  if (0 == pObject || 0 == pBrep)
    return CRhinoCommand::failure;

  bool bExplode = false;

  ON_Brep* p3dBrep = 0;
  int i, type = -1;
  if (1 == pBrep->m_F.Count())
  {
    p3dBrep = pBrep->DuplicateFace(0, false);
    type = 0;
  }
  else
  {
    p3dBrep = static_cast<ON_Brep*>(pBrep->Duplicate());
    type = 1;
  }

  if (0 == p3dBrep)
    return CRhinoCommand::failure;

  p3dBrep->Compact();
  for (i = 0; i < p3dBrep->m_F.Count(); i++)
    p3dBrep->RebuildEdges(p3dBrep->m_F[i], 0.00001, true, true);
  p3dBrep->ShrinkSurfaces();

  ReverseVReversedSurfaces(p3dBrep);

  CRhinoUnroll Unroller(p3dBrep, context.m_doc.AbsoluteTolerance(), 0.1);
  int irc = Unroller.PrepareFaces();
  if (0 == irc)
  {
    bool ok = Unroller.FlattenFaces();
    if (ok)
    {
      int flat_face_count = Unroller.CreateFlatBreps(bExplode, 2.0);
      if (flat_face_count)
      {
        ON_SimpleArray<ON_Brep*> flat_breps;
        ON_ClassArray< ON_SimpleArray<ON_Curve*> > flat_curves;
        ON_ClassArray< ON_SimpleArray<ON_3dPoint> > flat_points;
        ON_ClassArray< ON_SimpleArray<ON_TextDot*> > flat_dots;
        Unroller.CollectResults(flat_breps, flat_curves, flat_points, flat_dots);

        if (!bExplode && flat_breps.Count() > 1)
        {
          ON_Brep* pJoinedBrep = ON_Brep::New();
          if (pJoinedBrep)
          {
            for (i = 0; i < flat_breps.Count(); i++)
            {
              if (flat_breps[i] != 0)
                pJoinedBrep->Append(*flat_breps[i]);
            }
            RhinoJoinBrepNakedEdges(*pJoinedBrep);
            flat_breps.Empty();
            flat_breps.Append(pJoinedBrep);
          }
        }

        CRhinoObjectAttributes att = pObject->Attributes();
        att.m_uuid = ON_nil_uuid;
        att.RemoveFromAllGroups();

        for (i = 0; i < flat_breps.Count(); i++)
        {
          CRhinoBrepObject* flat_obj = new CRhinoBrepObject(att);
          flat_obj->SetBrep(flat_breps[i]);
          if (!context.m_doc.AddObject(flat_obj))
            delete flat_obj; // Don't leak...
        }
      }
    }

    delete p3dBrep; // Don't leak...
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

// Like FlipReversedSurfaces() but uses Reverse(1) instead of Transpose()
bool CCommandSampleUnrollSurface::ReverseVReversedSurfaces(ON_Brep* pBrep)
{
  if (0 == pBrep)
    return false;

  const int face_count = pBrep->m_F.Count();
  bool rc = true;
  for (int fi = 0; fi < face_count; fi++)
  {
    ON_BrepFace& face = pBrep->m_F[fi];
    if (face.m_bRev)
    {
      if (!face.Reverse(1))
        rc = false;
    }
  }

  return rc;
}

#pragma endregion

//
// END SampleUnrollSurface command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
