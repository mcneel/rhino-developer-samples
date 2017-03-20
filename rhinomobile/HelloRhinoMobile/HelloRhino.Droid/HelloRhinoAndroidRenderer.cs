//
// HelloRhinoAndroidRenderer.cs
// HelloRhino.Droid
//
// Created by dan (dan@mcneel.com) on 7/11/2014
// Copyright 2014 Robert McNeel & Associates.  All rights reserved.
//
using System.Drawing;
using Android.Opengl;
using OpenTK.Graphics.ES20;
using Rhino.DocObjects;
using RhinoMobile.Display;


namespace HelloRhino.Droid
{
  public class HelloRhinoAndroidRenderer : Java.Lang.Object, GLSurfaceView.IRenderer
  {

    enum InitializationState
    {
      Uninitialized,
      Initialized,
      ErrorDuringInitialization
    }

    #region members
    private InitializationState m_initialized = InitializationState.Uninitialized;
    private float m_scaleFactor = 1.0f;
    #endregion

    #region properties
    /// <value> The renderer associated with this view. </value>
    public ES2Renderer Renderer { get; private set; }

    /// <value> The ViewportInfo for this view </value>
    public ViewportInfo Camera { get; set; }

    /// <value> The BufferSize is set during the OnSurfaceChanged event handler. </value>
    public Size BufferSize { get; private set; }

    /// <value> The current screen scale or zoom factor.  1.0 is the default 1-to-1 value. </value>
    public float ScaleFactor { get { return m_scaleFactor; } set { m_scaleFactor = value; } }

    /// <value> The screenpoint about which the Camera should zoom toward or away from. </value>
    public PointF ZoomPoint { get; set; }
    #endregion

    #region IRenderer implementation
    public void OnDrawFrame (Javax.Microedition.Khronos.Opengles.IGL10 gl)
    {
      if (InitializeOpenGL () == InitializationState.Initialized) {
        if (Camera == null && App.Manager.CurrentModel.IsReadyForRendering)
          SetupCamera ();

        Renderer.FastDrawing = true;
        Renderer.ClearView ();

        if (App.Manager.CurrentModel.IsReadyForRendering)
          Renderer.RenderModel (App.Manager.CurrentModel, Camera);
      }
    }

    public void OnSurfaceChanged (Javax.Microedition.Khronos.Opengles.IGL10 gl, int width, int height)
    {
      System.Diagnostics.Debug.WriteLine ("OnSurfaceChanged to {0}, {1}", width, height);
      BufferSize = new Size (width, height);
      ResizeCamera ();
    }

    public void OnSurfaceCreated (Javax.Microedition.Khronos.Opengles.IGL10 gl, Javax.Microedition.Khronos.Egl.EGLConfig config)
    {
      System.Diagnostics.Debug.WriteLine ("OnSurfaceCreated");
    }
    #endregion

    #region OpenGL Setup, TearDown and Utils
    /// <summary>
    /// Tell opengl that we want to use OpenGL ES2 capabilities.
    /// This must be done right before the frame buffer is created.
    /// </summary>
    InitializationState InitializeOpenGL()
    {
      if (m_initialized == InitializationState.Uninitialized) {
        Renderer = new ES2Renderer ();
        Renderer.AndroidContext = App.Manager.ApplicationContext;

        // Initialize and setup some start states...
        m_initialized = InitializationState.Initialized;

        GL.ClearDepth (0.0f);
        GL.DepthRange (0.0f, 1.0f);
        GL.Enable (EnableCap.DepthTest);
        GL.DepthFunc (DepthFunction.Equal);
        GL.DepthMask (true);

        GL.BlendFunc (BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcAlpha);
        GL.Disable (EnableCap.Dither);
        GL.Disable (EnableCap.CullFace);
      }

      return m_initialized;
    }
    #endregion

    #region Camera Setup methods
    /// <summary>
    /// Initializes the camera on load...
    /// </summary>
    public void SetupCamera ()
    {
      if (Camera == null)
        LoadCamera ();

      // Fix up viewport values
      var cameraDir = Camera.CameraDirection;
      cameraDir.Unitize ();
      Camera.SetCameraDirection (cameraDir);

      var cameraUp = Camera.CameraUp;
      cameraUp.Unitize ();
      Camera.SetCameraUp (cameraUp);

      Renderer.Viewport = Camera;

      GL.Viewport (0, 0, BufferSize.Width, BufferSize.Height);
    }

    /// <summary>
    /// Find the Perspective viewport in the 3dm file and sets up the default view.
    /// </summary>
    public void LoadCamera ()
    {
      if (App.Manager.CurrentModel == null)
        return;

      Camera = new ViewportInfo ();
      bool cameraIsInitialized = false;

      int viewCount = App.Manager.CurrentModel.ModelFile.Views.Count;

      // find first perspective viewport projection in file
      if (viewCount > 0) {
        foreach (var view in App.Manager.CurrentModel.ModelFile.Views) {
          if (view.Viewport.IsPerspectiveProjection) {
            cameraIsInitialized = true;
            Camera = view.Viewport;
            Camera.TargetPoint = view.Viewport.TargetPoint;
            Camera.SetScreenPort (0, BufferSize.Width, 0, BufferSize.Height, 1, 1000);
            Camera.FrustumAspect = Camera.ScreenPortAspect;
            Camera.SetFrustumNearFar (App.Manager.CurrentModel.BBox);
            break;
          }
        }
      }

      // If there isn't one, then cook up a viewport from scratch...
      if (!cameraIsInitialized) {
        Camera.SetScreenPort (0, BufferSize.Width, 0, BufferSize.Height, 1, 1000);
        Camera.TargetPoint = new Rhino.Geometry.Point3d (0, 0, 0);
        var plane = new Rhino.Geometry.Plane (Rhino.Geometry.Point3d.Origin, new Rhino.Geometry.Vector3d (-1, -1, -1));
        Camera.SetCameraLocation(new Rhino.Geometry.Point3d (10, 10, 10));
        var dir = new Rhino.Geometry.Vector3d (-1, -1, -1);
        dir.Unitize ();
        Camera.SetCameraDirection (dir);
        Camera.SetCameraUp (plane.YAxis);
        Camera.SetFrustum (-1, 1, -1, 1, 0.1, 1000);
        Camera.FrustumAspect = Camera.ScreenPortAspect;
        Camera.IsPerspectiveProjection = true;
        Camera.Camera35mmLensLength = 50;
        if (App.Manager.CurrentModel != null) {
          if (App.Manager.CurrentModel.AllMeshes != null)
            Camera.DollyExtents (App.Manager.CurrentModel.AllMeshes, 1.0);
        }
      }

    }

    /// <summary>
    /// Resizes the camera to the current BufferSize
    /// </summary>
    public void ResizeCamera ()
    {
      if (Camera == null)
        return;
        
      System.Diagnostics.Debug.WriteLine ("Resizing Camera to {0}, {1}", BufferSize.Width, BufferSize.Height);

      Camera.SetScreenPort (0, BufferSize.Width, 0, BufferSize.Height, 1, 1000);
      Camera.FrustumAspect = Camera.ScreenPortAspect;

      Renderer.Viewport = Camera;

      GL.Viewport (0, 0, BufferSize.Width, BufferSize.Height);
    }
    #endregion

  }
}