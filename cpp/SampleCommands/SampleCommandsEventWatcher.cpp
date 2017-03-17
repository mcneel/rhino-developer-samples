#include "StdAfx.h"
#include "SampleCommandsPlugIn.h"
#include "SampleCommandsEventWatcher.h"

void CSampleCommandsEventWatcher::OnCloseDocument(CRhinoDoc& doc)
{
  UNREFERENCED_PARAMETER(doc);
  SampleCommandsPlugIn().ClearStringTable();
}