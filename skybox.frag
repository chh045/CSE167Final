#version 130
// fragment shader

// Once again, this preprocessor will only fail if your OpenGL version
// is horribly outdated.

// The fragment shader will get the values of these varying qualifiers
// from the vertex shader
varying vec4 position;
varying vec3 normal;
varying vec3 TexCoords;
// Setup a sampler for texture
uniform samplerCube skybox;
//varying vec4 gl_TexCoord[];

// Another sample fragment shader is provided in the Spring 2015 midterm 2.
// You'd definitely want to take a look at that to know how to modify the
// colors based off of the positions, lighting, and normals. Plus you need
// to review for midterm 2 anyways, so why not start soon?
void main(){
   /*
   // Variables used for calculations declared here
   vec3 lightDirection;

   vec3 finalColor = vec3(0.0, 0.0, 0.9);

   // Loop through the 8 lights supported by OpenGL.
   // Now you know what the light IDs are used for!
   for (int i = 0; i < 8; i++){
      // Calculate the light direction
      lightDirection = gl_LightSource[i].position.xyz - position.xyz;

      // Determine if the light is in front of the surface.
      // If it is, I want to highlight it yellow
      if(dot(normal, lightDirection) >= 0.0)
         finalColor = vec3(0.9, 0.9, 0.0);
   }
    */
    //gl_TexCoord[0] = gl_MultiTexCoord0;
    
    
    //Find out texture coordinate
    //vec2 st = gl_TexCoord[0].st;
    // Find out texture color at that coordinate
    //vec4 textureColor = texture2D(textureSampler, st);
    gl_FragColor = texture(skybox, TexCoords);
    // Set fragment color to the color we want
    //gl_FragColor = manipulate(textureColor);
    
    /*
    vec4 white = vec4(0, 0, 0, 1);
    vec4 black = vec4(1, 1, 1, 1);
    
    if (textureColor.x < 0.5) // black
        gl_FragColor = vec4(0, 0, 0, 0);
    else { // white
        vec3 nn = normal * 0.5;
        nn = nn + vec3(0.5, 0.5, 0.5);
        
        gl_FragColor = vec4(nn, 1);
    }
    */
    
    //fprintf(stderr, "here\n");
    
   // Return the color for this fragment. It will be blue if no light
   // shines on it, and yellow if it receives light. GO TRITONS!
   //gl_FragColor = vec4(finalColor, 1.0);
}