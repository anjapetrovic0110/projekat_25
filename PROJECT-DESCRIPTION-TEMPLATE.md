# Shadow Hall

168/2022 - Anja Petrović

Shadow Hall is an interactive 3D scene containing a statue inside a hall illuminated by three torches and directional
lighting. The application demonstrates multiple lighting types, point shadows, off-screen MSAA, framebuffer rendering, a
skybox, and an event-driven animation sequence.

## Controls

- W -> Move the camera forward
- S -> Move the camera backward
- A -> Move the camera left
- D -> Move the camera right
- Mouse movement -> Rotate the camera
- F -> Open or close the settings GUI
- E -> Start the timed animation sequence
- Escape -> Close the application

## Features

### Fundamental:

[x] Model with lighting
[x] Directional and point lighting with customizable colors and directional-light direction through the GUI
[x] E -> After 3 seconds, the torch lights turn off and the statue starts rotating -> After another 3 seconds, the torch
lights become red -> After another 9 seconds, the scene returns to its initial state

### Group A:

[ ] Frame-buffers with post-processing   
[x] Off-screen Anti-Aliasing
[ ] Parallax Mapping
[ ] Bloom with the use of HDR

### Group B:

[ ] Deferred Shading  
[x] Point Shadows
[ ] SSAO

### Engine improvement:

[x] Off-screen Anti-Aliasing implemented as a reusable `engine::graphics` component
[x] Point Shadows implemented as a reusable `engine::graphics` component

## Models:

Torch Stick:
https://sketchfab.com/3d-models/torch-stick-d8eadee1a5c14483aade99b1fe5bc150

Light Ball:
https://sketchfab.com/3d-models/light-ball-81d2919292fc438ba2c1513c4f77899a

Statue:
https://poly.pizza/m/b3L5PKB4fsx

Hall:
https://sketchfab.com/3d-models/egyptian-ruins-9a780c7a62fb438fa6fdbdfcfc38a484

## Textures

Torch textures: included with the Torch Stick model
https://sketchfab.com/3d-models/torch-stick-d8eadee1a5c14483aade99b1fe5bc150

Statue texture: included with the Statue model
https://poly.pizza/m/b3L5PKB4fsx

Hall textures: included with the Egyptian Ruins model
https://sketchfab.com/3d-models/egyptian-ruins-9a780c7a62fb438fa6fdbdfcfc38a484

Space skybox:
https://opengameart.org/content/space-skybox-1