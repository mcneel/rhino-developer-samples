using System;
using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  /// <summary>
  /// Utility class to handle array arguments and calculate offsets.
  /// </summary>
  class SampleCsArrayArgs
  {
    // Object counts
    public int CountX { get; set; }
    public int CountY { get; set; }
    public int CountZ { get; set; }

    // Distance between objects
    public double DistanceX { get; set; }
    public double DistanceY { get; set; }
    public double DistanceZ { get; set; }

    // The transformation plane
    public Rhino.Geometry.Plane Plane { get; set; }

    // The offsets to be used during transformation
    public List<Rhino.Geometry.Vector3d> Offsets;

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsArrayArgs()
    {
      CountX = 1;
      CountY = 1;
      CountZ = 1;
      DistanceX = 0.0;
      DistanceY = 0.0;
      DistanceZ = 0.0;
      Plane = Rhino.Geometry.Plane.WorldXY;
      Offsets = new List<Rhino.Geometry.Vector3d>();
    }

    /// <summary>
    /// Ensures that the capacity of the offset list is valid
    /// </summary>
    public void ValidateOffsets()
    {
      if (CountX < 1) CountX = 1;
      if (CountY < 1) CountY = 1;
      if (CountZ < 1) CountZ = 1;
      int offsetCount = CountX * CountY * CountZ;
      if (Offsets.Count != offsetCount)
      {
        Offsets.Clear();
        Offsets.Capacity = offsetCount;
        Offsets.AddRange(Enumerable.Repeat(Rhino.Geometry.Vector3d.Zero, offsetCount));
      }
    }

    /// <summary>
    /// Resets the offset list
    /// </summary>
    public void ResetOffsets()
    {
      ValidateOffsets();
      for (int i = 0; i < Offsets.Count; i++)
        Offsets[i] = Rhino.Geometry.Vector3d.Zero;
    }

    /// <summary>
    /// Calculates the offsets (to be used later during transformation)
    /// </summary>
    public void CalculateOffsets()
    {
      ValidateOffsets();

      Rhino.Geometry.Vector3d vx = Plane.XAxis;
      Rhino.Geometry.Vector3d vy = Plane.YAxis;
      Rhino.Geometry.Vector3d vz = Plane.ZAxis;

      if (DistanceX < 0) vx.Reverse();
      if (DistanceY < 0) vy.Reverse();
      if (DistanceZ < 0) vz.Reverse();

      vx = vx * Math.Abs(DistanceX);
      vy = vy * Math.Abs(DistanceY);
      vz = vz * Math.Abs(DistanceZ);

      int index = 0;
      for (int z = 0; z < CountZ; z++)
      {
        for (int y = 0; y < CountY; y++)
        {
          for (int x = 0; x < CountX; x++)
          {
            if (x == 0 && y == 0 && z == 0)
            {
              Offsets[index++] = Rhino.Geometry.Vector3d.Zero;
            }
            else
            {
              Rhino.Geometry.Vector3d offset = new Rhino.Geometry.Vector3d(
                vx.X * (double)x + vy.X * (double)y + vz.X * (double)z,
                vx.Y * (double)x + vy.Y * (double)y + vz.Y * (double)z,
                vx.Z * (double)x + vy.Z * (double)y + vz.Z * (double)z
                );

              Offsets[index++] = (offset.IsValid) ? offset : Rhino.Geometry.Vector3d.Zero;
            }
          }
        }
      }
    }
  }

  [System.Runtime.InteropServices.Guid("660f53d4-6390-467f-bbe0-3381d7524a0f")]
  public class SampleCsArray : Command
  {
    public SampleCsArray()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsArray"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Define a unit Brep box
      Rhino.Geometry.Point3d min = new Rhino.Geometry.Point3d(0.0, 0.0, 0.0);
      Rhino.Geometry.Point3d max = new Rhino.Geometry.Point3d(1.0, 1.0, 1.0);
      Rhino.Geometry.BoundingBox bbox = new Rhino.Geometry.BoundingBox(min, max);
      Rhino.Geometry.Brep brep = Rhino.Geometry.Brep.CreateFromBox(bbox);

      // Add a copy to the document
      doc.Objects.AddBrep(brep);

      // Create and define the arguments of the array
      SampleCsArrayArgs args = new SampleCsArrayArgs();
      args.CountX = 10;
      args.CountY = 10;
      args.CountZ = 10;
      args.DistanceX = 1.0;
      args.DistanceY = 1.0;
      args.DistanceZ = 1.0;
      // Calculate the offsets
      args.CalculateOffsets();

      // Array the unit Brep box
      for (int i = 0; i < args.Offsets.Count; i++)
      {
        // Skip the first one...
        if (!args.Offsets[i].IsZero)
        {
          Rhino.Geometry.Transform xform = Rhino.Geometry.Transform.Translation(args.Offsets[i]);
          if (xform.IsValid && xform != Rhino.Geometry.Transform.Identity)
          {
            Rhino.Geometry.Brep dupBrep = brep.DuplicateBrep();
            dupBrep.Transform(xform);
            if (dupBrep.IsValid)
              doc.Objects.AddBrep(dupBrep);
          }
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
