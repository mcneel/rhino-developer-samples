import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const circle = new rhino.Circle(10)

const arc1 = new rhino.Arc(circle, Math.PI/2)
const arc2 = new rhino.Arc({ X: 0, Y: 0, Z: 0 }, 10, Math.PI/2)
const arc3 = new rhino.Arc({ X: 0, Y: 0, Z: 0 }, { X: 1, Y: 1, Z: 1 }, { X: 1, Y: 2, Z: 3 })

info(arc1, 'Arc 1')
info(arc2, 'Arc 2')
info(arc3, 'Arc 3')

function info(arc, name) {

    console.log(`----- ${name} -----`)

    // Arc is a value type: its members are functions on rhino.Arc that take the arc first
    console.log(`${name} IsValid: ${rhino.Arc.isValid(arc)}`)
    console.log(`${name} IsCircle: ${rhino.Arc.isCircle(arc)}`)
    console.log(`${name} Radius: ${arc.Radius}`)
    console.log(`${name} Diameter: ${rhino.Arc.diameter(arc)}`)
    console.log(`${name} Center: ${JSON.stringify(rhino.Arc.center(arc))}`)
    console.log(`${name} Circumference: ${rhino.Arc.circumference(arc)}`)
    console.log(`${name} Length: ${rhino.Arc.Length(arc)}`)
    console.log(`${name} StartPoint: ${JSON.stringify(rhino.Arc.startPoint(arc))}`)
    console.log(`${name} MidPoint: ${JSON.stringify(rhino.Arc.midPoint(arc))}`)
    console.log(`${name} EndPoint: ${JSON.stringify(rhino.Arc.endPoint(arc))}`)
    console.log(`${name} AngleDomain: ${JSON.stringify(arc.AngleDomain)}`)
    console.log(`${name} StartAngle: ${rhino.Arc.startAngle(arc)}`)
    console.log(`${name} EndAngle: ${rhino.Arc.endAngle(arc)}`)
    console.log(`${name} AngleRadians: ${rhino.Arc.angle(arc)}`)
    console.log(`${name} StartAngleDegrees: ${rhino.Arc.startAngleDegrees(arc)}`)
    console.log(`${name} EndAngleDegrees: ${rhino.Arc.endAngleDegrees(arc)}`)
    console.log(`${name} AngleDegrees: ${rhino.Arc.angleDegrees(arc)}`)

    console.log(`${name} BoundingBox: ${JSON.stringify(rhino.Arc.boundingBox(arc))}`)
    console.log(`${name} PointAt: ${JSON.stringify(rhino.Arc.pointAt(arc, 0.1))}`)
    console.log(`${name} TangentAt: ${JSON.stringify(rhino.Arc.tangentAt(arc, 0.1))}`)
    console.log(`${name} ClosestParameter: ${rhino.Arc.closestParameter(arc, { X: 0, Y: 0, Z: 0 })}`)
    console.log(`${name} ClosestPoint: ${JSON.stringify(rhino.Arc.closestPoint(arc, { X: 0, Y: 0, Z: 0 }))}`)

    console.log()

}