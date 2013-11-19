#version 400

uniform sampler2D texture0;
#if( TEXTURE_COUNT > 1 )
uniform sampler2D texture1;
#endif
#if( TEXTURE_COUNT > 2 )
uniform sampler2D texture2;
#endif
#if( TEXTURE_COUNT > 3 )
uniform sampler2D texture3;
#endif

in vec2 tc;
out vec4 finalColor;

void main()
{
    finalColor = 
#if( TEXTURE_COUNT == 0 )
        vec4( 0., 0., 0., 0. )
#else
        texture2D( texture0, tc )
#endif
#if( TEXTURE_COUNT > 1 )
        + texture2D( texture1, tc )
#endif
#if( TEXTURE_COUNT > 2 )
        + texture2D( texture2, tc )
#endif
#if( TEXTURE_COUNT > 3 )
        + texture2D( texture3, tc )
#endif
        ;
}
