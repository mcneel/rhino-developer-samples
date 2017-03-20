//
// Main.cs
// HelloRhino.iOS
//
// Created by dan (dan@mcneel.com) on 9/09/2015
// Copyright 2015 Robert McNeel & Associates.  All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
using UIKit;

namespace HelloRhino.iOS
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
