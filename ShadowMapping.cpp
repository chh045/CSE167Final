/*            :::::::::::WARNING::::::::::::::
** PLEASE, DO NOT INCLUDE SAME HEADER MORE THAN ONCE.
** PLEASE, GLEE.H ALWAYS DEFINED BEFORE GLUT.H
** PLEASE, ADD ONLY HEADERS IN H-FILES IF YOU NEED TO USE IN THE HEADER
** OTHERWISE, PUT IT IN .CPP FILE
**/

#include "ShadowMapping.h"
#include <time.h>
#ifdef __APPLE__
#include <math.h>
#endif


/* for drawing multiple obj */
#define monsterNum 3
#define girlNum 3
#define offset -5
#define step_size 0.1
//#define turing_ratio 0.2

// generate a random float in [-step_size, step_size]
#define random_num -step_size+2*step_size*((float)rand()/RAND_MAX) 

const int girl_init_x = -3;
const int monster_init_x = -7;
const int girl_init_z = 0;
const int monster_init_z = 0;
std::vector<Vector3*> girl_pos; // postion
std::vector<Vector3*> monster_pos;
std::vector<Vector3*> girl_dir; // direction vector, store normalized vectors
std::vector<Vector3*> monster_dir; // direction vector, normalized
std::vector<bool> girl_collision;
std::vector<bool> monster_collision;

GLhandleARB ShadowMapping::currentlyBoundShadowID = 0x0;

void genCharacterPos() {
	std::srand(time(NULL));
	for (int i = 0; i < girlNum; i++) {
		// generate position
		girl_pos.push_back(new Vector3(girl_init_x, Globals::grd_depth, girl_init_z + i * offset));
		
		// generate direction vector
		Vector3 dir(random_num, 0, random_num);
		girl_dir.push_back(new Vector3(dir.normalize()));
		girl_collision.push_back(false);
	}

	for (int i = 0; i < monsterNum; i++) {
		// generate position
		monster_pos.push_back(new Vector3(monster_init_x, Globals::grd_depth, monster_init_z + i * offset));

		// generate direction vector
		Vector3 dir(random_num, 0, random_num);
		monster_dir.push_back(new Vector3(dir.normalize()));
		monster_collision.push_back(false);
	}
}

bool isTouchWall(Vector3 pos, float radius, Vector3 & reflect_vec) {
	float sq_l = Globals::grd_length / 2; // z
	float sq_w = Globals::grd_width / 2; // x
	float x = pos[0];
	float z = pos[2];

	// detect right wall
	if ((x + radius) > sq_w) {
		reflect_vec = Vector3(-1, 0, 0);
		return true;
	}
	// detect left wall
	if ((x - radius) < -sq_w) {
		reflect_vec = Vector3(1, 0, 0);
		return true;
	}
	// detect front wall (in front of camera)
	if ((z - radius) < -sq_l) {
		reflect_vec = Vector3(0, 0, 1);
		return true;
	}
	// detect back wall (in the back of camera)
	if ((z + radius) > sq_l) {
		reflect_vec = Vector3(0, 0, -1);
		return true;
	}
	return false;
}

void random_walk(float girl_r, float mons_r) {
	
	
	// girls
	for (int i = 0; i < girlNum; i++) {

		// calculate the new pos but dont apply
		Vector3 delta_dir = girl_dir.at(i)->multiply(step_size);
		Vector3 new_pos = girl_pos.at(i)->add(delta_dir);

		// detect if new_pos of girl touchs the wall
		Vector3 reflect_dir;
		if (isTouchWall(new_pos, girl_r, reflect_dir)) {
			Vector3 L = *(girl_dir.at(i));
			Vector3 new_dir = L + reflect_dir.multiply(2 * (L.multiply(-1).dot(reflect_dir)));
			*(girl_dir.at(i)) = new_dir;
			*(girl_pos.at(i)) = girl_pos.at(i)->add(*(girl_dir.at(i))*step_size);

			girl_collision.at(i) = true;
		}
		// else assign new_pos to current pos
		else {
			*(girl_pos.at(i)) = new_pos;
		}
	}

	for (int i = 0; i < monsterNum; i++) {
		// calculate the new pos but dont apply
		Vector3 delta_dir = monster_dir.at(i)->multiply(step_size);
		Vector3 new_pos = monster_pos.at(i)->add(delta_dir);

		// detect if new_pos of girl touchs the wall
		Vector3 reflect_dir;
		if (isTouchWall(new_pos, mons_r, reflect_dir)) {
			Vector3 L = *(monster_dir.at(i));
			Vector3 new_dir = L + reflect_dir.multiply(2 * (L.multiply(-1).dot(reflect_dir)));
			*(monster_dir.at(i)) = new_dir;
			*(monster_pos.at(i)) = monster_pos.at(i)->add(*(monster_dir.at(i))*step_size);

			monster_collision.at(i) = true;
		}
		// else assign new_pos to current pos
		else {
			*(monster_pos.at(i)) = new_pos;
		}
		/*
		// updating the position by direction vector 
		Vector3 delta_dir = monster_pos.at(i)->multiply(step_size);
		*(monster_pos.at(i)) = monster_pos.at(i)->add(delta_dir);

		// updating the direction vector
		Vector3 new_dir(random_num, 0, random_num);
		//new_dir = (*(monster_pos.at(i)) + new_dir.normalize()).normalize();
		*(monster_pos.at(i)) = new_dir.normalize();
		*/
	}
}

ShadowMapping::ShadowMapping(GLhandleARB shadowShader){
    
    shadowShaderId = shadowShader;
    shadowMapUniform = glGetUniformLocationARB(shadowShaderId,"ShadowMap");
    generateShadowFBO();
    //loadShadowShader(vert, frag);

}

ShadowMapping::~ShadowMapping(){
	glDeleteObjectARB(shadowShaderId);
}

ShadowMapping::ShadowMapping(const char *vert, const char *frag, bool isFile)
{
	if (isFile)
	{
		//Read in the vertex and fragment shaders
		//We must delete these after we are finished compiling the shaders
		char* vv = read(vert);
		char* vf = read(frag);

		//Setup the shader
		setup(vv, vf);

		//Delete the file data arrays we allocted
		delete[] vv;
		delete[] vf;
	}
	else
	{
		//Treat the vert and frag parameters as shader code and directly compile them
		setup(vert, frag);
	}

	shadowMapUniform = glGetUniformLocationARB(shadowShaderId, "ShadowMap");
	generateShadowFBO();
	/*cout << "shadow id: " << shadowShaderId << endl;
	cout << "shadowMapUniform: " << shadowMapUniform << endl;
	cout << "depthTextureId: " << depthTextureId << endl;*/

	// call local function to build girl_pos and monster_pos
	genCharacterPos();
}

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
    
  //  p_light[0] = light_mvnt * cos(3652/1000.0) + 100;
  //  p_light[2] = light_mvnt * sin(3652/1000.0) - 100;
}

void ShadowMapping::setTextureMatrix(void)
{
	// diego
	static double modelView[16];
	static double projection[16];

	const GLdouble bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0 };

	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixd(bias);
	glMultMatrixd(projection);
	glMultMatrixd(modelView);

	/* edisexy
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
	*/
	
}

// During translation, we also have to maintain the GL_TEXTURE8, used in the shadow shader
// to determine if a vertex is in the shadow.
void ShadowMapping::startTranslate(float x,float y,float z)
{
	// wong way	
	glPushMatrix();
	glTranslatef(x, y, z);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glTranslatef(x, y, z);

// edison chen
   /* glPushMatrix();
    glTranslatef(x,y,z);
    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    glPushMatrix();
    glTranslatef(x,y,z);*/
}

void ShadowMapping::endTranslate()
{
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void ShadowMapping::drawObjects(Group* group)
{
	// very hard coding, very bad..
	Globals::girl.bbox_on = Globals::bounding_box_on;
	
	float d = Globals::grd_depth;
	
	// Ground
	float sq_l = Globals::grd_length / 2;
	float sq_w = Globals::grd_width / 2;
	glColor4f(244.0 / 255, 164.0 / 255, 96.0 / 255, 1);
	//glColor4f(139.0/255, 69.0/255, 19.0/255, 1);
	glBegin(GL_QUADS);
	glVertex3f(-sq_w, d, sq_l);
	glVertex3f(sq_w, d, sq_l);
	glVertex3f(sq_w, d, -sq_l);
	glVertex3f(-sq_w, d, -sq_l);
	glEnd();
	

    // Ground

	/*
	Globals::grdTex->bind();
    glColor4f(0.3f, 0.3f, 0.3f, 1);
    glBegin(GL_QUADS);
	glTexCoord2f(0, 50);
    glVertex3f(-4000.0,d,10000.0);
	glTexCoord2f(50, 50);
    glVertex3f(4000.0,d,10000.0);
	glTexCoord2f(50, 0);
    glVertex3f(4000.0,d,-10000.0);
	glTexCoord2f(0, 0);
    glVertex3f(-4000.0,d,-10000.0);
    glEnd();
	Globals::grdTex->unbind();
	*/
	/*
	int length = Globals::grd_length;
	//Globals::grdTex->bind();
	glColor4f(0.3, 0.3, 0.3, 1);
	glBegin(GL_QUADS);
	
	length *= 4;
	cout << length << endl;
	
	//glNormal3f(0.0, 1.0, 0.0);
	//glTexCoord2f(0, 50); 
	glVertex3f(length, -1.5, -length*25);
	//glTexCoord2f(50, 50); 
	glVertex3f(length, -1.5, length*25);
	//glTexCoord2f(50, 0); 
	glVertex3f(-length, -1.5, length*25);
	//glTexCoord2f(0, 0); 
	glVertex3f(-length, -1.5, -length*25);

	glEnd();
	//Globals::grdTex->unbind();
	*/

	/*
	// try to draw the groud
	int length = Globals::grd_length;
	glBegin(GL_QUADS);

	glColor4f(0.3, 0.3, 0.3, 1);
	//glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(length, -1.5, -length);
	glVertex3f(length, -1.5, length);
	glVertex3f(-length, -1.5, length);
	glVertex3f(-length, -1.5, -length);
	glEnd();
	*/

	// draw cube
    glColor4f(0.9f,0.9f,0.9f,1);
    
    startTranslate(5,-18,-5);
    //glutSolidCube(4);
    endTranslate();
    
    startTranslate(-5,-10,-5);
    //glutSolidSphere(3, 5000, 5000);
    endTranslate();
    
    
    

	//startTranslate(13, -20, -10);
	glPushMatrix();
	glTranslatef(-30, -20, -10);
	glScalef(0.5f, 0.5f, 0.5f);
	Globals::tree.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, -20, -10);
	glScalef(0.5f, 0.5f, 0.5f);
	Globals::tree1.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30, -20, 10);
	glScalef(0.5f, 0.5f, 0.5f);
	Globals::tree2.draw();
	//cout << Globals::tree.current_depth << endl;;
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, -20, 10);
	glScalef(0.5f, 0.5f, 0.5f);
	Globals::tree.draw();
	glPopMatrix();
	//glTranslatef(-13, -20, -10);
	//endTranslate();

    /*
	startTranslate(3, -20, 8);
	Globals::bunny.render();
	endTranslate();
	*/

	startTranslate(-7, -20, 20);
	Globals::house.render();
	endTranslate();

	// drawing characters

	for (int i = 0; i < girl_pos.size(); i++) {
		if (girl_collision.at(i)) {
			Globals::girl.touched = true;
			girl_collision.at(i) = false;
		}
		
		float x = girl_pos.at(i)->operator[](0);
		float y = girl_pos.at(i)->operator[](1);
		float z = girl_pos.at(i)->operator[](2);
		startTranslate(x, y, z);
		Globals::girl.render();
		Globals::girl.touched = false;
		endTranslate();
	}
	for (int i = 0; i < monster_pos.size(); i++) {
		if (monster_collision.at(i)) {
			Globals::monster.touched = true;
			monster_collision.at(i) = false;
		}
		
		float x = monster_pos.at(i)->operator[](0);
		float y = monster_pos.at(i)->operator[](1);
		float z = monster_pos.at(i)->operator[](2);
		startTranslate(x, y, z);
		Globals::monster.render();
		Globals::monster.touched = false;
		endTranslate();
	}

	// update monster_pos and girl_pos

	if (Globals::movementOn)
	random_walk(Globals::girl.radius_bounding, Globals::monster.radius_bounding);

	//Globals::girl.render();
	
    
}


void ShadowMapping::renderScene(Group* group, bool spin)
{

	// Diego modified version. - Old records can be found in github only

	if (spin) {
		update();
	}

	//First step: Render from the light POV to a FBO, story depth values only
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);	//Rendering offscreen

														//Using the fixed pipeline to render to the depthbuffer
	

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0, 0, RENDER_WIDTH * SHADOW_MAP_RATIO, RENDER_HEIGHT* SHADOW_MAP_RATIO);

	// Clear previous frame values
	glClear(GL_DEPTH_BUFFER_BIT);

	//Disable color rendering, we only want to write to the Z-Buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	Vector3 l = Globals::light.getPosition();
	//std::cout << l_light[0] << std::endl;  // -15.9  |   | -15.9
	
	//setupMatrices(p_light[0], 20, p_light[2], -30, 3, 20);

	setupMatrices(p_light[0], p_light[1], p_light[2], l_light[0], l_light[1], l_light[2]);

	//setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);

	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	glCullFace(GL_FRONT);
	drawObjects(group);

	//Save modelview/projection matrice into texture7, also add a biais
	setTextureMatrix();


	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//glViewport(0, 0, 640, 480);
	glViewport(0, 0, 1350, 750);

	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


	Vector3 p_c = Globals::camera.getPosition();
	Vector3 l_c = Globals::camera.getLookAt();
	setupMatrices(p_c[0], p_c[1], p_c[2], l_c[0], l_c[1], l_c[2]);
}


char* ShadowMapping::read(const char *filename)
{
	char* shaderFile = 0;

	//Open the file
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		std::cerr << "File doesn't exist [" << filename << "]" << std::endl;
		std::exit(-1);
	}

	//Obtain the file size
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	//Alloc memory - will be deleted while setting the shader up
	shaderFile = new char[size + 1];

	//Copy the file to the shaderFile
	fread(shaderFile, sizeof(char), size, fp);
	shaderFile[size] = '\0'; //Eliminate the garbage at EOF
	fclose(fp);

	return shaderFile;
}

void ShadowMapping::setup(const char *vs, const char *fs)
{
	//Create two new Shader Object IDs
	GLhandleARB vid = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	GLhandleARB fid = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	//Pass the shader source code to OpenGL
	glShaderSourceARB(vid, 1, &vs, 0);
	glShaderSourceARB(fid, 1, &fs, 0);

	//Compile the shader files
	glCompileShaderARB(vid);
	glCompileShaderARB(fid);

	char glslLog[1024];
	GLsizei glslLogSize;

	//Get the error log for the Vertex shader
	glGetInfoLogARB(vid, 1024, &glslLogSize, glslLog);
	if (glslLogSize)
		std::cerr << "Vertex program log: " << glslLog << std::endl;

	//Get the error log for the Fragment shader
	glGetInfoLogARB(fid, 1024, &glslLogSize, glslLog);
	if (glslLogSize)
		std::cerr << "Fragment program log: " << glslLog << std::endl;

	//Create a new Shader Program
	shadowShaderId = glCreateProgramObjectARB();

	//Attach the Vertex and Fragment shaders to the Shader Program
	glAttachObjectARB(shadowShaderId, vid);
	glAttachObjectARB(shadowShaderId, fid);

	//Delete shader objects since they have been attached to a program
	glDeleteObjectARB(vid);
	glDeleteObjectARB(fid);

	//Link it!
	glLinkProgramARB(shadowShaderId);
}


void ShadowMapping::bind()
{
	if (currentlyBoundShadowID != shadowShaderId)
	{
		currentlyBoundShadowID = shadowShaderId;
		glUseProgramObjectARB(shadowShaderId);
		glBindTexture(GL_TEXTURE_2D, depthTextureId);
		glActiveTextureARB(GL_TEXTURE1_ARB);  // active
	}
}

void ShadowMapping::unbind()
{
	if (currentlyBoundShadowID != NULL)
	{
		currentlyBoundShadowID = NULL;
		glActiveTextureARB(GL_TEXTURE0_ARB);  // active
	    glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgramObjectARB(0);
	}
}
