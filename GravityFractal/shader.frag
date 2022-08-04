#version 450
out vec4 fragColor;
uniform vec2 iResolution;
uniform float iScale;
uniform vec2 iPartPos[6];
uniform float iPartMass[6];
void main (void)
{
	float G = 0.002;
	vec4 col_arr[6];
	col_arr[0] = vec4(0.92, 0.46, 0.19, 0);
	col_arr[1] = vec4(0.42, 0.42, 0.9, 0);
	col_arr[2] = vec4(0.31, 0.6, 0.2, 0);
	col_arr[3] = vec4(0.73, 0.91, 0.64, 0);
	col_arr[4] = vec4(0.21, 0.75, 0.42, 0);
	col_arr[5] = vec4(0.85, 0.22, 0.86, 0);
	
	vec2 coord = gl_FragCoord.xy / iResolution - vec2(0.5);
	coord.x /= iResolution.y / iResolution.x;
	vec4 color = vec4(0);
	
	coord /= iScale;
	
	vec2 pos = coord;
	vec2 vel = vec2(0);
	vec2 acl = vec2(0);
	
	for(int i = 0; i < 100; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			acl += normalize(iPartPos[j] - pos) * G * iPartMass[j] /
			(distance(iPartPos[j], pos) * distance(iPartPos[j], pos));
		}
		vel += acl;
		pos += vel;
	}
	float min_d = distance(iPartPos[0], pos);
	int id = -1;
	for(int i = 0; i < 6; i++)
	{
		if(distance(iPartPos[i], pos) <= min_d)
		{
			min_d = distance(iPartPos[i], pos);
			id = i;
		}
	}
	if(id == -1)
	{
		color = vec4(0);
	}
	else
	{
		color = col_arr[id];
	}
	color.a = log(min_d / 1000);
	fragColor = color;
}