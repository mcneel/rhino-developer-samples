using System;
using Xunit;
using Rhino.Geometry;

namespace SampleGHTests
{
  [Collection("Primitives.gh collection")]
  public class TestMorePrimitives
  {
    PrimitivesFixture fixture { get; set; }

    public TestMorePrimitives(PrimitivesFixture fixture)
    {
      this.fixture = fixture;
    }

    [Fact]
    public void TestLineAgain()
    {
      foreach (var obj in (fixture.Doc.Objects))
        if (obj is Grasshopper.Kernel.IGH_Param param)
          if (param.NickName == "TestLineOutput")
          {
            param.CollectData();
            param.ComputeData();

            Assert.Equal(1, param.VolatileData.DataCount);
            var data = param.VolatileData.AllData(true).GetEnumerator();
            data.Reset();
            data.MoveNext();
            var theLine = data.Current;
            Assert.True(theLine.CastTo(out Line line));
            Assert.Equal(Math.Sqrt(1.0 * 1.0 + -5.0 * -5.0 + 3.0 * 3.0), line.Length);
            return;
          }
      Assert.True(false, "Did not find oputput");
    }
  }
}
