#version 130
#define NO_DEFAULT_INCLUDE

uniform vec2 resolution;
uniform float time;

vec4 LFSR_Rand_Gen_4f(vec4 n){
  return (mod(n * mod(n*n*15731.0+789221.0, 2147483647.0) + 1376312589.0, 2147483647.0));
}

float noise3f(vec3 p){
	float res = 0.0;
	vec3 u = fract(p + vec3(0.0, 0.0, time * 0.1));
	vec3 ip = floor(p + vec3(0.0, 0.0, time * 0.1));
	u = u * u * (3.0 - 2.0 * u);

//	float n = ip.x*7.0 + ip.y*34.0 + ip.z*289.0;
//	vec4 rand1 = vec4(n) + vec4(0.0, 34.0, 289.0, 34.0+289.0);
//	vec4 rand2 = rand1 + vec4(7.0);
	float n = dot(ip, vec3(1.0, 57.0, 113.0));
	vec4 rand1 = vec4(n) + vec4(0.0, 57.0, 113.0, 57.0+113.0);
	vec4 rand2 = rand1 + vec4(1.0);

	rand1 = LFSR_Rand_Gen_4f(rand1);
	rand2 = LFSR_Rand_Gen_4f(rand2);

	rand1 = mix(rand1, rand2, u.x);
	rand1.xy = mix(rand1.xz, rand1.yw, u.y);
	res = mix(rand1.x, rand1.y, u.z);

	return 1.0 - res*(1.0/1073741824.0);
}

float PNoise3(vec3 v, int terms){
	int i;
	float result = 0.0;
	float amp = 1.0;
	float maxAmp = 0.0;

	for(i=0; i<terms; i++){
		result += noise3f(v) * amp;
		v *= 2.0;
		maxAmp += amp;
		amp *= 0.5;
	}
	return result / maxAmp;
}

float RNoise3(vec3 v, int terms){
	int i;
	float result = 0.0;
	float amp = 1.0;
	float maxAmp = 0.0;
	float r = 0.0;
	float prev = 0.0;

	for(i=0; i<terms; i++){
		r = noise3f(v);
		r = 1.0 - abs(r);
		r = r * r;
	//	r = r * 2.0;
		r = exp(r * 1.1) - 1.0;
		result += r * amp * prev;
		prev = r;
		v *= 3.0;
	//	amp *= 0.5;
		amp *= 1.0;
		maxAmp += amp * 2.0;
	}
	return result / maxAmp;
}

float band(vec2 uv, float ftime1, float ftime2)
{
	float value = (exp((uv.x - 0.5) * (10.0 * sin(time * ftime1)) + 0.25 * cos(time * ftime2)) - 1.0) * 5 - 10.0 * uv.y;
//	return clamp(0.1 * min(1.0, exp(value * 1.0) - 1.0), 0.0, 1.0);
	return clamp(0.15 * min(1.0, value * 0.05), 0.0, 1.0);
}

void main(){
	vec2 uv = gl_FragCoord.xy / resolution;

	vec3 color = vec3(0.0);
//	color = vec3(uv, 0.0);

//	color = vec3(0.5 + 0.5 * sin(time), uv);
//	color *= 0.25;

	color.r += band(uv + vec2(0.1 * sin(time + 0.5), uv.y), -0.18, 0.25);
	color.g += band(uv.yx, -0.15, 0.25);
	color.b += band(vec2(uv.x, 1.0-uv.y), 0.1, 0.25);

//	color.r += band(uv, -0.12, 0.25);
//	color.g += band(uv, -0.12, 0.25);
	color.b += band(uv.yx + vec2(0.1 * sin(time + 0.5), uv.y), -0.12, 0.25);

	float value = RNoise3(vec3(uv * 1.5, 0.0), 4);
	value = exp(value - 0.5) - 1.0;
	vec3 v = clamp(color + value, vec3(0.0), vec3(1.0));

	color += v;

	gl_FragColor = vec4(color, 1.0);
}