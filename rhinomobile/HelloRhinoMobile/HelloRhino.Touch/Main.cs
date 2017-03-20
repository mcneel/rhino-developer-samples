//
// Main.cs
// HelloRhino.Touch
//
// Created by dan (dan@mcneel.com) on 9/18/2013
// Copyright 2013 Robert McNeel & Associates.  All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
using MonoTouch.UIKit;

namespace HelloRhino.Touch
{
	public class Application
	{
		/// <summary>
		/// This is the main entry point of the application.  If you want to 
		/// use a different Application Delegate class from "AppDelegate" you 
		/// can specify it here.
		/// </summary>
		static void Main (string[] args)
		{
			UIApplication.Main (args, null, "AppDelegate");
		}
	}
}
