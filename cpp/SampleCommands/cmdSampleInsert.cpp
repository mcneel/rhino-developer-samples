#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleInsert command
//

class CGetBlockInsertPoint : public CRhinoGetPoint
{
public:
  CGetBlockInsertPoint();
  ~CGetBlockInsertPoint();

  bool CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

public:
  ON_SimpleArray<const CRhinoObject*> m_block_objects;
  ON_Xform m_xform;
  bool m_draw;
};

CGetBlockInsertPoint::CGetBlockInsertPoint()
{
  m_xform.Identity();
  m_draw = false;
}

CGetBlockInsertPoint::~CGetBlockInsertPoint()
{
}

bool CGetBlockInsertPoint::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform)
{
  UNREFERENCED_PARAMETER(vp);
  ON_3dVector v = pt - BasePoint();
  xform.Translation(v);
  return true;
}

void CGetBlockInsertPoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p)
{
  m_draw = CalculateTransform(vp, pt, m_xform);
  if (!m_draw)
    m_xform.Identity();
  CRhinoGetPoint::OnMouseMove(vp, flags, pt, p);
}

void CGetBlockInsertPoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  ON_Color saved_color = dp.ObjectColor();

  int i, count = m_block_objects.Count();
  if (m_draw && count > 0)
  {
    dp.PushModelTransform(m_xform);
    for (i = 0; i < count; i++)
    {
      const CRhinoObject* obj = m_block_objects[i];
      if (obj)
      {
        dp.SetObjectColor(obj->ObjectDrawColor(TRUE));
        obj->Draw(dp);
        if (dp.InterruptDrawing())
          break;
      }
    }
    dp.PopModelTransform();
  }

  dp.SetObjectColor(saved_color);

  CRhinoGetPoint::DynamicDraw(dp, pt);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleInsert : public CRhinoCommand
{
public:
  CCommandSampleInsert() {}
  ~CCommandSampleInsert() {}
  UUID CommandUUID()
  {
    // {CE5EDF1A-7D47-4A5C-9868-7FA6260BA456}
    static const GUID SampleInsertCommand_UUID =
    { 0xCE5EDF1A, 0x7D47, 0x4A5C, { 0x98, 0x68, 0x7F, 0xA6, 0x26, 0x0B, 0xA4, 0x56 } };
    return SampleInsertCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleInsert"; }
  const wchar_t* LocalCommandName() { return L"SampleInsert"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleInsert object
static class CCommandSampleInsert theSampleInsertCommand;

CRhinoCommand::result CCommandSampleInsert::RunCommand(const CRhinoCommandContext& context)
{
  // Prompt for a block to insert
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Name of block to insert");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  // Validate the return string
  ON_wString block_name = gs.String();
  block_name.TrimLeftAndRight();
  if (block_name.IsEmpty())
    return CRhinoCommand::cancel;

  // Find the specified block
  CRhinoInstanceDefinitionTable& block_table = context.m_doc.m_instance_definition_table;
  int block_index = block_table.FindInstanceDefinition(block_name);
  if (block_index < 0)
  {
    ::RhinoApp().Print(L"Unable to insert \"%s\". Block not found.\n", block_name);
    return CRhinoCommand::cancel;
  }

  // Obtain the block definition
  const CRhinoInstanceDefinition* block = block_table[block_index];
  if (!block || block->IsDeleted())
    return CRhinoCommand::failure;

  // Prompt the user to pick an insertion point.
  CGetBlockInsertPoint gp;
  gp.SetCommandPrompt(L"Insertion point");
  gp.SetBasePoint(ON_origin);
  // Add the block definition geometry to our getpoint so we 
  // can dynamically draw the stuff...
  block->GetObjects(gp.m_block_objects);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  // Calculate the xform one more time (to be accurate)
  ON_Xform xform;
  gp.CalculateTransform(gp.View()->Viewport(), gp.Point(), xform);

  // Add the block to Rhino
  CRhinoInstanceObject* obj = block_table.AddInstanceObject(block_index, xform);
  if (obj)
  {
    context.m_doc.Redraw();
    return CRhinoCommand::success;
  }

  ::RhinoApp().Print(L"Error creating block reference.\n");
  return CRhinoCommand::failure;
}

//
// END SampleInsert command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
