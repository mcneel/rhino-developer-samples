/////////////////////////////////////////////////////////////////////////////
// SampleSerializeData.h
//

#pragma once

class CSampleSerializeData
{
public:
  CSampleSerializeData();
  ~CSampleSerializeData();

  int Integer() const;
  void SetInteger(int value);

  double Double() const;
  void SetDouble(double value);

  const wchar_t* String() const;
  void SetString(const wchar_t* value);

  int Points(ON_SimpleArray<ON_3dPoint>& points);
  void SetPoints(const ON_SimpleArray<ON_3dPoint>& points);

  const ON_Mesh* Mesh() const;
  void SetMesh(ON_Mesh* mesh); // ~CSampleSerializeData() will delete this mesh

  const ON_Brep* Brep() const;
  void SetBrep(ON_Brep* brep); // ~CSampleSerializeData() will delete this brep

  const ON_Curve* Curve() const;
  void SetCurve(ON_Curve* curve); // ~CSampleSerializeData() will delete this curve

  int Curves(ON_SimpleArray<const ON_Curve*>& curves);
  void SetCurves(const ON_SimpleArray<ON_Curve*>& curves); // ~CSampleSerializeData() will delete these curves

	bool Read(ON_BinaryArchive& archive);
	bool Write(ON_BinaryArchive& archive);

  void Destroy();

protected:
  int m_integer;
  double m_double;
  ON_wString m_string;
  ON_3dPointArray m_point_list;
  ON_Mesh* m_mesh;
  ON_Brep* m_brep;
  ON_Curve* m_curve;
  ON_CurveArray m_curve_list;

private:
  // No copy constructor
  CSampleSerializeData(const CSampleSerializeData& src);
  // No operator=
  CSampleSerializeData& operator=(const CSampleSerializeData& src);
};

