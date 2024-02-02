# OpenGL 3D Engine
### Installation
### Dependencies
Install Dependencies with vcpkg
In case you don't have vcpkg installed, you can install it from [here]( https://github.com/microsoft/vcpkg) and add vcpkg to your PATH.
Then navigate to the root directory and run the following command to install the dependencies.

```sh 
vcpkg install
```


### 3D Model

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
