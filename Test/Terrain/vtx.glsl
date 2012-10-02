uniform sampler2D texunit1;
uniform sampler2D texunit2;

uniform mat4 ii_ModelViewProjectionMatrix;
uniform mat3 ii_NormalMatrix;

attribute vec3 ii_Vertex3;
attribute vec2 ii_TexCoord20;
attribute vec3 ii_Normal;

varying vec2 texcoord;
varying vec3 color;

extern void ii_lighting( in vec3 );

void main()
{
    texcoord = ii_TexCoord20;

    vec4 tc = texture2D(texunit1, texcoord);
    vec3 nc = texture2D(texunit2, texcoord).xyz;

    float z = tc.r;
    vec4 v = vec4( ii_Vertex3.xy, (z * 0.1), 1.0 );

    //gl_Position = ii_ModelViewProjectionMatrix * vec4( ii_Vertex3, 1 );
    gl_Position = ii_ModelViewProjectionMatrix * v ;

    //vec3 normal = abs(ii_Normal);//normalize( ii_NormalMatrix * ii_Normal );
    //vec3 normal = normalize( ii_NormalMatrix * (ii_Normal) );

    vec3 normal = normalize( ii_NormalMatrix * nc );

    ii_lighting(normal);
}

