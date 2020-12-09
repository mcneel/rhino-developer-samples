const fs = require('fs')
const rhino3dm = require('rhino3dm')

let file3dmpath = ""

process.argv.forEach(function (val, index, array) {
    if (index === 2) {
        file3dmpath = val
    }
})

rhino3dm().then((rhino)=>{

    let buffer = fs.readFileSync(file3dmpath);
    let arr = new Uint8Array(buffer)
    let file3dm = rhino.File3dm.fromByteArray(arr)
    let objects = file3dm.objects();

    for(var i=0; i<objects.count; i++) {
        let geometry = objects.get(i).geometry()
        console.log(geometry)
      }

    // do something with geometry
})