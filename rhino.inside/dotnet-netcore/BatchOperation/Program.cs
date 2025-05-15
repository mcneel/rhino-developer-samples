using Rhino.Runtime.InProcess;

namespace BatchOperation
{
    class Program
    {
        public static string PathToMyFiles { get; set; } = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "files");

        static Program()
        {
            RhinoInside.Resolver.Initialize();
            Console.WriteLine($"Loading Rhino @ {RhinoInside.Resolver.RhinoSystemDirectory}");
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Please wait while Rhino is starting...");
            try
            {
                RhinoProgram.BatchOperation();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
            }
            finally
            {
                Console.WriteLine("press any key to exit");
                Console.ReadKey();
            }
        }

        static class RhinoProgram
        {
            public static void BatchOperation()
            {
                using (new RhinoCore())
                {
                    var files3dm = Directory.GetFiles(PathToMyFiles, "*.3dm");
                    for (int i = 0; i < files3dm.Length; i++)
                    {
                        var doc = Rhino.RhinoDoc.OpenHeadless(files3dm[i]);
                        var obj = doc.Objects.FindByLayer(doc.Layers.CurrentLayer).First();
                        Console.WriteLine($"File number {i +1} has {doc.Objects.Count} objects in the model");
                        Console.WriteLine($"First element found has type: {obj.GetType().Name}");
                    }
                }
            }
        }
    }
}
