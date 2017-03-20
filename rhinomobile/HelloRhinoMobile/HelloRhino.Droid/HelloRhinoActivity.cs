//
// HelloRhinoActivity.cs
// HelloRhino.Droid
//
// Created by dan (dan@mcneel.com) on 9/19/2013
// Copyright 2013 Robert McNeel & Associates.  All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
using Android.App;
using Android.OS;
using Android.Content.PM;
using Android.Widget;
using Android.Views;
using Android.Opengl;

using RhinoMobile;
using RhinoMobile.Model;


namespace HelloRhino.Droid
{
  // These following ConfigurationChanges flags keep the EGL context from being destroyed when the device is rotated 
  // or the keyboard is displayed (these are the recommended settings for GL-based activities)
  [Activity(Label = "HelloRhino", ConfigurationChanges=ConfigChanges.Orientation | ConfigChanges.KeyboardHidden | ConfigChanges.ScreenSize, MainLauncher = true)]
	public class HelloRhinoActivity : Activity
	{ 
    #region properties
    private GLSurfaceView SurfaceView { get; set; }
    #endregion

		#region methods
    /// <summary>
    /// OnCreate is called by Android when this activity is created
    /// </summary>
    protected override void OnCreate (Bundle savedInstanceState)
    {
      base.OnCreate (savedInstanceState);

      App.Manager.ApplicationContext = base.ApplicationContext;

      App.Manager.Setup ();

      // The SurfaceView is inflated from the layout file when the ContentView is set...
      SetContentView (Resource.Layout.Main);
      // ...we can then hold onto a reference to that view using its id
      SurfaceView = FindViewById<GLSurfaceView> (Resource.Id.hellorhinoview);
     
      var intLayout = FindViewById<RelativeLayout> (Resource.Id.relLayout1);
      if (App.Manager.CurrentModel.IsReadyForRendering) {
        RunOnUiThread (() => intLayout.Visibility = ViewStates.Invisible);
      } else {
        RunOnUiThread (() => intLayout.Visibility = ViewStates.Visible);
      }

      var warningImage = FindViewById<ImageView> (Resource.Id.imageView1);
      RunOnUiThread (() => warningImage.Visibility = ViewStates.Invisible);

      var cancelButton = FindViewById<Button> (Resource.Id.cancelButton);
      cancelButton.Click += (sender, e) => App.Manager.CurrentModel.CancelModelPreparation ();

      // Tell the model to prepare itself for display...
      App.Manager.CurrentModel.MeshPrep += new MeshPreparationHandler (ObserveMeshPrep);

      // Tell the model to prepare itself for display...
      if (!App.Manager.CurrentModel.IsReadyForRendering)
        App.Manager.CurrentModel.Prepare ();
    }

    /// <summary>
    /// The following call pauses the rendering thread.
    /// If your OpenGL application is memory intensive,
    /// you should consider de-allocating objects that
    /// consume significant memory here.
    /// </summary>
    protected override void OnPause ()
    {
      base.OnPause ();
     
      SurfaceView.OnPause ();
    }
      
    /// <summary>
    /// The following call resumes a paused rendering thread.
    /// If you de-allocated graphic objects for onPause()
    /// this is a good place to re-allocate them.
    /// </summary>
    protected override void OnResume ()
    {
      base.OnResume ();

      SurfaceView.OnResume ();
    }
		#endregion

		#region Model Initialization Events
		private void ObserveMeshPrep (RMModel model, MeshPreparationProgress progress)
		{
			// Success
			if (progress.PreparationDidSucceed) {
				var initProgress = FindViewById<RelativeLayout> (Resource.Id.relLayout1);
				RunOnUiThread (() => initProgress.Visibility = ViewStates.Gone);
			}

			// Still working
			if (!progress.PreparationDidSucceed && progress.FailException == null) {
				// can't really update the progress bar on Android :(
			}

			// Failure or Cancellation
			if (progress.FailException != null) {
				var progressBar = FindViewById<ProgressBar> (Resource.Id.ProgressBar01);
				RunOnUiThread (() => progressBar.Visibility = ViewStates.Invisible);

				var warningImage = FindViewById<ImageView> (Resource.Id.imageView1);
				RunOnUiThread (() => warningImage.Visibility = ViewStates.Visible);

				var initLabel = FindViewById<TextView> (Resource.Id.txtText);
				RunOnUiThread (() => initLabel.Text = progress.FailException.Message);
			}
		}
		#endregion
	}
}