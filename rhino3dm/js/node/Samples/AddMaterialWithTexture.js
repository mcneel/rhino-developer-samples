import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

//create model
const model = new rhino.File3dm()

const b = fs.readFileSync('./rhinologo.png');
const a = new Uint8Array(b)

// name the embedded file as the texture refers to it, then add it to the model
const ef_fileName = 'rhinologo.png'
const ef = rhino.File3dmEmbeddedFile.fromByteArray(a, ef_fileName)
console.log('Length: ' + ef.length)
console.log('Filename: ' + ef.filename)

model.embeddedFiles.add(ef)
console.log('how many embedded files: ' + model.embeddedFiles.count)

const texture = new rhino.Texture()
texture.fileName = ef_fileName

const material = new rhino.Material()
material.toPhysicallyBased()
material.physicallyBased.baseColor = { R: 1, G: 0, B: 0, A: 0 }
material.physicallyBased.metallic = 0.7
material.physicallyBased.roughness = 0.5
material.setBitmapTexture(texture)

const index = model.materials.addMaterial(material)

const sphere = rhino.Brep.createFromSphere( new rhino.Sphere( { X: 0, Y: 0, Z: 0 }, 10 ) )

const oa = new rhino.ObjectAttributes()
oa.materialSource = rhino.ObjectMaterialSource.MaterialFromObject
oa.materialIndex = index

model.objects.add(sphere, oa)

//save model
let opts = new rhino.File3dmWriteOptions()
opts.version = 8
let buffer = model.toByteArray(opts)
fs.writeFileSync('materialWithEmbeddedFile_js.3dm', buffer)
