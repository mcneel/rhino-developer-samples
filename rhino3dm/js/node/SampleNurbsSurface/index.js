import * as fs from 'fs' //only if running in node.js
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const rational = false
const dim = 3
const u_cv_count = 6
const v_cv_count = 6
const u_degree = 3
const v_degree = 3
const u_order = u_degree + 1
const v_order = v_degree + 1

const ns = rhino.NurbsSurface.create(dim, rational, u_order, v_order, u_cv_count, v_cv_count)

//generate knots
ns.knotsU().createUniformKnots(1.0)
ns.knotsV().createUniformKnots(1.0)

//create points
for (let u = 0; u < u_cv_count; u++) {
    for (let v = 0; v < v_cv_count; v++) {
        ns.points().set(u, v, [u, v, u+v, 1.0])
    }
}

console.log('Is Surface Valid: ' + ns.isValid)

const doc = new rhino.File3dm()

const layer_index = doc.layers().addLayer( 'NurbsSurface', { r: 255, g: 0, b: 0, a: 255 } )
const oa = new rhino.ObjectAttributes()
oa.layerIndex = layer_index

const uuid = doc.objects().add(ns, oa);
console.log('id of object added to doc: ' + uuid)

const options = new rhino.File3dmWriteOptions()
options.version = 8
let buff = doc.toByteArrayOptions(options)

fs.writeFileSync("file.3dm", buff);