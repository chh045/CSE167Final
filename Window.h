#ifndef CSE167_Window_h
#define CSE167_Window_h

class Window	  // OpenGL output window related routines
{
	
public:
    
    static int width, height; 	            // window size
    
    static void initialize(void);
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void draw();
	static void keyboardCallback(unsigned char, int, int);

	static void specialKeyCallback(int key, int x, int y);
	static void mouseButton(int button, int state, int x, int y);
	static void mouseMotion(int x, int y);
	
	static bool mouse_rotate_on;
	static int last_x, last_y;


};

#endif