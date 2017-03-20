//
// App.cs
// HelloRhino.Touch
//
// Created by dan (dan@mcneel.com) on 9/17/2013
// Copyright 2013 Robert McNeel & Associates.  All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
using System;
using RhinoMobile.Model;
using System.Collections.Generic;
using System.IO;
using System.Linq;

#if __IOS__
using MonoTouch.Foundation;
#endif

#if __ANDROID__
using Android.Net;
using Android.App;
using Android.OS;
#endif

namespace HelloRhino.Touch
{
	/// <summary>
	/// App is a singleton meant to be accessible app-wide.  This App class
	/// is for example purposes only and may be relocated into a Core library
	/// of your application itself.
	/// </summary>
	public sealed class App
	{
		#region members
		private static volatile App m_manager;
		private static object m_syncRoot = new object();
		#endregion

		#region properties
		/// <summary>
		/// CurrentModel is the model currently being displayed in the ModelView
		/// </summary>
		public RMModel CurrentModel { get; set; }

		/// <summary>
		/// Manager creates a threadsafe singleton of App
		/// </summary>
		public static App Manager
		{
			get {
				if (m_manager == null)
				{
					lock(m_syncRoot)
					{
						if (m_manager == null)
							m_manager = new App ();
					}
				}
				return m_manager;
			}
		}
		#endregion

		#region constructors
		private App() 
		{ 

		}
		#endregion

		/// <summary>
		/// This Setup method is for example purposes only.  It is placed in this the
		/// RhinoMobile library for convenience only, but can be located elsewhere and 
		/// need not be called setup (which may or may not be obvious).
		/// </summary>
		public void Setup ()
		{
			// Load a sample model...
			RMModel sampleModel = new RMModel ();
			sampleModel.Title = "Rhino Logo";
			sampleModel.BundleName = "Rhino Logo";
			sampleModel.LoadFromBundle ();
			CurrentModel = sampleModel;
		}

	}
}