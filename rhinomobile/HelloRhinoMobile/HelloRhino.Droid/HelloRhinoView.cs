//
// HelloRhinoView.cs
// HelloRhino.Droid
//
// Created by dan (dan@mcneel.com) on 7/11/2014
// Copyright 2014 Robert McNeel & Associates.  All rights reserved.
//
using System;
using Android.Views;
using Android.Content;
using Android.Opengl;
using Android.Widget;
using RhinoMobile.Display;


namespace HelloRhino.Droid
{
  public sealed class HelloRhinoView : GLSurfaceView
  {
    #region properties
    /// <value> Android GLSurface Views require a Renderer that conforms to GLSurfaceView.IRenderer. </value>
    private HelloRhinoAndroidRenderer AndroidRenderer { get; set; }

    /// <value> The ZoomDetector has a ZoomScaleListener which handles zooming of the Camera. </value>
    public ScaleGestureDetector ZoomDetector { get; set; }

    /// <value> The OrbitDollyDetector handles orbit and dolly (panning) gestures. </value>
    public OrbitDollyGestureDetector OrbitDollyDetector { get; set; }

    /// <value> TODO: Documentation has not yet been entered on this subject. </value>
    public GestureDetector DoubleTapDetector { get; set; }
    #endregion

    #region constructors and disposal
    public HelloRhinoView (Context context, Android.Util.IAttributeSet attrs) : base (context, attrs)
    {
      // Create an OpenGL ES 2.0 context.
      SetEGLContextClientVersion (2);

      // Set the Renderer for drawing on the GLSurfaceView
      AndroidRenderer = new HelloRhinoAndroidRenderer ();
      SetRenderer (AndroidRenderer);

      // The renderer only renders when the surface is created, or when requestRender() is called.
      this.RenderMode = Rendermode.WhenDirty;

      ZoomDetector = new ScaleGestureDetector(context, new ZoomScaleListener(AndroidRenderer));
      OrbitDollyDetector = new OrbitDollyGestureDetector ();
      DoubleTapDetector = new GestureDetector (new DoubleTapListener (AndroidRenderer));
      DoubleTapDetector.SetOnDoubleTapListener(new DoubleTapListener (AndroidRenderer));
    }
    #endregion

    #region Gesture Handling methods
    /// <summary> OnTouchEvent is called for every touch event on this view </summary>
    public override bool OnTouchEvent (MotionEvent e)
    {
      if (App.Manager.CurrentModel == null || !App.Manager.CurrentModel.IsReadyForRendering)
        return false;
        
      ZoomDetector.OnTouchEvent(e);
      OrbitDollyDetector.OnTouchEvent (e);
      DoubleTapDetector.OnTouchEvent (e);
      
      if (OrbitDollyDetector.State == GestureDetectorState.Changed) {
        if (OrbitDollyDetector.HasSingleFinger) {
          AndroidRenderer.Camera.GestureOrbit (AndroidRenderer.BufferSize, OrbitDollyDetector.AnchorLocation, OrbitDollyDetector.CurrentLocation);
          OrbitDollyDetector.AnchorLocation = OrbitDollyDetector.CurrentLocation;
        }

        if (OrbitDollyDetector.HasTwoFingers) {
          AndroidRenderer.Camera.LateralPan (OrbitDollyDetector.StartLocation, OrbitDollyDetector.MidpointLocation, false, false);
          OrbitDollyDetector.StartLocation = OrbitDollyDetector.MidpointLocation;
        }
      }

      AndroidRenderer.ZoomPoint = OrbitDollyDetector.MidpointLocation;

      RequestRender ();
  
      return true;
    }
    #endregion

  }
}

