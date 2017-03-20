//
// OrbitDollyGestureDetector.cs
// HelloRhino.Droid
//
// Created by dan (dan@mcneel.com) on 7/9/2014
// Copyright 2014 Robert McNeel & Associates.  All rights reserved.
//
using System;
using System.Drawing;
using Android.Views;
using RhinoMobile.Display;
using System.Diagnostics;

namespace HelloRhino.Droid
{

  public enum GestureDetectorState
  {
    Possible,
    Began,
    Changed,
    Ended,
    Cancelled,
    Failed
  };

  public class OrbitDollyGestureDetector : GestureDetector
  {
    #region properties
    /// <value> The current state of the gesture detector. </value>
    public GestureDetectorState State { get; private set; }

    /// <value> The Id of the currently active leading finger.  This is used to determine which finger should be used for an orbit. </value>
    public int TrackingFingerId { get; set; }

    /// <value> The starting position of the TrackingFinger of the detector. </value>
    public PointF StartLocation { get; set; }

    /// <value> The anchor location is the used for orbit gestures to hold the position of the
    /// startLocation in case the finger switches from one finger to two or more. </value>
    public PointF AnchorLocation { get; set; }

    /// <value> The point position of the tracking finger. </value>
    public PointF CurrentLocation { get; set; }

    /// <value> A constant for unknown finger Ids (also known as pointer ids). </value>
    private static int InvalidFingerId { get { return -1; } }

    /// <value> The Id for the first finger (also known as a pointer id). </value>
    private int FirstFingerId { get; set; }

    /// <value> The Id for the first second (also known as a pointer id). </value>
    private int SecondFingerId { get; set; }

    /// <value> The midpoint location between two fingers. </value>
    public PointF MidpointLocation { get; set; }

    /// <value> True if the detector has exactly one finger. </value>
    public bool HasSingleFinger { get; private set; }

    /// <value> True if the detector has exactly two fingers. </value>
    public bool HasTwoFingers { get; private set; }

    /// <value> True if the detector currently has more than two fingers. </value>
    public bool HasMoreThanTwoFingers { get; private set; }

    /// <value> True if the gesture is just beginning.  False on after Reset has been called. </value>
    private bool UserHasBegunGesture { get; set; }
    #endregion

    #region constructors and disposal
    public OrbitDollyGestureDetector () : base (new GestureDetector.SimpleOnGestureListener()) //HelloRhinoView view
    {
      //m_view = view;
      Reset ();
    }
    #endregion

    #region methods
    public override bool OnTouchEvent (MotionEvent ev)
    {
      // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
      MotionEventActions action = ev.Action & MotionEventActions.Mask;
      int pointerIndex;
      int fingersRemaining;

      switch (action) {

      case MotionEventActions.Down: // Primary Finger...
        HasSingleFinger = true;
        HasTwoFingers = false;
        HasMoreThanTwoFingers = false;
        TrackingFingerId = ev.GetPointerId(0);
        AnchorLocation = new PointF (ev.GetX (TrackingFingerId), ev.GetY (TrackingFingerId));
        UserHasBegunGesture = true;
        State = GestureDetectorState.Began;
        //Debug.WriteLine ("Primary Finger Down with Id = {0}", TrackingFingerId);
        break;

      case MotionEventActions.PointerDown: // Secondary Finger(s)...        

        if (ev.PointerCount == 2) {
          SecondFingerId = ev.ActionIndex;
          //Debug.WriteLine ("Second Finger Down with Id = {0}", SecondFingerId);
          HasSingleFinger = false;
          HasTwoFingers = true;
          HasMoreThanTwoFingers = false;

          var trackingFingerLocation = new PointF (ev.GetX (TrackingFingerId), ev.GetY (TrackingFingerId));
          var secondFingerLocation = new PointF (ev.GetX (SecondFingerId), ev.GetY (SecondFingerId));

          MidpointLocation = MidPointBetween (trackingFingerLocation, secondFingerLocation);
          StartLocation = MidpointLocation;

          break;
        } 
          
        // Ignore any fingers above 2
        if (ev.PointerCount > 2) {
          HasSingleFinger = false;
          HasTwoFingers = false;
          HasMoreThanTwoFingers = true;
          //Debug.WriteLine ("More than two fingers down with new finger Id = {0}  IGNORING", ev.ActionIndex);
          break;
        }
        break;

      case MotionEventActions.Move:
        if (HasSingleFinger || HasTwoFingers) {
          pointerIndex = ev.FindPointerIndex (TrackingFingerId);
          CurrentLocation = new PointF (ev.GetX (pointerIndex), ev.GetY (pointerIndex));
        }

        if (HasTwoFingers) {
          var trackingFingerLocation = new PointF (ev.GetX (TrackingFingerId), ev.GetY (TrackingFingerId));
          var secondFingerLocation = new PointF (ev.GetX (SecondFingerId), ev.GetY (SecondFingerId));
          MidpointLocation = MidPointBetween (trackingFingerLocation, secondFingerLocation);
        }

        State = GestureDetectorState.Changed;
        break;

      
      case MotionEventActions.Cancel:
        fingersRemaining = ev.PointerCount - 1;
        if (fingersRemaining == 0) {
          State = GestureDetectorState.Cancelled;
          Reset ();
        }
        break;

      case MotionEventActions.Up:
        TrackingFingerId = InvalidFingerId;
        //Debug.WriteLine ("All Fingers Up, Gesture Ended");
        Reset ();
        State = GestureDetectorState.Ended;
        break;

      case MotionEventActions.PointerUp:
        fingersRemaining = ev.PointerCount - 1;
        //Debug.WriteLine ("Secondary Finger Up, fingers remaining = {0}", fingersRemaining);

        if (fingersRemaining == 1) {
          // check to make sure that the finger that went up is for the gesture we're tracking. 
          pointerIndex = (int) (ev.Action & MotionEventActions.PointerIndexMask) >> (int) MotionEventActions.PointerIndexShift;
          int pointerId = ev.GetPointerId(pointerIndex);
          if (pointerId == TrackingFingerId)
          {
            // This was our tracking finger going up. Choose a new tracking finger and adjust accordingly
            int newPointerIndex = pointerIndex == 0 ? 1 : 0;
            TrackingFingerId = ev.GetPointerId(newPointerIndex);
          }
            
          HasSingleFinger = true;
          HasTwoFingers = false;
          HasMoreThanTwoFingers = false;
          AnchorLocation = new PointF (ev.GetX (TrackingFingerId), ev.GetY (TrackingFingerId));
          CurrentLocation = new PointF (ev.GetX (TrackingFingerId), ev.GetY (TrackingFingerId));
        }

        if (fingersRemaining == 2) {
          // stubbed
        }

        break;
      }
        
      return true;

    }

    /// <summary>
    /// Resets the detector to its initial state.
    /// </summary>
    public void Reset () 
    {
      HasSingleFinger = false;
      HasTwoFingers = false;
      HasMoreThanTwoFingers = false;
      UserHasBegunGesture = false;

      State = GestureDetectorState.Possible;
    }
    #endregion

    #region Utility methods
    /// <returns> The point equidistant to the two points. </returns>
    private static PointF MidPointBetween (PointF location0, PointF location1) 
    {
      var midpoint = new PointF();
     
      midpoint.X = (location0.X + location1.X) / 2;
      midpoint.Y = (location0.Y + location1.Y) / 2;

      return midpoint;
    }
    #endregion
  }
}

