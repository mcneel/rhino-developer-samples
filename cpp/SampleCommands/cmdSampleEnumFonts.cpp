#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEnumFonts command
//

#pragma region SampleEnumFonts command

class CCommandSampleEnumFonts : public CRhinoCommand
{
public:
  CCommandSampleEnumFonts() {}
  ~CCommandSampleEnumFonts() {}
  UUID CommandUUID()
  {
    // {AC834561-9FAD-4E15-B75C-57227D5C60A0}
    static const GUID SampleEnumFontsCommand_UUID =
    { 0xAC834561, 0x9FAD, 0x4E15, { 0xB7, 0x5C, 0x57, 0x22, 0x7D, 0x5C, 0x60, 0xA0 } };
    return SampleEnumFontsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleEnumFonts"; }
  const wchar_t* LocalCommandName() const { return L"SampleEnumFonts"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleEnumFonts object
static class CCommandSampleEnumFonts theSampleEnumFontsCommand;

int CALLBACK EnumFontFamiliesExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lpntme);
  UNREFERENCED_PARAMETER(FontType);

  if (0 != lpelfe && 0 != lParam)
  {
    ON_ClassArray<ON_wString>* p_font_names = (ON_ClassArray<ON_wString>*)lParam;
    ON_wString str(lpelfe->elfFullName);
    p_font_names->Append(str);
  }
  return 1;
}

CRhinoCommand::result CCommandSampleEnumFonts::RunCommand( const CRhinoCommandContext& context )
{
  UNREFERENCED_PARAMETER(context);

  ON_ClassArray<ON_wString> font_names;

  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));
  wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Arial");
  lf.lfCharSet = ANSI_CHARSET;
 
  HDC hDC = ::GetDC(NULL);
  EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)EnumFontFamiliesExProc, (LPARAM)&font_names, 0);
  ReleaseDC(NULL, hDC);

  for (int i = 0; i < font_names.Count(); i++)
    RhinoApp().Print(L"%s\n", (const wchar_t*)font_names[i]);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleEnumFonts command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
