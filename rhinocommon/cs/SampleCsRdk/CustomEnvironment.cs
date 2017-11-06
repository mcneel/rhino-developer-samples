using System;
using System.Collections.Generic;
using Rhino.Render;

namespace SampleCsRdk
{
  [System.Runtime.InteropServices.Guid("5F97DC31-9750-491E-9FE3-060B8BFD1FA6")]
  public class CustomEnvironment : RenderEnvironment
    {
        public override string TypeName { get { return "C# Custom Environment"; } }
        public override string TypeDescription { get { return "My first custom .NET environment"; } }

        public CustomEnvironment()
        {
          Fields.Add("background", Rhino.Display.Color4f.Black, "Background");
        }

        protected override void OnAddUserInterfaceSections()
        {
          AddAutomaticUserInterfaceSection("Parameters", 0);
        }

        public override void SimulateEnvironment(ref SimulatedEnvironment sim, bool forDataOnly)
        {
          //if( TryGetAutomticUiField("background", out background) )
          Rhino.Display.Color4f color;
          if (Fields.TryGetValue("background", out color) )
            sim.BackgroundColor = color.AsSystemColor();

            base.SimulateEnvironment(ref sim, forDataOnly);
        }

        //ColorField m_color = new ColorField("background", "Background", Rhino.Display.Color4f.Black);
    }
}
