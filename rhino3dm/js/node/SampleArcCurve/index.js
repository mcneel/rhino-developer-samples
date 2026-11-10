import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const circle = new rhino.Circle(10)
const arc1 = new rhino.Arc(circle, Math.PI/2)

const ac1 = new rhino.ArcCurve(arc1)
const ac2 = new rhino.ArcCurve(arc1, 0, 0.2)
const ac3 = new rhino.ArcCurve(circle)
const ac4 = new rhino.ArcCurve(circle, 0, 0.2)
const ac5 = new rhino.ArcCurve(ac1)

info(ac1, 'ArcCurve 1')
info(ac2, 'ArcCurve 2')
info(ac3, 'ArcCurve 3')
info(ac4, 'ArcCurve 4')
info(ac5, 'ArcCurve 5')

function info(arc, name) {

    console.log(`----- ${name} -----`)
    console.log(`${name} IsCompleteCircle: ${arc.isCompleteCircle}`)
    console.log(`${name} Radius: ${arc.radius}`)
    console.log(`${name} AngleRadians: ${arc.angleRadians}`)
    console.log(`${name} AngleDegrees: ${arc.angleDegrees}`)
    
    console.log(`${name} Arc: ${JSON.stringify(arc.arc)}`)

    console.log()

}