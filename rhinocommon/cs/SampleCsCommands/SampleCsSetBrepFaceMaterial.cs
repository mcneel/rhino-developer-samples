using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
    public class SampleCsSetBrepFaceMaterial : Command
    {
        public SampleCsSetBrepFaceMaterial()
        {
        }
        public override string EnglishName
        {
            get { return "SampleCsSetBrepFaceMaterial"; }
        }

        protected override Result RunCommand(RhinoDoc doc, RunMode mode)
        {
            ObjRef objRef = null;
            using (GetObject go = new GetObject())
            {
                go.SetCommandPrompt("Select brep object to set face materials to");
                go.GeometryFilter = ObjectType.Brep;
                var goRes = go.Get();
                if (goRes != GetResult.Object)
                {
                    return go.CommandResult();
                }
                objRef = go.Object(0);
            }

            if (doc.Objects.FindId(objRef.ObjectId) is BrepObject brepObject)
            {
                // Create red material
                Material redMaterial = new Material();
                redMaterial.DiffuseColor = System.Drawing.Color.Red;
                int redMaterialIndex = doc.Materials.Add(redMaterial);

                // Create blue material with red material on one channel
                Material blueMaterial = new Material();
                blueMaterial.DiffuseColor = System.Drawing.Color.Blue;
                int redChannelIndex = blueMaterial.MaterialChannelIndexFromId(doc.Materials[redMaterialIndex].Id, true);
                int blueMaterialIndex = doc.Materials.Add(blueMaterial);

                // Change object material to blue material
                brepObject.Attributes.MaterialIndex = blueMaterialIndex;
                brepObject.Attributes.MaterialSource = ObjectMaterialSource.MaterialFromObject;

                // Set every second face material to red material channel
                Brep brep = brepObject.BrepGeometry;
                for (int i = 0; i < brep.Faces.Count; i++)
                {
                    if (i % 2 == 0)
                        brep.Faces[i].MaterialChannelIndex = redChannelIndex;
                    else
                        brep.Faces[i].ClearMaterialChannelIndex();
                }

                // Commit changes
                brepObject.CommitChanges();
            }
            return Result.Success;
        }
    }
}
