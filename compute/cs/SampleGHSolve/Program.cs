using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Rhino.Compute;
using Newtonsoft.Json;

namespace SampleGHSolve
{
    class Program
    {
        static void Main(string[] args)
        {
            ComputeServer.WebAddress = "http://localhost:5000/"; // port 5000 is rhino.compute, 8081 is compute.geometry
            // ComputeServer.ApiKey = "";

            var definitionName = "SpikyThing.gh";
            var definitionPath = System.Reflection.Assembly.GetExecutingAssembly().Location;
            definitionPath = Path.GetDirectoryName(definitionPath);
            definitionPath = Path.Combine(definitionPath, definitionName);

            var trees = new List<GrasshopperDataTree>();
          
            var value1 = new GrasshopperObject(10);
            var param1 = new GrasshopperDataTree("Radius");
            param1.Add("0", new List<GrasshopperObject> { value1 } );
            trees.Add(param1);

            var value2 = new GrasshopperObject(35);
            var param2 = new GrasshopperDataTree("Count");
            param2.Add("0", new List<GrasshopperObject> { value2 });
            trees.Add(param2);

            var value3 = new GrasshopperObject(35);
            var param3 = new GrasshopperDataTree("Length");
            param3.Add("0", new List<GrasshopperObject> { value3 });
            trees.Add(param3);

            var result = Rhino.Compute.GrasshopperCompute.EvaluateDefinition(definitionPath, trees);
            var data = result[0].InnerTree.First().Value[0].Data;
            var parsed = JsonConvert.DeserializeObject<Dictionary<string, string>>(data);
            var obj = Rhino.FileIO.File3dmObject.FromJSON(parsed);

            Console.WriteLine("Result: {0}", obj.GetType());

            Console.WriteLine("Press any key to continue...");
            Console.ReadKey();


        }
    }
}
