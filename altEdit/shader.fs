#version 410

uniform vec2 iResolution;
uniform vec2 iMouse;
uniform float iTime;

out vec4 fColor;

#define MAX_STEPS 100
#define MAX_DIST 100.0
#define SURF_DIST 0.01


float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdPlane( vec3 p )
{
	return p.y;
}

float sdSphere( vec3 p, float s )
{
    return length(p)-s;
}

float GetDist(vec3 p)
{
	vec4 s = vec4(0, 1, 5, 1);
	vec3 b = vec3(0.75, 0.75, 0.75);

//	float sphereDist = length(p-s.xyz)-s.w;
	float sphereDist = sdSphere(p-s.xyz, s.w);
	float boxDist = sdBox(p-s.xyz, b);
	float planeDist = p.y;

	float d = min(boxDist, planeDist);
	return d;
}










float RayMarch(vec3 ro, vec3 rd)
{
	float dO = 0.0;

	for(int i = 0; i < MAX_STEPS; i++)
	{
		vec3 p = ro + rd * dO;
		float dS = GetDist(p);
		dO += dS;
		if (dO > MAX_DIST || dS < SURF_DIST)
			break;
	}

	return dO;
}


vec3 GetNormal(vec3 p)
{
	float d = GetDist(p);
	vec2 e = vec2(0.01, 0);

	vec3 n = d - vec3(
		GetDist(p-e.xyy),
		GetDist(p-e.yxy),
		GetDist(p-e.yyx)
	);

	return normalize(n);
}

float GetLight(vec3 p)
{
	vec3 lightPos = vec3(0,5,3);
	lightPos.xz += vec2(sin(iTime), cos(iTime)) * 2.0;
	vec3 l = normalize(lightPos-p);
	vec3 n = GetNormal(p);

	float dif = clamp(dot(n, l), 0.0, 1.0);
	float d = RayMarch(p + n * SURF_DIST*2.0, l);
	if (d < length(lightPos-p))
		dif *= 0.1;

	return dif;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = (fragCoord- 0.5 * iResolution.xy) / iResolution.y;
	vec3 col = vec3(0);

	vec3 ro = vec3(0,1,0);
	vec3 rd = normalize(vec3(uv.x, uv.y, 1));

	float d  = RayMarch(ro, rd);
    
    vec3 p = ro + rd * d;
    
    float dif = GetLight(p);
	col = vec3(dif);
    col = pow(col, vec3(.4545));	// gamma correction
	fragColor = vec4(col, 1.0);
}
     
void main()
{
	mainImage(fColor, gl_FragCoord.xy);
}
