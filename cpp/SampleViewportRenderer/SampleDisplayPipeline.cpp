#include "StdAfx.h"
#include "SampleDisplayPipeline.h"
#include "SampleViewportRendererPlugIn.h"

IMPLEMENT_DYNAMIC_PIPELINE(CSampleDisplayPipeline, CRhinoDisplayPipeline, L"SampleViewportRenderer", "64BAFCF4-FC30-43C2-969F-50FCFB160029", true, true);

bool CSampleDisplayPipeline::ShowFrameBuffer(HDC hDC)
{
  // Rhino is asking the pipeline to show it's image.
  // So we'll ask for the most recent rendered image and show that.
  CDC* pDestDC = CDC::FromHandle(hDC);
  if (nullptr == pDestDC)
    return false;

  // Get the destination - viewports device context - and it's size.
  CWnd* pDestWnd = pDestDC->GetWindow();
  if (nullptr == pDestWnd)
    return false;

  RECT destRect;
  pDestWnd->GetClientRect(&destRect);

  const int iDestWidth = destRect.right - destRect.left;
  const int iDestHeight = destRect.bottom - destRect.top;

  // Get the rendered image, it's device context and size.
  CRhinoDib* pDib = SampleViewportRendererPlugIn().LockRenderImage();
  if (0 == pDib)
    return false;

  CDC* pSrcDC = *pDib;
  if (nullptr != pSrcDC)
  {
    const int iSrcWidth = pDib->Width();
    const int iSrcHeight = pDib->Height();

    // Copy the rendered image into viewport using either BitBlt or StretchBlt depending
    // on if the device contexts' sizes are the same or not.
    if (iSrcWidth == iDestWidth && iSrcHeight == iDestHeight)
      pDestDC->BitBlt(0, 0, iSrcWidth, iSrcHeight, pSrcDC, 0, 0, SRCCOPY);
    else
      pDestDC->StretchBlt(0, 0, iDestWidth, iDestHeight, pSrcDC, 0, 0, iSrcWidth, iSrcHeight, SRCCOPY);
  }

  // Allow others to access the rendered image again
  SampleViewportRendererPlugIn().UnlockRenderImage(pDib);

  // Tell that the image has been shown in viewport
  SampleViewportRendererPlugIn().AcknowledgeRedraw();

  return true;
}