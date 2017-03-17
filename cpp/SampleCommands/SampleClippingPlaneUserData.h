#pragma once

class CSampleClippingPlaneUserData : public ON_UserData
{
  // Opennurbs classes that are saved in .3dm files require
  // an ON_OBJECT_DECLARE call in their declaration.
  ON_OBJECT_DECLARE(CSampleClippingPlaneUserData);

public:
  CSampleClippingPlaneUserData();
  ~CSampleClippingPlaneUserData();

  /*
  NOTE WELL:
    Because the members of this class are class are types
    that have fully functional copy constructors and
    operator=s, it is NOT necessary to provide an explicit
    a copy constructor and operator=.  In fact, it would
    be best to use the ones C++ generates.
    They are included in this example to demonstrate the
    correct way to implement a copy constructor and
    operator= because this is a task that trips up
    many people that attempt to implement them.
  */
  CSampleClippingPlaneUserData(const CSampleClippingPlaneUserData& src);
  CSampleClippingPlaneUserData& operator=(const CSampleClippingPlaneUserData& src);

  /*
  Returns:
    Uuid used to identify this type of user data.
    This is the value saved in m_userdata_uuid and
    passed to ON_Object::GetUserData().
  */
  static ON_UUID Id();

  static ON_UUID PlugInId();

  // override virtual ON_UserData::GetDescription()
  BOOL GetDescription(ON_wString& description);

  // override virtual ON_UserData::Archive()
  BOOL Archive() const;

  // override virtual ON_UserData::Read()
  BOOL Read(ON_BinaryArchive& archive);

  // override virtual ON_UserData::Write()
  BOOL Write(ON_BinaryArchive& archive) const;

public:
  // Static helper functions
  static bool HasUserData(const CRhinoObject* object);
  static bool AddUserData(CRhinoDoc& doc, const CRhinoObject* object, const ON_SimpleArray<ON_UUID>& viewport_ids);
  static bool SetUserData(CRhinoDoc& doc, const CRhinoObject* object, const ON_SimpleArray<ON_UUID>& viewport_ids);
  static bool GetUserData(const CRhinoObject* object, ON_SimpleArray<ON_UUID>& viewport_ids);
  static bool RemoveUserData(CRhinoDoc& doc, const CRhinoObject* object);

  static int StandardViewportIds(CRhinoDoc& doc, ON_SimpleArray<UUID>& viewport_ids);

public:
  ON_SimpleArray<ON_UUID> m_viewport_ids;
};