import rhino3dm from 'rhino3dm'
const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const pc = new rhino.PointCloud()

pc.add({ X: 0, Y: 0, Z: 0 })
pc.add({ X: 0, Y: 0, Z: 0 }, { X: 0, Y: 0, Z: 0 })
pc.add({ X: 0, Y: 0, Z: 0 }, {r:255, g:0, b:0, a:0})
pc.add({ X: 0, Y: 0, Z: 0 }, { X: 0, Y: 0, Z: 1 }, {r:255, g:0, b:0, a:0})
pc.addPointValue({ X: 0, Y: 0, Z: 0 }, 1.234)
pc.add({ X: 10, Y: 10, Z: 10 }, { X: 1, Y: 1, Z: 1 }, {r:100, g:100, b:100, a:100}, 1.234)

console.log(`Point Cloud Length: ${pc.count}`)

const pts = pc.getPoints()
const cols = pc.getColors()
const norms = pc.getNormals()
const vals = pc.getPointValues()

console.log(pts)
console.log(cols)
console.log(norms)
console.log(vals)