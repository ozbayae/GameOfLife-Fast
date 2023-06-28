#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aOffset;

out vec3 fColor;

mat4 BuildTranslation(vec3 delta)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(delta, 1.0));
}


void main()
{
    //put green in fColor
    fColor = vec3(0.0, 1.0, 0.0);
    //translate the vertex by aOffset
    gl_Position = BuildTranslation(aOffset) * vec4(aPos, 1.0);
}

