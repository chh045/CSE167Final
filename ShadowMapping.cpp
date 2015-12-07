//
//  ShadowMapping.cpp
//  CSE167Final
//
//  Created by Huang Edison on 12/4/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#include "ShadowMapping.h"

#ifdef __APPLE__
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Globals.h"
#include "Vector3.h"


ShadowMapping::ShadowMapping(GLhandleARB shadowShader){
    
    shadowShaderId = shadowShader;
    shadowMapUniform = glGetUniformLocationARB(shadowShaderId,"ShadowMap");
    
    generateShadowFBO();
    //loadShadowShader(vert, frag);
}

ShadowMapping::~ShadowMapping(){
    
}


/*
// Loading shader function
GLhandleARB ShadowMapping::loadShader(const char* filename, unsigned int type)
{
    FILE *pfile;
    GLhandleARB handle;
    const GLcharARB* files[1];
    
    // shader Compilation variable
    GLint result;				// Compilation code result
    GLint errorLoglength ;
    char* errorLogText;
    GLsizei actualErrorLogLength;
    
    char buffer[400000];
    memset(buffer,0,400000);
    
    // This will raise a warning on MS compiler
    pfile = fopen(filename, "rb");
    if(!pfile)
    {
        printf("Sorry, can't open file: '%s'.\n", filename);
        exit(0);
    }
    
    fread(buffer,sizeof(char),400000,pfile);
    //printf("%s\n",buffer);
    
    
    fclose(pfile);
    
    handle = glCreateShaderObjectARB(type);
    if (!handle)
    {
        //We have failed creating the vertex shader object.
        printf("Failed creating vertex shader object from file: %s.",filename);
        exit(0);
    }
    
    files[0] = (const GLcharARB*)buffer;
    glShaderSourceARB(
                      handle, //The handle to our shader
                      1, //The number of files.
                      files, //An array of const char * data, which represents the source code of theshaders
                      NULL);
    
    glCompileShaderARB(handle);
    
    //Compilation checking.
    glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
    
    // If an error was detected.
    if (!result)
    {
        //We failed to compile.
        printf("Shader '%s' failed compilation.\n",filename);
        
        //Attempt to get the length of our error log.
        glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
        
        //Create a buffer to read compilation error message
        errorLogText = (char *)malloc(sizeof(char) * errorLoglength);
        
        //Used to get the final length of the log.
        glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
        
        // Display errors.
        printf("%s\n",errorLogText);
        
        // Free the buffer malloced earlier
        free(errorLogText);
    }
    
    return handle;
}

void ShadowMapping::loadShadowShader(const char* vert, const char* frag)
{
    GLhandleARB vertexShaderHandle;
    GLhandleARB fragmentShaderHandle;
    
    vertexShaderHandle   = loadShader(vert, GL_VERTEX_SHADER);
    fragmentShaderHandle = loadShader(frag, GL_FRAGMENT_SHADER);
    
    shadowShaderId = glCreateProgramObjectARB();
    
    glAttachObjectARB(shadowShaderId,vertexShaderHandle);
    glAttachObjectARB(shadowShaderId,fragmentShaderHandle);
    glLinkProgramARB(shadowShaderId);
    
    
    shadowMapUniform = glGetUniformLocationARB(shadowShaderId,"ShadowMap");
}
 */


void ShadowMapping::generateShadowFBO()
{
    int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_RATIO;
    int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_RATIO;
    
    //GLfloat borderColor[4] = {0,0,0,0};
    
    GLenum FBOstatus;
    
    // Try to use a texture depth component
    glGenTextures(1, &depthTextureId);
    glBindTexture(GL_TEXTURE_2D, depthTextureId);
    
    // GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Remove artefact on the edges of the shadowmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    
    //glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );
    
    
    
    // No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // create a framebuffer object
    glGenFramebuffersEXT(1, &fboId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
    
    // Instruct openGL that we won't bind a color texture with the currently binded FBO
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depthTextureId, 0);
    
    // check FBO status
    FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
        printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
    
    // switch back to window-system-provided framebuffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
}


void ShadowMapping::setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,RENDER_WIDTH/RENDER_HEIGHT,10,40000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /*
    Vector3 position(position_x, position_y, position_z);
    Vector3 lookAt(lookAt_x, lookAt_y, lookAt_z);
    Vector3 up(0, 1, 0);
    
    if (buffer) {
        temp_camera_position = Globals::camera.getPosition();
        temp_camera_lookat = Globals::camera.getLookAt();
        temp_camera_up = Globals::camera.getUp();
        gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
        //Globals::camera.set(position, lookAt, up);
    }
    else{
        //Globals::drawData.setMatrix(Globals::camera.getInverseMatrix());
        gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
        //Globals::camera.set(position, lookAt, up);
        //Globals::camera.set(temp_camera_position, temp_camera_lookat, temp_camera_up);
    }
    //position.print("setupMatrices");
     */
    gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
}

// This update only change the position of the light.
//int elapsedTimeCounter = 0;
void ShadowMapping::update(void)
{
    
    p_light[0] = light_mvnt * cos(glutGet(GLUT_ELAPSED_TIME)/1000.0);
    p_light[2] = light_mvnt * sin(glutGet(GLUT_ELAPSED_TIME)/1000.0);
    
    //p_light[0] = light_mvnt * cos(3652/1000.0);
    //p_light[2] = light_mvnt * sin(3652/1000.0);
}

void ShadowMapping::setTextureMatrix(void)
{
    static double modelView[16];
    static double projection[16];
    
    // This is matrix transform every coordinate x,y,z
    // x = x* 0.5 + 0.5
    // y = y* 0.5 + 0.5
    // z = z* 0.5 + 0.5
    // Moving from unit cube [-1,1] to [0,1]
    const GLdouble bias[16] = {
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0};
    
    // Grab modelview and transformation matrices
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    
    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    
    glLoadIdentity();
    glLoadMatrixd(bias);
    
    // concatating all matrice into one.
    glMultMatrixd (projection);
    glMultMatrixd (modelView);
    
    // Go back to normal matrix mode
    glMatrixMode(GL_MODELVIEW);
}

// During translation, we also have to maintain the GL_TEXTURE8, used in the shadow shader
// to determine if a vertex is in the shadow.
void ShadowMapping::startTranslate(float x,float y,float z)
{

    glPushMatrix();
    glTranslatef(x,y,z);
    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    glPushMatrix();
    glTranslatef(x,y,z);
}

void ShadowMapping::endTranslate()
{
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void ShadowMapping::drawObjects(Group* group)
{
    
    /*
    // draw test room
    skybox_shader->bind();
    Globals::testRoomTex.bind();
    Globals::testRoom.draw(Globals::drawData);
    Globals::testRoomTex.unbind();
    skybox_shader->unbind();
    */
    
    
    
    // Ground
    glColor4f(0.3f,0.3f,0.3f,1);
    glBegin(GL_QUADS);
    //glVertex3f(-35,-5,-35);
    //glVertex3f(-35,-5, 15);
    //glVertex3f( 15,-5, 15);
    //glVertex3f( 15,-5,-35);
    
    //--
    glVertex3f(-4000.0,-1.5,10000.0);
    glVertex3f(4000.0,-1.5,10000.0);
    glVertex3f(4000.0,-1.5,-10000.0);
    glVertex3f(-4000.0,-1.5,-10000.0);
    //--
    glEnd();
    
    glColor4f(0.9f,0.9f,0.9f,1);

    //startTranslate(0, 4, -16);
    
    startTranslate(5,0,-5);
    glutSolidCube(4);
    endTranslate();
    
    startTranslate(-5,5,-5);
    glutSolidSphere(3, 50, 50);
    endTranslate();
    
    
    startTranslate(3,-0.5,8);
    Globals::bunny.render();
    endTranslate();
    
    //startTranslate(0, -1, 0);
    //Globals::tree.draw();
    //endTranslate();
    
    //startTranslate(0, 1, 0);
    //Globals::particle.draw();
    //endTranslate();
     
     
    
}


void ShadowMapping::renderScene(Group* group, bool spin)
{
     
     if (spin) {
     update();
     }
     
    
    //First step: Render from the light POV to a FBO, story depth values only
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	//Rendering offscreen
    
    //Using the fixed pipeline to render to the depthbuffer
    glUseProgramObjectARB(0);
    
    // In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
    glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);
    
    // Clear previous frame values
    glClear( GL_DEPTH_BUFFER_BIT);
    
    //Disable color rendering, we only want to write to the Z-Buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    
    Vector3 l = Globals::light.getPosition();
    std::cout<<p_light[0]<<std::endl;
    setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
    
    //setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
    
    // Culling switching, rendering only backface, this is done to avoid self-shadowing
    glCullFace(GL_FRONT);
    drawObjects(group);
    
    //Save modelview/projection matrice into texture7, also add a biais
    setTextureMatrix();
    
    
    // Now rendering from the camera POV, using the FBO to generate shadows
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
    
    glViewport(0,0,640,480);
    
    //Enabling color write (previously disabled for light POV z-buffer rendering)
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    // Clear previous frame values
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Using the shadow shader
    glUseProgramObjectARB(shadowShaderId);
    glUniform1iARB(shadowMapUniform,7);
    glActiveTextureARB(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D,depthTextureId);
    
    
    Vector3 p_c = Globals::camera.getPosition();
    Vector3 l_c = Globals::camera.getLookAt();
    setupMatrices(p_c[0],p_c[1],p_c[2],l_c[0],l_c[1],l_c[2]);
    
    
    //setupMatrices(temp_camera_position[0],temp_camera_position[1],temp_camera_position[2],
    //              temp_camera_lookat[0],temp_camera_lookat[1],temp_camera_lookat[2], false);
    //setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2], false);
    
    
    glCullFace(GL_BACK);
    drawObjects(group);
    
    // DEBUG only. this piece of code draw the depth buffer onscreen
    
    /*
    glUseProgramObjectARB(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4f(1,1,1,1);
    glActiveTextureARB(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,depthTextureId);
    glEnable(GL_TEXTURE_2D);
    glTranslated(0,0,-1);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0);glVertex3f(0,0,0);
    glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
    glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
    glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);
    */
    
    // glEnd();
    glDisable(GL_TEXTURE_2D);
     
}

/*
 
 if (spin) {
 update();
 }
 
 //First step: Render from the light POV to a FBO, story depth values only
 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	//Rendering offscreen
 
 //Using the fixed pipeline to render to the depthbuffer
 glUseProgramObjectARB(0);
 
 // In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
 glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);
 
 // Clear previous frame values
 glClear( GL_DEPTH_BUFFER_BIT);
 
 //Disable color rendering, we only want to write to the Z-Buffer
 glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
 
 Vector3 l = Globals::light.getPosition();
 std::cout<<p_light[0]<<std::endl;
 setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
 
 //setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
 
 // Culling switching, rendering only backface, this is done to avoid self-shadowing
 glCullFace(GL_FRONT);
 drawObjects(group);
 
 //Save modelview/projection matrice into texture7, also add a biais
 setTextureMatrix();
 
 
 // Now rendering from the camera POV, using the FBO to generate shadows
 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
 
 glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);
 
 //Enabling color write (previously disabled for light POV z-buffer rendering)
 glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
 
 // Clear previous frame values
 //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 //Using the shadow shader
 glUseProgramObjectARB(shadowShaderId);
 glUniform1iARB(shadowMapUniform,7);
 glActiveTextureARB(GL_TEXTURE7);
 glBindTexture(GL_TEXTURE_2D,depthTextureId);
 
 
 Vector3 p_c = Globals::camera.getPosition();
 Vector3 l_c = Globals::camera.getLookAt();
 setupMatrices(p_c[0],p_c[1],p_c[2],l_c[0],l_c[1],l_c[2]);
 
 
 //setupMatrices(temp_camera_position[0],temp_camera_position[1],temp_camera_position[2],
 //              temp_camera_lookat[0],temp_camera_lookat[1],temp_camera_lookat[2], false);
 //setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2], false);
 
 
 glCullFace(GL_BACK);
 drawObjects(group);
 
 // DEBUG only. this piece of code draw the depth buffer onscreen
 
 glUseProgramObjectARB(0);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glColor4f(1,1,1,1);
 glActiveTextureARB(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D,depthTextureId);
 glEnable(GL_TEXTURE_2D);
 glTranslated(0,0,-1);
 glBegin(GL_QUADS);
 glTexCoord2d(0,0);glVertex3f(0,0,0);
 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
 glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);
 

// glEnd();
glDisable(GL_TEXTURE_2D);


//glutSwapBuffers();

 */



