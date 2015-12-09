//#version 130
// vertex shader backup

// This preprocessor call should only fail if your OpenGL version
// is older than 3.0, which was released in August 2008.
// "varying" provides an interface between the vertex and fragment shaders.
// Anything that is "varying" is shared between the two.
varying vec4 position;
varying vec3 normal;
//varying vec4 gl_TexCoord[];

// Oh look, this looks just like C
void main(){

   normal = normalize(gl_Normal);
   
 
   position = gl_ModelViewMatrix * gl_Vertex;

 
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   

   

}