
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

static void PrintTexture(int num, const wchar_t* slot, const CRhRdkContent* pChild)
{
  if (nullptr != pChild)
  {
    auto* pFileBased = dynamic_cast<const IRhRdkFileBasedContent*>(pChild);
    if (nullptr != pFileBased)
    {
      RhinoApp().Print(L"Material(%d) %s texture is '%s'\n", num, slot, static_cast<const wchar_t*>(pFileBased->Filename()));
    }
    else
    {
      RhinoApp().Print(L"Material(%d) %s texture is not file-based\n", num, slot);
    }
  }
  else
  {
    RhinoApp().Print(L"Material(%d) %s slot is empty\n", num, slot);
  }
}

CRhinoCommand::result CCommandSamplePrintMaterialTextures::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  auto* pIt = pDoc->Contents().NewIterator(CRhRdkContent::Kinds::Material, CRhRdkDocument::it_Normal);

  int index = 0;
  const CRhRdkContent* pContent = nullptr;
  while (nullptr != (pContent = pIt->Next()))
  {
    PrintTexture(index, L"bitmap"      , pContent->FindChild(CS_MAT_BITMAP_TEXTURE));
    PrintTexture(index, L"bump"        , pContent->FindChild(CS_MAT_BUMP_TEXTURE));
    PrintTexture(index, L"transparency", pContent->FindChild(CS_MAT_TRANSPARENCY_TEXTURE));
    PrintTexture(index, L"environment" , pContent->FindChild(CS_MAT_ENVIRONMENT_TEXTURE));

    index++;
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
