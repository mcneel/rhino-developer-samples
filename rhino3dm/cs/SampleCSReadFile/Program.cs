// See https://aka.ms/new-console-template for more information
using Rhino.FileIO;

Console.WriteLine("Hello, World!");

//read file
var file3dm = File3dm.Read("../../../RectifiedArchimedeanSolids8.3dm");
Console.WriteLine("Number of objects in file {0}", file3dm.Objects.Count);

