#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomViewCapture command
//

#pragma region SampleCustomViewCapture command

class CCommandSampleCustomViewCapture : public CRhinoCommand
{
public:
  CCommandSampleCustomViewCapture() {}
  ~CCommandSampleCustomViewCapture() {}
  UUID CommandUUID()
  {
    // {381342E6-B967-46DE-8A2D-D163F28F9379}
    static const GUID SampleCustomViewCaptureCommand_UUID =
    { 0x381342E6, 0xB967, 0x46DE, { 0x8A, 0x2D, 0xD1, 0x63, 0xF2, 0x8F, 0x93, 0x79 } };
    return SampleCustomViewCaptureCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCustomViewCapture"; }
  const wchar_t* LocalCommandName() const { return L"SampleCustomViewCapture"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

public:
  // This is a version of CRhinoDoc::RenderToDC that accepts a custom
  // viewport background color.
  bool SampleRenderToDC( 
    CRhinoObjectIterator& it,
    CDC* pDC, 
    int width,
    int height,
    const ON_3dmView& view_info,
    bool bIgnoreHighlights,
    bool bDrawTitle,
    bool bDrawConstructionPlane,
    bool bDrawWorldAxes,
    bool bGhosted,
    ON_Color background_color = ON_UNSET_COLOR
    ) const;

  // This is a general purpose version of CRhinoDoc::DrawToDC that
  // accepts custom display pipeline attributes.
  bool SampleDrawDisplayModeToDC(
    CRhinoObjectIterator& it,
    CDC* pDC, 
    int width,
    int height,
    const ON_3dmView& view_info,
    CDisplayPipelineAttributes& da
    ) const;
};

// The one and only CCommandSampleCustomViewCapture object
static class CCommandSampleCustomViewCapture theSampleCustomViewCaptureCommand;

CRhinoCommand::result CCommandSampleCustomViewCapture::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return CRhinoCommand::failure;

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  if (!gf.DisplayFileDialog(CRhinoGetFileDialog::save_bitmap_dialog))
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return nothing;

  CRect rect;
  view->GetClientRect(rect);

  CRhinoDib dib;
  if (dib.CreateDib(rect.Width(), rect.Height(), 32, true))
  {
    CRhinoObjectIterator it(
      CRhinoObjectIterator::normal_or_locked_objects, 
      CRhinoObjectIterator::active_and_reference_objects 
      );

    SampleRenderToDC(it, dib, dib.Width(), dib.Height(),
      view->ActiveViewport().View(),
      true, false, false, false, false,
      RGB(0, 255, 255) // Cyan
      );

    dib.WriteToFile(filename);
  }

  return CRhinoCommand::success;
}

bool CCommandSampleCustomViewCapture::SampleRenderToDC( 
  CRhinoObjectIterator& it,
  CDC* pDC, 
  int width,
  int height,
  const ON_3dmView& view_info,
  bool bIgnoreHighlights,
  bool bDrawTitle,
  bool bDrawConstructionPlane,
  bool bDrawWorldAxes,
  bool bGhosted,
  ON_Color background_color
  ) const
{
  if (0 == pDC)
    return false;

  CRhinoViewport vp;
  vp.SetView(view_info);  
  vp.SetScreenSize(width, height);
  vp.SetDisplayMode(view_info.m_display_mode);
  vp.EnableGhostedShade(bGhosted ? true : false);

  CRhinoDisplayPipeline_OGL dp(pDC, vp);

  CDisplayPipelineAttributes da = *CRhinoDisplayAttrsMgr::StdRenderedAttrs(); 
  da.m_bUseDocumentGrid = false;
  da.m_bDrawGrid = bDrawConstructionPlane;
  da.m_bDrawAxes = da.m_bDrawGrid;
  da.m_bDrawWorldAxes = bDrawWorldAxes;
  da.m_bIgnoreHighlights = bIgnoreHighlights;
  da.m_pIteratorOverride = &it;

  if (background_color != ON_UNSET_COLOR)
  {
    da.m_eFillMode = FBFM_SOLID_COLOR;
    da.m_SolidColor = background_color;
  }

  return dp.DrawToDC(pDC, width, height, &da);      
}

bool CCommandSampleCustomViewCapture::SampleDrawDisplayModeToDC( 
  CRhinoObjectIterator& it,
  CDC* pDC, 
  int width,
  int height,
  const ON_3dmView& view_info,
  CDisplayPipelineAttributes& da
  ) const
{
  if (0 == pDC)
    return false;

  CRhinoViewport vp;
  vp.SetView(view_info);  
  vp.SetScreenSize(width, height);
  vp.SetDisplayMode(view_info.m_display_mode);

  CRhinoDisplayPipeline_OGL dp(pDC, vp);
  return dp.DrawToDC(pDC, width, height, &da);      
}

#pragma endregion

//
// END SampleCustomViewCapture command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
