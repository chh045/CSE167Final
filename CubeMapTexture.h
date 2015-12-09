#ifndef CSE167_CubeMapTexture_h
#define CSE167_CubeMapTexture_h





class CubeMapTexture
{
	unsigned int textureID;
	static unsigned char* loadPPM(const char*, int&, int&);

public:
	CubeMapTexture(void) {};
	CubeMapTexture(
		const char* rtTex, 
		const char* ltTex,
		const char* tpTex, 
		const char* dnTex, 
		const char* ftTex, 
		const char* bkTex, bool repeatTex_on);
	//~CubeMapTexture(void) {};
	//GLuint Load(std::vector<const char*>);

	void bind(void);
	void unbind(void);
};

#endif