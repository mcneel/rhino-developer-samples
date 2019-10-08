#include "stdafx.h"
#include "ON_XformTable.h"

// ON_XformTable class definition

// The one and only ON_XformTable object
static ON_XformTable theXformTable;

ON_XformTable& TheXformTable()
{
  return theXformTable;
}

int ON_XformTable::Count() const
{
  size_t count = m_map.size();
  return static_cast<int>(count);
}

bool ON_XformTable::Contains(ON_UUID uuid) 
{
  ON_XformMap::iterator it = m_map.find(uuid);
  return it != m_map.end();
}

bool ON_XformTable::Add(ON_UUID uuid, const ON_SimpleArray<ON_Xform>& xforms)
{
  bool rc = false;
  ON_XformMap::iterator it = m_map.find(uuid);
  if (it == m_map.end())
  {
    ON_XformArray arr(xforms);
    m_map.insert(std::make_pair(uuid, arr));
    rc = true;
  }
  return rc;
}

bool ON_XformTable::Update(ON_UUID uuid, const ON_SimpleArray<ON_Xform>& xforms)
{
  bool rc = false;
  ON_XformMap::iterator it = m_map.find(uuid);
  if (it != m_map.end())
  {
    it->second = xforms;
    rc = true;
  }
  return rc;
}

bool ON_XformTable::Remove(ON_UUID uuid)
{
  bool rc = false;
  ON_XformMap::iterator it = m_map.find(uuid);
  if (it != m_map.end())
  {
    m_map.erase(it);
    rc = true;
  }
  return rc;
}

int ON_XformTable::GetIds(ON_SimpleArray<ON_UUID>& uuids)
{
  ON_XformMap::iterator it;
  for (ON_XformMap::iterator it = m_map.begin(); it != m_map.end(); it++)
  {
    ON_UUID id = it->first;
    uuids.Append(id);
  }
  return uuids.Count();
}

unsigned int ON_XformTable::GetRuntimeSerialNumber(ON_UUID uuid)
{
  unsigned int rc = 0;
  ON_XformMap::iterator it = m_map.find(uuid);
  if (it != m_map.end())
    rc = it->second.RuntimeSerialNumber();
  return rc;
}

int ON_XformTable::GetTransforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>& xforms)
{
  int rc = 0;
  ON_XformMap::iterator it = m_map.find(uuid);
  if (it != m_map.end())
  {
    xforms.Append(it->second.Count(), it->second.ToArray());
    rc = xforms.Count();
  }
  return rc;
}

void ON_XformTable::Empty()
{
  return m_map.clear();
}
