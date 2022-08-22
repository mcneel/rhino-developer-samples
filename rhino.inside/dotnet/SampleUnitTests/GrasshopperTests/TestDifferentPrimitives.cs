using System;
using Xunit;
using Rhino.Geometry;


namespace GrasshopperTests
{
  public class DifferentPrimitivesFixture : Rhino.Test.GHFileFixture
  {
    public DifferentPrimitivesFixture() : base("DifferentPrimitives.gh") { }
  }

  public class TestDifferentPrimitives : IClassFixture<DifferentPrimitivesFixture>
  {
    DifferentPrimitivesFixture fixture { get; set; }

    public TestDifferentPrimitives(DifferentPrimitivesFixture fixture)
    {
      this.fixture = fixture;
    }

    [Fact]
    public void TestCircle()
    {
      foreach (var obj in (fixture.Doc.Objects))
        if (obj is Grasshopper.Kernel.IGH_Param param)
          if (param.NickName == "TestCircleOutput2")
          {
            param.CollectData();
            param.ComputeData();

            Assert.Equal(1, param.VolatileData.DataCount);
            var data = param.VolatileData.AllData(true).GetEnumerator();
            data.Reset();
            data.MoveNext();
            var theCircle = data.Current;
            Assert.True(theCircle.CastTo(out Circle circle));
            Assert.Equal(5.0, circle.Radius);
            Assert.Equal(Math.PI * 2.0 * 5.0, circle.Circumference);
            return;
          }
      Assert.True(false, "Did not find oputput");
    }

    [Fact]
    public void TestLine()
    {
      foreach (var obj in (fixture.Doc.Objects))
        if (obj is Grasshopper.Kernel.IGH_Param param)
          if (param.NickName == "TestLineOutput2")
          {
            param.CollectData();
            param.ComputeData();

            Assert.Equal(1, param.VolatileData.DataCount);
            var data = param.VolatileData.AllData(true).GetEnumerator();
            data.Reset();
            data.MoveNext();
            var theLine = data.Current;
            Assert.True(theLine.CastTo(out Line line));
            Assert.Equal(Math.Sqrt(-3.0 * -3.0 + 8.0 * 8.0 + -4.0 * -4.0), line.Length);
            return;
          }
      Assert.True(false, "Did not find oputput");
    }

  }
}
