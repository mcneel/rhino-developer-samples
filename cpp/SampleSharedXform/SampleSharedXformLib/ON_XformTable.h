#pragma once

#include "ON_XformArray.h"

// ON_XformTable class declaration

class ON_XformTable
{
public:
  ON_XformTable() = default;

  int Count() const;
  bool Contains(ON_UUID uuid);
  bool Add(ON_UUID uuid, const ON_SimpleArray<ON_Xform>& xforms);
  bool Update(ON_UUID uuid, const ON_SimpleArray<ON_Xform>& xforms);
  bool Remove(ON_UUID uuid);

  int GetIds(ON_SimpleArray<ON_UUID>& uuids);
  unsigned int GetRuntimeSerialNumber(ON_UUID uuid);
  int GetTransforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>& xforms);

  void Empty();

private:
  struct compare_uuids
  {
    bool operator()(const ON_UUID& a, const ON_UUID& b) const
    {
      return memcmp(&a, &b, sizeof(ON_UUID)) < 0;
    }
  };

  typedef std::map<ON_UUID, ON_XformArray, compare_uuids> ON_XformMap;
  ON_XformMap m_map;

private:
  ON_XformTable(const ON_XformTable&) = delete;
  ON_XformTable& operator=(const ON_XformTable&) = delete;
};

// Returns the one and only ON_XformTable object
ON_XformTable& TheXformTable();