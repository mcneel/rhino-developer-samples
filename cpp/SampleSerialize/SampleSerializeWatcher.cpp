/////////////////////////////////////////////////////////////////////////////
// SampleSerializeWatcher.cpp

#include "StdAfx.h"
#include "SampleSerializeWatcher.h"
#include "SampleSerializePlugIn.h"
#include "SampleSerializeData.h"

void CSampleSerializeWatcher::OnCloseDocument(CRhinoDoc& doc)
{
  // When a document is closed, destroy the data.
  SampleSerializePlugIn().Data().Destroy();
}
