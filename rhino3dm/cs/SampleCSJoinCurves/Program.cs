using System;
using Rhino.Geometry;
using Rhino.FileIO;
using System.Collections.Generic;

namespace SampleCSJoinCurves
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            //read file
            var file3dm = File3dm.Read("../../models/curves.3dm");
            Console.WriteLine("Number of objects in file {0}", file3dm.Objects.Count);

            //get objects

            var curves = new List<Curve>();

            foreach ( var obj in file3dm.Objects )
            {
                if ( obj.Geometry is Curve )
                {
                    Curve crv = (Curve)obj.Geometry;
                    curves.Add( crv );
                }
            }

            //call join

            var joined = Curve.JoinCurves(curves)[0];
            Console.WriteLine("Is curve valid {0}", joined.IsValid);

            //save new file

            File3dm newFile = new File3dm();
            newFile.Objects.AddCurve(joined);
            newFile.Write("newFile.3dm", null);
            
        }
    }
}
