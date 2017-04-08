#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

in vec3 v_color;
out vec3 FragColor;
//! [0]
void main()
{
    // Set fragment color from texture
    FragColor = v_color;
}
//! [0]

