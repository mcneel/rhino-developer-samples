// See https://aka.ms/new-console-template for more information
using Rhino.FileIO;

Console.WriteLine("Hello, World!");

var path = Directory.GetCurrentDirectory();

File3dm file3dm = new File3dm();

//the path will be different whether you are using dotnet run or debugging
if(path.Contains("net7.0")){
    file3dm = File3dm.Read("../../../RectifiedArchimedeanSolids8.3dm");
} else{
    file3dm = File3dm.Read("RectifiedArchimedeanSolids8.3dm");
} 

Console.WriteLine("Number of objects in file {0}", file3dm.Objects.Count);

