#include <iostream>
#include "CubeMapTexture.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "glee.h"
#endif

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)? (CHECK THIS)
// -Z (back)?
CubeMapTexture::CubeMapTexture(
	const char* rtTex, const char* ltTex,
	const char* tpTex, const char* dnTex,
	const char* ftTex, const char* bkTex) {

	std::vector<const char*> fv;
	fv.push_back(rtTex);
	fv.push_back(ltTex);
	fv.push_back(tpTex);
	fv.push_back(dnTex);
	fv.push_back(ftTex);
	fv.push_back(bkTex);

	// create ID for texture
	GLuint tID;
	glGenTextures(1, &tID); // this func ensures the id is unique 
	textureID = tID;

	// set the currentTextureUnit to be default, could be skipped?
	glActiveTexture(GL_TEXTURE0);

	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	// bind
	glBindTexture(GL_TEXTURE_CUBE_MAP, tID);

	for (GLuint i = 0; i < fv.size(); i++)
	{
		tdata = loadPPM(fv[i], twidth, theight);

		//If the image wasn't loaded, can't continue
		if (tdata == NULL) {
			std::cerr << "no texture loaded\n";
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMapTexture::bind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void CubeMapTexture::unbind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* CubeMapTexture::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	size_t read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	//Open the texture file
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	//Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	//Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	//Set the width and height
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	//Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	//Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);

	//If the read was a failure, error
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}