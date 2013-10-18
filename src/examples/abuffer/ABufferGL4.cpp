/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0 : Linked Lists
 * Copyright Cyril Crassin, June/July 2010
 *
 * V1.0 Basic algorithm with a fixed number of layers.
 * V2.0 Introduces a new algorithm: Linked List of Fragment Pages
 * Fragments are stored in pages in a shared pool.
 * The pool size is increased dynamically based on 
 * the storage need. (cf. ABufferGL4_LinkedList.cpp).
 *
 * It uses either EXT_shader_image_load_store or 
 * GL_NV_shader_buffer_load+GL_NV_shader_buffer_store
 * to fill a list of fragments per pixels during the 
 * rasterisation. 
 * Then in a "resolve" pass, this list is sorted by z 
 * and fragments are blended in order.
 * Fragments are 4x 32bits float components containing 
 * RGB + Z.
 * 
 * There are two paths, one using textures 
 * (EXT_shader_image_load_store) and one using global memory
 * (GL_NV_shader_buffer_load+GL_NV_shader_buffer_store) to 
 * store the A-Buffer. 
 * On GF100, using textures for the basic algorithm is a little bit faster.
 *
 * This example also uses Direct State Access to remove 
 * multiple bindings. (EXT_direct_state_access)
**/

#include <jagSG/Common.h>
#include <jagDraw/Common.h>
#include <jagDisk/ReadWrite.h>
#include <jagDisk/Options.h>

#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "ShadersManagment.h"
#include "Vector.h"
#include "Matrix.h"
#include "InfoWidget.h"

#include "ABufferGL4Parameters.h"

#include "ABufferGL4_Basic.h"
#include "ABufferGL4_LinkedList.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
namespace bpo = boost::program_options;

// If rendering a JAG model:
jagSG::NodePtr jagRoot;

//Filename of the mesh to use
const std::string modelDir( "C:/Projects/JAG/projectinfo/a-buffer/ABufferGL4LinkedList/" );
const std::string modelNames[]={
    modelDir + std::string("models/dragon_vrip.ply"),
    modelDir + std::string("models/happy_vrip.ply"),
    modelDir + std::string("models/Armadillo.ply"),
    modelDir + std::string("models/bun_zipper.ply"),
    modelDir + std::string("models/xyzrgb_dragon4.ply")
};


//Program parameters//
ABufferAlgorithm pABufferAlgorithm=ABuffer_LinkedList;

int pUseABuffer=1;
int pABufferUseSorting=1;
int pResolveAlphaCorrection=1;
int pResolveGelly=1;

int pABufferUseTextures=0;
int pSharedPoolUseTextures=0;

bool pDispNumFragments=false;

NemoGraphics::Vector4f pBackgroundColor=NemoGraphics::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);    //0.15f

//////////////////////


//Types
typedef float AbufferType;
typedef unsigned int uint;

//View properties & manip
NemoGraphics::Vector2i windowSize=NemoGraphics::Vector2i(512, 512);
NemoGraphics::Vector2i moldpos;
NemoGraphics::Vector3f viewPos(0.0, 0.0, -1.0);
NemoGraphics::Vector3f viewOrient(0.0f, 0.0f, 0.0f);
NemoGraphics::Vector3f modelOffset(-0.5f, -0.3f, -0.2f);    //Dragon
float viewMoveRate( 1.f );
//NemoGraphics::Vector3f viewOrient(0.0f, 200.0f, 0.0f);
//NemoGraphics::Vector3f modelOffset(-0.53f, -0.2f, -0.35f);    //XYZRGB dragon
bool mclicked;
int mbutton;

//MVP matrices
GLfloat projMatrix[16];
NemoGraphics::Mat4f projectionMatrix; //Projection matrix
NemoGraphics::Mat4f modelViewMatrix;

InfoWidget infoWidget;

//Full screen quad vertices definition
const GLfloat quadVArray[] = {
   -1.0f, -1.0f, 0.0f, 1.0f,
   1.0f, -1.0f, 0.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 1.0f,    
   1.0f, -1.0f, 0.0f, 1.0f,
   1.0f, 1.0f, 0.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 1.0f  
};


//Timing
const int timingNumFrames=2000;    //Use 0 to disable
uint frameCounter=0;
int timeCounter;

//Mesh attributes storage
std::vector<float>        meshVertexPositionList;
std::vector<float>        meshVertexNormalList;
std::vector<float>        meshVertexColorList;
std::vector<uint>        meshTriangleList;

//Mesh VBOs
GLuint vertexBufferName=0;
GLuint vertexBufferModelPosID=0;
GLuint vertexBufferModelColorID=0;
GLuint vertexBufferModelNormalID=0;
GLuint vertexBufferModelIndexID=0;

//Mesh loading from a formated file: not compiled by default
void loadMeshFile(const std::string &meshFileName);

//Vectors read/write helpers: for mesh loading
template<class T> void writeStdVector(const std::string &fileName, std::vector<T> &vec);
template<class T> void readStdVector(const std::string &fileName, std::vector<T> &vec);


void initShaders(void) {

    //Shader dynamic macro setting
    resetShadersGlobalMacros();

    setShadersGlobalMacro("ABUFFER_SIZE", ABUFFER_SIZE);
    setShadersGlobalMacro("SCREEN_WIDTH", windowSize.x);
    setShadersGlobalMacro("SCREEN_HEIGHT", windowSize.y);

    setShadersGlobalMacro("BACKGROUND_COLOR_R", pBackgroundColor.x);
    setShadersGlobalMacro("BACKGROUND_COLOR_G", pBackgroundColor.y);
    setShadersGlobalMacro("BACKGROUND_COLOR_B", pBackgroundColor.z);
    
    setShadersGlobalMacro("USE_ABUFFER", pUseABuffer);
    setShadersGlobalMacro("ABUFFER_USE_TEXTURES", pABufferUseTextures);
    setShadersGlobalMacro("SHAREDPOOL_USE_TEXTURES", pSharedPoolUseTextures);
    
    setShadersGlobalMacro("ABUFFER_RESOLVE_USE_SORTING", pABufferUseSorting);
    setShadersGlobalMacro("ABUFFER_RESOLVE_ALPHA_CORRECTION", pResolveAlphaCorrection);
    setShadersGlobalMacro("ABUFFER_RESOLVE_GELLY", pResolveGelly);

    setShadersGlobalMacro("ABUFFER_PAGE_SIZE", ABUFFER_PAGE_SIZE);

    setShadersGlobalMacro("ABUFFER_DISPNUMFRAGMENTS", pDispNumFragments);
    

    //Shaders loading
    if(pABufferAlgorithm==ABuffer_LinkedList)
        initShaders_LinkedList();
    else
        initShaders_Basic();

    checkGLError ("initShader");
}


//Mesh file loading
void loadMesh(std::string fileName){
#if LOAD_RAW_MESH==0
    /////////Mesh
    loadMeshFile(fileName);
    writeStdVector(fileName+"_pos.raw", meshVertexPositionList);
    writeStdVector(fileName+"_normal.raw", meshVertexNormalList);
    //writeStdVector(fileName+"_color.raw", meshVertexColorList);
    writeStdVector(fileName+"_triangles.raw", meshTriangleList);
#else
    readStdVector(fileName+"_pos.raw", meshVertexPositionList);
    readStdVector(fileName+"_normal.raw", meshVertexNormalList);
    //readStdVector(fileName+"_color.raw", meshVertexColorList);
    readStdVector(fileName+"_triangles.raw", meshTriangleList);
#endif

    std::cout<<"\nMesh num triangles: "<<meshTriangleList.size()/3<<"\n\n";
}

//Initialize A-Buffer storage. 
void initABuffer(){
    if(pABufferAlgorithm==ABuffer_LinkedList)
        initABuffer_LinkedList();
    else
        initABuffer_Basic();    
}


void initMesh(int modelNum=0){
    //Mesh loading
    loadMesh(modelNames[modelNum]);

    //Mesh VBOs
    if(!vertexBufferModelPosID)
        glGenBuffers (1, &vertexBufferModelPosID);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferModelPosID);
    glBufferData (GL_ARRAY_BUFFER, sizeof(float)*meshVertexPositionList.size(), &(meshVertexPositionList[0]), GL_STATIC_DRAW);

    /*glGenBuffers (1, &vertexBufferModelColorID);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferModelColorID);
    glBufferData (GL_ARRAY_BUFFER, sizeof(float)*meshVertexColorList.size(), &(meshVertexColorList[0]), GL_STATIC_DRAW);*/

    if(!vertexBufferModelNormalID)
        glGenBuffers (1, &vertexBufferModelNormalID);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferModelNormalID);
    glBufferData (GL_ARRAY_BUFFER, sizeof(float)*meshVertexNormalList.size(), &(meshVertexNormalList[0]), GL_STATIC_DRAW);

    if(!vertexBufferModelIndexID)
        glGenBuffers (1, &vertexBufferModelIndexID);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferModelIndexID);
    glBufferData (GL_ARRAY_BUFFER, sizeof(uint)*meshTriangleList.size(), &(meshTriangleList[0]), GL_STATIC_DRAW);
    checkGLError ("init mesh VBOs");
}

//Global init function
void init(void) {

    //Full screen quad initialization
    glGenBuffers (1, &vertexBufferName);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferName);
    glBufferData (GL_ARRAY_BUFFER, sizeof(quadVArray), quadVArray, GL_STATIC_DRAW);
    checkGLError ("initBuffer");

    initMesh();

    initShaders ();
    initABuffer();


    //Disable backface culling to keep all fragments
    glDisable(GL_CULL_FACE);
    //Disable depth test
    glDisable(GL_DEPTH_TEST);
    //Disable stencil test
    glDisable(GL_STENCIL_TEST);
    //Disable blending
    glDisable(GL_BLEND);

    glDepthMask(GL_FALSE);
    
}

//Dump GL infos
void dumpInfo(void) {
   printf ("Vendor: %s\n", glGetString (GL_VENDOR));
   printf ("Renderer: %s\n", glGetString (GL_RENDERER));
   printf ("Version: %s\n", glGetString (GL_VERSION));
   printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
   checkGLError ("dumpInfo");
}


void drawQuad(GLuint prog) {

    glUseProgram (prog);

    glEnableVertexAttribArray (glGetAttribLocation(prog, "vertexPos"));

    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferName);

    glVertexAttribPointer (glGetAttribLocation(prog, "vertexPos"), 4, GL_FLOAT, GL_FALSE,
                           sizeof(GLfloat)*4, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //checkGLError ("drawQuad");
}



bool drawJag( false );
jagDraw::DrawGraphPtr drawGraphTemplate;
jagSG::CollectionVisitor collect;
jagDraw::DrawInfo drawInfo;
float aspectRatio( 1.f );

void convertMatrix( gmtl::Matrix44d& result, const NemoGraphics::Mat4f& in )
{
    result.set( in[0], in[4], in[8], in[12], in[1], in[5], in[9], in[13],
        in[2], in[6], in[10], in[14], in[3], in[7], in[11], in[15] );
}

void jagInit()
{
    const gmtl::Point3d jagCenter( jagRoot->getBound()->getCenter() );
    const double radius( jagRoot->getBound()->getRadius() );
    modelOffset = NemoGraphics::Vector3f( -(float)jagCenter[0], -(float)jagCenter[1], -(float)jagCenter[2] );
    viewPos.z *= ((float)radius * 1.75f);
    viewMoveRate = (float)radius;

    drawGraphTemplate.reset( new jagDraw::DrawGraph() );
    collect.setDrawGraphTemplate( drawGraphTemplate );
    drawGraphTemplate->resize( 1 );

    // Specify the names of FFP transform emulation matrix uniforms.
    jagDraw::TransformCallback* xformCB( collect.getTransformCallback() );
    xformCB->setMatrixUniformName( jagBase::TransformD::PROJ, "projectionMat" );
    xformCB->setMatrixUniformName( jagBase::TransformD::MODEL_VIEW, "modelViewMat" );
    xformCB->setMatrixUniformName( jagBase::TransformD::MODEL_VIEW_INV_TRANS_4, "modelViewMatIT" );
    xformCB->setRequiredMatrixUniforms( jagBase::TransformD::PROJ |
        jagBase::TransformD::MODEL_VIEW | jagBase::TransformD::MODEL_VIEW_INV_TRANS_4 );

    jagRoot->setMaxContexts( 1 );
    drawInfo._id = 0;
}

void drawJagModel( const GLuint prog )
{
    collect.reset();

    gmtl::Matrix44d mv, p;
    convertMatrix( mv, modelViewMatrix );
    gmtl::setPerspective( p, 60., (double)aspectRatio, .1, 50000. );
    collect.setViewProj( mv, p );

    jagRoot->accept( collect );

    jagDraw::DrawGraphPtr drawGraph( collect.getDrawGraph() );

    double minZ, maxZ;
    collect.getNearFar( minZ, maxZ );
    gmtl::setPerspective( p, 60., (double)aspectRatio, minZ, maxZ );

    drawGraph->setViewProj( mv, p );
    drawInfo._externalProgramID = prog;
    drawGraph->execute( drawInfo );

    // Dirty the state that JAG set.
    drawInfo._current.clear( jagDraw::DrawablePrep::VertexArrayObject_t );
    glBindVertexArray( 0 );

    checkGLError( "drawJagModel" );
}

void drawModel(GLuint prog) {
    
    glUseProgram (prog);

    if( drawJag )
    {
        drawJagModel( prog );
        return;
    }

    glEnableVertexAttribArray (glGetAttribLocation(prog, "vertexPos"));
    glEnableVertexAttribArray (glGetAttribLocation(prog, "vertexNormal"));

    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferModelPosID);
    glVertexAttribPointer (glGetAttribLocation(prog, "vertexPos"), 3, GL_FLOAT, GL_FALSE,0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferModelNormalID);
    glVertexAttribPointer (glGetAttribLocation(prog, "vertexNormal"), 3, GL_FLOAT, GL_FALSE,0, NULL);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vertexBufferModelIndexID);
    glDrawElements(    GL_TRIANGLES, meshTriangleList.size(), GL_UNSIGNED_INT, NULL);

    //checkGLError ("drawModel");
}


//Global display function
void display(void) {

    bool frameOK=true;

    do{

        //Clear color buffer
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor (pBackgroundColor.x, pBackgroundColor.y, pBackgroundColor.z, pBackgroundColor.w);
        glClear (GL_COLOR_BUFFER_BIT);

        //Build view transformation matrix
        modelViewMatrix = NemoGraphics::Mat4f::identity();
        modelViewMatrix *= NemoGraphics::Mat4f::translation(viewPos);
        modelViewMatrix *= NemoGraphics::Mat4f::rotation(NemoGraphics::Vector3f(1.0, 0.0, 0.0), viewOrient.x);
        modelViewMatrix *= NemoGraphics::Mat4f::rotation(NemoGraphics::Vector3f(0.0, 1.0, 0.0), viewOrient.y);
        modelViewMatrix *= NemoGraphics::Mat4f::translation(modelOffset);
        
        //Clear A-Buffer
        if(pUseABuffer){ 
            if(pABufferAlgorithm==ABuffer_LinkedList)
                displayClearABuffer_LinkedList();
            else
                displayClearABuffer_Basic();
        }
        
        //Render the model into the A-Buffer
        {
            if(pABufferAlgorithm==ABuffer_LinkedList)
                displayRenderABuffer_LinkedList( modelViewMatrix, projectionMatrix);
            else
                displayRenderABuffer_Basic( modelViewMatrix, projectionMatrix);
        }

        //"Resolve" A-Buffer
        if( pUseABuffer ){
            if(pABufferAlgorithm==ABuffer_LinkedList)
                displayResolveABuffer_LinkedList();
            else
                displayResolveABuffer_Basic();
        }

        glUseProgram (0);

        frameCounter++;
        if(frameCounter==timingNumFrames){
            glFinish();
            int timeInterval=glutGet(GLUT_ELAPSED_TIME)-timeCounter;

            std::cout<<"FPS: "<<float(timingNumFrames)*1000.0f/float(timeInterval)<<"\n";

            timeCounter=glutGet(GLUT_ELAPSED_TIME);
            frameCounter=0;
        }


        infoWidget.draw();
        
        
        if(pUseABuffer && pABufferAlgorithm==ABuffer_LinkedList)
            frameOK=!display_LinkedList_ManageSharedPool();
        else
            frameOK=true;

    }while(!frameOK);

    glutSwapBuffers();

    
    int time;
    time = glutGet(GLUT_ELAPSED_TIME);


    checkGLError ("display");
   
}



void reshape (int w, int h) {
    windowSize.x=w;
    windowSize.y=h;

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    std::cout<<w<<"x"<<h<<"\n";
    initShaders(); 

    initABuffer();

    aspectRatio = float(w) / float(h);
    projectionMatrix.perspective(60.0f, aspectRatio, 0.010f, 20.0f);

}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 't':
        pABufferUseTextures=!pABufferUseTextures;
        if(pABufferUseTextures)
            infoWidget.print("Texture storage mode");
        else
            infoWidget.print("Global memory storage mode");
        break;
    case 'a':
        pUseABuffer=!pUseABuffer;
        if(pUseABuffer)
            infoWidget.print("ABuffer ON");
        else
            infoWidget.print("ABuffer OFF");
        break;
    case 's':
        pABufferUseSorting=!pABufferUseSorting;
        if(pABufferUseSorting)
            infoWidget.print("Resolve: fragment sorting mode");
        else
            infoWidget.print("Resolve: closest fragment mode");
        break;
    case 'c':
        pResolveAlphaCorrection=!pResolveAlphaCorrection;
        if(pResolveAlphaCorrection)
            infoWidget.print("Resolve: blending WITH alpha correction");
        else
            infoWidget.print("Resolve: blending WITHOUT alpha correction");
        break;
    case 'g':
        pResolveGelly=!pResolveGelly;
        if(pResolveGelly)
            infoWidget.print("Resolve: GELLY");
        else
            infoWidget.print("Resolve: ALPHA BLENDING");
        break;
    case 'x':
        pABufferAlgorithm= pABufferAlgorithm==ABuffer_Basic ? ABuffer_LinkedList:ABuffer_Basic;
        if(pABufferAlgorithm==ABuffer_Basic)
            infoWidget.print("Algorithm: Basic");
        else
            infoWidget.print("Algorithm: Linked Lists");

        break;
    case 'n':
        pDispNumFragments=!pDispNumFragments;
        if(pDispNumFragments)
            infoWidget.print("Display num. fragments ON");
        else
            infoWidget.print("Display num. fragments OFF");

        break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
        initMesh(key-'1');
        break;
   }

    initShaders();
    initABuffer();
}


unsigned int moveOp[] = { 1, 3, 2 };
//Called when a mouse button is pressed or released
void mouse_click(int button, int state, int x, int y){
    mclicked=state;
    moldpos=NemoGraphics::Vector2i(x, y);
    if( state == GLUT_UP )
        mbutton ^= moveOp[button];
    else if( state == GLUT_DOWN )
        mbutton |= moveOp[button];
}

//Called when a mouse move and a button is pressed
void mouse_motion(int x, int y){
    NemoGraphics::Vector2i mmotion=NemoGraphics::Vector2i(x, y)-moldpos;
    NemoGraphics::Vector2f mmotionf=NemoGraphics::Vector2f(mmotion)/NemoGraphics::Vector2f(windowSize);

    if(mbutton==1){
        viewOrient.x=viewOrient.x+mmotionf.y*100.0f;
        viewOrient.y=viewOrient.y+mmotionf.x*100.0f;
    }else if(mbutton==3){
        viewPos.x=viewPos.x+mmotionf.x*1.0f;
        viewPos.y=viewPos.y-mmotionf.y*1.0f;
    }else if(mbutton==2){
        viewPos.z=viewPos.z+mmotionf.y*viewMoveRate;
    }
    moldpos=NemoGraphics::Vector2i(x, y);
}

int main(int argc, char** argv) {

    //
    // Begin standard jag argument parsing.
    //
    std::vector< int > winsize;
    bpo::options_description desc( "Options" );
    // Add test/demo options
    desc.add_options()
        ( "help,h", "Help text" )
        ( "version,v", bpo::value< double >(), "OpenGL context version. Default: 4.0." )
        ( "winsize,w", bpo::value< std::vector< int > >( &winsize )->multitoken(), "Window width and height. Default: 300 300." )
        ( "file,f", bpo::value< std::string >(), "Model to load." )
        ;

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );
    if( vm.count( "help" ) > 0 )
    {
        std::cout << desc << std::endl;
        return( 1 );
    }

    double version( 4.0 );
    if( vm.count( "version" ) > 0 )
        version = vm[ "version" ].as< double >();
    double versionMajor;
    modf( version, &versionMajor );
    float versionMinor = (float)( version * 10. - versionMajor * 10. );

    if( winsize.size() != 2 )
    {
        winsize.clear();
        winsize.push_back( 300 ); winsize.push_back( 300 );
    }
    windowSize = NemoGraphics::Vector2i( winsize[0], winsize[1] );

    std::string fileName( "" );
    if( vm.count( "file" ) > 0 )
    {
        fileName = vm[ "file" ].as< std::string >();

        // Tell plugin what names to use for vertex and normal attribs.
        jagDisk::Options options;
        options.addOptions()
            ( "vertexAttribName", std::string( "vertexPos" ) )
            ( "normalAttribName", std::string( "vertexNormal" ) )
            ;
        boost::any anyTemp( jagDisk::read( fileName, &options ) );
        try {
            jagRoot = boost::any_cast< jagSG::NodePtr >( anyTemp );
        }
        catch( boost::bad_any_cast bac )
        {
            bac.what();
        }

        if( jagRoot != NULL )
        {
            drawJag = true;
            jagInit();
        }
    }
    //
    // End argument parsing
    //

    std::cout<<"\n"    <<"Fast Single-Pass A-Buffer V2.0 with Linked Lists using OpenGL 4.0\n"
                    <<"Cyril Crassin, July 2010\n\n";

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);

    //Init OpenGL 4.0 context
    glutInitContextVersion ( (int)versionMajor, (int)versionMinor );
    glutInitContextProfile(GLUT_CORE_PROFILE ); 
    glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE); //needed for glutBitmapCharacter
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE /*| GLUT_DEBUG */); //Can be uses for compatibility with openGL 2.x

    glutInitWindowSize (winsize[0], winsize[1]);
    glutInitWindowPosition (100, 100); 
    glutCreateWindow ("OpenGL 4.0 ABuffer Sample V2.0 : Linked Lists by Cyril Crassin 2010");
//    checkGLError ("glutCreateWindow");
    
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    const jagDraw::platformContextID pCtxId = static_cast< GLuint >( glutGetWindow() );
    jagDraw::jagDrawContextID contextID = cs->registerContext( pCtxId );

    cs->setActiveContext( contextID );
    cs->initContext();
      checkGLError ("context init");
    /*
    //Init glew
    glewExperimental=GL_TRUE;
    glewInit();
      checkGLError ("glewInit");
    */

    //Display GL info
    dumpInfo ();

    //Init everything
    init ();

    std::cout<<"\n"    <<"Keys: \n\t'a' Enable/Disable A-Buffer\n"
                    <<"\t'x' Switch between ABuffer Algorithms.\n"
                    <<"\t's' Enable/Disable fragments sorting. Disable= closest fragment kept during resolve.\n"
                    <<"\t'g' Swith between Alpha-Blending and Gelly resolve modes.\n"
                    <<"\t'c' Enable/Disable alpha correction when in Alpha-Blending mode.\n"
                    <<"\t't' Swith between using textures or global memory for A-Buffer storage.\n"
                    <<"\t'n' Display the number of fragments per pixel.\n"
                    <<"\t'1'-'4' Change mesh.\n\n";
                    
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    glutMotionFunc     ( mouse_motion );
    glutMouseFunc     ( mouse_click );
    glutIdleFunc     ( display );

    checkGLError ("main");
    glutMainLoop();

    return 0;
}


//Writing a vector to a raw file
template<class T>
void writeStdVector(const std::string &fileName, std::vector<T> &vec){
    std::ofstream dataOut( fileName.c_str(), std::ios::out | std::ios::binary);
    dataOut.write((const char *)&(vec[0]), vec.size()*sizeof(T));
    dataOut.close();
}
//Reading a vector from a raw file
template<class T>
void readStdVector(const std::string &fileName, std::vector<T> &vec){
    struct stat results;
    if (stat(fileName.c_str(), &results) == 0){
        uint numElem=results.st_size/sizeof(T);

        vec.resize(numElem);

        std::ifstream dataIn( fileName.c_str(), std::ios::in | std::ios::binary);
        dataIn.read((char *)&(vec[0]), vec.size()*sizeof(T));
        dataIn.close();
    }else{
        std::cout<<"Unable to open file: "<<fileName<<"\n";
    }
}

//Mesh loading from a formated file: not used by default to remove the dependency with assimp
//->Link with assimp (http://assimp.sourceforge.net/) to use it.
#if LOAD_RAW_MESH==0

//Assimp
#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Outptu data structure
#include <aiPostProcess.h> // Post processing flags
#pragma comment(lib, "assimp.lib")

 void loadMeshFile(const std::string &meshFileName){
    std::cout<<"Loading "<<meshFileName<<" ... \n";

    const aiScene* aiscene;
    aiscene=NULL;

    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    /*const aiScene* scene = importer.ReadFile( meshFileName,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);*/

    aiscene = importer.ReadFile( meshFileName, aiProcess_GenSmoothNormals );

    // If the import failed, report it
    if( !aiscene)    {
        std::cout<< importer.GetErrorString();
        return ;
    }

    // Now we can access the file's contents.
    //DoTheSceneProcessing( scene);

    //std::cout<<aiscene->mRootNode->mNumMeshes<<" !\n";

    uint previousNumVertex=meshVertexPositionList.size();

    aiMesh* aimesh  = aiscene->mMeshes[0];

    bool hasColor=aimesh->GetNumColorChannels();

    //Compu BBox
    float bboxMinX=1000000.0f;float bboxMinY=1000000.0f;float bboxMinZ=1000000.0f;
    float bboxMaxX=-1000000.0f;float bboxMaxY=-1000000.0f;float bboxMaxZ=-1000000.0f;
    for(uint v=0; v < aimesh->mNumVertices; ++v){
        float valX=aimesh->mVertices[v].x;
        float valY=aimesh->mVertices[v].y;
        float valZ=aimesh->mVertices[v].z;
        if(valX<bboxMinX)
            bboxMinX=valX;
        if(valX>bboxMaxX)
            bboxMaxX=valX;

        if(valY<bboxMinY)
            bboxMinY=valY;
        if(valY>bboxMaxY)
            bboxMaxY=valY;

        if(valZ<bboxMinZ)
            bboxMinZ=valZ;
        if(valZ>bboxMaxZ)
            bboxMaxZ=valZ;
    }

    float sizeX=(bboxMaxX-bboxMinX);
    float sizeY=(bboxMaxY-bboxMinY);
    float sizeZ=(bboxMaxZ-bboxMinZ);
    float maxSize=max(sizeX, max(sizeY, sizeZ));

    for(uint v=0; v < aimesh->mNumVertices; ++v){

        meshVertexPositionList.push_back( (aimesh->mVertices[v].x-bboxMinX)/(maxSize) );
        meshVertexPositionList.push_back( (aimesh->mVertices[v].y-bboxMinY)/(maxSize) );
        meshVertexPositionList.push_back( (aimesh->mVertices[v].z-bboxMinZ)/(maxSize) );
        //meshVertexPositionList.push_back( 1.0f );

        if(hasColor){
            meshVertexColorList.push_back( aimesh->mColors[0][v].r );
            meshVertexColorList.push_back( aimesh->mColors[0][v].g );
            meshVertexColorList.push_back( aimesh->mColors[0][v].b );
            meshVertexColorList.push_back( aimesh->mColors[0][v].a );
        }else{
            meshVertexColorList.push_back( 0.9f );
            meshVertexColorList.push_back( 0.9f );
            meshVertexColorList.push_back( 0.9f );
            meshVertexColorList.push_back( 1.0f );
        }
       

        meshVertexNormalList.push_back( aimesh->mNormals[v].x );
        meshVertexNormalList.push_back( aimesh->mNormals[v].y );
        meshVertexNormalList.push_back( aimesh->mNormals[v].z );
        //meshVertexNormalList.push_back( 1.0f );

    }
    //

    for(uint t=0; t < aimesh->mNumFaces; ++t){
        meshTriangleList.push_back( aimesh->mFaces[t].mIndices[0] );
        meshTriangleList.push_back( aimesh->mFaces[t].mIndices[1] );
        meshTriangleList.push_back( aimesh->mFaces[t].mIndices[2] );
    }

    std::cout<<meshFileName<<" Loaded \n";
}
#endif