import rhino3dm from 'rhino3dm'
const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const file3dm = new rhino.File3dm()

console.log(`Render Content Count: ${[...file3dm.renderMaterials].length}`)

// rhino3dm.js 9.2 cannot create render content: File3dmRenderMaterial has no constructor, as in RhinoCommon.
// This sample reads a render material from a model instead.
const model = await fetch( '../../models/RenderContent.3dm' )
const source = rhino.File3dm.fromByteArray( new Uint8Array( await model.arrayBuffer() ) )
const rm = [...source.renderMaterials][0]

console.log(`Render Content kind: ${rm.kind}`)
console.log(`Render Content Is Child: ${rm.isChild}`)
console.log(`Render Content isTopLevel: ${rm.isTopLevel}`)
console.log(`Render Content id: ${rm.id}`)
console.log(`Render Content typeName: ${rm.typeName}`)
console.log()
console.log(`Render Content parent: ${rm.parent}`)
console.log(`Render Content children: ${rm.children.length}`)
console.log(`Render Content topLevel: ${rm.topLevel.id}`)
//console.log(`Render Content setTypeName: ${rm.setTypeName()}`)
//console.log(`Render Content childSlotOn: ${rm.childSlotOn()}`)
//console.log(`Render Content setChildSlotOn: ${rm.setChildSlotOn()}`)
//console.log(`Render Content childSlotAmount: ${rm.childSlotAmount()}`)
console.log(`Render Content XML: ${rm.xML(true)}`)
//console.log(`Render Content setXML: ${rm.setXML()}`)
//console.log(`Render Content setChild: ${rm.setChild()}`)
//console.log(`Render Content findChild: ${rm.findChild()}`)
//console.log(`Render Content deleteChild: ${rm.deleteChild()}`)
//console.log(`Render Content getParameter: ${rm.getParameter()}`)
//console.log(`Render Content setParameter: ${rm.setParameter()}`)
console.log()
console.log(`Render Content name: ${rm.name}`)
console.log(`Render Content typeId: ${rm.typeId}`)
console.log(`Render Content renderEngineId: ${rm.renderEngineId}`)
console.log(`Render Content plugInId: ${rm.plugInId}`)
console.log(`Render Content notes: ${rm.notes}`)
console.log(`Render Content tags: ${rm.tags}`)
console.log(`Render Content groupId: ${rm.groupId}`)
console.log(`Render Content hidden: ${rm.hidden}`)
console.log(`Render Content reference: ${rm.reference}`)
console.log(`Render Content autoDelete: ${rm.autoDelete}`)
console.log(`Render Content childSlotName: ${rm.childSlotName}`)
console.log()

rm.name = 'someMaterial'
// In rhino3dm.js 9.2 the other render content properties are read-only, as in RhinoCommon:
//rm.notes = 'the beginning of some great notes'
//rm.tags = 'some tags'
//rm.hidden = true
//rm.reference = true
//rm.autoDelete = true
//rm.childSlotName = 'someNameChild'
//rm.renderEngineId = '12345678-9999-9999-9999-999999999999'
//rm.plugInId = '12345678-1111-1111-9999-999999999999'
//rm.groupId = '12345678-1111-1111-0000-999999999999'

console.log(`Render Content name: ${rm.name}`)
console.log(`Render Content notes: ${rm.notes}`)
console.log(`Render Content tags: ${rm.tags}`)
console.log(`Render Content hidden: ${rm.hidden}`)
console.log(`Render Content renderEngineId: ${rm.renderEngineId}`)
console.log(`Render Content plugInId: ${rm.plugInId}`)
console.log(`Render Content groupId: ${rm.groupId}`)

console.log()

file3dm.renderMaterials.add(rm)
console.log(`Render Content Count: ${[...file3dm.renderMaterials].length}`)

const _rm = [...file3dm.renderMaterials][0]

console.log(`Render Content kind: ${_rm.kind}`)
console.log(`Render Content Is Child: ${_rm.isChild}`)
console.log(`Render Content isTopLevel: ${_rm.isTopLevel}`)
console.log(`Render Content id: ${_rm.id}`)
console.log(`Render Content typeName: ${_rm.typeName}`)
console.log()
console.log(`Render Content parent: ${_rm.parent}`)
console.log(`Render Content children: ${_rm.children.length}`)
console.log(`Render Content topLevel: ${_rm.topLevel.id}`)
//console.log(`Render Content setTypeName: ${_rm.setTypeName()}`)
//console.log(`Render Content childSlotOn: ${_rm.childSlotOn()}`)
//console.log(`Render Content setChildSlotOn: ${_rm.setChildSlotOn()}`)
//console.log(`Render Content childSlotAmount: ${_rm.childSlotAmount()}`)
console.log(`Render Content XML: ${_rm.xML(true)}`)
console.log(_rm.xML(false))
//console.log(`Render Content setXML: ${_rm.setXML()}`)
//console.log(`Render Content setChild: ${_rm.setChild()}`)
//console.log(`Render Content findChild: ${_rm.findChild()}`)
//console.log(`Render Content deleteChild: ${_rm.deleteChild()}`)
console.log(`Render Content getParameter: ${_rm.getParameter('transparency')}`)
//console.log(`Render Content setParameter: ${_rm.setParameter()}`)
console.log()
console.log(`Render Content name: ${_rm.name}`)
console.log(`Render Content typeId: ${_rm.typeId}`)
console.log(`Render Content renderEngineId: ${_rm.renderEngineId}`)
console.log(`Render Content plugInId: ${_rm.plugInId}`)
console.log(`Render Content notes: ${_rm.notes}`)
console.log(`Render Content tags: ${_rm.tags}`)
console.log(`Render Content groupId: ${_rm.groupId}`)
console.log(`Render Content hidden: ${_rm.hidden}`)
console.log(`Render Content reference: ${_rm.reference}`)
console.log(`Render Content autoDelete: ${_rm.autoDelete}`)
console.log(`Render Content childSlotName: ${_rm.childSlotName}`)
console.log()

// rhino3dm.js 9.2 has no copy constructor for render content; adding it to a model again makes a copy with a new id
file3dm.renderMaterials.add(rm)
const newrm = [...file3dm.renderMaterials][1]
console.log('copy (rm)')
console.log(`Render Content name: ${newrm.name}`)
console.log(`Render Content typeId: ${newrm.typeId}`)
console.log(`Render Content renderEngineId: ${newrm.renderEngineId}`)
console.log(`Render Content plugInId: ${newrm.plugInId}`)
console.log(`Render Content notes: ${newrm.notes}`)
console.log(`Render Content tags: ${newrm.tags}`)
console.log(`Render Content groupId: ${newrm.groupId}`)
console.log(`Render Content hidden: ${newrm.hidden}`)
console.log(`Render Content reference: ${newrm.reference}`)
console.log(`Render Content autoDelete: ${newrm.autoDelete}`)
console.log(`Render Content childSlotName: ${newrm.childSlotName}`)
console.log()

console.log('toMaterial')

const material = _rm.toMaterial()

console.log(`Material transparency: ${material.transparency}`)