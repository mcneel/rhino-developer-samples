#pragma once

// ON_XformArray class declaration

class ON_XformArray
{
public:
  ON_XformArray();
  ~ON_XformArray() = default;
  ON_XformArray(const ON_XformArray& src) = default;
  ON_XformArray& operator=(const ON_XformArray& src) = default;

  ON_XformArray(const ON_SimpleArray<ON_Xform>& src);
  ON_XformArray& operator=(const ON_SimpleArray<ON_Xform>& src);

  const ON_Xform& operator[](int) const;
  const ON_Xform& GetAt(int i) const;
  bool SetAt(int i, const ON_Xform& src);

  unsigned int RuntimeSerialNumber() const;

  int Count() const;
  void Append(const ON_Xform& src);
  void Append(int count, const ON_Xform* src);
  void Remove();
  void RemoveAt(int i);

  const ON_Xform* ToArray() const;

  void Empty();

private:
  ON_SimpleArray<ON_Xform> m_xforms;
  unsigned int m_serialno;
};

