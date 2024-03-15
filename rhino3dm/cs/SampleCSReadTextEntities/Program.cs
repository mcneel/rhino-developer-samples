using Rhino.FileIO;

Console.WriteLine("---Sample: Read Text Entities from file---");

var path = Directory.GetCurrentDirectory();

Console.WriteLine("Current Directory: {0}",path);

File3dm file3dm = new File3dm();

//the path will be different whether you are using dotnet run or debugging
if(path.Contains("net7.0")){
    file3dm = File3dm.Read("../../../../../models/textEntities_r8.3dm");
} else{
    file3dm = File3dm.Read("../../models/textEntities_r8.3dm");
} 

Console.WriteLine("Number of objects in file {0}", file3dm.Objects.Count);

foreach( var ro in file3dm.Objects )
{
    var te = ro.Geometry as Rhino.Geometry.TextEntity;
    if( te != null) {
        Console.WriteLine("-----------");
        Console.WriteLine("Plain text: {0}", te.PlainText);
        Console.WriteLine("Rich text: {0}", te.RichText);
        Console.WriteLine("Plain text with fields: {0}", te.PlainTextWithFields);
    }
}