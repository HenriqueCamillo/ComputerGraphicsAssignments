varying vec3 TexCoords;
uniform samplerCube skybox;

vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform float ka; 

void main() {    
    vec3 ambient = ka * lightColor;             
    vec4 texture = textureCube(skybox, TexCoords);
    vec4 result = vec4(ambient, 1.0) * texture;
    gl_FragColor = result;
}