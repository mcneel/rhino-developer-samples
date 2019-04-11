#pragma once

class CSampleDocumentUserData : public ON_UserData
{
  // Opennurbs classes that are saved in .3dm files require
// an ON_OBJECT_DECLARE call in their declaration.
  ON_OBJECT_DECLARE(CSampleDocumentUserData);

public:
  CSampleDocumentUserData();
  CSampleDocumentUserData(const CSampleDocumentUserData& src);
  ~CSampleDocumentUserData() = default;
  CSampleDocumentUserData& operator=(const CSampleDocumentUserData& src);

  // Returns uuid used to identify this type of user data.
  static ON_UUID Id();

  // Returns uuid used to identify the owning plug-in.
  static ON_UUID PlugInId();

  // Public interface

  int Count() const;
  ON_wString GetAt(int index) const;
  int Find(const wchar_t* str) const;
  int Add(const wchar_t* str);
  bool Delete(int index);
  bool Delete(const wchar_t* str);
  void Clear();

  // ON_UserData overrides

  bool GetDescription(ON_wString& description) override;
  bool Archive() const override;
  bool Read(ON_BinaryArchive& archive) override;
  bool Write(ON_BinaryArchive& archive) const override;

private:
  const int m_major_version = 1;
  const int m_minor_version = 0;
  ON_ClassArray<ON_wString> m_strings;
};