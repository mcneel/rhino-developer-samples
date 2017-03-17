#include "StdAfx.h"
#include "SampleLocalizationHelpers.h"

bool ON_LoadString(unsigned int resource_id, ON_String& str)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CString cstr;
  cstr.LoadString(resource_id);
  if (!str.IsEmpty())
  {
    str = cstr;
    return true;
  }

  return false;
}

bool ON_LoadString(unsigned int resource_id, ON_wString& str)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CString cstr;
  cstr.LoadString(resource_id);
  if (!cstr.IsEmpty())
  {
    str = cstr;
    return true;
  }

  return false;
}
