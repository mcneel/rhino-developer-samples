import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()

const doc = new rhino.File3dm()

const g = new rhino.Group()
doc.groups().add(g)

for( let i = 1; i < 10; i ++ ) {
    
    const circle = new rhino.Circle(i)
    doc.objects().add(circle.toNurbsCurve(), null)
    const ro = doc.objects().get(i - 1)
    ro.attributes().addToGroup(0)
    
}

const options = new rhino.File3dmWriteOptions()
options.version = 8
let buffer = doc.toByteArrayOptions(options)
fs.writeFileSync("fileWithGroups.3dm", buffer);