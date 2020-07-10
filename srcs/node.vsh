#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in float type;
out vec3 color;

uniform mat4 v_matrix;
uniform mat4 w_matrix;
uniform mat4 m_matrix;

uniform vec3 color_no_winner;
uniform vec3 color_signal;
uniform vec3 color_winner;
uniform vec3 color_winner2;

uniform float point_size;

uniform float depth_steepness;
uniform float depth_offset;

void main()
{
    float color_scaler = 1.0;

    if(type == 1.0) // signal
    {
        color = color_signal;
        gl_PointSize = 4 * point_size;
    }
    else if(type == 2.0) // winner
    {
        color = color_winner;
        gl_PointSize = 2 * point_size + 2;
    }
    else if(type == 3.0) // winner2
    {
        color = color_winner2;
        gl_PointSize = 2 * point_size;
    }
    else // no winner
    {
        color = color_no_winner;
        color_scaler = depth_steepness * position.z + depth_offset;
        gl_PointSize = point_size;
    }

    color = color_scaler * color;
    gl_Position = v_matrix * w_matrix * m_matrix * vec4(position.xyz, 1);
}
