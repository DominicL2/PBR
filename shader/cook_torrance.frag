varying vec2 v_texcoord;
varying vec3 v_viewPos;
varying vec3 v_lightPos;
varying mat3 v_TBN;
varying vec3 v_surface;

uniform sampler2D texAlbedo;
uniform sampler2D texNormalMap;
uniform sampler2D texRoughnessMap;
uniform sampler2D texMetallicMap;

float getRambertianDiffuse(vec3 normal, vec3 light)
{
	return max(dot(normal, light), 0.0);
}

float distributionTrowbridgeReitzGGX(vec3 normal, vec3 halfVector, float roughness) 
{
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha; 
	float NdotH = max(dot(normal, halfVector), 0.0);
	float NdotH2 = pow(NdotH, 2.0);
	float denom = NdotH2 * (alpha2 - 1.0) + 1.0;
	return alpha2 / (3.14 * pow(denom, 2.0));
}

float geometrySchlickGGX(float NdotV, float k) 
{
	float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float geometrySmith(vec3 normal, vec3 view, vec3 light, float roughness) 
{
	float k = pow(roughness + 1.0, 2.0) / 8.0;
	float NdotV = max(dot(normal, view), 0.0);
    float NdotL = max(dot(normal, light), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, k);
    float ggx2 = geometrySchlickGGX(NdotL, k);
	return ggx1 * ggx2;
}

vec3 fresnel(vec3 F0, float cosTheta, vec3 color, float metalness)
{	
	F0 = mix(F0, color, vec3(metalness, metalness, metalness));
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


vec3 getCookTorranceSpecular(vec3 normal, vec3 light, vec3 view, vec3 roughnessMap, vec3 metallicMap, vec3 color)
{
	vec3 halfVector = normalize(light + view);
	float cosTheta = dot(normal, light);
	float D = distributionTrowbridgeReitzGGX(normal, halfVector, roughnessMap.g);
	float G = geometrySmith(normal, view, light, roughnessMap.g);
	vec3 F = fresnel(vec3(0.3, 0.3, 0.3), cosTheta, color, metallicMap.b);
	vec3 DFG = D * F * G;

	float NdotV = max(dot(normal, view), 0.0);
	float NdotL = max(dot(normal, light), 0.0);
	float denom =  3.14 * NdotL * NdotV;
	
	return DFG / denom;
}

void main()
{
	vec4 baseColor		= texture2D(texAlbedo, v_texcoord);
	vec3 normalMap		= texture2D(texNormalMap, v_texcoord).rgb;
	vec3 roughnessMap	= texture2D(texRoughnessMap, v_texcoord).rgb;
	vec3 metallicMap	= texture2D(texMetallicMap, v_texcoord).rgb;

	/// Change space form tangent to world
	normalMap = normalMap * 2.0 - 1.0;
	normalMap = normalize(v_TBN * normalMap);
	
	vec3 light = normalize(v_lightPos - v_surface);
	vec3 view = normalize(v_viewPos - v_surface);

	float diffuse = getRambertianDiffuse(normalMap, light);
	vec3 specular = getCookTorranceSpecular(normalMap, light, view, roughnessMap, metallicMap, baseColor.xyz);
	vec3 Kd = 1.0 - specular;

	gl_FragColor = vec4(baseColor.xyz * ((Kd * diffuse) + specular), 1.0);
}