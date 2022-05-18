varying vec2 v_texcoord;
varying vec3 v_viewPos;
varying vec3 v_lightPos;
varying mat3 v_TBN;
varying vec3 v_surface;

uniform vec3 u_ambientW;
uniform vec3 u_lightColor;  

uniform sampler2D texAlbedo;
uniform sampler2D texNormalMap;
uniform sampler2D texRoughnessMap;
uniform sampler2D texMetallicMap;
uniform sampler2D texEmissiveMap;

vec3 getRambertianDiffuse(vec3 color)
{
	return color / 3.141592;
}

vec3 getOrenNayarDiffuse(vec3 color, vec3 normal, vec3 view, vec3 light, float roughness) 
{
	float roguness2 = roughness * roughness;
	float NdotL = max(dot(normal, light), 0.0);
	float NdotV = max(dot(normal, view), 0.0);
	vec3 lambertian = color / 3.141592;
	float A = 1.0 - (roguness2 / (roguness2 + 0.33)) * 0.5;
	float B = 0.45 * roguness2 / (roguness2 + 0.09);
	float alpha = max(acos(NdotL), acos(NdotV));
	float beta = min(acos(NdotL), acos(NdotV));
	float gamma = max(0.0, dot(view - normal * NdotV, light - normal * NdotL));

	return lambertian * (A + B * gamma * sin(alpha) * tan(beta));
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
	float roughness		= texture2D(texRoughnessMap, v_texcoord).r;
	float metallic		= texture2D(texMetallicMap, v_texcoord).r;
	vec3 lightColor		= vec3(23.47, 21.31, 20.79);
	vec4 emissiveMap	= texture2D(texEmissiveMap, v_texcoord);

	/// Change space form tangent to world
	normalMap = normalMap * 2.0 - 1.0;
	normalMap = normalize(v_TBN * normalMap);
	
	vec3 light		= normalize(v_lightPos - v_surface);
	vec3 view		= normalize(v_viewPos - v_surface);
	vec3 halfVector = normalize(light + view);
	float NdotV		= max(dot(normalMap, view), 0.0);
	float NdotL		= max(dot(normalMap, light), 0.0); /// Cosine Theta
	float dist		= length(v_lightPos - v_surface);

	vec3 illuminance = u_lightColor * (1.0 / (dist * dist));
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, baseColor.xyz, metallic);

	float 	D = distributionTrowbridgeReitzGGX(normalMap, halfVector, roughness);
	float 	G = geometrySmith(NdotV, NdotL, roughness);
	vec3	F = fresnel(F0, NdotL, baseColor.xyz);

	vec3 diffuse = getOrenNayarDiffuse(baseColor.xyz, normalMap, view, light, roughness); //getRambertianDiffuse(baseColor.xyz);
	vec3 specular = (D * F * G) / max((3.141592 * NdotL * NdotV), 0.001);
	
	vec3 Ks = F;
	vec3 Kd = vec3(1.0) - Ks;
	Kd *= 1.0 - metallic;

	vec3 reflection = ((Kd * diffuse) + specular) * illuminance * NdotL;
	gl_FragColor = vec4(vec3(baseColor.xyz * u_ambientW) + reflection, 1.0) + emissiveMap;
}