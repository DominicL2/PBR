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
	return alpha2 / (3.141592 * pow(denom, 2.0));
}

float geometrySchlickGGX(float NdotV, float k) 
{
	float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float geometrySmith(float NdotV, float NdotL, float roughness) 
{
	float k = pow(roughness + 1.0, 2.0) / 8.0;
    float ggx1 = geometrySchlickGGX(NdotV, k);
    float ggx2 = geometrySchlickGGX(NdotL, k);
	return ggx1 * ggx2;
}

vec3 fresnel(vec3 F0, float cosTheta, vec3 color)
{	
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
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
	
	vec3 light		= normalize(v_lightPos - v_surface);
	vec3 view		= normalize(v_viewPos - v_surface);
	vec3 halfVector = normalize(light + view);
	float NdotV		= max(dot(normalMap, view), 0.0);
	float NdotL		= max(dot(normalMap, light), 0.0); /// Cosine Theta

	float 	D = distributionTrowbridgeReitzGGX(normalMap, halfVector, roughnessMap.g);
	float 	G = geometrySmith(NdotV, NdotL, roughnessMap.g);
	vec3	F = fresnel(vec3(0.3, 0.3, 0.3), NdotL, baseColor.xyz);

	float diffuse = getRambertianDiffuse(normalMap, light);
	vec3 specular = (D * F * G) / (3.141592 * NdotL * NdotV);
	
	vec3 Ks = F;
	vec3 Kd = 1.0 - Ks;
	Kd *= 1.0 - metallicMap.b;

	gl_FragColor = vec4(baseColor.xyz * ((Kd * diffuse) + (Ks * specular)), 1.0);
}