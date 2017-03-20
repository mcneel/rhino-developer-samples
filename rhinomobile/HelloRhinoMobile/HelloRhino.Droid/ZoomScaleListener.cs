//
// ZoomScaleListener.cs
// HelloRhino.Droid
//
// Created by dan (dan@mcneel.com) on 9/17/2013
// Copyright 2013 Robert McNeel & Associates.  All rights reserved.
//
using System.Drawing;

using Android.Views;

using RhinoMobile;
using RhinoMobile.Display;

namespace HelloRhino.Droid
{
	class ZoomScaleListener : ScaleGestureDetector.SimpleOnScaleGestureListener
	{
    #region properties
    private HelloRhinoAndroidRenderer AndroidRenderer { get; set; }
    #endregion

		#region constructors
    public ZoomScaleListener(HelloRhinoAndroidRenderer androidRenderer)
		{
      AndroidRenderer = androidRenderer;
		}
		#endregion

		#region methods
		public override bool OnScale(ScaleGestureDetector detector)
		{
      AndroidRenderer.ScaleFactor *= detector.ScaleFactor;

			if (App.Manager.CurrentModel != null && App.Manager.CurrentModel.IsReadyForRendering) {
        AndroidRenderer.Camera.Magnify (AndroidRenderer.BufferSize, AndroidRenderer.ScaleFactor, 0, AndroidRenderer.ZoomPoint);
			}

      AndroidRenderer.ScaleFactor = 1.0f;

			return true;
		}
		#endregion
	}
}

