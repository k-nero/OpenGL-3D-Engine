# OpenGL 3D Engine
### Installation
<h4 align="left">Dependencies</h4>
- stb single-file public domain libraries for C/C++.<br/>
- OpenGL core profile ver 4.2 or above.<br/>
- Assimp,  Open-Asset-Importer-Library Repository. Loads 40+ 3D-file-formats into one unified and clean data structure.<br/>
- OpenGL Mathematics (GLM)<br/>
- GLFW3 A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input<br/>
<h4 align="left">3D model</h4>
- 3D models are contained in ./models. <br/?
- place textures in ./textures
### Render a models
Create a global models as following 

```c++
Model model3D;
```

Initialize models before enter render loop

```c++
model3D = Model();
model3D.LoadModel("models/scene.obj");
```

Render models in ObjectRenderer(const Shader * shader)

```c++
model = translate(model, vec3(0.0f, -10.0f, 5.0f));
model = scale(model, vec3(50.0f, 50.0f, 50.0f));
shader->SetModel(model);
shinyMaterial.UseMaterial(uniformSpecularMaterial, uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
model3D.RenderModel(*shader);
```
