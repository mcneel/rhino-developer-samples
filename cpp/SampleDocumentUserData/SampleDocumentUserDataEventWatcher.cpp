/////////////////////////////////////////////////////////////////////////////
// SampleDocumentUserDataEventWatcher.cpp
//

#include "StdAfx.h"
#include "SampleDocumentUserDataPlugIn.h"
#include "SampleDocumentUserDataEventWatcher.h"

void CSampleDocumentUserDataEventWatcher::OnCloseDocument( CRhinoDoc& )
{
  CSampleDocumentUserDataPlugIn& plugin = SampleDocumentUserDataPlugIn();
  plugin.ClearStringTable();
}