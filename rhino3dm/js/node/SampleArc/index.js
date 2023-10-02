import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

const circle = new rhino.Circle(10)

const arc1 = new rhino.Arc(circle, Math.PI/2)
const arc2 = new rhino.Arc([0,0,0], 10, Math.PI/2)
const arc3 = rhino.Arc.createFromPoints([0,0,0], [1,1,1], [1,2,3])

info(arc1, 'Arc 1')
info(arc2, 'Arc 2')
info(arc3, 'Arc 3')

function info(arc, name) {

    console.log(`----- ${name} -----`)

    console.log(`${name} IsValid: ${arc.isValid}`)
    console.log(`${name} IsCircle: ${arc.isCircle}`)
    console.log(`${name} Radius: ${arc.radius}`)
    console.log(`${name} Diameter: ${arc.diameter}`)
    console.log(`${name} Center: ${arc.center}`)
    console.log(`${name} Circumference: ${arc.circumference}`)
    console.log(`${name} Length: ${arc.length}`)
    console.log(`${name} StartPoint: ${arc.startPoint}`)
    console.log(`${name} MidPoint: ${arc.midPoint}`)
    console.log(`${name} EndPoint: ${arc.endPoint}`)
    console.log(`${name} AngleDomain: ${arc.angleDomain}`)
    console.log(`${name} StartAngle: ${arc.startAngle}`)
    console.log(`${name} EndAngle: ${arc.endAngle}`)
    console.log(`${name} AngleRadians: ${arc.angleRadians}`)
    console.log(`${name} StartAngleDegrees: ${arc.startAngleDegrees}`)
    console.log(`${name} EndAngleDegrees: ${arc.endAngleDegrees}`)
    console.log(`${name} AngleDegrees: ${arc.angleDegrees}`)

    console.log(`${name} BoundingBox: ${arc.boundingBox()}`)
    console.log(`${name} PointAt: ${arc.pointAt(0.1)}`)
    console.log(`${name} TangentAt: ${arc.tangentAt(0.1)}`)
    console.log(`${name} ClosestParameter: ${arc.closestParameter([0,0,0])}`)
    console.log(`${name} ClosestPoint: ${arc.closestPoint([0,0,0])}`)

    console.log()

}