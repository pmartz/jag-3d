#version 400

uniform samplerBuffer texture;
uniform int index;

out vec4 colorOut;
void main()
{
    colorOut = texelFetch( texture, index );
}
