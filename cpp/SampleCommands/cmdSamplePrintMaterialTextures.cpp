
#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePrintMaterialTextures command
//

#pragma region SamplePrintMaterialTextures command

class CCommandSamplePrintMaterialTextures : public CRhinoCommand
{
public:
  CCommandSamplePrintMaterialTextures() = default;
  ~CCommandSamplePrintMaterialTextures() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x515DCFC6, 0xA4F9, 0x42D1, { 0x95, 0xA0, 0x16, 0xFD, 0x19, 0x23, 0xE3, 0x39 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SamplePrintMaterialTextures"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSamplePrintMaterialTextures object
static class CCommandSamplePrintMaterialTextures theSamplePrintMaterialTexturesCommand;

static void PrintChild(const CRhRdkContent& child)
{
  RhinoApp().Print(L"Child slot: '%s'", static_cast<const wchar_t*>(child.ChildSlotName()));

  if (child.IsKind(CRhRdkContent::Kinds::Texture))
  {
    auto sDesc = ON_wString(L"'") + child.TypeDescription() + L"'";

    auto* pFileBased = dynamic_cast<const IRhRdkFileBasedContent*>(&child);
    if (nullptr != pFileBased)
    {
      sDesc = sDesc + L", Filename: '" + pFileBased->Filename() + L"'";
    }

    RhinoApp().Print(L", Description: %s\n", static_cast<const wchar_t*>(sDesc));
  }
  else
  {
    RhinoApp().Print(L" is not a texture\n");
  }
}

static void PrintContent(const CRhRdkContent& c, int count)
{
  auto it = c.GetChildIterator();

  const auto* pChild = it.GetNextChild();
  if (nullptr != pChild)
  {
    while (nullptr != pChild)
    {
      RhinoApp().Print(L"Material[%d]: ", count);

      PrintChild(*pChild);

      pChild = it.GetNextChild();
    }
  }
  else
  {
    RhinoApp().Print(L"Material[%d]: No textures found\n", count);
  }
}

CRhinoCommand::result CCommandSamplePrintMaterialTextures::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  int count = 0;
  auto* pIt = doc->Contents().NewIterator(CRhRdkContent::Kinds::Material, CRhRdkDocument::it_Normal);

  const auto* pContent = pIt->Next();
  if (nullptr != pContent)
  {
    while (nullptr != pContent)
    {
      PrintContent(*pContent, count++);

      pContent = pIt->Next();
    }
  }
  else
  {
    RhinoApp().Print(L"No materials found\n");
  }

  delete pIt;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePrintMaterialTextures command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
