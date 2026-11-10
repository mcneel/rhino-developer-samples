import rhino3dm from 'rhino3dm'
let sphere
export async function setupCounter(element) {
  const rhino = await rhino3dm()
  let counter = 1
  const setCounter = (count) => {
    const sphere = new rhino.Sphere({ X: 0, Y: 0, Z: 0 }, count);
    counter = count
    element.innerHTML = `radius is ${counter} and diameter is ${rhino.Sphere.diameter(sphere)}`
  }
  element.addEventListener('click', () => setCounter(counter + 1))
  setCounter(1)
}
