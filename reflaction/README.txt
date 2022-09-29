The user interface may be used through the upper left imgui window. 

1) In the normal category, you can check the vertex and face normal.

2) Global variables can be changed in the constant category.

3) 3 scenarios can be changed in the scenario category.

4) You can change each light source properties and the number of active light sources in the light source category.

5) You can change the type of Environment Mapping in Environment Mapping category.

6) You can enable phong shading by toggling using phong shading inside Environment Mapping category.

7) If you select the Environment Mapping type as refractive or fresnel, you can change the refractive index, material type and chromatic abberration in the refraction category.

8) If the Environment Mapping type is set to fresenel, you can adjust the fresenel power in the Environment Mapping category.

9) You can adjust the camera position in the Camera Control category.

b. Any assumption that you make on how to use the application that, if violated, might cause the application to fail. 


1. Construct the 6 texture maps for cub mapping algorithm 
	- use FBO
	- position the camera at the center of the reflecting / refracting object
	- Aim the camera along on fo the wolrd space axis
	- Set the camera field of view to 90 and ratio to 1
	- Render the scene to texture map using Frame Buffer Object

2. Environment mapping algorithm in the fragment shader
	- calculate the reflection / refraction of the view vector with respect to the fragment normal
	- use the transformed reflection / refraction vector to choose the cube map side and to calculate the texture coordinate
	- split refraction computation into multiple bins based on refraction dispersion proportional to the wavelength of light. Use bins(RGB) for three wavelengths of light

3. Scene setup
	- main object is rendered with environment map on it
	- remove the bottom plane
	- create skybox
		- always centered at the camera position 
		- sky box is rendered as cube
		- render it first with z - buffer turned off when rendering it
		- Turn z - buffer back on after rendering the sky box
	- render the 6 side of the cube maps on screen in this order (left, right, bottom, top, back, front)

4. User input
	- Environment Mapping
		- Only reflection
		- Only refraction
		- Combination of both
	- Phong shading + Environment mapping

5. User Interactivity
	- Use user input for the object refractive index and assume that the camera is in air
	- Experiment with the values below to implement different materials 


OS: window10
GPU:NVIDIA GeForce MX250
OpenGL Driver version: 4.6.0 -Build 26.20.100.7870
