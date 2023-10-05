import rhino3dm from 'rhino3dm'
const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const pc = new rhino.PointCloud()

pc.add([0,0,0])
pc.addPointNormal([0,0,0], [0,0,0])
pc.addPointColor([0,0,0], {r:255, g:0, b:0, a:0})
pc.addPointNormalColor([0,0,0], [0,0,1], {r:255, g:0, b:0, a:0})
pc.addPointValue([0,0,0], 1.234)
pc.addPointNormalColorValue([10,10,10], [1,1,1], {r:100, g:100, b:100, a:100}, 1.234)

console.log(`Point Cloud Length: ${pc.count}`)

const pts = pc.getPoints()
const cols = pc.getColors()
const norms = pc.getNormals()
const vals = pc.getValues()

console.log('----- POINTS -----')
console.log(pts)
console.log()

console.log('----- COLORS -----')
console.log(cols)
console.log()

console.log('----- NORMALS -----')
console.log(norms)
console.log()

console.log('----- VALUES -----')
console.log(vals)
console.log()


