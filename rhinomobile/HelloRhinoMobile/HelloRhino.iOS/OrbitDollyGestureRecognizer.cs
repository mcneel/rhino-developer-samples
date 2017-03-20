//
// OrbitDollyGestureRecognizer.cs
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

using UIKit;
using CoreGraphics;
using Foundation;

namespace HelloRhino.iOS
{
	public class OrbitDollyGestureRecognizer : UIPanGestureRecognizer
	{
		#region properties
		/// <value> The tracking touch is the first touch of the gesture. </value>
		public UITouch TrackingTouch { get; private set; }

		/// <value> The starting position of the TrackingTouch of the gesture in the view. </value>
		public CGPoint StartLocation { get; set; }

		/// <value> The anchor location is the used for orbit gestures to hold the position of the
		/// startLocation in case the gesture switches from one touch to two or more. </value>
		public CGPoint AnchorLocation { get; set; }

		/// <value> The point position of the tracking touch. </value>
		public CGPoint CurrentLocation { get; set; }

		/// <value> The midpoint location between two touches. </value>
		public CGPoint MidpointLocation { get; set; }

		/// <value> True if the gesture has exactly one touch. </value>
		public bool HasSingleTouch { get; private set; }

		/// <value> True if the gesture has exactly two touches. </value>
		public bool HasTwoTouches { get; private set; }

		/// <value> True if the gesture currently has more than two touches. </value>
		public bool HasMoreThanTwoTouches { get; private set; }

		/// <value> True if the gesture is just beginning.  False on after Reset has been called. </value>
		private bool UserHasBegunGesture { get; set; }
		#endregion

		#region methods	
		/// <summary>
		/// TouchesBegan is called whenever a new touch is added to the gesture.
		/// </summary>
		public override void TouchesBegan (NSSet touches, UIEvent evt)
		{
			NSSet AllTouchesInRecognizer = evt.TouchesForGestureRecognizer (this);

			if (!UserHasBegunGesture) {
				UITouch touch = (UITouch)touches.AnyObject;

				TrackingTouch = touch;
				AnchorLocation = touch.LocationInView (this.View);
				UserHasBegunGesture = true;
				State = UIGestureRecognizerState.Began;
			}

			// Ignore any touches above 2
			if (AllTouchesInRecognizer.Count > 2) {
				HasSingleTouch = false;
				HasTwoTouches = false;
				HasMoreThanTwoTouches = true;
				Reset ();
			}

			if (AllTouchesInRecognizer.Count == 1) {
				HasSingleTouch = true;
				HasTwoTouches = false;
				HasMoreThanTwoTouches = false;
			}

			if (AllTouchesInRecognizer.Count == 2) {
				MidpointLocation = MidPointBetweenTouches (AllTouchesInRecognizer);
				StartLocation = MidpointLocation;
				HasSingleTouch = false;
				HasTwoTouches = true;
				HasMoreThanTwoTouches = false;
			}

		}

		/// <summary>
		/// TouchesMoved is called whenever the touches in the set change position.
		/// </summary>
		public override void TouchesMoved (NSSet touches, UIEvent evt)
		{
			CurrentLocation = TrackingTouch.LocationInView (this.View);

			if (HasTwoTouches)
				MidpointLocation = MidPointBetweenTouches (evt.TouchesForGestureRecognizer (this));

			State = UIGestureRecognizerState.Changed;
		}

		/// <summary>
		/// Resets the gestureRecognizer to its initial state.
		/// </summary>
		public override void Reset () 
		{
			HasSingleTouch = false;
			HasTwoTouches = false;
			HasMoreThanTwoTouches = false;
			UserHasBegunGesture = false;

			if (State == UIGestureRecognizerState.Possible)
				State = UIGestureRecognizerState.Failed;
		}

		/// <summary>
		/// TouchesEnded decrements the touches down from two or more touches and sets the
		/// state of the GestureRecognizer.
		/// </summary>
		public override void TouchesEnded (NSSet touches, UIEvent evt)
		{
			NSSet AllTouchesInRecognizer = evt.TouchesForGestureRecognizer (this);
			int touchesLeft = (int)(AllTouchesInRecognizer.Count - touches.Count);

			if (touchesLeft == 0) {
				HasSingleTouch = false;
				HasTwoTouches = false;
				HasMoreThanTwoTouches = false;
				State = UIGestureRecognizerState.Ended;
				Reset ();
			}

			if (touchesLeft == 1) {
				// Just to be uber-safe...
				foreach (UITouch touch in AllTouchesInRecognizer) {
					if (!touches.Contains(touch)) {
						TrackingTouch = touch;
						AnchorLocation = touch.LocationInView (this.View);
						CurrentLocation = touch.LocationInView (this.View);
					}
				}

				HasSingleTouch = true;
				HasTwoTouches = false;
				HasMoreThanTwoTouches = false;
			}

			if (touchesLeft == 2) {
				// stubbed
			}

		}

		/// <summary>
		/// TouchesCancelled is called when touches are cancelled - ie:
		/// go off the screen or cancelled by some other gesture.
		/// </summary>
		public override void TouchesCancelled (NSSet touches, UIEvent evt)
		{
			if (touches.Count == 0) {
				State = UIGestureRecognizerState.Cancelled;
				Reset ();
			}
		}

		/// <returns> The point equidistant to the first two touches in the set. </returns>
		private CGPoint MidPointBetweenTouches (NSSet touches) 
		{
			CGPoint midpoint = new CGPoint();

			var bothTouches = touches.ToArray<UITouch> ();
			UITouch touch0 = bothTouches [0];
			UITouch touch1 = bothTouches [1];
			CGPoint location0 = touch0.LocationInView (this.View);
			CGPoint location1 = touch1.LocationInView (this.View);
			// calculate the point midway between the two touch points
			midpoint.X = (location0.X + location1.X) / 2;
			midpoint.Y = (location0.Y + location1.Y) / 2;

			return midpoint;
		}
		#endregion

	}
}