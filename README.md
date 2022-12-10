# Ray Tracer

This is a real-time Ray Tracer. It uses a Application template called Walnut that utilizes the DearImGUI library and the Vulkan SDK

## Getting Started
Once cloned go into scripts and run setup.bat, this will generate your solution. Once you have opened your solution all that is needed is to run it and it will start rendering. The Render starts with two Spheres one that acts as the floor and the other acting as the main focus of the camera. The application will also generate a list of options:
-Render(Renders a frame)
-Reset(Resets the Accumulation Index used if there is ghosting)
-Accumulation(Renders out the noise over time)
-Add Sphere
-Add Material
-Material Roughness
-Material color
-Material metallic factor(Metallic factor is not implemented yet), 
-Sphere postion
-Sphere size
-Light direction

##Required files
-Vulkan SDK is require to run this program
