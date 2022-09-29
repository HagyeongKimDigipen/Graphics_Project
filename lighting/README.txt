How to use parts of your user interface that is NOT specified in the assignment description. 

The user interface may be used through the upper left imgui window. (you can change imgui window size by dragging bottom right side)

1) In the shader & normal category, you can change the shader and check the vertex and face normal.

2) Global variables can be changed in the constant category.

3) In the matrial category, you can set uv type, cpu, and gpu calculation modes.

4) 3 scenarios can be changed in the scenario category.

5) You can change each light source properties and the number of active light sources in the light source category.

6) Changing certain conditions in the GUI control sometimes resets other conditions as well. It's not that the function is not made, but the gui setting is wrong. 
Please set the changed part again.

b. Any assumption that you make on how to use the application that, if violated, might cause the application to fail. 

Link errors can be occured in relation to the newly added stbi image library. 
=> Check if #defineSTB_IMAGE_IMPLEMENT is added to Texture.cpp or h. if you have still error after adding that please contact me by email.

An error may occur when loading an object.
=> This is an object file in a form that is not yet supported. Please use the object file added to the imgui.

Changing certain conditions in the GUI control sometimes resets other conditions as well.  
=> It's not that the function is unfinished but it can be Gui setting problem. Please set the changed part again.

If you toggle multiple functions in ImGui, there may be errors that do not work. There is a high probability of an error about adding a function that made by me to ImGui. 
=> Check the code to see if require element been implemented. 


- Phong illumination model
	- Phong lighting
	- Phong shading 
	- Blinn shading
	- reload shader(There's no reload button, but if you press the shader, it will be automatically reloaded.)

- Light Setup
    - 16 light sources with only a diffuse color that rotates round center object.
    - Point lights
    - Directional lights
    - Spotlights lights
    - 3 Scenario
    - Uniform block 
    - quad plane

- Material Setup
    	- texture coordinates for model
   	- Texture may have data in the range [0, 255] 
    	- Mapping from texture colors to material coefficients  
	- CPU calculation
	- GPU calculaiton

    - Texture Entity
	- vertex position
	- vertex normal

- Light properitex
	- Global
		- The number of active lights.
		- attenuation coefficients
		- atmospherc(fog) color
		- Global ambient color

	- Per-light values
		- Light type(point, spot or directional)
		- Ambient, diffuse and specular color
		- Inner and outer angle for spot light  
		- Spot falloff   
-GUI
   - changing the number of active lights
   - select the light types
   - toggle secenarios

OS: window10
GPU:NVIDIA GeForce MX250
OpenGL Driver version: 4.6.0 -Build 26.20.100.7870

