# Learning OpenGL

This repository contains a C++ application made for testing OpenGL
functions. It was developed alongside a [OpenGL YouTube tutorial](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2) by *The Cherno*.

The purpose of this project was to gain familiarity with OpenGL and GPU 
programming.

## Features

### Core functionality
Core OpenGL functionality is abstracted into relatively easy-to-use classes.
  > See the [`TestTexture2D.cpp`](./LearningOpenGL/src/tests/TestTexture2D.cpp) file for an example on using the abstracted classes to render
  > quads with a texture.

- **Display** - handles window creation and deletion with GLFW3. This is also where the OpenGL context is initialized.
  1. Create a *Display* object.
  2. Use the `.WindowShouldClose()` method to keep a while loop iterating as long as the window is open.
  3. Use the `.EndFrame()` method in each loop to swap the buffers (display the frame) and poll events.

- **GLErrorManager** - adds a macro for wrapping every OpenGL function in
  error-handling.
  1. Wrap an OpenGL function in `GLCall( <glFunction> )`.
  2. If the function causes an error, it will call a debug break.

- **VertexBufferLayout** - handles the specifications for vertex attributes. By pushing data types to the layout, this class takes care
of figuring out the vertex attribute stride.
  
  1. Create a *VertexBufferLayout* and push data types to it to define a vertex.
  2. When creating Attribute Pointers with OpenGL, reference the layout's list of attributes. Each *VertexBufferAttribute* has a
     - **type** - GLEnum for the attribute type.
     - **count** - number of values in this attribute.
     - **normalized** - GLEnum for whether this attribute should be normalized.
  3. Use the *VertexBufferLayout*'s stored information for the **stride**.

- **IndexBuffer** - stores the reference to the OpenGL index buffer, which specifies how the vertices are organized into primitives.
  > When you bind an Index Buffer in OpenGL, it becomes part of the currently bound Vertex Array Object's state, so this abstraction isn't
  > necessarily optimal.

- **VertexBuffer** - stores the reference to the OpenGL vertex buffer, containing all of the vertices.
  > This abstraction, along with the *IndexBuffer*, mostly serves to 
  > connect the OpenGL instances on the GPU with objects on the CPU, particularly for object destruction.
  > In these object's destructors, the OpenGL methods to free up GPU memory
  > are also called, creating a 1-1 relationship between the GPU and CPU object.

- **VertexArrayObject** - stores the VAO and handles the creation of attribute pointers from a *VertexBuffer* and a *VertexBufferLayout*.
  1. Create a *VertexArrayObject*.
  2. Create a *VertexBuffer* with the raw vertex information.
  3. Create a *VertexBufferLayout* to specify how the vertices are organized.
  4. Call this *VertexArrayObject*'s `.AddBuffer(...)` method to create all of the attribute pointers to link vertices to this VAO.

- **Shader** - creates and stores a shader program based on filepaths for the vertex and fragment shader sourcecode in the constructor.

- **Renderer** - contains methods for issuing a draw call.
  1. Create and add a buffer to a *VertexArrayObject*.
  2. Create a *Shader*.
  3. Create an *IndexBuffer* to specify the primitives.
  4. Pass these 3 objects into the `.Draw(...)` function.
  > This draws every 3 indices as a `GL_TRIANGLE`, and it draws the entire
  > index buffer.

- **Texture** - wraps the creation and deletion of a `GL_TEXTURE_2D`.

### Test framework
A test framework is provided to create and switch between examples.
- **Test** - base class for any test projects to extend.
  1. Extend the *Test* class.
  2. Initialize the 'scene' in the constructor.
  3. Override OnRender() with any rendering code.
  4. Override OnImGuiRender() with any ImGui functions for interactive UI.
  5. Register the new class with the `Main.cpp` file's *TestMenu* so it can be selected at runtime.

- **TestMenu** - handles the list of *Test* classes that exist.
  > When a *Test* is selected, it will be created as a new heap object.
  >
  > When a *Test* is closed, the heap object will be deleted.
  >
  > At the end of the `Main.cpp` program, the *TestMenu* is also deleted.

### Tests
- **TestClearColor** - demonstrates OpenGL's clear color method.
- **TestTexture2D** - demonstrates rendering 2 Quads with a texture, 
  as well as the ability to move the location of the Quads individually.
  > Quads are moved through a uniform set between individual draw calls.

## Resources
### shaders
Currently just a basic vertex and fragment shader for rendering shapes
with a texture.

### textures
Example textures for the Texture2D test.

## Dependencies and Vendor
This project depends on 
- **OpenGL** (built into most computers)
- **GLFW** (creates the window and OpenGL context)
- **GLEW** (provides access to the OpenGL methods)

The vendor folder contains
- **GLM** - math library made with OpenGL compatibility in mind.
- **stb_image** - library for reading image files on the CPU.
- **imgui** - GUI library for creating simple and interactive debugging UIs.


## Future Work
I may or may not continue to develop in this environment with the Test framework.

My goal is to rewrite my [Computer Graphcis project](https://github.com/AlexWills37/ComputerGraphics) with OpenGL and shaders, and to use shaders
to implement the remaining chapters of Gabriel Gambetta's [Computer Graphics textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/) (Shading and Textures).

From there, I would like to explore using shaders and rendering specifications in existing game engines like Unity.