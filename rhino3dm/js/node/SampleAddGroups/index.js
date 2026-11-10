import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()

const doc = new rhino.File3dm()

const groupIndex = doc.allGroups.addGroup()

for( let i = 1; i < 10; i ++ ) {
    
    const circle = new rhino.Circle(i)
    const id = doc.objects.add(rhino.Circle.toNurbsCurve(circle), null)
    const ro = doc.objects.findId(id)
    ro.attributes.addToGroup(groupIndex)
    
}

const options = new rhino.File3dmWriteOptions()
options.version = 8
let buffer = doc.toByteArray(options)
fs.writeFileSync("fileWithGroups.3dm", buffer);
