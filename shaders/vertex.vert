attribute vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
attribute vec2 texture_coord;
varying vec2 out_texture;

void main() {
    gl_Position = projection * view * model * vec4(position,1.0);
    out_texture = vec2(texture_coord);
}