/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/
#ifndef ABUFFERGL4PARAMETERS_H
#define ABUFFERGL4PARAMETERS_H
#include "InfoWidget.h"


//Number of layers in the A-Buffer
#define ABUFFER_SIZE            16
//Mesh is loaded from a raw file instead of a formated file->requires assimp
#define LOAD_RAW_MESH            1

#define ABUFFER_PAGE_SIZE        4

//Parameters
enum ABufferAlgorithm{ABuffer_Basic, ABuffer_LinkedList};
extern ABufferAlgorithm pABufferAlgorithm;
extern int pABufferUseTextures;
extern int pSharedPoolUseTextures;

extern NemoGraphics::Vector2i windowSize;


extern InfoWidget infoWidget;

//Because current glew does not define it
#ifndef GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV
#define GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV             0x00000010
#endif

#endif