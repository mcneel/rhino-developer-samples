//
// App.cs
// HelloRhino.Droid
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

namespace HelloRhino.Droid
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
		#if __ANDROID__
		/// <remarks>
		/// ANDROID ONLY
		/// <para>This property is for passing along a reference to Android.Content.Context.ApplicationContext
		/// property from the Droid project into the project for usage app-wide.  This should be set with
		/// caution and not abused. </para>
		/// </remarks>
		public Android.Content.Context ApplicationContext { get; set; }
		#endif

		/// <summary>
		/// CurrentModel is the model currently being displayed in the ModelView
		/// </summary>
		public RMModel CurrentModel { get; set; }

		/// <summary>
		/// Fast Drawing is the quick PerVertex lighting drawing model without anti-aliasing
		/// </summary>
		public bool FastDrawing { get; set; }

		/// <summary>
		/// Enables and disables per-vertex lighting
		/// </summary>
		public bool UsePerVertexLighting { get; set; }

		/// <summary>
		/// Enables and disables anti-aliasing
		/// </summary>
		public bool UseAntiAliasing { get; set; }

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
			UsePerVertexLighting = true;
			UseAntiAliasing = true;
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
			sampleModel.LoadFromBundle (ApplicationContext);
			CurrentModel = sampleModel;
		}

	}
}