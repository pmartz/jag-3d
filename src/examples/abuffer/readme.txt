This is a port of the ABuffer OpenGL example found at:
    http://blog.icare3d.org/2010/07/opengl-40-abuffer-v20-linked-lists-of.html

The purpose of this example is to show how an existing OpenGL program
can be modified to render geometry in a jagSG scene graph. The code was
modified minimally. Comparisons against the original code base are
encouraged.

To avoid having the JAG project depend on GLEW, the ABuffer example was
modified to use JAG's native gl3w interface for OpenGL.

A second example, "abufferJag", shows how the ABuffer algorithm can
be incorporated into JAG data structures.
