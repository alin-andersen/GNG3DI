#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in float  age;
out vec3 color;

uniform mat4 v_matrix;
uniform mat4 w_matrix;
uniform mat4 m_matrix;

uniform vec3 line_color;
uniform float mode;
uniform float depth_steepness;
uniform float depth_offset;
uniform float age_steepness;
uniform float age_offset;

void main()
{
    float color_scaler;

    if(mode == 0.0)
    {
        color_scaler = depth_steepness * position.z + depth_offset;
    }
    else
    {
        color_scaler = age_steepness * age + age_offset;
    }

    color = color_scaler * line_color;
    gl_Position = v_matrix * w_matrix * m_matrix * vec4(position.xyz, 1);
}
