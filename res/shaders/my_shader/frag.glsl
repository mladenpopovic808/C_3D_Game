#version 330

in vec3 pass_pos;
in vec3 pass_color;
in vec2 pass_uv;


out vec4 final_color;

// sampler2D je "handle" preko kog mozemo uzeti uzorak iz 2D teksture
// Nacin na koji se uzorak uzima je definisam parametrima date teksture
// (filtriranje, ponavljanje, mipmape, itd.)
// slicno kao i rafgl_bilinear_sample ili rafgl_point_sample
uniform sampler2D albedo;

uniform vec3 uni_light_color; 
uniform vec3 uni_light_direction; 
uniform vec3 uni_light_source; ///camera_position

void main()
{
	///centar
	vec3 center_light_source = vec3(-1.0,-1.0,-1.0);
	vec3 light_color_1 = vec3(1.0, 0.7, 0.4);

	float light_coef;
	
	light_coef = 1.0;  ///jacina svetla
	
	///koeficijent koji odredjuje koliko daleko "lampa" da baca svetlost,Ako ne bismo imali ovaj koeficijent,uvek bi bila svaka tacka osvetljena maximum,pa zelimo da to malo smanjimo 
	float light_distance_from_camera=3.0;
	
	float dist_from_camera = clamp(light_distance_from_camera - distance(pass_pos, uni_light_source), 0, 1);
	
	///svetlost od centra boxa                           ///delimo sa nekim brojem,zato sto je distanca velika,ako ne bismo delili bilo bi mracno
	float dist_from_center = clamp(distance(pass_pos, center_light_source)/5, 0, 1);

	
	///izvlacimo iz texture po uv koordinatama.Isto kao na vezbama 
	vec4 albedo_color = texture(albedo, pass_uv);
	///taj pixel koji smo izvukli,osvetljavamo dodatno: (ukoliko ostavimo samo albedo collor,bice osvetljeno kao na slikama koje smo importovali
	vec3 fcolor = albedo_color.xyz * (uni_light_color * light_coef) * (clamp(dist_from_camera + dist_from_center * light_color_1, 0, 1));
	
	final_color = vec4(fcolor, 1.0);
}