import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

//create model
const model = new rhino.File3dm()

const b = fs.readFileSync('./rhinologo.png');
const a = new Uint8Array(b)

const ef = rhino.EmbeddedFile.fromByteArray(a)
console.log(ef.length)

const ef_fileName = 'rhinologo.png'
ef.fileName = ef_fileName
console.log('Filename: ' + ef.fileName)
model.embeddedFiles().add(ef)

console.log('how many embedded files: ' + model.embeddedFiles().count)

const texture = new rhino.Texture()
texture.fileName = ef_fileName

const material = new rhino.Material()
material.toPhysicallyBased()
material.physicallyBased().baseColor = { r: 1, g: 0, b: 0, a: 0 }
material.physicallyBased().metallic = 0.7
material.physicallyBased().roughness = 0.5
material.setBitmapTexture(texture)

const index = model.materials().add(material)

const sphere = rhino.Brep.createFromSphere( new rhino.Sphere( [ 0,0,0 ], 10 ) )

const oa = new rhino.ObjectAttributes()
oa.materialSource = rhino.ObjectMaterialSource.MaterialFromObject
oa.materialIndex = index

model.objects().add(sphere, oa)

//save model
let opts = new rhino.File3dmWriteOptions()
opts.version = 8
let buffer = model.toByteArrayOptions(opts)
fs.writeFileSync('materialWithEmbeddedFile_js.3dm', buffer)
