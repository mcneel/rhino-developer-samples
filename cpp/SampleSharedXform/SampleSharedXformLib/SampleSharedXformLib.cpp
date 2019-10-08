#include "StdAfx.h"
#include "SampleSharedXformLib.h"
#include "ON_XformTable.h"

// ON_XformTable C function interface

DLL_C_FUNCTION
int ON_XformTable_Count()
{
  return TheXformTable().Count();
}

DLL_C_FUNCTION
bool ON_XformTable_Contains(ON_UUID uuid)
{
  return TheXformTable().Contains(uuid);
}

DLL_C_FUNCTION
bool ON_XformTable_Add(ON_UUID uuid, const ON_SimpleArray<ON_Xform>* ptr)
{
  bool rc = false;
  if (ptr)
    rc = TheXformTable().Add(uuid, *ptr);
  return rc;
}

DLL_C_FUNCTION
bool ON_XformTable_Update(ON_UUID uuid, const ON_SimpleArray<ON_Xform>* ptr)
{
  bool rc = false;
  if (ptr)
    rc = TheXformTable().Update(uuid, *ptr);
  return rc;
}

DLL_C_FUNCTION
bool ON_XformTable_Remove(ON_UUID uuid)
{
  return TheXformTable().Remove(uuid);
}

DLL_C_FUNCTION
int ON_XformTable_Ids(ON_SimpleArray<ON_UUID>* uuids)
{
  int rc = 0;
  if (uuids)
    rc = TheXformTable().GetIds(*uuids);
  return rc;
}

DLL_C_FUNCTION
unsigned int ON_XformTable_RuntimeSerialNumber(ON_UUID uuid)
{
  return TheXformTable().GetRuntimeSerialNumber(uuid);
}

DLL_C_FUNCTION
int ON_XformTable_Transforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>* ptr)
{
  int rc = 0;
  if (ptr)
    rc = TheXformTable().GetTransforms(uuid, *ptr);
  return rc;
}

DLL_C_FUNCTION
void ON_XformTable_Empty()
{
  TheXformTable().Empty();
}

// ON_SimpleArray<ON_Xform> C interface

DLL_C_FUNCTION ON_SimpleArray<ON_Xform>* ON_SimpleArrayXform_New()
{
  return new ON_SimpleArray<ON_Xform>();
}

DLL_C_FUNCTION void ON_SimpleArrayXform_CopyValues(const ON_SimpleArray<ON_Xform>* ptr, ON_Xform* vals)
{
  if (ptr && vals)
  {
    int count = ptr->Count();
    if (count > 0)
    {
      const ON_Xform* source = ptr->Array();
      ::memcpy(vals, source, count * sizeof(ON_Xform));
    }
  }
}

DLL_C_FUNCTION void ON_SimpleArrayXform_Append(ON_SimpleArray<ON_Xform>* ptr, const ON_Xform* val)
{
  if (ptr && val)
    ptr->Append(*val);
}

DLL_C_FUNCTION int ON_SimpleArrayXform_Count(const ON_SimpleArray<ON_Xform>* ptr)
{
  int rc = 0;
  if (ptr)
    rc = ptr->Count();
  return rc;
}

DLL_C_FUNCTION void ON_SimpleArrayXform_Get(ON_SimpleArray<ON_Xform>* ptr, int index, ON_Xform* val)
{
  if (ptr && val && 0 <= index && index < ptr->Count())
    *val = (*ptr)[index];
  else
    *val = ON_Xform::Unset;
}

DLL_C_FUNCTION void ON_SimpleArrayXform_Delete(ON_SimpleArray<ON_Xform>* ptr)
{
  if (ptr)
    delete ptr;
}

// CSampleSharedXformTable class definition

int CSampleSharedXformTable::Count()
{
  return ON_XformTable_Count();
}

bool CSampleSharedXformTable::Contains(ON_UUID uuid)
{
  return ON_XformTable_Contains(uuid);
}

int CSampleSharedXformTable::Ids(ON_SimpleArray<ON_UUID>& uuids)
{
  return ON_XformTable_Ids(&uuids);
}

unsigned int CSampleSharedXformTable::RuntimeSerialNumber(ON_UUID uuid)
{
  return ON_XformTable_RuntimeSerialNumber(uuid);
}

int CSampleSharedXformTable::Transforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>& xforms)
{
  return ON_XformTable_Transforms(uuid, &xforms);
}
