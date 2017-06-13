attribute vec4 a_color;  
attribute vec4 a_position;  
varying vec4 v_color;  
uniform mat4 u_MVPMatrix;  
void main()  
{  
    v_color = a_color;  
    gl_Position = u_MVPMatrix * a_position;  
}