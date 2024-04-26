using Grasshopper.Kernel;
using Grasshopper.Kernel.Components;
using Grasshopper.Kernel.Parameters;
using Grasshopper.Kernel.Types;
using Rhino.Geometry;
using System;
using System.Collections.Generic;
using System.Drawing;

namespace SampleGhTools.Components.Other
{
    public class SampleCustomPreviewComponent : GH_CustomPreviewComponent
    {
        private List<GH_CustomPreviewItem> _previewItems;
        private BoundingBox _previewBB;

        public override BoundingBox ClippingBox => _previewBB;
        public override bool IsBakeCapable => _previewItems.Count > 0;

        public SampleCustomPreviewComponent() : base()
        {
            Name = "SampleCustomPreviewComponent";
            NickName = "SampleCustomPreviewComponent";
            Category = "Test";
            SubCategory = "Samples";
        }

        protected override void RegisterInputParams(GH_Component.GH_InputParamManager inputs)
        {
            inputs.AddGeometryParameter("Geometries", "G", "Geometry to preview", GH_ParamAccess.item);

            var mat = new Param_OGLShader();
            mat.SetPersistentData(new GH_Material(Color.Azure));

            inputs.AddParameter(mat, "Material", "M", "The material override", GH_ParamAccess.item);
        }

        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            //No outputs
        }

        protected override void SolveInstance(IGH_DataAccess data)
        {
            IGH_GeometricGoo geometry = null;
            GH_Material material = null;

            if (data.GetData(0, ref geometry) && data.GetData(1, ref material) && geometry.IsValid)
            {
                if (geometry is IGH_PreviewData)
                {
                    var display_material = (material.Value == null) ? material.Value : new Rhino.Display.DisplayMaterial();

                    var previewItem = new GH_CustomPreviewItem
                    {
                        Geometry = (IGH_PreviewData)geometry,
                        Shader = display_material,
                        Colour = display_material.Diffuse,
                        Material = material
                    };

                    _previewItems.Add(previewItem);

                    _previewBB.Union(geometry.Boundingbox);
                }
                else
                {
                    AddRuntimeMessage(GH_RuntimeMessageLevel.Warning, geometry.TypeName + " does not support previews");
                }
            }
        }

        public override void DrawViewportMeshes(IGH_PreviewArgs args)
        {
            if (!Locked && _previewItems.Count > 0)
            { 
                if (Attributes.Selected)
                {
                    var pma = new GH_PreviewMeshArgs(args.Viewport, args.Display, args.ShadeMaterial_Selected, args.MeshingParameters);

                    foreach (GH_CustomPreviewItem pi in _previewItems)
                    {
                        pi.Geometry.DrawViewportMeshes(pma);
                    }
                }
                else
                {
                    foreach (GH_CustomPreviewItem pi in _previewItems)
                    {
                        //Use the item shader.
                        var pma = new GH_PreviewMeshArgs(args.Viewport, args.Display, pi.Shader, args.MeshingParameters);

                        pi.Geometry.DrawViewportMeshes(pma);
                    }
                }
            }
        }

       // [Obsolete]
        public override void AppendRenderGeometry(GH_RenderArgs args)
        {
            foreach (GH_CustomPreviewItem item in _previewItems)
            {
                item.PushToRenderPipeline(args);
            }
        }

        protected override void BeforeSolveInstance()
        {
            _previewItems = new List<GH_CustomPreviewItem>();
            _previewBB = BoundingBox.Empty;

            base.BeforeSolveInstance();
        }

        protected override System.Drawing.Bitmap Icon
        {
            get { return null; }
        }

        public override Guid ComponentGuid
        {
            get { return new Guid("79C587F7-7A0D-473D-A5AA-825F53742AA0"); }
        }
    }
}