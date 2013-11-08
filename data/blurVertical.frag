#version 400

#define PCT_BLUR 0.018f

uniform sampler2D texture0;

in vec2 tc;
out vec4 finalColor;

vec4 getSample( float offset, float weight )
{
    vec4 c = texture2D( texture0, vec2( tc.s, tc.t + offset ) ) * weight;
    if( c.a == 0.f )
        c.rgb = vec3( 0.f );
    return( c );
}

void main()
{
    vec4 s0 = getSample( -PCT_BLUR, 0.15f );
    vec4 s1 = getSample( -PCT_BLUR*0.5f, 0.2f );
    vec4 s2 = getSample( 0.f, 0.3f );
    vec4 s3 = getSample( PCT_BLUR*0.5f, 0.2f );
    vec4 s4 = getSample( PCT_BLUR, 0.15f );

    finalColor = s0 + s1 + s2 + s3 + s4;
}
