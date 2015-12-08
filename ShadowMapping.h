/*            :::::::::::WARNING::::::::::::::
** PLEASE, DO NOT INCLUDE SAME HEADER MORE THAN ONCE.
** PLEASE, GLEE.H ALWAYS DEFINED BEFORE GLUT.H
** PLEASE, ADD ONLY HEADERS IN H-FILES IF YOU NEED TO USE IN THE HEADER
** OTHERWISE, PUT IT IN .CPP FILE
**/

#ifndef ShadowMapping_h
#define ShadowMapping_h

#include "Globals.h"

#ifdef __APPLE__
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SHADOW_MAP_RATIO 2
#define RENDER_WIDTH 1280.0
#define RENDER_HEIGHT 960.0


/*
#ifdef _WIN32
// As microsoft did not maintain openGL after version 1.1, Windows platform need to go throught this crap ; macosX and Linux are fine.
// This block simply retries openGL function needed for this example.
// I recommend to use GLEW instead of going this way. This is done this way only to ease beginner's compilation and portability


PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

// FrameBuffer (FBO) gen, bin and texturebind
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;


// Shader functions
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB ;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB   ;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB ;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB        ;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB       ;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB        ;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB         ;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB  ;
PFNGLUNIFORM1IARBPROC            glUniform1iARB           ;
PFNGLACTIVETEXTUREARBPROC		  glActiveTextureARB;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB          ;

void getOpenGLFunctionPointers(void)
{
    // FBO
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
    glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		wglGetProcAddress("glGenFramebuffersEXT");
    glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress("glBindFramebufferEXT");
    glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
    glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
    
    //Shaders
    glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
    glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
    glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
    glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
    glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
    glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
    glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
    glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
    glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
    glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
    glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
    
}
#endif
*/


class ShadowMapping{
    
private:
    
	char* read(const char *filename);

	void setup(const char *vs, const char *fs);

	Vector3 temp_camera_position, temp_camera_lookat, temp_camera_up;
    //Camera position
    float p_camera[3] = {32,20,0};

    //Camera lookAt
    float l_camera[3] = {2,0,-10};

    //Light position
    float p_light[3] = {30,20,30};

    //Light lookAt
    float l_light[3] = {0,0,-5};

    //Light mouvement circle radius
    float light_mvnt = 30.0f;

    // Hold id of the framebuffer for light POV rendering
    GLuint fboId;

    // Z values will be rendered to this texture when using fboId framebuffer
    GLuint depthTextureId;

    // Use to activate/disable shadowShader
    GLhandleARB shadowShaderId;
    GLuint shadowMapUniform;

	static GLhandleARB currentlyBoundShadowID;

    
public:
    ShadowMapping(GLhandleARB);
    ~ShadowMapping();
	ShadowMapping(const char *vert, const char *frag, bool isFile = true);



    void generateShadowFBO();
    void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z);
    void update(void);
    void setTextureMatrix(void);
    void startTranslate(float x,float y,float z);
    void endTranslate();

    void drawObjects(Group*);
    void renderScene(Group*, bool);


	void bind();
	void unbind();

};




#endif /* ShadowMapping_hpp */
