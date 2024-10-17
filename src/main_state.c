#include <main_state.h>
#include <glad/glad.h>
#include <math.h>
#include <rafgl.h>
#include <game_constants.h>
#define vert_count 36

typedef struct _vertex_t
{
    vec3_t position;
    vec3_t color;
    float alpha;
    float u, v;
    vec3_t normal;
} vertex_t;

vertex_t vertex(vec3_t pos, vec3_t  col, float alpha, float u, float v, vec3_t normal)
{
    vertex_t vert;

    vert.position = pos;
    vert.color = col;
    vert.alpha = alpha;
    vert.u = u;
    vert.v = v;
    vert.normal = normal;

    return vert;
}
vertex_t vertices[vert_count];

static GLuint vao, vbo;
static GLuint shader_program_id, uni_M, uni_VP, uni_light_color, uni_light_direction, uni_light_source;
static rafgl_raster_t raster;

static rafgl_texture_t texture_water;
static rafgl_texture_t texture_stone;


vec3_t light_direction = vec3m(-1.0f, -1.0f, -1.0f);

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
    rafgl_raster_load_from_image(&raster, "res/images/voda.png");

    rafgl_texture_init(&texture_water);

    rafgl_texture_load_from_raster(&texture_water, &raster);

    glBindTexture(GL_TEXTURE_2D, texture_water.tex_id);


    rafgl_raster_load_from_image(&raster, "res/images/sunce.jpg");

    rafgl_texture_init(&texture_stone);
    rafgl_texture_load_from_raster(&texture_stone, &raster);

    glBindTexture(GL_TEXTURE_2D, texture_stone.tex_id);



    glBindTexture(GL_TEXTURE_2D, 0);

    float sizeCoef=1.0f;
    float colorCoef=1.0f;

    float xVal=5.0f;
    float yVal=5.0f;
    float zVal=5.0f;
    float wallWidth=5.0f;


    ///mi smo na y=0, y=-1 je ispod nas,i to ce sluziti kao y za "pod"
    ///y=2 ce biti plafon
    //http://www.songho.ca/opengl/gl_projectionmatrix.html

   /// Mi smo okrenuti tamo gde bi isao suprotan smer Z ose  http://www.songho.ca/opengl/gl_projectionmatrix.html

    ///Pod                                                               alfa u i v sam uzeo sa vezbi
    ///donji trougao
    vertices[0] = vertex(vec3(-wallWidth, -1.0f, wallWidth), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ);///gornji desni cosak
    vertices[1] = vertex(vec3(wallWidth, -1.0f, wallWidth), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ); ///donji desni cosak
    vertices[2] = vertex(vec3(wallWidth, -1.0f, -wallWidth), RAFGL_BLUE, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ); ///donji levi cosak

    ///gornji trougao
    vertices[3] = vertex(vec3(-wallWidth, -1.0f, wallWidth), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///gornji desni cosak
    vertices[4] = vertex(vec3(wallWidth, -1.0f, -wallWidth), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///donji levi cosak
    vertices[5] = vertex(vec3(-wallWidth,-1.0f,-wallWidth), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji levi cosak

    //Plafon
    ///gornji troguao
    vertices[6] = vertex(vec3(-sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ);///gornji levi
    vertices[7] = vertex(vec3(-sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji desni
    vertices[8] = vertex(vec3(sizeCoef, 2.0f, -sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ); ///donji levi

    ///donji trougao
    vertices[9] = vertex(vec3(sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji levi
    vertices[10] = vertex(vec3(-sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji desni
    vertices[11] = vertex(vec3(sizeCoef, 2.0f, sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);///donji desni

    /// Zidovi-Mi smo okrenuti tamo gde bi isao suprotan smer Z ose //http://www.songho.ca/opengl/gl_projectionmatrix.html

    ///levi zid

    vertices[12] = vertex(vec3(-sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ); ///donji desni (kad se okrenes prema levom trouglu)
    vertices[13] = vertex(vec3(-sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji desni
    vertices[14] = vertex(vec3(-sizeCoef, -1.0f, -sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji
    ///donji trougao
    vertices[15] = vertex(vec3(-sizeCoef, -1.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji desni
    vertices[16] = vertex(vec3(-sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji levi
    vertices[17] = vertex(vec3(-sizeCoef, -1.0f, sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);///donji levi

    ///desni
    ///gornji trougao
    vertices[18] = vertex(vec3(sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ); ///gornji desni
    vertices[19] = vertex(vec3(sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ); ///gornji levi
    vertices[20] = vertex(vec3(sizeCoef, -1.0f, -sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ); ///donji desni

    ///donji
    vertices[21] = vertex(vec3(sizeCoef, -1.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ); ///donji desni
    vertices[22] = vertex(vec3(sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji levi
    vertices[23] = vertex(vec3(sizeCoef, -1.0f, sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);///donji levi

    ///zadnji
    vertices[24] = vertex(vec3(-sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ);///gornji desni
    vertices[25] = vertex(vec3(sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gorni levi
    vertices[26] = vertex(vec3(-sizeCoef, -1.0f, -sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji desni

    vertices[27] = vertex(vec3(-sizeCoef, -1.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji desni
    vertices[28] = vertex(vec3(sizeCoef, 2.0f, -sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji levi
    vertices[29] = vertex(vec3(sizeCoef, -1.0f, -sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);///donji levi

    ///prednji
    vertices[30] = vertex(vec3(-sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ); ///gornji levi
    vertices[31] = vertex(vec3(sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ); ///gornji desni
    vertices[32] = vertex(vec3(-sizeCoef, -1.0f, sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji levi

    vertices[33] = vertex(vec3(-sizeCoef, -1.0f, sizeCoef), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);///donji levi
    vertices[34] = vertex(vec3(sizeCoef, 2.0f, sizeCoef), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);///gornji desni
    vertices[35] = vertex(vec3(sizeCoef, -1.0f, sizeCoef), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);///donji desni

    shader_program_id = rafgl_program_create_from_name("my_shader");
    uni_M = glGetUniformLocation(shader_program_id, "uni_M");
    uni_VP = glGetUniformLocation(shader_program_id, "uni_VP");
    uni_light_color = glGetUniformLocation(shader_program_id, "uni_light_color");
    uni_light_direction = glGetUniformLocation(shader_program_id, "uni_light_direction");
    uni_light_source = glGetUniformLocation(shader_program_id, "uni_light_source");

    ///kreio vao i vbo
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    ///bindujemo vao i vbo da su aktivni
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    ///ubacujemo nas vertex niz u vbo,na osnovu prosledjenog niza vertices,znace da su podaci klase vertex,GL_STATIC_DRAW je podatak o tome cemu ce da sluze nasi podaci staticni(nece se menjati)
    glBufferData(GL_ARRAY_BUFFER, vert_count * sizeof(vertex_t), vertices, GL_STATIC_DRAW);

    ///u vertexu ucitavamo podatke na pozicijama 0,1,2,3,4(pozocija,color,alpha,uv,normal)

    glEnableVertexAttribArray(0);
 ///false znaci da podaci nisu normalizovani              pomeraj       0 je odakle pocinje
    ///                  3 je koliko floata ima
    ///Ovo je sve isto kao sa vezbi
    ///svaki naredni ce imati pomeraj zbirFloatovaKojeDoNjegaSadrze * sizeof(vec3_t)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) 0);

     ///boja                                                                         ///3*sizeof(float)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (sizeof(vec3_t)));
    ///alfa
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (2 * sizeof(vec3_t)));
    ///UV
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (2 * sizeof(vec3_t) + 1* sizeof(float)));
    ///Normala
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (2 * sizeof(vec3_t) + 3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}


mat4_t model;///polozaj,velicina modela...
mat4_t view; ///definise polozaj posmatraca(kamere).Sluzi za transformacije svega ostalog,tako da kamera uvek bude u nuli koord sist.
mat4_t projection;///projektuje 3d u 2d
mat4_t view_projection;


float fov = 90.0f; ///privid perspektive

vec3_t camera_position = vec3m(0.0f, 0.0f, 4.0f);///gde je kamera,to prosledjujemo kao light source,zato sto hocemo da iz nas (kamere) izlazi svetlost
vec3_t camera_target = vec3m(0.0f, 0.0f, 0.0f);///gde gleda kamera,tj gde se nalazi objekat koji gledamo
vec3_t camera_up = vec3m(0.0f, 1.0f, 0.0f);///rotacija kamere
vec3_t aim_dir = vec3m(0.0f, 0.0f, -1.0f);
float camera_angle = -M_PI * 0.5f;
float angle_speed = 0.5f * M_PI;
float move_speed = 4.0f;
float time = 0;


void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
///sve isto kao sa vezbi
    time += delta_time;

    if(game_data->keys_down['A']) camera_angle -= angle_speed * delta_time;
    if(game_data->keys_down['D']) camera_angle += angle_speed * delta_time;

    aim_dir = vec3(cosf(camera_angle), 0.0f, sinf(camera_angle));

    if(game_data->keys_down['W']) camera_position = v3_add(camera_position, v3_muls(aim_dir, move_speed * delta_time));
    if(game_data->keys_down['S']) camera_position = v3_add(camera_position, v3_muls(aim_dir, move_speed * delta_time * -1.0f));


    /// Vclip(krajnja pozicija pixela)=Projection x View x Model x Local(vertices),mnozi se s desna na levo.
    ///model i local se mnoze u vertexu,i u vertexu se model_local mnozi sa view_projection,i dobija se gl_Position


    camera_target = v3_add(camera_position, aim_dir);

    ///odnos visine i sirine prozora
    float aspect = ((float)(game_data->raster_width)) / game_data->raster_height;

    ///fov(ugao),odnos visine i sirine,near i far ravni(dublje od near ne idi,i dalje od far ne idi).To prosledjujemo da bi se napravila projection matrica
    projection = m4_perspective(fov, aspect, 0.01f, 100.0f);

    ///posmatraj kao da je matrica 4x4 i unesi ove pozicije.S obzirom da mi radimo sa vektor3,on ce da poslednju (tj dodace 4.tu) kolonu i red obrgli sa 0-ama,
    ///a samo na donjem desnom cosku da stavi 1,kao da je jedinicna
    ///znaci 4x4 ce biti,ovi atributi navedeni su nizovi od 3 floata
    view = m4_look_at(camera_position, camera_target, camera_up);
    ///model matrica nam je u pocetku jedinicna
    model = m4_identity();

    view_projection = m4_mul(projection, view);
}

void main_state_render(GLFWwindow *window, void *args)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program_id);

    glEnable(GL_TEXTURE_2D);
    glBindVertexArray(vao);

    glUniformMatrix4fv(uni_M, 1, GL_FALSE, (void*) model.m);
    glUniformMatrix4fv(uni_VP, 1, GL_FALSE, (void*) view_projection.m);

    glUniform3f(uni_light_color, RAFGL_WHITE.x, RAFGL_WHITE.y, RAFGL_BLUE.z);
    glUniform3f(uni_light_direction, light_direction.x, light_direction.y, light_direction.z);
    glUniform3f(uni_light_source, camera_position.x, camera_position.y, camera_position.z);


    glBindTexture(GL_TEXTURE_2D, texture_water.tex_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, texture_stone.tex_id);
    glDrawArrays(GL_TRIANGLES, 6, vert_count - 6);

    glBindVertexArray(0);

}


void main_state_cleanup(GLFWwindow *window, void *args)
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteShader(shader_program_id);
}
