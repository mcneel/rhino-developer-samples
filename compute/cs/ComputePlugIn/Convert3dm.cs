//using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HelloRhinoCompute
{
    static class Convert3dm
    {
        public static string Convert3dmToPdf(string data)
        {
            var decodedData = Convert.FromBase64String(data);
            var file3dm = Rhino.FileIO.File3dm.FromByteArray(decodedData);

            // create headless doc and add geometry from 3dm file
            var doc = Rhino.RhinoDoc.CreateHeadless(null);
            foreach (var obj in file3dm.Objects)
            {
                doc.Objects.Add(obj.Geometry, obj.Attributes);
            }

            // set "front" view for export
            var view = doc.Views.Add("view1", Rhino.Display.DefinedViewportProjection.Front, new System.Drawing.Rectangle(0,0,1,1), false);
            doc.Views.ActiveView = view;

            // export!
            var id = Guid.NewGuid();
            string path = Path.GetTempPath();
            string filename = Path.Combine(path, "convert3dm_" + id + ".pdf");
            
            if (!doc.Export(filename))
            {
                throw new InvalidOperationException("Export failed");
            }
            
            byte[] bytes = File.ReadAllBytes(filename);
            return Convert.ToBase64String(bytes);
        }

        public static string Convert3dmToDwg(string data, Guid layerId)
        {
            var decodedData = Convert.FromBase64String(data);
            var file3dm = Rhino.FileIO.File3dm.FromByteArray(decodedData);
            
            // get layer from id
            var layer = file3dm.AllLayers.FindId(layerId);
            if (layer == null)
            {
                throw new InvalidOperationException($"Couldn't find layer with id: {layerId}");
            }

            // create headless doc and add objects from layer
            var doc = Rhino.RhinoDoc.CreateHeadless(null);
            foreach (var obj in file3dm.Objects.FindByLayer(layer))
            {
                doc.Objects.Add(obj.Geometry, obj.Attributes);
            }

            // export!
            var id = Guid.NewGuid();
            string path = Path.GetTempPath();
            string filename = Path.Combine(path, "convert3dm_" + id + ".dwg");

            if (!doc.Export(filename))
            {
                throw new InvalidOperationException("Something went wrong");
            }

            byte[] bytes = File.ReadAllBytes(filename);
            return Convert.ToBase64String(bytes);
        }
    }
}
