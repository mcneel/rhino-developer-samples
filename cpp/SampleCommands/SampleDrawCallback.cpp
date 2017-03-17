#include "stdafx.h"
#include "SampleDrawCallback.h"

const unsigned int DRAWCALLBACK_CHANNELS = CSupportChannels::SC_CALCBOUNDINGBOX |
                                           CSupportChannels::SC_DRAWBACKGROUND  |
                                           CSupportChannels::SC_POSTDRAWOBJECTS |
                                           CSupportChannels::SC_DRAWFOREGROUND  |
                                           CSupportChannels::SC_DRAWOVERLAY;

CSampleDrawCallback::CSampleDrawCallback()
  : CRhinoDisplayConduit(DRAWCALLBACK_CHANNELS)
{
}

CSampleDrawCallback::~CSampleDrawCallback(void)
{
}

ON_BoundingBox CSampleDrawCallback::BoundingBox(CRhinoViewport&, CRhinoDoc&)
{
  return ON_BoundingBox();
}

void CSampleDrawCallback::DrawBackground(CRhinoViewport&, CRhinoDoc&)
{
}

void CSampleDrawCallback::DrawMiddleground(CRhinoViewport&, CRhinoDoc&)
{
}

void CSampleDrawCallback::DrawForeground(CRhinoViewport&, CRhinoDoc&)
{
}

void CSampleDrawCallback::DrawDecorations(CRhinoView*, CDC*, CRhinoViewport&, CRhinoDoc&)
{
}

bool CSampleDrawCallback::DrawEverything(CRhinoView&, ON_BoundingBox, CDC*, CRhinoViewport&, CRhinoDoc&)
{
  return false;
}

bool CSampleDrawCallback::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool&)
{
  CRhinoViewport* pVP = dp.GetRhinoVP();
  if (nullptr == pVP)
    return true;

  CRhinoView* pView = dp.GetRhinoView();
  if (nullptr == pView)
    return true;

  CRhinoDoc* pDoc = pView->GetDocument();
  if (nullptr == pDoc)
    return true;

  if (ChannelSupported(nChannel & CSupportChannels::SC_CALCBOUNDINGBOX))
    m_pChannelAttrs->m_BoundingBox = BoundingBox(*pVP, *pDoc);
  else if (ChannelSupported(nChannel & CSupportChannels::SC_DRAWBACKGROUND))
    DrawBackground(*pVP, *pDoc);
  else if (ChannelSupported(nChannel & CSupportChannels::SC_POSTDRAWOBJECTS))
    DrawMiddleground(*pVP, *pDoc);
  else if (ChannelSupported(nChannel & CSupportChannels::SC_DRAWFOREGROUND))
    DrawForeground(*pVP, *pDoc);
  else if (ChannelSupported(nChannel & CSupportChannels::SC_DRAWOVERLAY))
    DrawDecorations(pView, nullptr, *pVP, *pDoc);

  return true;
}