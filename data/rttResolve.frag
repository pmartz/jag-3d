#version 400

uniform sampler2D opaque;

in vec2 tc;
out vec4 finalColor;

void main()
{
    finalColor = texture2D( opaque, tc );
    //finalColor = vec4(1.,0.,0.,1.);
}
