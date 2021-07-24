uniform vec3 catLightPos; 
uniform vec3 sunPos;
uniform float ka; 
uniform float kd;

uniform vec3 viewPos;
uniform float ks;
uniform float ns; 

vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 catLightColor = vec3(1.0, 0.0, 0.0);

varying vec2 out_texture; 
varying vec3 out_normal; 
varying vec3 out_fragPos; 
uniform sampler2D samplerTexture;

void main(){
    // Ambient light
    vec3 ambient = ka * lightColor;             

    // Diffuse light
    vec3 catNorm = normalize(out_normal); 
    vec3 catLightDir = normalize(catLightPos - out_fragPos); 
    float catDiff = max(dot(catNorm, catLightDir), 0.0); 
    vec3 catDiffuse = kd * catDiff * catLightColor; 
    
    // Specular light
    vec3 catViewDir = normalize(viewPos - out_fragPos); 
    vec3 catReflectDir = reflect(-catLightDir, catNorm); 
    float catSpec = pow(max(dot(catViewDir, catReflectDir), 0.0), ns);
    vec3 catSpecular = ks * catSpec * catLightColor;    

    // Diffuse light
    vec3 sunNorm = normalize(out_normal); 
    vec3 sunDir = normalize(sunPos - out_fragPos); 
    float sunDiff = max(dot(sunNorm, sunDir), 0.0); 
    vec3 sunDiffuse = kd * sunDiff * lightColor; 
    
    // Specular light
    vec3 sunViewDir = normalize(viewPos - out_fragPos); 
    vec3 sunReflectDir = reflect(-sunDir, sunNorm); 
    float sunSpec = pow(max(dot(sunViewDir, sunReflectDir), 0.0), ns);
    vec3 sunSpecular = ks * sunSpec * lightColor;    
 
 
    // Result
    vec4 texture = texture2D(samplerTexture, out_texture);
    vec4 result = vec4(ambient + catDiffuse + sunDiffuse + catSpecular + sunSpecular,1.0) * texture;
    gl_FragColor = result;
}