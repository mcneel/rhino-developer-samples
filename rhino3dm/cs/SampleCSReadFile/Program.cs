using Rhino.FileIO;

// note: the 3dm file is copied to the output directory when this project is built

File3dm file3dm = new File3dm();
file3dm = File3dm.Read("RectifiedArchimedeanSolids8.3dm");
Console.WriteLine("Number of objects in file {0}", file3dm.Objects.Count);

