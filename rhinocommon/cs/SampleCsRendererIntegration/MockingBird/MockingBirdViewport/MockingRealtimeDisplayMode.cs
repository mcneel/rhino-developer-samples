using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using Rhino;
using Rhino.DocObjects;
using Rhino.Render;

namespace MockingBirdViewport
{
	public class MockingRealtimeDisplayMode : RealtimeDisplayMode
	{
		private DateTime _startTime;
		private bool _isCompleted;
		private bool _started;
		private int _currentPass;
		private int _width;
		private int _height;
		private MockingRender reng;
		private Thread _theThread;

		public override bool StartRenderer(int w, int h, RhinoDoc doc, ViewInfo view, ViewportInfo viewportInfo, bool forCapture,
			RenderWindow renderWindow)
		{
			_started = true;
			// prepare render, get a changequeue
			_width = (int)w;
			_height = (int)h;
			reng = new MockingRender(Rhino.PlugIns.PlugIn.IdFromName("MockingBirdViewport"), doc.RuntimeSerialNumber, view, renderWindow)
			{
				MaxPasses = 50
			};
			reng.MaxPassesCompleted += Reng_MaxPassesCompleted;
			reng.PassRendered += Reng_PassRendered;
			reng.RenderReset += Reng_RenderReset;
			reng.RenderStarted += Reng_RenderStarted;
			MaxPassesChanged += MockingRealtimeDisplayMode_MaxPassesChanged;
			
			// start rendering
			_startTime = DateTime.Now;
			_isCompleted = false;

			_theThread = new Thread(reng.ColorPixels) {Name = "MockingBirdViewport thread"};
			_theThread.Start();
			
			return true;
		}

		private void MockingRealtimeDisplayMode_MaxPassesChanged(object sender, HudMaxPassesChangedEventArgs e)
		{
			reng.MaxPasses = e.MaxPasses;
		}

		private void Reng_RenderStarted(object sender, EventArgs e)
		{
			_started = true;
		}

		private void Reng_RenderReset(object sender, EventArgs e)
		{
			_isCompleted = false;
		}

		private void Reng_PassRendered(object sender, PassRenderedEventArgs e)
		{
			_currentPass = e.Pass;
			SignalRedraw();
		}

		private void Reng_MaxPassesCompleted(object sender, EventArgs e)
		{
			_isCompleted = true;
		}

		public override void GetRenderSize(out int width, out int height)
		{
			width = _width;
			height = _height;
		}

		public override int LastRenderedPass()
		{
			return _currentPass;
		}

		public override bool IsFrameBufferAvailable(ViewInfo view)
		{
			return true;
		}

		public override bool IsRendererStarted()
		{
			return _started;
		}

		public override bool OnRenderSizeChanged(int width, int height)
		{
			return true;
		}

		public override void ShutdownRenderer()
		{
			// stop our engine and free resources
			reng.Shutdown();
		}

		public override int HudLastRenderedPass()
		{
			return _currentPass;
		}

		public override int HudMaximumPasses()
		{
			return reng.MaxPasses;
		}

		public override bool HudAllowEditMaxPasses()
		{
			return true;
		}

		public override string HudProductName()
		{
			return "Mockingbird";
		}

		public override bool HudRendererLocked()
		{
			return false;
		}

		public override bool HudRendererPaused()
		{
			return false;
		}
		public override bool HudShowMaxPasses()
		{
			return true;
		}

		public override bool HudShowPasses()
		{
			return true;
		}

		public override bool HudShowCustomStatusText()
		{
			return true;
		}

		public override DateTime HudStartTime()
		{
			return _startTime;
		}

		public override string HudCustomStatusText()
		{
			return "painting pixels";
		}

		public override bool IsCompleted()
		{
			return _isCompleted;
		}

	}
}
