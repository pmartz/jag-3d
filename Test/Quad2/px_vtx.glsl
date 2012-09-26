uniform mat4 omat;
attribute vec2 vtxpos;
attribute vec2 vtxtex;

varying vec2 texcoord;

void main()
{
    texcoord = vtxtex;
    //gl_Position = vec4(vtxpos, 0, 1);
    gl_Position = omat * vec4(vtxpos, 0, 1);
}

