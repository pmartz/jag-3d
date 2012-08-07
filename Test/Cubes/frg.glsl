uniform sampler2D texunit0;
uniform sampler2D texunit1;

varying vec2 texcoord;
varying vec4 ii_FrontColor;
varying vec3 color;

void main()
{
    vec4 tex0 = texture2D(texunit0, texcoord);
    vec4 tex1 = texture2D(texunit1, texcoord);
    //gl_FragColor =  ii_FrontColor * tex1;
    gl_FragColor =  ii_FrontColor * tex0 * tex1;
    //gl_FragColor =  ii_FrontColor * tex0;
    //gl_FragColor = ii_FrontColor;
    //gl_FragColor = vec4( 1, 0, 0, 1 );
}

