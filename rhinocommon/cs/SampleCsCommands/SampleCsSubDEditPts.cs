using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System;
using System.Collections.Generic;

namespace SampleCsCommands
{
    public class SampleCsSubDEditPts : Command
    {
        public SampleCsSubDEditPts()
        {
            // Rhino only creates one instance of each command class defined in a
            // plug-in, so it is safe to store a refence in a static property.
            Instance = this;
        }

        ///<summary>The only instance of this command.</summary>
        public static SampleCsSubDEditPts Instance
        {
            get; private set;
        }

        ///<returns>The command name as it appears on the Rhino command line.</returns>
        public override string EnglishName
        {
            get { return "SampleCsSubDEditPts"; }
        }

        protected override Result RunCommand(RhinoDoc doc, RunMode mode)
        {
            var go = new GetObject();
            go.SetCommandPrompt("Select SubD points");
            go.GeometryFilter = ObjectType.Grip;
            go.SubObjectSelect = true;
            go.GetMultiple(1, 0);
            if (go.CommandResult() != Result.Success)
                return go.CommandResult();

            Dictionary<Guid, List<int>> objectVertexIndices = new Dictionary<Guid, List<int>>();
            foreach (var objRef in go.Objects())
            {
                if (objRef.Object() is GripObject gripObj)
                {
                    if (doc.Objects.FindId(gripObj.OwnerId) is SubDObject subdObj)
                    {
                        if (!objectVertexIndices.TryGetValue(gripObj.OwnerId, out List<int> vis))
                        {
                            vis = new List<int>();
                            objectVertexIndices.Add(gripObj.OwnerId, vis);
                        }
                        vis.Add(gripObj.Index);
                    }
                }
            }

            foreach (var item in objectVertexIndices)
            {
                var objectId = item.Key;
                var vertexIndices = item.Value;
                if (doc.Objects.FindId(objectId) is SubDObject subdObj)
                {
                    if (subdObj.Geometry is SubD subD)
                    {
                        SubD newSubD = subD.Duplicate() as SubD;
                        if (newSubD != null)
                        {
                            Point3d offset = new Point3d(0.0, 0.0, 1.0);
                            List<Point3d> surfacePts = new List<Point3d>();
                            int vi = 0;
                            SubDVertex vert = newSubD.Vertices.First;
                            while (vert != null)
                            {
                                if (vertexIndices.Contains(vi))
                                {
                                    surfacePts.Add(vert.SurfacePoint() + offset);
                                }
                                else
                                {
                                    surfacePts.Add(vert.SurfacePoint());
                                }
                                vert = vert.Next;
                                vi++;
                            }
                            if (newSubD.InterpolateSurfacePoints(surfacePts.ToArray()))
                            {
                                doc.Objects.Replace(subdObj.Id, newSubD);
                            }
                            else
                            {
                                return Result.Failure;
                            }
                        }
                    }
                }
            }

            return Result.Success;
        }
    }
}
