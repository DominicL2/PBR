precision mediump float;

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
uniform sampler2D texAlbedo;
uniform sampler2D texNormalMap;
uniform sampler2D texRoughnessMap;

vec3 getRambertianDiffuse(vec3 weight, vec3 color, vec3 normal, vec3 light)
{
	return vec3(weight * color * max(dot(normal, light), 0.0));	
}

float distributionTrowbridgeReitzGGX(vec3 normal, vec3 halfVector, float roughness) 
{
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float NdotH = max(dot(normal, halfVector), 0.0);
	
	float denom = 3.14*pow(NdotH * NdotH * (alpha2 - 1.0) + 1.0, 2.0);
	return alpha2 / denom;
}

float geometrySchlickGGX(vec3 normal, vec3 view, float roughness) 
{
	float NdotV = max(dot(normal, view), 0.0);
	float k = pow((roughness + 1.0), 2.0) / 8.0;	
	float denom = NdotV * (1.0 - k) + k;

	return NdotV / denom;
}

float fresnel(float ior, vec3 view, vec3 halfVector)
{
	float f0 = pow(abs((1.0 - ior) / (1.0 + ior)), 2.0);
	
	float VdotH = max(dot(view, halfVector), 0.0);
	return f0 + (1.0 - f0) * pow(1.0 - VdotH, 5.0);
}


float getCookTorranceSpecular(vec3 normal, vec3 light, vec3 view, vec3 color, float roughness)
{
	vec3 halfVector = normalize(light + view);
	float DFG = distributionTrowbridgeReitzGGX(normal, halfVector, roughness) * fresnel(1.45, view, halfVector) * geometrySchlickGGX(normal, view, roughness);
	float denom = 4.0 * max(dot(normal, light), 0.0) * max(dot(normal, view), 0.0);
	
	return DFG / denom;
}

void main()
{
	vec4 baseColor	= texture2D(texAlbedo, v_texcoord);
	vec3 normal		= texture2D(texNormalMap, v_texcoord).rgb;
	vec3 roughness	= texture2D(texRoughnessMap, v_texcoord).rgb;
	
	/// Change space form tangent to world
	normal = normal * 2.0 - 1.0;
	normal = vec3(normal.z, normal.y, normal.x);
	normal = normalize(v_TBN * normal);
	
	vec3 light = normalize(v_lightPos - v_surface);
	vec3 view = normalize(v_viewPos - v_surface);


	vec3 ambient = ambientW * baseColor.xyz;
	vec3 diffuse = diffuseW * getRambertianDiffuse(diffuseW, baseColor.xyz, normal, light);
	vec3 specular = specularW * getCookTorranceSpecular(normal, light, view, baseColor.xyz, roughness);
	
	//gl_FragColor = vec4(calcPhongModel(baseColor.xyz, normalize(v_normal)), 1.0);
}