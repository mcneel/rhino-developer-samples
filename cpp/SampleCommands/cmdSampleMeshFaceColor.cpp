#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshFaceColor command
//

class CSampleMeshFaceColorConduit : public CRhinoDisplayConduit
{
public:
  CSampleMeshFaceColorConduit(unsigned int runtime_object_serial_number);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate);

private:
  unsigned int m_runtime_object_serial_number;
  ON_SimpleArray<unsigned int> m_mesh_face_colors;
};

CSampleMeshFaceColorConduit::CSampleMeshFaceColorConduit(unsigned int runtime_object_serial_number)
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWOBJECT)
  , m_runtime_object_serial_number(runtime_object_serial_number)
{
  srand((unsigned int)time(0));
}

bool CSampleMeshFaceColorConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  if ((nChannel == CSupportChannels::SC_DRAWOBJECT))
  {
    if (m_pChannelAttrs && m_pChannelAttrs->m_pObject)
    {
      if (m_pChannelAttrs->m_pObject->RuntimeSerialNumber() == m_runtime_object_serial_number)
      {
        // One time initialization
        if (0 == m_mesh_face_colors.Count())
        {
          const CRhinoMeshObject* mesh_obj = CRhinoMeshObject::Cast(m_pChannelAttrs->m_pObject);
          if (mesh_obj)
          {
            const ON_Mesh* mesh = mesh_obj->Mesh();
            if (mesh)
            {
              const int mesh_face_count = mesh->FaceCount();
              m_mesh_face_colors.SetCapacity(mesh_face_count);
              m_mesh_face_colors.SetCount(mesh_face_count);
              for (int i = 0; i < mesh_face_count; i++)
                m_mesh_face_colors[i] = RGB(rand() % 255, rand() % 255, rand() % 255);
            }
          }
        }


        if (m_mesh_face_colors.Count())
        {
          // store current object's color
          CColorVec ObjColor(m_pDisplayAttrs->m_ObjectColor);

          m_pChannelAttrs->m_bDrawObject = false;

          // Start with the current attributes - always a good idea for proper conforming
          CDisplayPipelineAttributes da(*m_pDisplayAttrs);

          da.m_pMaterial->m_FrontMaterial.SetTransparency(0.0);
          da.m_pMaterial->m_BackMaterial.SetTransparency(0.0);
          da.m_pMaterial->m_FrontMaterial.m_bFlatShaded = true;
          da.m_pMaterial->m_BackMaterial.m_bFlatShaded = true;

          // If we're not drawing surfaces, then we don't want to draw any shaded polygons...
          da.m_bShadeSurface = dp.DrawingSurfaces();

          // Here's where things can get ugly due the way this conduit is implemented.
          // Since the object is always selected (in this example), that means we'll be drawing
          // both the shaded polygons and the wires. However, I'm changing this up a little so that
          // we don't draw the wires if we're shaded, and instead, I blend the shaded polygons with
          // the object's (highlighted) color...

          // So, if the object is highlighted AND we're drawing wires AND the original display
          // mode is a shaded mode, then we don't want to draw anything. Mesh objects are unique
          // in that they can be both drawing wires and surfaces at the same time. Thus, we must
          // check for exclusivity on just wires only...
          bool bWiresOnly = dp.DrawingWires() && !dp.DrawingSurfaces();

          if (da.m_IsHighlighted && bWiresOnly && m_pDisplayAttrs->m_bShadeSurface)
            return true; // don't draw anything...

          for (int i = 0; i < m_mesh_face_colors.Count(); i++)
          {
            CColorVec color((COLORREF)m_mesh_face_colors[i]);

            // If you want the highlight blending, then just uncomment the 2 lines below.
            // since this conduit always runs with highlighted objects, then the following
            // code will always get hit, and you will not see your true face colors (ever),
            // so you might want to get rid of this altogether. It is only here to show how
            // this can be done. However, since you're overriding the drawing of this object,
            // then something really should be done about object selection, otherwise there
            // will be no way for users to know/see whether or not the object is selected.

            //if ( da.m_IsHighlighted )
            //  color = (color + ObjColor) * 0.5f; // blend the object color with the face color 50/50...

            da.m_pMaterial->m_FrontMaterial.m_diffuse = color;
            da.m_pMaterial->m_BackMaterial.m_diffuse = color;
            da.m_ObjectColor = color;
            da.m_MeshWireColor = color;

            // Here's yet another situation that you might want to handle. If we're drawing 
            // shaded polygons, then we might not want to draw the wires. Or if we do, then
            // we might want those wires some fixed color. However, keep in mind that hiding
            // and showing of mesh wires is controlled by the display mode. But, if an object
            // is selected, then its "show wires" flag will always be set at this point. And
            // since this conduit only works on selected objects, wires will always be on no 
            // matter what the user sets in the current display mode. So there is really no
            // way to tell if wires should be on of off at this point.
            if (da.m_bShadeSurface)
            {
              // We're here because we're about to draw shaded polygons. For now, I'm always
              // going to force the wires to be black...
              da.m_MeshWireColor = 0;
              da.m_bShowMeshWires = true; // not really needed because of what I said above,
                                          // but here for clarity...
            }

            dp.DrawFace(m_pChannelAttrs->m_pObject, i, &da);
          }
        }
      }
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMeshFaceColor : public CRhinoCommand
{
public:
  CCommandSampleMeshFaceColor() {}
  ~CCommandSampleMeshFaceColor() {}
  UUID CommandUUID()
  {
    // {6EA9AC1B-A2FE-4F66-B6BC-D316E9326255}
    static const GUID SampleMeshFaceColorCommand_UUID =
    { 0x6EA9AC1B, 0xA2FE, 0x4F66, { 0xB6, 0xBC, 0xD3, 0x16, 0xE9, 0x32, 0x62, 0x55 } };
    return SampleMeshFaceColorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshFaceColor"; }
  const wchar_t* LocalCommandName() { return L"SampleMeshFaceColor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshFaceColor object
static class CCommandSampleMeshFaceColor theSampleMeshFaceColorCommand;

CRhinoCommand::result CCommandSampleMeshFaceColor::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoMeshObject* mesh_obj = CRhinoMeshObject::Cast(go.Object(0).Object());
  if (0 == mesh_obj)
    return CRhinoCommand::failure;

  CSampleMeshFaceColorConduit conduit(mesh_obj->RuntimeSerialNumber());
  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Regen();

  return CRhinoCommand::success;
}

//
// END SampleMeshFaceColor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
