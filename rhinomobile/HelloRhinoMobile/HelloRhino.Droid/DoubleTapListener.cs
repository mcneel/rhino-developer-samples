//
// DoubleTapListener.cs
// HelloRhino.Droid
//
// Created by dan (dan@mcneel.com) on 7/14/2014
// Copyright 2014 Robert McNeel & Associates.  All rights reserved.
//
using System;
using Android.Views;

namespace HelloRhino.Droid
{
  public class DoubleTapListener : GestureDetector.SimpleOnGestureListener, GestureDetector.IOnDoubleTapListener
  {
    #region properties
    private HelloRhinoAndroidRenderer AndroidRenderer { get; set; }
    #endregion

    #region constructors and disposal
    public DoubleTapListener (HelloRhinoAndroidRenderer androidRenderer)
    {
      AndroidRenderer = androidRenderer;
    }
    #endregion

    #region methods
    public override bool OnDoubleTap(MotionEvent e)
    {
      System.Diagnostics.Debug.WriteLine ("DoubleTap detected");

      // TODO: do something in response to double taps...

      return true;
    }
    #endregion
  }
}

