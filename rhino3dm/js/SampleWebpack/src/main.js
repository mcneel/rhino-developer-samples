import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()

const sphere = new rhino.Sphere([0,0,0], Math.random() * 10)
console.log(sphere.diameter)

alert(`the sphere diameter is ${sphere.diameter}`)