import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const model = new rhino.File3dm()
model.applicationName = 'nodejs'
model.applicationDetails = 'rhino-developer-samples'
model.applicationUrl = 'https://rhino3d.com'
model.startSectionComments = 'hello'

const circle = new rhino.Circle(10.0)

const bbox = new rhino.BoundingBox(-10,-10,0, 10,10,20)
const box = new rhino.Box(bbox)
const extrusion = rhino.Extrusion.createBoxExtrusion( box, false )

const objects = []
objects.push(circle.toNurbsCurve(), extrusion)

const attributes = []
const oa = new rhino.ObjectAttributes()
attributes.push(oa, oa)

const point = [0,0,0]
const index = model.instanceDefinitions().add('test', 'an idef', 'https://www.rhino3d.com', 'tag', point, objects, attributes)


const idef = model.instanceDefinitions().findIndex(index)
const xform = new rhino.Transform(100)
const iref = new rhino.InstanceReference(idef.id, xform)
model.objects().addInstanceObject(iref, null)

let opts = new rhino.File3dmWriteOptions()
opts.version = 8
let buffer = model.toByteArrayOptions(opts)
fs.writeFileSync('idef_js.3dm', buffer)




