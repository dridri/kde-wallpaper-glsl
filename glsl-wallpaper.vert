#version 130
#define NO_DEFAULT_INCLUDE

void main(){
	gl_Position = gl_ProjectionMatrix * gl_Vertex;
}
