#pragma once

/*
Description:
  If you need to add custom drawing code to Rhino, derive a class from
  CSampleDrawCallback and override the virtual function where you want
  to draw something.
  
  To enable, call:
    CSampleDrawCallback::Enable();
  When you are done, call:
    CSampleDrawCallback::Disable();

Remarks:
  The legacy CRhinoDrawCallback class, found in prior Rhino versions, has been
  deprecated. If you used this class extensively in you plug-in, you can use
  this class as a replacment.
*/
class CSampleDrawCallback : public CRhinoDisplayConduit
{
public:
  CSampleDrawCallback();
  virtual ~CSampleDrawCallback(void);

  /*
  Description:
    Called when a view is regenerated and can be used to specify a box you want
    to make sure is included in the view frustum. This is a good way to make
    sure temporary geometry drawn by DrawBackground, DrawMiddleground, and
    DrawForeground is inside of the near and far clipping planes.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  Returns:
    A bounding box
  */
  virtual ON_BoundingBox BoundingBox(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called when a view is regenerated and should be used for things like
    drawing background images into the "back" buffer.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  */
  virtual void DrawBackground(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called when a view is regenerated and should be used for things like drawing
    temporary geometry into the "back" buffer. Objects drawn in DrawMiddleground
    are depth clipped and depth buffered. If your objects are getting clipped
    and you don't want them to be clipped, the draw them in DrawForeground.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  Remarks:
    DrawMiddleground is a good place to draw shaded objects. If you want to draw
    wireframe objects, then you should do it in a DrawForeground override.
  */
  virtual void DrawMiddleground(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called when a view is regenerated and should be used for drawing "decorations"
    like the world axis icon into the "back" buffer. Wireframe objects drawn in
    DrawForeground are not depth clipped or depth buffered.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  Remarks:
    DrawMiddleground is a good place to draw shaded objects. If you want to draw
    wireframe objects, then you should do it in a DrawForeground override.
  */
  virtual void DrawForeground(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called right after the "back" buffer is blitted to the screen and should be
    used for drawing things like tracking lines or rubber bands.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  */
  virtual void DrawDecorations(CRhinoView* pVP, CDC* pDC, CRhinoViewport& vp, CRhinoDoc& doc);

  // Obsolete
  virtual bool DrawEverything(CRhinoView& view, ON_BoundingBox bbox, CDC* pDC, CRhinoViewport& vp, CRhinoDoc& doc);

private:
  // CRhinoDisplayConduit::ExecConduit override
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate) override;
};

