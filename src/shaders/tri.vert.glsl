#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

struct Point{
    vec2 inPosition;
    vec2 inTexCoord;
};

layout(location = 0) out vec2 fragTexCoord;

Point points[3] = Point[](
   Point(vec2(0.0, -0.5), vec2(0.0, 0.0)), 
   Point(vec2(0.5, 0.5), vec2(1.0, 0.0)), 
   Point(vec2(-0.5, 0.5), vec2(0.0, 1.0))
);

void main() {
    gl_Position = vec4(points[gl_VertexIndex].inPosition, 0.0, 1.0);
    fragTexCoord = points[gl_VertexIndex].inTexCoord;
}