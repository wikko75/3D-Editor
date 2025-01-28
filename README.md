# 3D-Editor

Simple 3D objects editor

## `Prerequisites`
- C++ 23
- [CMake](https://cmake.org/) - minimum version 3.25
- OpenGL support
- Build tool, ex. [Ninja](https://github.com/ninja-build/ninja)



## `Building & Running`

``` Bash
cmake -S . -B build -G Ninja && cmake --build build
```


## `Features`
The editor provides tools for transforming 3D models -  translation, rotation, and scaling within the scene. Vertex editing is the only available mode, allowing to modify properties like color and position of each vertex of a model. The editor supports saving the edit state to a .ini file, which can later be reloaded.

---

### `Editor GUI`

<p align="center">
  <img src="https://github.com/user-attachments/assets/c90de483-fd79-4b72-8aae-c2148f2a9ef9" alt="Editor GUI" width="90%">
</p>


--- 
