#version 120
varying float xpos;
varying float ypos;
varying float zpos;
varying vec3 normal;

uniform sampler2D myTexture;

void main(){
    
    vec4 color = texture2D(myTexture, gl_TexCoord[0].st);
 //   if (color[0] < 0.5 && color[1] < 0.5 && color[2] < 0.5) {
//        discard;
//    }
//    else{
//        gl_FragColor = vec4 (normal.x * 0.2 + 0.1, normal.y *0.3*0.5, normal.z * 0.6 * 0.5, 0.5);
//    }
 
 gl_FragColor = vec4 (normal.x * 0.2 + 0.1, normal.y *0.3*0.5, normal.z * 0.6 * 0.5, 0.2);




}