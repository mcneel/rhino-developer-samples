// WARNING
//
// This file has been generated automatically by Xamarin Studio to store outlets and
// actions made in the UI designer. If it is removed, they will be lost.
// Manual changes to this file may not be handled correctly.
//
using Foundation;
using System.CodeDom.Compiler;

namespace HelloRhino.iOS
{
	partial class HelloRhinoViewController
	{
		[Outlet]
		UIKit.UIButton CancelButton { get; set; }

		[Outlet]
		UIKit.UILabel InitializingLabel { get; set; }

		[Outlet]
		UIKit.UIView InitPrepView { get; set; }

		[Outlet]
		UIKit.UIProgressView ProgressBar { get; set; }

		[Outlet]
		UIKit.UIImageView WarningSymbol { get; set; }

		[Action ("CancelPrep:")]
		partial void CancelPrep (Foundation.NSObject sender);
		
		void ReleaseDesignerOutlets ()
		{
			if (CancelButton != null) {
				CancelButton.Dispose ();
				CancelButton = null;
			}

			if (InitializingLabel != null) {
				InitializingLabel.Dispose ();
				InitializingLabel = null;
			}

			if (InitPrepView != null) {
				InitPrepView.Dispose ();
				InitPrepView = null;
			}

			if (ProgressBar != null) {
				ProgressBar.Dispose ();
				ProgressBar = null;
			}

			if (WarningSymbol != null) {
				WarningSymbol.Dispose ();
				WarningSymbol = null;
			}
		}
	}
}
