import rhino3dm from 'rhino3dm'
let sphere
export async function setupCounter(element) {
  const rhino = await rhino3dm()
  let counter = 1
  const setCounter = (count) => {
    const sphere = new rhino.Sphere([0, 0, 0], count);
    counter = count
    element.innerHTML = `radius is ${counter} and diameter is ${sphere.diameter}`
  }
  element.addEventListener('click', () => setCounter(counter + 1))
  setCounter(1)
}
