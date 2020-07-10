#version 330 core

in vec3 color;

void main()
{
    float depth_scaler = 1.0;//(gl_FragCoord.z / gl_FragCoord.w);// / (gl_DepthRange.far - gl_DepthRange.near);
    gl_FragColor = vec4(depth_scaler * color, 1);
}
