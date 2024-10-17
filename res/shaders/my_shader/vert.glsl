#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in float alpha;
layout (location = 3) in vec2 uv;
layout (location = 4) in vec3 normal;

out vec3 pass_color;
out vec2 pass_uv;
out vec3 pass_normal;
out vec3 pass_pos;

uniform mat4 uni_M;
uniform mat4 uni_VP;

void main()
{
	///model*local(vertex) pozicija pixela trenutnog koji se obradjuje
	vec4 world_position = uni_M * vec4(position, 1.0);
	
	///konacna pozicija
	/// Vclip(krajnja pozicija pixela)=Projection x View x Model x Local(vertices),mnozi se s desna na levo.
	gl_Position = uni_VP * world_position;
	
	pass_color = color;
	pass_uv = uv;
	///prosledjujemo poziciju pixela 
	pass_pos = (world_position).xyz;
}