import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()

const sphere = new rhino.Sphere({ X: 0, Y: 0, Z: 0 }, Math.random() * 10)
console.log(rhino.Sphere.diameter(sphere))

alert(`the sphere diameter is ${rhino.Sphere.diameter(sphere)}`)