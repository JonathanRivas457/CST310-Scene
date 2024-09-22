# CST310-Scene

## Data Load
Objects are loaded by reading the json and spawning cubes based on the characteristics defined there

## Controls

### Movement and Rotation
- **Arrow Keys:**
  - **Right Arrow**: Rotate the object 5 degrees to the right.
  - **Left Arrow**: Rotate the object 5 degrees to the left.
  - **Up Arrow**: Rotate the object 5 degrees upward.
  - **Down Arrow**: Rotate the object 5 degrees downward.

### Translation
- **W**: Move the object forward along the Z-axis.
- **S**: Move the object backward along the Z-axis.
- **A**: Move the object left along the X-axis.
- **D**: Move the object right along the X-axis.
- **Q**: Move the object up along the Y-axis.
- **E**: Move the object down along the Y-axis.

### Resizing
- **1**: Increase the object's width by 0.05 units.
- **2**: Decrease the object's width by 0.05 units.
- **3**: Increase the object's height by 0.05 units.
- **4**: Decrease the object's height by 0.05 units.
- **5**: Increase the object's length by 0.05 units.
- **6**: Decrease the object's length by 0.1 units.

### Object Selection
- **/**: Cycle through available objects (cubes) in the scene. The current object will be selected for transformation.

### Save Scene to JSON
- **.**: Save the properties of all cubes in the scene to a JSON file named `sceneObjects.json`. The JSON will include the type, position, rotation, and dimensions of each cube.

### Logging
- **L**: Log the current object's properties to the console.
