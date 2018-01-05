using System;
using System.Drawing;
using System.Threading;
using System.Timers;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Render;

namespace MockingBirdViewport
{
	public class PassRenderedEventArgs : EventArgs
	{
		public int Pass { get; private set; }
		public PassRenderedEventArgs(int pass)
		{
			Pass = pass;
		}
	}

	public class MockingRender
	{
		private MockingChangeQueue cq;
		private RenderWindow rw;
		public MockingRender(Guid plugInId, uint docSerialNumber, ViewInfo rhinoView, RenderWindow renderWindow)
		{
			cq = new MockingChangeQueue(plugInId, docSerialNumber, rhinoView);
			cq.ChangesReady += Cq_ChangesReady;
			cq.CreateWorld();
			rw = renderWindow;
			Passes = 0;
		}

		private void Cq_ChangesReady(object sender, EventArgs e)
		{
			Reset();
		}

		private Color4f _color = Color4f.White;

		public event EventHandler<PassRenderedEventArgs> PassRendered;
		private int _passes;
		public int Passes
		{
			get { return _passes; }
			private set
			{
				if (value > _passes)
				{
					_passes = value;
					PassRendered?.Invoke(this, new PassRenderedEventArgs(_passes));
				}
				else
				{
					_passes = value;
				}
			}
		}

		private int _maxPasses;
		public int MaxPasses
		{
			get { return _maxPasses; }
			set
			{
				_maxPasses = value;
				_completionTriggered = false;
			}
		}

		private bool _completionTriggered;
		public event EventHandler MaxPassesCompleted;
		public event EventHandler RenderStarting;
		public event EventHandler RenderStarted;

		private void ColorPass(int pass)
		{
			var fac = pass/(float)MaxPasses;
			var c = Color4f.FromArgb(1.0f, _color.R*fac, _color.G*fac, _color.B*fac);
			using (var channel = rw.OpenChannel(RenderWindow.StandardChannels.RGBA))
			{
				var size = rw.Size();
				for (var x = 0; x < size.Width; x++)
				{
					for (var y = 0; y < size.Height; y++)
					{
						channel.SetValue(x, y, c);
					}
					if (_shutdown) break;
				}
			}
		}

		public void ColorPixels()
		{

			RenderStarting?.Invoke(this, EventArgs.Empty);
			Passes = 0;

			RenderStarted?.Invoke(this, EventArgs.Empty);

			while (true)
			{
				while (Passes < MaxPasses)
				{

					ColorPass(Passes);
					Thread.Sleep(10);
					if (_shutdown) break;
					Passes += 1;
				}
				if (!_completionTriggered && Passes >= MaxPasses)
				{
					_completionTriggered = true;
					MaxPassesCompleted?.Invoke(this, EventArgs.Empty);

				}
				Thread.Sleep(10);
				if (_shutdown) break;
			}
		}

		public event EventHandler RenderReset;
		public void Reset()
		{
			Passes = 0;
			_completionTriggered = false;
			RenderReset?.Invoke(this, EventArgs.Empty);
		}

		private bool _shutdown;
		public void Shutdown()
		{
			_shutdown = true;
		}

	}
}