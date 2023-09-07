using Rhino.FileIO;

File3dm file3dm = File3dm.Read("../../models/RenderContent.3dm");

Console.WriteLine("Number of objects in file {0}", file3dm.Objects.Count);
Console.WriteLine("Number of embedded files in file {0}", file3dm.EmbeddedFiles.Count());

foreach( var ef in file3dm.EmbeddedFiles )
{

    var fn = Path.GetFileName(ef.Filename);
    var tmpPath = Path.GetTempPath();
    var path = Path.Combine(tmpPath, fn);

    if(ef.SaveToFile(path))
        Console.WriteLine("Embedded File saved to: {0}", path);

}