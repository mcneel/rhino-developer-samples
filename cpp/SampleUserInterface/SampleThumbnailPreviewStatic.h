#pragma once

// Owner-drawn static control
class CSampleThumbnailPreviewStatic : public CStatic
{
  DECLARE_DYNAMIC(CSampleThumbnailPreviewStatic)

public:
  CSampleThumbnailPreviewStatic();
  virtual ~CSampleThumbnailPreviewStatic();

  bool UpdateDib(const wchar_t* filename);
  void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
  DECLARE_MESSAGE_MAP()

private:
  CRhinoDib m_dib;
};
