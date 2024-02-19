import * as fs from 'fs' //only if running in node.js
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const buffer = fs.readFileSync('../../../models/nurbssurface.3dm')
const arr = new Uint8Array(buffer)
const file3dm = rhino.File3dm.fromByteArray(arr)

const objects = file3dm.objects()
const cnt = objects.count

console.log(`Number of Objects: ${cnt}`)



for ( let i = 0; i < cnt; i ++ ) {
    const object = objects.get( i )
    const brep = object.geometry()

    console.log(`Brep Vertices Count: ${brep.vertices().count}`)
    console.log(`Brep Vertex: ${brep.vertices().get(3).location}`)
    console.log(`Brep Vertex Index: ${brep.vertices().get(3).vertexIndex}`)
    console.log(`Brep Edge Count: ${brep.vertices().get(3).edgeCount}`)
    console.log(`Brep Edge Indices: ${brep.vertices().get(3).edgeIndices}`)
    console.log(`Brep Edge Index: ${brep.vertices().get(3).edgeIndices[1]}`)

    const surface = brep.surfaces().get( 0 )

    console.log( surface.domain( 0 ) ) //domain in u
    console.log( surface.domain( 1 ) ) //domain in v

    surface.setDomain( 0, [0,1] )
    surface.setDomain( 1, [0,1] )

    console.log( surface.domain( 0 ) ) //domain in u
    console.log( surface.domain( 1 ) ) //domain in v


    const nurbsSurface = surface.toNurbsSurface()

    const knotsU = nurbsSurface.knotsU()
    const knotsV = nurbsSurface.knotsV()
    console.log(knotsU.toList())
    console.log(knotsV.toList())

    /*
    const knots = geometry.knots()
    console.log(knots.get(5))
    const knotList = knots.toList()
    console.log(knotList)

    */

    const n_surface = rhino.NurbsSurface.create(3, nurbsSurface.isRational, nurbsSurface.orderU, nurbsSurface.orderV, nurbsSurface.points.countU, nurbsSurface.points.countV );
    n_surface.points = nurbsSurface.points;
    n_surface.knotsU = nurbsSurface.knotsU;
    n_surface.knotsV = nurbsSurface.knotsV;
    n_surface.setDomain(0, [0,1]); 
    n_surface.setDomain(1, [0,1]);

    file3dm.objects().add(n_surface, null);
    
}



const options = new rhino.File3dmWriteOptions()
options.version = 8
let buff = file3dm.toByteArrayOptions(options)
fs.writeFileSync("file.3dm", buff);

