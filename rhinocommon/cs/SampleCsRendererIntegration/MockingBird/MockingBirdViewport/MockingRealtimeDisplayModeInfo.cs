using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Rhino.Render;

namespace MockingBirdViewport
{
	public class MockingRealtimeDisplayModeInfo : RealtimeDisplayModeClassInfo
	{
		public override string Name => "MockingRealtimeMode";

		public override Guid GUID => new Guid("F14A3A24-C2FB-4216-9D2A-9636EF3869FA");

		public override Type RealtimeDisplayModeType => typeof(MockingRealtimeDisplayMode);

		public override bool DrawOpenGl => false;
	}
}
