uniform sampler2D texunit0;
varying vec2 texcoord;

void main()
{
    gl_FragColor =  texture2D(texunit0, texcoord);
}

