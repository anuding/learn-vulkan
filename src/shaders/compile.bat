
glslangValidator -V tri.vert.glsl
glslangValidator -V tri.frag.glsl
copy *.spv ..\out\build\Vulkan-x64-Debug\assets\spv_shaders\
move *.spv ..\..\assets\spv_shaders\