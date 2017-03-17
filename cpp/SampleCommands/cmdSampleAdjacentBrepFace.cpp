#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAdjacentBrepFace command
//

#pragma region SampleAdjacentBrepFace command

class CCommandSampleAdjacentBrepFace : public CRhinoCommand
{
public:
  CCommandSampleAdjacentBrepFace() {}
  ~CCommandSampleAdjacentBrepFace() {}
  UUID CommandUUID()
  {
    // {50F82802-80A6-44D9-8225-7002C497C46F}
    static const GUID SampleAdjacentBrepFaceCommand_UUID =
    { 0x50F82802, 0x80A6, 0x44D9, { 0x82, 0x25, 0x70, 0x02, 0xC4, 0x97, 0xC4, 0x6F } };
    return SampleAdjacentBrepFaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAdjacentBrepFace"; }
  const wchar_t* LocalCommandName() const { return L"SampleAdjacentBrepFace"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleAdjacentBrepFace object
static class CCommandSampleAdjacentBrepFace theSampleAdjacentBrepFaceCommand;

// QUESTION:
// Given a trim curve of a surface, how to get the surface on the other side
// of the trim (if there are adjacent surfaces along this trim)?

// ANSWER:
// An ON_BrepEdge is connector for joining ON_BrepTrim objects and representing
// a 3-D location of the seam they form.  An Edge can connect 1 or more trims.
// In the case of a simple "joined edge" there will be 2 trims, one from each
// adjacent face. In a closed face, like a cylinder, the trims will be different
// on each side of the face, and they will both be in the same loop and belong to
// the same face.
static const ON_BrepFace* GetOtherFace(const ON_BrepTrim* trim)
{
  ON_BrepFace* face = 0;
  if (0 == trim)
    return 0;

  ON_Brep* brep = trim->Brep();
  if (0 == brep)
    return 0;

  const ON_BrepEdge* edge = trim->Edge();
  if (0 == edge)
    return 0;

  // Trim count on a proper joined edge should be 2.
  const int trim_count = edge->TrimCount();
  if (trim_count < 2)
    return 0;  // Not joined
  if (trim_count > 2)
    return 0;  // Joined to more than one other surface (non-manifold)

  for (int i = 0; i < trim_count; i++)
  {
    // Look for a trim that's not the same as the input one
    // edge->m_ti is an array of indexes of the trims connected to an edge
    const ON_BrepTrim* other_trim = brep->Trim(edge->m_ti[i]);
    if (0 != other_trim && trim != other_trim)
    {
      // There is another trim on the edge, and its not the same as the input one.
      // So get the face it belonge to.
      face = other_trim->Face();

      // You can also get the loop on the other side of the edge
      //const ON_BrepLoop* other_loop = other_trim->Loop();

      // Or the surface geometry of the face
      //const ON_Surface* srf = face->SurfaceOf();

      break;
    }
  }

  return face;
}

CRhinoCommand::result CCommandSampleAdjacentBrepFace::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleAdjacentBrepFace command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
