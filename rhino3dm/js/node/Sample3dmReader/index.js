// can use imports since in package.json we have set "type":"module"
import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

let file3dmpath = ""

process.argv.forEach(function (val, index, array) {
    if (index === 2) {
        file3dmpath = val
    }
})

const rhino = await rhino3dm()

const buffer = fs.readFileSync(file3dmpath);
const arr = new Uint8Array(buffer)
const file3dm = rhino.File3dm.fromByteArray(arr)
const objects = file3dm.objects();

const stats = {}

for ( let i = 0; i < objects.count; i ++ ) {

    const geometry = objects.get(i).geometry()
    const prop = geometry.objectType.constructor.name.substring(11)
    increment(stats, prop)

}

function increment(obj, prop) {

    if ( !Object.hasOwn(obj, prop) ) {
        obj[prop] = 0
    }
    obj[prop]++

}

console.log(stats)

// do something with geometry
