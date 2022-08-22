using System;
using Xunit;
using Rhino.Geometry;

namespace SampleGHTests
{
//  [CollectionDefinition("Primitives.gh collection")]
  public class PrimitivesCollection : ICollectionFixture<PrimitivesFixture>
  {
    // This class has no code, and is never created. Its purpose is simply
    // to be the place to apply [CollectionDefinition] and all the
    // ICollectionFixture<> interfaces.
  }

  public class PrimitivesFixture : Rhino.Test.GHFileFixture
  {
    public PrimitivesFixture() : base("Primitives.gh") { }
  }

//  [Collection("Primitives.gh collection")]
  public class TestPrimitives : IClassFixture<PrimitivesFixture>
  {
    PrimitivesFixture fixture { get; set; }

    public TestPrimitives(PrimitivesFixture fixture)
    {
      this.fixture = fixture;
    }

    [Fact]
    public void TestCircle()
    {
      foreach (var obj in (fixture.Doc.Objects))
        if (obj is Grasshopper.Kernel.IGH_Param param)
          if (param.NickName == "TestCircleOutput")
          {
            param.CollectData();
            param.ComputeData();

            Assert.Equal(1, param.VolatileData.DataCount);
            var data = param.VolatileData.AllData(true).GetEnumerator();
            data.Reset();
            data.MoveNext();
            var theCircle = data.Current;
            Assert.True(theCircle.CastTo(out Circle circle));
            Assert.Equal(1.0, circle.Radius);
            Assert.Equal(Math.PI * 2.0, circle.Circumference);
            return;
          }
      Assert.True(false, "Did not find oputput");
    }

    [Fact]
    public void TestLine()
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
            Assert.Equal(Math.Sqrt(1.0*1.0 + -5.0*-5.0 + 3.0*3.0), line.Length);
            return;
          }
      Assert.True(false, "Did not find oputput");
    }
  }
}
