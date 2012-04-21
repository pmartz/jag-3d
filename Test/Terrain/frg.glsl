uniform sampler2D texunit0;
uniform sampler2D texunit1;
uniform sampler2D texunit2;

varying vec2 texcoord;
varying vec4 ii_FrontColor;
varying vec3 color;

void main()
{
    //gl_FragColor =  vec4( texcoord, 0, 1 );
    vec4 tex0  = texture2D(texunit0, texcoord);
    vec4 tex1  = texture2D(texunit1, texcoord);
    vec4 tex2  = texture2D(texunit2, texcoord);
    gl_FragColor = tex2;
    //gl_FragColor = vec4( 0.4, 0.8, 0.4, 1.0 );
    //gl_FragColor =  ii_FrontColor * texture2D(texunit0, texcoord);
    ///gl_FragColor =  texture2D(texunit2, texcoord);
    //gl_FragColor =  vec4( color, 1 );
}

