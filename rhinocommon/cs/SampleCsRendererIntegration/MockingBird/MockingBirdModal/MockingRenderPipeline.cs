using System.Drawing;
using System.Threading;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.PlugIns;
using Rhino.Render;

namespace MockingBirdModal
{
	public class MockingRenderPipeline : RenderPipeline
	{
		private readonly MockingRenderContext m_rc;
		public MockingRenderPipeline(RhinoDoc doc, RunMode mode, RenderPlugIn plugin, MockingRenderContext rc)
			: base(doc, mode, plugin, RenderSize(doc),
					"MockingBird (modal)", Rhino.Render.RenderWindow.StandardChannels.RGBA, false, false)
		{
			m_rc = rc;
		}


		protected override bool OnRenderBegin()
		{
			return m_rc.StartRenderThread(m_rc.Renderer, "MockingBird Modal Rendering thread");
		}

		protected override bool OnRenderBeginQuiet(Size imageSize)
		{
			return OnRenderBegin();
		}

		protected override bool OnRenderWindowBegin(RhinoView view, Rectangle rectangle)
		{
			return false;
		}

		protected override void OnRenderEnd(RenderEndEventArgs e)
		{
			// stop render engine here.
			m_rc.StopRendering();
		}

		protected override bool ContinueModal()
		{
			return !m_rc.Done;
		}

		public override bool SupportsPause()
		{
			return true;
		}

		public override void PauseRendering()
		{
			m_rc.PauseRendering();
		}

		public override void ResumeRendering()
		{
			m_rc.ResumeRendering();
		}
	}
}
