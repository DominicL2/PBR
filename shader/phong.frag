varying vec2 v_texcoord;                         
varying vec3 v_normal;                           
varying vec3 v_viewPos;                          
varying vec3 v_lightPos;                         
varying mat3 v_TBN;                              
varying vec3 v_surface;                          
uniform vec3 ambientW;                           
uniform vec3 diffuseW;                           
uniform vec3 specularW;                          
uniform vec3 defaultColor;                       
uniform float shiness;       
uniform vec3 u_lightColor;                    
uniform sampler2D texAlbedo;                     
uniform sampler2D texNormalMap;                  
vec3 calcPhongModel(vec3 color, vec3 normal)     
{                                                                                
	vec3 ambient = ambientW * color;                                               
	vec3 light = normalize(v_lightPos - v_surface);                                
	vec3 view = normalize(v_viewPos - v_surface);                                  
	vec3 diffuse = diffuseW * color * max(dot(normal, light), 0.0);                
	vec3 reflection = reflect(-light, normal);    
	vec3 specular = specularW * pow(max(dot(reflection, view), 0.0), shiness) * u_lightColor;     
	return (ambient + diffuse + specular);                                         
}                                                                                
void main()                                                                      
{                                                                                
	vec4 baseColor = texture2D(texAlbedo, v_texcoord);                             
	if (defaultColor.x > 0.0 || defaultColor.y > 0.0 || defaultColor.z > 0.0) {    
		baseColor = vec4(defaultColor, 1.0);                                          
	}                                                                              
	vec3 normal = texture2D(texNormalMap, v_texcoord).rgb;                         
	if (normal.r > 0.0 || normal.g > 0.0 || normal.b > 0.0) {                      
	    normal = normal * 2.0 - 1.0;                                              
	    normal = normalize(v_TBN * normal);                                       
	} else {                                                                      
	    normal = normalize(v_normal);                                             
	}                                                                             
	gl_FragColor = vec4(calcPhongModel(baseColor.xyz, normal), 1.0);               
}