//
// HelloRhinoViewController.cs
// HelloRhino.iOS
//
// Created by dan (dan@mcneel.com) on 9/09/2015
// Copyright 2013 Robert McNeel & Associates.  All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
using System;
using System.Timers;

using Foundation;
using UIKit;
using GLKit;
using OpenGLES;

using RhinoMobile.Model;
using RhinoMobile.Display;
using Rhino.DocObjects;
using OpenTK.Graphics.ES20;
using ObjCRuntime;
using CoreAnimation;
using CoreGraphics;
using System.Drawing;


namespace HelloRhino.iOS
{
	[Register ("HelloRhinoViewController")]
	public partial class HelloRhinoViewController : UIViewController, IDisposable
	{
		#region members
		private int m_activeGesturesCount;
    private bool m_fastDrawing;
		#endregion

		#region properties
		/// <value> This the EAGLContext controlled by the GLKViewController. </value>
		protected EAGLContext Context { get; set; }

		/// <value> Returns the View cast as a GLKView </value>
		new GLKView View { get  { return (GLKView)base.View; }  }

		/// <value> The renderer associated with this view. </value>
		public ES2Renderer Renderer { get; private set; }

		/// <value> The ViewportInfo for this view </value>
		protected ViewportInfo Camera { get; set; }

		/// <value> OrbitDollyGestureRecognizer listens for single and two-finger pan-like events. </value>
		protected OrbitDollyGestureRecognizer OrbitDollyRecognizer { get; private set; }

		/// <value> ZoomRecognizer listens for pinch gestures </value>
		protected UIPinchGestureRecognizer ZoomRecognizer { get; private set; }

		/// <value> The double-tap gesture recognizer listens for double-taps. </value>
		protected UITapGestureRecognizer DoubleTapGestureRecognizer { get; private set; }

		/// <value> The initial starting view position. </value>
		protected ViewportInfo InitialPosition { get; set; }

		/// <value> The last position of the Viewport before any other change. </value>
		protected ViewportInfo LastPosition { get; set; }

		/// <value> The Viewport to restore from - for tweening. </value>
		protected ViewportInfo RestoreViewStartViewport { get; set; }

		/// <value> The Viewport to restore to - for tweening </value>
		protected ViewportInfo RestoreViewFinishViewport { get; set; }

		/// <value> The startTime of the restore view action. </value>
		protected DateTime RestoreViewStartTime { get; set; }

		/// <value> The total time of the restore view action. </value>
		protected TimeSpan RestoreViewTotalTime { get; set; }

		/// <value> True if the Camera is at the default, initial camera position. </value>
		public bool CameraIsAtInitialPosition { get; private set; }

		/// <value> True if the Camera should start the restore to initial position. </value>
		public bool ShouldStartRestoreToInitialPosition { get; private set; }

		/// <value> The AnimationTimer for creating temporary animation loops. </value>
		private NSTimer AnimationTimer { get; set; }

		/// <value> True if the Camera is currently being animated back to the last position. </value>
		public bool IsInAnimatedRestoreView { get; private set; }

		/// <value> Determines which mode we are in, fast or high-quality drawing. </value>
		private bool FastDrawing { 
			get { return m_fastDrawing; }

			set {
				m_fastDrawing = value;
				View.DrawableMultisample = m_fastDrawing ? GLKViewDrawableMultisample.None : GLKViewDrawableMultisample.Sample4x;
			}
		}
			
		/// <value> True is an iPhone or iPodTouch, False if it's an iPad. </value>
		private static bool UserInterfaceIdiomIsPhone {
			get { return UIDevice.CurrentDevice.UserInterfaceIdiom == UIUserInterfaceIdiom.Phone; }
		}

    /// <value> A count of the active number of gesture recognizers currently firing. </value>
		private int ActiveGesturesCount {
			get {
				return m_activeGesturesCount;
			}

			set {
				m_activeGesturesCount = value;

				if (m_activeGesturesCount == 0)
					RedrawDetailed ();
			}
		}
		#endregion

		#region GestureRecognizerDelegate
		class GestureDelegate : UIGestureRecognizerDelegate
		{
			/// <summary>
			/// GestureDelegate is stubbed for custom delegate handling
			/// </summary>
			public GestureDelegate (HelloRhinoViewController controller)
			{
				//stubbed for delegate handling...for example:
				//m_controller = controller;
			}

			/// <summary>
			/// ShouldReceiveTouch is required for the UIGestureRecognizer protocol
			/// </summary>
			public override bool ShouldReceiveTouch(UIGestureRecognizer recognizer, UITouch touch)
			{
				return true;
			}

			/// <summary>
			/// Ensure that the pinch, pan and rotate gestures are all recognized simultaneously
			/// </summary>
			public override bool ShouldRecognizeSimultaneously (UIGestureRecognizer gestureRecognizer, UIGestureRecognizer otherGestureRecognizer)
			{
				// Orbit and Zoom 
				if ( (gestureRecognizer is UIPinchGestureRecognizer) && (otherGestureRecognizer is OrbitDollyGestureRecognizer) )
					return true;

				// Orbit and Double Tap
				if ((gestureRecognizer is OrbitDollyGestureRecognizer) && (otherGestureRecognizer is UITapGestureRecognizer))
					return true;

				return false;
			}
		}
		#endregion

		#region constructors and disposal
		public HelloRhinoViewController () : base (UserInterfaceIdiomIsPhone ? "HelloRhinoViewController_iPhone" : "HelloRhinoViewController_iPad", null)
		{

		}

		/// <summary>
		/// Passively reclaims unmanaged resources when the class user did not explicitly call Dispose().
		/// </summary>
		~ HelloRhinoViewController () { Dispose (false); }

		/// <summary>
		/// Actively reclaims unmanaged resources that this instance uses.
		/// </summary>
		public new void Dispose()
		{
			try {
				Dispose(true);
				GC.SuppressFinalize(this);
			}
			finally {
				base.Dispose (true);
				NSNotificationCenter.DefaultCenter.RemoveObserver (this);
			}
		}

		/// <summary>
		/// <para>This method is called with argument true when class user calls Dispose(), while with argument false when
		/// the Garbage Collector invokes the finalizer, or Finalize() method.</para>
		/// <para>You must reclaim all used unmanaged resources in both cases, and can use this chance to call Dispose on disposable fields if the argument is true.</para>
		/// </summary>
		/// <param name="disposing">true if the call comes from the Dispose() method; false if it comes from the Garbage Collector finalizer.</param>
		private new void Dispose (bool disposing)
		{
			// Free unmanaged resources...

			// Free managed resources...but only if called from Dispose
			// (If called from Finalize then the objects might not exist anymore)
			if (disposing) {

				ReleaseDesignerOutlets ();

				TearDownGL ();

				View.Dispose ();

				if (EAGLContext.CurrentContext == Context)
					EAGLContext.SetCurrentContext (null);

				NSNotificationCenter.DefaultCenter.RemoveObserver (this);
			}	
		}

		public override void DidReceiveMemoryWarning ()
		{
			// Releases the view if it doesn't have a superview.
			base.DidReceiveMemoryWarning ();

			if (IsViewLoaded && View.Window == null) {
				View.Dispose ();

				TearDownGL ();

				if (EAGLContext.CurrentContext == Context)
					EAGLContext.SetCurrentContext (null);

				Context.Dispose ();
			}

			// Release any cached data, images, etc that aren't in use.
		}
		#endregion

		#region View Lifecycle methods
		/// <summary>
		/// ViewDidLoad is called by CocoaTouch when the view had loaded.
		/// </summary>
		public override void ViewDidLoad ()
		{
			base.ViewDidLoad ();

			SetupGL ();

			SetupGestureRecognizers ();

			// Show the initialization view...
			InitPrepView.Hidden = true;
			WarningSymbol.Hidden = true;
			ProgressBar.Progress = 0.0f;

			// Subscribe to mesh prep events in the model...
			App.Manager.CurrentModel.MeshPrep += new MeshPreparationHandler (ObserveMeshPrep);
		}

		/// <summary>
		/// ViewDidAppear is called by CocoaTouch just after the view did appear.
		/// </summary>
		public override void ViewDidAppear (bool animated)
		{
			base.ViewDidAppear (animated);

			InitPrepView.Hidden = false;

			// Tell the model to prepare itself for display...
			App.Manager.CurrentModel.Prepare ();
		}

		/// <summary>
		/// ViewWillDisappear is called by CocoaTouch just before the view will disappear.
		/// </summary>
		public override void ViewWillDisappear (bool animated)
		{
			base.ViewWillDisappear (animated);

			App.Manager.CurrentModel.MeshPrep -= ObserveMeshPrep;
		}
		#endregion

		#region Device Rotation methods
		/// <summary>
		/// This view should autorotate to all interface orientations.
		/// </summary>
		public override bool ShouldAutorotate ()
		{
			return true;
		}

		/// <summary>
		/// Called just before the View will rotate, giving us an opportunity to resize the view based on new bounds
		/// </summary>
		public override void WillRotate (UIInterfaceOrientation toInterfaceOrientation, double duration)
		{
			base.WillRotate (toInterfaceOrientation, duration);

			// If it not coming from LandscapeLeft or Right and going to landscape left or right...
			bool currentOrientationIsLandscape = (InterfaceOrientation == UIInterfaceOrientation.LandscapeLeft || InterfaceOrientation == UIInterfaceOrientation.LandscapeRight);
			bool toOrientationIsLandscape = (toInterfaceOrientation == UIInterfaceOrientation.LandscapeLeft || toInterfaceOrientation == UIInterfaceOrientation.LandscapeRight);

			if (!(currentOrientationIsLandscape && toOrientationIsLandscape)) {
				FastDrawing = true;
				ResizeCamera ();
				View.SetNeedsDisplay ();
			}

		}

		public override void DidRotate (UIInterfaceOrientation fromInterfaceOrientation)
		{
			base.DidRotate (fromInterfaceOrientation);

			// If it not coming from LandscapeLeft or Right and going to landscape left or right...
			bool currentOrientationIsLandscape = (InterfaceOrientation == UIInterfaceOrientation.LandscapeLeft || InterfaceOrientation == UIInterfaceOrientation.LandscapeRight);
			bool toOrientationIsLandscape = (fromInterfaceOrientation == UIInterfaceOrientation.LandscapeLeft || fromInterfaceOrientation == UIInterfaceOrientation.LandscapeRight);

			if (!(currentOrientationIsLandscape && toOrientationIsLandscape)) {
				RedrawDetailed ();
			}
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

			// save initial viewport settings for restoreView
			InitialPosition = new ViewportInfo (Camera);
			LastPosition = new ViewportInfo (Camera);
			CameraIsAtInitialPosition = true;

			GL.Viewport (0, 0, (int)View.Bounds.Size.Width, (int)View.Bounds.Size.Height);
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
						Camera.SetScreenPort (0, (int)View.Bounds.Size.Width, 0, (int)View.Bounds.Size.Height, 1, 1000);
						Camera.FrustumAspect = Camera.ScreenPortAspect;
						Camera.SetFrustumNearFar (App.Manager.CurrentModel.BBox);
						break;
					}
				}
			}

			// If there isn't one, then cook up a viewport from scratch...
			if (!cameraIsInitialized) {
				Camera.SetScreenPort (0, (int)View.Bounds.Size.Width, 0, (int)View.Bounds.Size.Height, 1, 1000);
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
		/// Resizes the camera (to be called just before rotation as bounds width and height are swapped)
		/// </summary>
		public void ResizeCamera ()
		{
			if (Camera == null)
				return;

			var newRectangle = View.Bounds;
			Camera.SetScreenPort (0, (int)(newRectangle.Height - 1), 0, (int)(newRectangle.Width - 1), 0, 0);

			double newPortAspect = (newRectangle.Size.Height / newRectangle.Size.Width);
			Camera.FrustumAspect = newPortAspect;

			if (App.Manager.CurrentModel != null)
				SetFrustum (Camera, App.Manager.CurrentModel.BBox);

			// Fix up viewport values
			var cameraDir = Camera.CameraDirection;
			cameraDir.Unitize ();
			Camera.SetCameraDirection (cameraDir);

			var cameraUp = Camera.CameraUp;
			cameraUp.Unitize ();
			Camera.SetCameraUp (cameraUp);

			Renderer.Viewport = Camera;

			GL.Viewport (0, 0, (int)newRectangle.Height, (int)newRectangle.Width);
		}

		/// <summary>
		/// Dynamically set the frustum based on the clipping
		/// </summary>
		protected void SetFrustum (ViewportInfo viewport, Rhino.Geometry.BoundingBox bBox)
		{
			ClippingInfo clipping = new ClippingInfo();
			clipping.bbox = bBox;
			if (ClippingPlanes.CalcClippingPlanes (viewport, clipping))
				ClippingPlanes.SetupFrustum (viewport, clipping);
		}
		#endregion

		#region OpenGL Setup, TearDown and Utils
		/// <summary>
		/// Sets up and initializes OpenGL ES
		/// </summary>
		void SetupGL()
		{
			// Setup the Rendering Context...
			Context = new EAGLContext (EAGLRenderingAPI.OpenGLES2);
			if (Context == null)
				Console.WriteLine ("Failed to create ES context");

			// Initialize the Renderer...
			Renderer = new ES2Renderer ();

			// Initialize the View...
			View.Context = Context;
			View.EnableSetNeedsDisplay = true;
			View.ClearsContextBeforeDrawing = false;
			View.DrawableDepthFormat = GLKViewDrawableDepthFormat.Format24;
			View.DrawableMultisample = GLKViewDrawableMultisample.None;
			View.DrawInRect += Draw;

			EAGLContext.SetCurrentContext (Context);

			GL.ClearDepth (0.0f);
			GL.DepthRange (0.0f, 1.0f);
			GL.Enable (EnableCap.DepthTest);
			GL.DepthFunc (DepthFunction.Equal);
			GL.DepthMask (true);

			GL.BlendFunc (BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcAlpha);
			GL.Disable (EnableCap.Dither);
			GL.Disable (EnableCap.CullFace);
		}

		/// <summary>
		/// Destroys all OpenGL ES resources associated with this view and the model being drawn.
		/// </summary>
		protected void TearDownGL ()
		{
			EAGLContext.SetCurrentContext (Context);

			// Delete all the VBOs in the current model...
			if (App.Manager.CurrentModel != null) {
				if (App.Manager.CurrentModel.DisplayObjects != null) {
					// Destroy all buffers on all meshes
					foreach (var obj in App.Manager.CurrentModel.DisplayObjects) {
						var mesh = obj as DisplayMesh;
						if (mesh != null) {
							if (mesh.VertexBufferHandle != Globals.UNSET_HANDLE) {
								uint vbo = mesh.VertexBufferHandle;
								GL.DeleteBuffers (1, ref vbo);
								mesh.VertexBufferHandle = Globals.UNSET_HANDLE;
							}
							if (mesh.IndexBufferHandle != Globals.UNSET_HANDLE) {
								uint ibo = mesh.IndexBufferHandle;
								GL.DeleteBuffers (1, ref ibo);
								mesh.IndexBufferHandle = Globals.UNSET_HANDLE;
							}
						}
					}

					// Destroy all buffers on all transparent meshes
					foreach (var obj in App.Manager.CurrentModel.TransparentObjects) {
						var mesh = obj as DisplayMesh;
						if (mesh != null) {
							if (mesh.VertexBufferHandle != Globals.UNSET_HANDLE) {
								uint vbo = mesh.VertexBufferHandle;
								GL.DeleteBuffers (1, ref vbo);
								mesh.VertexBufferHandle = Globals.UNSET_HANDLE;
							}
							if (mesh.IndexBufferHandle != Globals.UNSET_HANDLE) {
								uint ibo = mesh.IndexBufferHandle;
								GL.DeleteBuffers (1, ref ibo);
								mesh.IndexBufferHandle = Globals.UNSET_HANDLE;
							}
						}
					}
				}
			}

			Renderer.Dispose ();
		}

		/// <summary>
		/// CaptureImage saves an image of the OpenGL view to the photo album.
		/// </summary>
		public bool CaptureImage()
		{
			bool didSaveImageToPhotoAlbum = false;
		
			if (View != null) {
			
				UIImage snapshot = View.Snapshot ();
				snapshot.SaveToPhotosAlbum((img, err) => {
					if(err != null) {
						Console.WriteLine("error saving image: {0}", err);
						didSaveImageToPhotoAlbum = false;
					} else {
						Console.WriteLine ("image saved to photo album");
						didSaveImageToPhotoAlbum = true;
					}
				});
			}

			return didSaveImageToPhotoAlbum;
		}
		#endregion

		#region Draw
		void Draw (object sender, GLKViewDrawEventArgs args)
		{
			Renderer.FastDrawing = true;
			Renderer.ClearView ();
			Renderer.RenderModel (App.Manager.CurrentModel, Camera);
		}
		#endregion

		#region Model Initialization Events
		private void ObserveMeshPrep (RMModel model, MeshPreparationProgress progress)
		{
			// Success
			if (progress.PreparationDidSucceed) {

				InitPrepView.InvokeOnMainThread (delegate {
					InitPrepView.Hidden = true;
					SetupCamera ();
					EnableAllGestureRecognizers ();
					App.Manager.CurrentModel.MeshPrep -= new MeshPreparationHandler (ObserveMeshPrep);
					PerformSelector (new Selector ("RedrawDetailed"), null, 0.25);
				});

			}

			// Still working
			if (!progress.PreparationDidSucceed && progress.FailException == null) {
				ProgressBar.BeginInvokeOnMainThread (delegate {
					ProgressBar.SetProgress ((float)progress.MeshProgress, true);
				});
			}

			// Failure or Cancellation
			if (progress.FailException != null) {
				ProgressBar.BeginInvokeOnMainThread (delegate {
					ProgressBar.Hidden = true;
				});

				WarningSymbol.BeginInvokeOnMainThread (delegate {
					WarningSymbol.Hidden = false;
				});

				InitializingLabel.BeginInvokeOnMainThread (delegate {
					InitializingLabel.Text = progress.FailException.Message;
				});
			}
		}

		partial void CancelPrep (Foundation.NSObject sender)
		{
			App.Manager.CurrentModel.CancelModelPreparation();
		}
		#endregion

		#region Gesture Handling methods
		/// <summary>
		/// This view's owner (ModelView) receives the: ShouldRecognizeSimultaneouslyWithGestureRecognizer
		/// callback for each of its delegates.  
		/// </summary>
		public void SetupGestureRecognizers ()
		{
			// Pinch - Zoom
			ZoomRecognizer = new UIPinchGestureRecognizer (this, new Selector ("ZoomCameraWithGesture:"));
			ZoomRecognizer.Enabled = false;
			ZoomRecognizer.Delegate = new GestureDelegate (this);
			View.AddGestureRecognizer (ZoomRecognizer);

			// Orbit & Dolly
			OrbitDollyRecognizer = new OrbitDollyGestureRecognizer ();
			OrbitDollyRecognizer.AddTarget (this, new Selector ("OrbitDollyCameraWithGesture:"));
			OrbitDollyRecognizer.MaximumNumberOfTouches = 2;
			OrbitDollyRecognizer.Enabled = false;
			OrbitDollyRecognizer.Delegate = new GestureDelegate (this);
			View.AddGestureRecognizer (OrbitDollyRecognizer);

			// Zoom Extents / Restore Last View
			DoubleTapGestureRecognizer = new UITapGestureRecognizer ();
			DoubleTapGestureRecognizer.AddTarget (this, new Selector ("ZoomExtentsWithGesture:"));
			DoubleTapGestureRecognizer.NumberOfTapsRequired = 2;
			DoubleTapGestureRecognizer.Enabled = false;
			View.AddGestureRecognizer (DoubleTapGestureRecognizer);
		}

		public void EnableAllGestureRecognizers ()
		{
			foreach (UIGestureRecognizer recognizer in View.GestureRecognizers)
				recognizer.Enabled = true;
		}

		public void DisableAllGestureRecognizers ()
		{
			foreach (UIGestureRecognizer recognizer in View.GestureRecognizers)
				recognizer.Enabled = false;
		}

		/// <summary>
		/// ZoomExtentsWithGesture is called when a DoubleTapGesture is detected.
		/// </summary>
		[Export("ZoomExtentsWithGesture:")]
		private void ZoomExtentsWithGesture (UIGestureRecognizer gesture)
		{
			if (Camera == null)
				return;

			if (gesture.State == UIGestureRecognizerState.Ended) {

				ShouldStartRestoreToInitialPosition = CameraIsAtInitialPosition;

				ViewportInfo targetPosition = new ViewportInfo();

				if (ShouldStartRestoreToInitialPosition) {
					// animate from current position (which is initial position) back to last position
					targetPosition = LastPosition;
				} else {
					// animate from current position to initial position
					targetPosition = InitialPosition;
					LastPosition = new ViewportInfo(Camera);
				}

				StartRestoreViewTo (targetPosition);
			}
		}

		/// <summary>
		/// ZoomCameraWithGesture is called in response to ZoomRecognizer events.
		/// </summary>
		[Export("ZoomCameraWithGesture:")]
		private void ZoomCameraWithGesture (UIPinchGestureRecognizer gesture)
		{
			if (Camera == null)
				return;

			if (gesture.State == UIGestureRecognizerState.Began)
				ActiveGesturesCount++;

			if (gesture.State == UIGestureRecognizerState.Changed) {
				if (gesture.NumberOfTouches > 1) {
					FastDrawing = true;
					CGPoint zoomPoint = OrbitDollyRecognizer.MidpointLocation;
          Camera.Magnify ((Size)View.Bounds.Size.ToRoundedCGSize (), gesture.Scale, 0, (PointF)zoomPoint);
          gesture.Scale = 1.0f;
				}
					
				View.SetNeedsDisplay ();
			}

			if (gesture.State == UIGestureRecognizerState.Ended || gesture.State == UIGestureRecognizerState.Cancelled)
				ActiveGesturesCount--;
		}

		/// <summary>
		/// OrbitDollyCameraWithGesture is called in response to OrbitDollyRecognizer events.
		/// </summary>
		[Export("OrbitDollyCameraWithGesture:")]
		private void OrbitDollyCameraWithGesture (OrbitDollyGestureRecognizer gesture)
		{
			if (Camera == null)
				return;

			if (gesture.State == UIGestureRecognizerState.Began)
				ActiveGesturesCount++;

			if (gesture.State == UIGestureRecognizerState.Changed) {
				FastDrawing = true;
				CameraIsAtInitialPosition = false;

				if (gesture.HasSingleTouch) {
          Camera.GestureOrbit ((Size)View.Bounds.Size.ToRoundedCGSize (), gesture.AnchorLocation, gesture.CurrentLocation);
					gesture.AnchorLocation = gesture.CurrentLocation;
				}

				if (gesture.HasTwoTouches) {
          Camera.LateralPan (gesture.StartLocation, gesture.MidpointLocation, false, false);
					gesture.StartLocation = gesture.MidpointLocation;
				}
					
				View.SetNeedsDisplay ();
			}

			if (gesture.State == UIGestureRecognizerState.Ended || gesture.State == UIGestureRecognizerState.Cancelled)
				ActiveGesturesCount--;

		}
		#endregion

		#region Animate Restore View
		/// <summary>
		/// StartRestoreViewTo is a helper method that is called by ZoomExtentsWithGesture to
		/// return the viewport back to it's "home" view.
		/// </summary>
		private void StartRestoreViewTo (Rhino.DocObjects.ViewportInfo targetPosition)
		{
			if (Camera == null)
				return;

			IsInAnimatedRestoreView = true;

			View.UserInteractionEnabled = false;
			RestoreViewStartTime = DateTime.Now;
			RestoreViewTotalTime = new TimeSpan (0, 0, 0, 0, 500);

			RestoreViewStartViewport = new ViewportInfo(Camera); // start from current position
			RestoreViewFinishViewport = new ViewportInfo(targetPosition); // end on the target position

			// fix frustum aspect to match current screen aspect
			RestoreViewFinishViewport.FrustumAspect = Camera.FrustumAspect;

			AnimationTimer = NSTimer.CreateScheduledTimer (0.0, this, new Selector ("AnimateRestoreView"), null, true);
			AnimationTimer.Fire ();
		}

		/// <summary>
		/// Tween from original view
		/// </summary>
		[Export ("AnimateRestoreView")]
		private void AnimateRestoreView ()
		{
			FastDrawing = true;
			var restoreViewCurrentTime = DateTime.Now;																				

			var currentTime = restoreViewCurrentTime;																						
			var startTime = RestoreViewStartTime;																							
			var timeElapsed = currentTime.Subtract (startTime);																	
			var timeElapsedInMs = timeElapsed.TotalMilliseconds;																
			var totalTimeOfAnimationInMs = RestoreViewTotalTime.TotalMilliseconds;						
			double percentCompleted = timeElapsedInMs / totalTimeOfAnimationInMs;

			if (percentCompleted > 1) {
				// Animation is completed. Perform one last draw.
				percentCompleted = 1;
				IsInAnimatedRestoreView = false;
				View.UserInteractionEnabled = true;
				CameraIsAtInitialPosition = !ShouldStartRestoreToInitialPosition;
				AnimationTimer.Invalidate ();
			}

			// Get some data from the starting view
			Rhino.Geometry.Point3d sourceTarget = RestoreViewStartViewport.TargetPoint;
			Rhino.Geometry.Point3d sourceCamera = RestoreViewStartViewport.CameraLocation;
			double sourceDistance = sourceCamera.DistanceTo (sourceTarget);
			Rhino.Geometry.Vector3d sourceUp = RestoreViewStartViewport.CameraUp;
			sourceUp.Unitize ();

			// Get some data from the ending view
			Rhino.Geometry.Point3d targetTarget = RestoreViewFinishViewport.TargetPoint;
			Rhino.Geometry.Point3d targetCamera = RestoreViewFinishViewport.CameraLocation;
			double targetDistance = targetCamera.DistanceTo (targetTarget);
			Rhino.Geometry.Vector3d targetCameraDir = targetCamera - targetTarget;
			Rhino.Geometry.Vector3d targetUp = RestoreViewFinishViewport.CameraUp;
			targetUp.Unitize ();

			// Adjust the target camera location so that the starting camera to target distance
			// and the ending camera to target distance are the same.  Doing this will calculate
			// a constant rotational angular momentum when tweening the camera location.
			// Further down we independently tween the camera to target distance.
			targetCameraDir.Unitize ();
			targetCameraDir *= sourceDistance;
			targetCamera = targetCameraDir + targetTarget;

			// calculate interim viewport values
			double frameDistance = ViewportInfoExtensions.CosInterp(sourceDistance, targetDistance, percentCompleted);

			Rhino.Geometry.Point3d frameTarget = new Rhino.Geometry.Point3d();

			frameTarget.X = ViewportInfoExtensions.CosInterp(sourceTarget.X, targetTarget.X, percentCompleted);
			frameTarget.Y = ViewportInfoExtensions.CosInterp(sourceTarget.Y, targetTarget.Y, percentCompleted);
			frameTarget.Z = ViewportInfoExtensions.CosInterp(sourceTarget.Z, targetTarget.Z, percentCompleted);

			var origin = Rhino.Geometry.Point3d.Origin;
			Rhino.Geometry.Point3d frameCamera = origin + (ViewportInfoExtensions.Slerp((sourceCamera - origin), (targetCamera - origin), percentCompleted));
			Rhino.Geometry.Vector3d frameCameraDir = frameCamera - frameTarget;

			// adjust the camera location along the camera direction vector to preserve the target location and the camera distance
			frameCameraDir.Unitize();
			frameCameraDir *= frameDistance;
			frameCamera = frameCameraDir + frameTarget;

			Rhino.Geometry.Vector3d frameUp = new Rhino.Geometry.Vector3d(ViewportInfoExtensions.Slerp (sourceUp, targetUp, percentCompleted));

			if (percentCompleted >= 1) {
				// put the last redraw at the exact end point to eliminate any rounding errors
				Camera.SetTarget (RestoreViewFinishViewport.TargetPoint, RestoreViewFinishViewport.CameraLocation, RestoreViewFinishViewport.CameraUp);
			} else {	
				Camera.SetTarget (frameTarget, frameCamera, frameUp);
			}

			SetFrustum (Camera, App.Manager.CurrentModel.BBox);

			View.SetNeedsDisplay ();

			if (!IsInAnimatedRestoreView) {
				// FastDrawing is still enabled and we just scheduled a draw of the model at the final location.
				// This entirely completes the animation. Now schedule one more redraw of the model with FastDrawing disabled
				// and this redraw will be done at exactly the same postion.  This prevents the final animation frame
				// from jumping to the final location because the final draw will take longer with FastDrawing disabled.
				PerformSelector (new Selector ("RedrawDetailed"), null, 0.05);
			}
		} 

		/// <summary>
		/// Redraw the final frame of an animation in "slow drawing" mode
		/// </summary>
		[Export("RedrawDetailed")]
		public void RedrawDetailed ()
		{
			FastDrawing = false;
			View.SetNeedsDisplay ();
		}
		#endregion

	}
}