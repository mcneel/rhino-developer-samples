
#include "stdafx.h"

/** \class CSampleRdkRemoveUnusedContentCmd 

  To remove all unused render materials programatically you can run the RenderRemoveUnusedMaterials command.

    const auto doc_sn = doc.RuntimeSerialNumber();
    RhinoApp().RunScript(doc_sn, L"RenderRemoveUnusedMaterials", 0);

  However, you may only want to remove a specific unused render content. This command demonstrates how to do that.

*/
static class CSampleRdkRemoveUnusedContentCmd : public CRhinoCommand
{
protected:
  virtual UUID CommandUUID() override { static const UUID uuid = { 0xc27b0216, 0x3463, 0x4349, { 0xaf, 0xdd, 0x66, 0xc8, 0x5e, 0x75, 0xdc, 0xbe } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleRdkRemoveUnusedContent"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
}
theSampleRdkRemoveUnusedContentCmd;

CRhinoCommand::result CSampleRdkRemoveUnusedContentCmd::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  // This example finds and deletes a material called 'Custom'.
  const wchar_t* name = L"Custom";

  // Find the content by name.
  CRhRdkContentArray aContent;
  pDoc->Contents().Find(CRhRdkContent::Kinds::Material, name, false, false, aContent);
  if (aContent.Count() == 0)
  {
    RhinoApp().Print(L"'%s' was not found\n", name);
    return failure;
  }

  // Check if the content is in use (assume only 1 match).
  const auto& c = *aContent[0];
  if (c.UseCount() > 0)
  {
    RhinoApp().Print(L"'%s' is in use\n", name);
    return failure;
  }

#ifdef RHINO_7_8_UPWARDS // The following is only supported from Rhino 7.8 onwards due to a bug in earlier versions.
  // Detach the content from the document.
  auto& contents = pDoc->Contents().BeginChange(RhRdkChangeContext::Program);
  auto* pDetach = contents.Detach(c);
  contents.EndChange();
#else
  // This method is not recommended but is the only way until Rhino 7.8 if you want undo support.
  auto& rdkDoc = CRhRdkDocument::Get(*pDoc).BeginChange(RhRdkChangeContext::Program);
  auto* pDetach = rdkDoc.DetachContent(c, CRhRdkEventWatcher::DetachReason::Delete);
  rdkDoc.EndChange();
#endif

  if (nullptr != pDetach)
  {
    // Delete the detached content while allowing undo.
    CRhRdkContentArrayNC aContentNC;
    aContentNC.Append(pDetach);
    CRhRdkContentUndo cu(*pDoc);
    cu.DeleteTopLevelContent(aContentNC);
  }

  return success;
}
