// 2D_JUMPING_BALL.cpp : 
//
#include <iostream>
#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

static unsigned int programId;
#define PI 3.14159265358979323846

unsigned int VAO_MONTAGNE, VAO_PRATO, VAO_SOLE, VAO_LUNA, VAO_TEST, VAO_CIELO, VAO_PALLA, VAO_PalaEolica, VAO_NAVICELLA, VAO_PALLINE, VAO_PALLINE2, VAO_NUVOLA1, VAO_NUVOLA2;
unsigned int VBO_M, VBO_P, VBO_S, VBO_L, VBO_T, VBO_C, VBO_Pa, VBO_PE, MatProj, MatModel, VBO_N, VBO_Pn, VBO_Pn2, VBO_Nuv1, VBO_Nuv2;

// Include GLM; libreria matematica per le opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

mat4 Projection;  //Matrice di proiezione
mat4 Model; //Matrice per il cambiamento da Sistema di riferimento dell'oggetto OCS a sistema di riferimento nel Mondo WCS

// viewport size
int width = 1200;
int height = 720;

float angolo = 0.0;
typedef struct { float x, y, r, g, b, a; } Pointxy;
int Numero_di_pezzi = 128;
int nVertices_montagna = 2 * Numero_di_pezzi;
Pointxy* Punti = new Pointxy[nVertices_montagna];
int vertices_Prato = 6;
Pointxy* Prato = new Pointxy[vertices_Prato];
int vertices_Cielo = 6;
Pointxy* Cielo = new Pointxy[vertices_Cielo];
int nTriangles_sole = 30;
int vertices_sole = 3 * 2 * nTriangles_sole;
Pointxy* Sole = new Pointxy[vertices_sole];
Pointxy* Luna = new Pointxy[vertices_sole];							//AGGIUNTA LUNA
Pointxy* Test = new Pointxy[4];
Pointxy* Navicella = new Pointxy[vertices_sole];
Pointxy* Palline = new Pointxy[vertices_sole];
Pointxy* Palline2 = new Pointxy[vertices_sole];

Pointxy* Nuvola1 = new Pointxy[vertices_sole * 7];
Pointxy* Nuvola2 = new Pointxy[vertices_sole * 7];
int nTriangles_palla = 30;
int vertices_palla = 3 * 2 * nTriangles_palla;
Pointxy* Palla = new Pointxy[vertices_palla];
int nTriangles_PalaEolica = 4;
int vertices_PalaEolica = 3 * (nTriangles_PalaEolica + 2);
Pointxy* PalaEolica = new Pointxy[vertices_PalaEolica];

//parametri sole
	//alone
float t = 0.1;
int checkt = 0;
int contsole = 0;
int contalieno = width + 298;

float a = 1.0;
float t1;
float t2;


int fire = 0;
float xproiett = 0.0;
float yproiett = 0.0;

int fgravity = 0;
double bwidth;


//parametri pala eolica
float contangolo = 0;


//paramtri test
float testArray[4][2];


// parametri della palla
int		distacco_da_terra = 0;			// distacco da terra 
float	delta = 15;
double	VelocitaOrizzontale = 0; //velocita orizzontale (pixel per frame)

float	posx = float(width) / 2; //coordinate sul piano della posizione iniziale della palla
float	posy = float(height) * 0.2;

bool pressing_left = false;
bool pressing_right = false;

vec4 col_rosso = { 1.0,0.0,0.0,1.0 };
vec4 col_nero = { 0.0,0.0,0.0,1.0 };

vec4 col_top_palline2;
vec4 col_bottom_palline2;

vec4 col_top_palline;
vec4 col_bottom_palline;





int alienconst = 2;
float randhigh = float(rand() % 25 + 71) / 100;
int gravity = 0;

int spawnpallinax = 0;
int spawnpallinay = 0;
int checktime = 0;

float rt = (rand() % 101) / 100;
float gt = (rand() % 101) / 100;
float bt = (rand() % 101) / 100;

float rb = (rand() % 101) / 100;
float gb = (rand() % 101) / 100;
float bb = (rand() % 101) / 100;


int c = 1;
int contnuvola = width + 149;
int contnuvola2 = -399;
int checkn2 = 0;
int constnuvola = -1;
int constnuvola2 = -1;

int x_normalized;
vec4 col_top;
vec4 col_bottom;

vec4 col_top_nuvola;
vec4 col_top_nuvola2;
vec4 col_bottom_nuvola;
vec4 col_bottom_nuvola2;

float shadow_scale;
double bheight;



/// ///////////////////////////////////////////////////////////////////////////////////
///									Gestione eventi
///////////////////////////////////////////////////////////////////////////////////////
void keyboardPressedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		pressing_left = true;
		break;
	case 'd':
		pressing_right = true;
		break;
	case 'x':
		if (fire == 0) {
			
			xproiett = posx;
			yproiett = posy;
			fgravity = 0;
			fire = 1;
		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void keyboardReleasedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		pressing_left = false;
		break;
	case 'd':
		pressing_right = false;
		break;
	default:
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////// 
void disegna_pianoxy(float x, float y, float width, float height, vec4 color_top, vec4 color_bot, Pointxy* piano)
{
	piano[0].x = x;	piano[0].y = y;
	piano[0].r = color_bot.r; piano[0].g = color_bot.g; piano[0].b = color_bot.b; piano[0].a = color_bot.a;
	piano[1].x = x + width;	piano[1].y = y;
	piano[1].r = color_top.r; piano[1].g = color_top.g; piano[1].b = color_top.b; piano[1].a = color_top.a;
	piano[2].x = x + width;	piano[2].y = y + height;
	piano[2].r = color_bot.r; piano[2].g = color_bot.g; piano[2].b = color_bot.b; piano[2].a = color_bot.a;

	piano[3].x = x + width;	piano[3].y = y + height;
	piano[3].r = color_bot.r; piano[3].g = color_bot.g; piano[3].b = color_bot.b; piano[3].a = color_bot.a;
	piano[4].x = x;	piano[4].y = y + height;
	piano[4].r = color_top.r; piano[4].g = color_top.g; piano[4].b = color_top.b; piano[4].a = color_top.a;
	piano[5].x = x;	piano[5].y = y;
	piano[5].r = color_bot.r; piano[5].g = color_bot.g; piano[5].b = color_bot.b; piano[5].a = color_bot.a;
}

void disegna_cerchio(int nTriangles, int step, vec4 color_top, vec4 color_bot, Pointxy* Cerchio) {
	int i;
	float stepA = (2 * PI) / nTriangles;

	int comp = 0;
	// step = 1 -> triangoli adiacenti, step = n -> triangoli distanti step l'uno dall'altro
	for (i = 0; i < nTriangles; i += step)
	{
		Cerchio[comp].x = cos((double)i * stepA);
		Cerchio[comp].y = sin((double)i * stepA);
		Cerchio[comp].r = color_top.r; Cerchio[comp].g = color_top.g; Cerchio[comp].b = color_top.b; Cerchio[comp].a = color_top.a;

		Cerchio[comp + 1].x = cos((double)(i + 1) * stepA);
		Cerchio[comp + 1].y = sin((double)(i + 1) * stepA);
		Cerchio[comp + 1].r = color_top.r; Cerchio[comp + 1].g = color_top.g; Cerchio[comp + 1].b = color_top.b; Cerchio[comp + 1].a = color_top.a;

		Cerchio[comp + 2].x = 0.0;
		Cerchio[comp + 2].y = 0.0;
		Cerchio[comp + 2].r = color_bot.r; Cerchio[comp + 2].g = color_bot.g; Cerchio[comp + 2].b = color_bot.b; Cerchio[comp + 2].a = color_bot.a;
		comp += 3;
	}
}

void disegna_base(int nTriangles, int step, vec4 color_top, vec4 color_bot, float raggiox, float raggioy, Pointxy* Cerchio) {
	int i;
	float stepA = (2 * PI) / nTriangles;

	int comp = 0;
	// step = 1 -> triangoli adiacenti, step = n -> triangoli distanti step l'uno dall'altro
	for (i = 0; i < nTriangles; i += step)
	{
		Cerchio[comp].x = cos((double)i * stepA) * raggiox;
		Cerchio[comp].y = sin((double)i * stepA) * raggioy;
		Cerchio[comp].r = color_top.r; Cerchio[comp].g = color_top.g; Cerchio[comp].b = color_top.b; Cerchio[comp].a = color_top.a;

		Cerchio[comp + 1].x = cos((double)(i + 1) * stepA) * raggiox;
		Cerchio[comp + 1].y = sin((double)(i + 1) * stepA) * raggioy;
		Cerchio[comp + 1].r = color_top.r; Cerchio[comp + 1].g = color_top.g; Cerchio[comp + 1].b = color_top.b; Cerchio[comp + 1].a = color_top.a;

		Cerchio[comp + 2].x = 0.0;
		Cerchio[comp + 2].y = 0.0;
		Cerchio[comp + 2].r = color_bot.r; Cerchio[comp + 2].g = color_bot.g; Cerchio[comp + 2].b = color_bot.b; Cerchio[comp + 2].a = color_bot.a;
		comp += 3;
	}
}


void disegna_cerchio_luna(int nTriangles, int step, vec4 color_top, vec4 color_bot, Pointxy* Cerchio) {
	int i;
	float stepA = (2 * PI) / nTriangles;

	int comp = 0;
	// step = 1 -> triangoli adiacenti, step = n -> triangoli distanti step l'uno dall'altro
	for (i = 0; i < nTriangles; i += step)
	{
		Cerchio[comp].x = cos((double)i * stepA);
		Cerchio[comp].y = sin((double)i * stepA);
		Cerchio[comp].r = 0.5; Cerchio[comp].g = 0.5; Cerchio[comp].b = 0.5; Cerchio[comp].a = 0.8;


		Cerchio[comp + 1].x = cos((double)(i + 1) * stepA);
		Cerchio[comp + 1].y = sin((double)(i + 1) * stepA);
		Cerchio[comp + 1].r = 0.5; Cerchio[comp + 1].g = 0.5; Cerchio[comp + 1].b = 0.5; Cerchio[comp + 1].a = 0.8;

		Cerchio[comp + 2].x = 0.0;
		Cerchio[comp + 2].y = 0.0;
		Cerchio[comp + 2].r = 0.5; Cerchio[comp + 2].g = 0.5; Cerchio[comp + 2].b = 0.5; Cerchio[comp + 2].a = 0.8;
		comp += 3;
	}
}



void disegna_PalaEolica(int nTriangles, Pointxy* PalaEolica) {

	int i, cont;
	Pointxy* Sostegno;
	int vertici = 6;
	Sostegno = new Pointxy[vertici];

	//Costruisco la geometria della pala ed i suoi colori
	vec4 col_top = { 0.0, 1.0, 0.0, 1.0 };
	vec4 col_bottom = { 0.5, 0.5, 0.05, 0.8 };

	disegna_cerchio(nTriangles * 2, 2, col_top, col_bottom, PalaEolica);
	disegna_pianoxy(-0.1, -1.0, 1.0, 1.0, col_nero, col_nero, Sostegno);

	cont = 3 * nTriangles;
	for (i = 0; i < 6; i++)
	{
		PalaEolica[cont + i].x = Sostegno[i].x;
		PalaEolica[cont + i].y = Sostegno[i].y;
		PalaEolica[cont + i].r = Sostegno[i].r;	PalaEolica[cont + i].g = Sostegno[i].g;	PalaEolica[cont + i].b = Sostegno[i].b; PalaEolica[cont + i].a = Sostegno[i].a;
	}
}

double lerp(double a, double b, double amount) {
	//Interpolazione lineare tra a e b secondo amount
	return (1 - amount) * a + amount * b;
}


void disegna_palla(int nTriangles, float r, float g, float b, Pointxy* Palla) {

	int i, cont;
	Pointxy* Ombra;
	int vertici = 3 * nTriangles;
	Ombra = new Pointxy[vertici];

	//Costruisco la geometria della palla ed i suoi colori
	vec4 col_bottom = { 1.0, 0.8, 0.0, 1.0 };
	vec4 col_top = { r,g,b, 1.0 };
	disegna_cerchio(nTriangles, 1, col_top, col_bottom, Palla);

	//Costruisco la geometria dell'ombra ed i suoi colori.
	col_top = { 0.49,0.49,0.49, 0.0 };
	col_bottom = { 0.0, 0.0, 0.0, 0.6 };
	disegna_cerchio(nTriangles, 1, col_top, col_bottom, Ombra);

	//Appendo a Palla la sua Ombra
	cont = 3 * nTriangles;
	for (i = 0; i < 3 * nTriangles; i++)
	{
		Palla[cont + i].x = Ombra[i].x;
		Palla[cont + i].y = Ombra[i].y;
		Palla[cont + i].r = Ombra[i].r;	Palla[cont + i].g = Ombra[i].g;	Palla[cont + i].b = Ombra[i].b;	Palla[cont + i].a = Ombra[i].a;
	}
}

void disegna_sole(int nTriangles, Pointxy* Sole) {
	int i, cont;
	Pointxy* OutSide;
	int vertici = 3 * nTriangles;
	OutSide = new Pointxy[vertici];


	vec4 col_top_sole = { 1.0, 1.0, 1.0, 1.0 };
	vec4 col_bottom_sole = { 1.0, 0.8627, 0.0, 1.0 };
	if (a == 0.0) {
		col_top_sole = { 0.96, 0.9, 0.0, 0.62 };
		col_bottom_sole = { 0.96, 0.9, 0.0, 0.62 };
	}
	disegna_cerchio(nTriangles, 1, col_top_sole, col_bottom_sole, Sole);

	col_top_sole = { 1.0, 1.0, 1.0, 0.0 };
	col_bottom_sole = { 1.0, 0.8627, 0.0, t };
	disegna_cerchio(nTriangles, 1, col_top_sole, col_bottom_sole, OutSide);

	cont = 3 * nTriangles;
	for (i = 0; i < 3 * nTriangles; i++)
	{
		Sole[cont + i].x = OutSide[i].x;
		Sole[cont + i].y = OutSide[i].y;
		Sole[cont + i].r = OutSide[i].r; Sole[cont + i].g = OutSide[i].g; Sole[cont + i].b = OutSide[i].b; Sole[cont + i].a = OutSide[i].a;
	}
}

void disegna_navicella(int nTriangles, float raggiox, float raggioy, vec4 col_top_sole, vec4 col_bottom_sole, Pointxy* Sole) {
	int i, cont;
	Pointxy* OutSide;
	int vertici = 3 * nTriangles;
	OutSide = new Pointxy[vertici];

	disegna_base(nTriangles, 1, col_top_sole, col_bottom_sole, raggiox, raggioy, Sole);


}



void disegna_luna(int nTriangles, Pointxy* Luna) {
	int i, cont;
	Pointxy* OutSide;
	int vertici = 3 * nTriangles;
	OutSide = new Pointxy[vertici];

	vec4 col_top_sole = { 1.0, 1.0, 1.0, 1.0 };
	vec4 col_bottom_sole = { 1.0, 0.8627, 0.0, 1.0 };
	disegna_cerchio_luna(nTriangles, 1, col_top_sole, col_bottom_sole, Luna);

	col_top_sole = { 1.0, 1.0, 1.0, 0.0 };
	col_bottom_sole = { 1.0, 0.8627, 0.0, t };
	disegna_cerchio_luna(nTriangles, 1, col_top_sole, col_bottom_sole, OutSide);

	cont = 3 * nTriangles;
	for (i = 0; i < 3 * nTriangles; i++)
	{
		Luna[cont + i].x = OutSide[i].x;
		Luna[cont + i].y = OutSide[i].y;
		Luna[cont + i].r = OutSide[i].r; Luna[cont + i].g = OutSide[i].g; Luna[cont + i].b = OutSide[i].b; Luna[cont + i].a = OutSide[i].a;
	}
}


void disegna_montagne(float x0, float y0, int altezza_montagna, int larghezza_montagne, int numero_di_montagne, vec4 color_top, vec4 color_bot, Pointxy* v_montagna)
{
	float dimensione_pezzi = larghezza_montagne / (float)Numero_di_pezzi;
	float frequenza = PI * numero_di_montagne;

	int n_vertici = 0;

	for (int i = 0; i < Numero_di_pezzi; i += 1)
	{

		v_montagna[n_vertici].x = x0 + i * dimensione_pezzi;
		v_montagna[n_vertici].y = y0;
		v_montagna[n_vertici].r = color_bot.r; v_montagna[n_vertici].g = color_bot.g; v_montagna[n_vertici].b = color_bot.b; v_montagna[n_vertici].a = color_bot.a;

		v_montagna[n_vertici + 1].x = x0 + i * dimensione_pezzi;
		v_montagna[n_vertici + 1].y = y0 + altezza_montagna * abs(sin(i / (float)Numero_di_pezzi * frequenza));
		v_montagna[n_vertici + 1].r = color_top.r; v_montagna[n_vertici + 1].g = color_top.g; v_montagna[n_vertici + 1].b = color_top.b; v_montagna[n_vertici + 1].a = color_top.a;

		n_vertici += 2;
	}
}


//Movimento della palla in orizzontale
void update(int a)
{
	bool moving = false;

	if (pressing_left)
	{
		VelocitaOrizzontale -= 1;
		moving = true;
	}
	if (pressing_right)
	{
		VelocitaOrizzontale += 1;
		moving = true;
	}
	if (!moving) {   // Se non mi sto muovendo con i tasti a e d,
		// decremento od incremento la velocita' iniziale fino a portarla
		// a zero e la palla continua a rimbalzare sul posto
		if (VelocitaOrizzontale > 0)
		{
			VelocitaOrizzontale -= 1;
			if (VelocitaOrizzontale < 0)
				VelocitaOrizzontale = 0;
		}
		if (VelocitaOrizzontale < 0)
		{
			VelocitaOrizzontale += 1;
			if (VelocitaOrizzontale > 0)
				VelocitaOrizzontale = 0;
		}
	}

	//Aggioramento della posizione in x della pallina, che regola il movimento orizzontale
	posx += VelocitaOrizzontale;

	//Se la palla urta i bordi dello schermo 
	// ovvero assume una posizione x<0 o x> width (bordi viewport)
	// la pallina rimbalza ai bordi dello schermo attenuando la velocità
	if (posx < 0.0) {
		posx = 0.0;
		VelocitaOrizzontale = -VelocitaOrizzontale * 0.8;
	}
	if (posx > width) {
		posx = (float)width;
		VelocitaOrizzontale = -VelocitaOrizzontale * 0.8;
	}

	// Gestione del rimbalzo in relazione all'altezza da terra
	//Rimbalzo -20 < distacco_da_terra < 100
	distacco_da_terra += (delta * (1 - (distacco_da_terra / 120)));
	//invertire direzione del jump
	if ((distacco_da_terra > 100) && (delta > 0))
		delta = -delta;
	if ((distacco_da_terra < -20) && (delta < 0))
		delta = -delta;

	glutPostRedisplay();
	glutTimerFunc(24, update, 0);
}





void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_C_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_C_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}




int num_test = 9;
float pallar = 1.0;
float pallag = 0.0;
float pallab = 0.0;

float nuvolar = 1.0;
float nuvolag = 1.0;
float nuvolab = 1.0;

float nuvolar2 = 1.0;
float nuvolag2 = 1.0;
float nuvolab2 = 1.0;

void init(void)
{
	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");


	//Costruzione geometria e colori del CIELO
	vec4 col_top = { 0.3,0.6,1.0,1.0 };
	vec4 col_bottom = { 0.0 , 0.1, 1.0, 1.0 };
	disegna_pianoxy(0, height * 0.5, width, height * 0.5, col_bottom, col_top, Cielo);
	//Generazione del VAO del Cielo
	glGenVertexArrays(1, &VAO_CIELO);
	glBindVertexArray(VAO_CIELO);
	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, vertices_Cielo * sizeof(Pointxy), &Cielo[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Costruzione geometria e colori del PRATO
	col_top = { 0.1, 0.5, 0.1, 1.0 };
	col_bottom = { 0.8, 1.0, 0.2, 1.0 };
	disegna_pianoxy(0, 0, width, height * 0.5, col_bottom, col_top, Prato);
	//Genero un VAO Prato
	glGenVertexArrays(1, &VAO_PRATO);
	glBindVertexArray(VAO_PRATO);
	glGenBuffers(1, &VBO_P);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_P);
	glBufferData(GL_ARRAY_BUFFER, vertices_Prato * sizeof(Pointxy), &Prato[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//Costruzione geometria e colori del SOLE
	//Genero il VAO del SOLE
	disegna_sole(nTriangles_sole, Sole);
	glGenVertexArrays(1, &VAO_SOLE);
	glBindVertexArray(VAO_SOLE);
	glGenBuffers(1, &VBO_S);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_S);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Sole[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	col_top = { 0.0, 0.0, 1.0, 1.0 };
	col_bottom = { 0.0, 0.0, 1.0, 1.0 };


	disegna_navicella(nTriangles_sole, 0.4, 0.4, col_top, col_bottom, Navicella);
	glGenVertexArrays(1, &VAO_NAVICELLA);
	glBindVertexArray(VAO_NAVICELLA);
	glGenBuffers(1, &VBO_N);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Navicella[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	disegna_luna(nTriangles_sole, Luna);
	glGenVertexArrays(1, &VAO_LUNA);
	glBindVertexArray(VAO_LUNA);
	glGenBuffers(1, &VBO_L);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_L);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Luna[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Costruzione geometria e colori delle montagne
	col_top = { 0.5451, 0.2706, 0.0745, 1.0000 };
	col_bottom = { 0.4980, 0.0353, 0.1843, 1.0000 };
	disegna_montagne(0, 0, 100, width, 3, col_bottom, col_top, Punti);
	//Genero un VAO Montagne
	glGenVertexArrays(1, &VAO_MONTAGNE);
	glBindVertexArray(VAO_MONTAGNE);
	glGenBuffers(1, &VBO_M);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_M);
	glBufferData(GL_ARRAY_BUFFER, nVertices_montagna * sizeof(Pointxy), &Punti[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Costruzione geometria e colori della PALLA
	disegna_palla(nTriangles_palla, pallar, pallag, pallab, Palla);
	glGenVertexArrays(1, &VAO_PALLA);
	glBindVertexArray(VAO_PALLA);
	glGenBuffers(1, &VBO_Pa);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pa);
	glBufferData(GL_ARRAY_BUFFER, vertices_palla * sizeof(Pointxy), &Palla[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Costruzione geometria e colori delle PALE EOLICHE
	disegna_PalaEolica(nTriangles_PalaEolica, PalaEolica);
	glGenVertexArrays(1, &VAO_PalaEolica);
	glBindVertexArray(VAO_PalaEolica);
	glGenBuffers(1, &VBO_PE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PE);
	glBufferData(GL_ARRAY_BUFFER, vertices_PalaEolica * sizeof(Pointxy), &PalaEolica[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//disegna test triangle
	glGenVertexArrays(1, &VAO_TEST);
	glBindVertexArray(VAO_TEST);
	glGenBuffers(1, &VBO_T);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Pointxy), &Test[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//NUVOLA 1
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	glGenVertexArrays(1, &VAO_NUVOLA1);
	glBindVertexArray(VAO_NUVOLA1);
	glGenBuffers(1, &VBO_Nuv1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//NUVOLA 2
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	

	//Palline d'attacco
	disegna_cerchio(nTriangles_sole, 1, col_top_palline2, col_bottom_palline2, Palline2);
	glGenVertexArrays(1, &VAO_PALLINE2);
	glBindVertexArray(VAO_PALLINE2);
	glGenBuffers(1, &VBO_Pn2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pn2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Palline2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Palline navicella
	disegna_cerchio(nTriangles_sole, 1, col_top_palline, col_bottom_palline, Palline);
	glGenVertexArrays(1, &VAO_PALLINE);
	glBindVertexArray(VAO_PALLINE);
	glGenBuffers(1, &VBO_Pn);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pn);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Palline[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//Definisco il colore assegnato allo schermo
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutSwapBuffers();
}



void drawScene(void)
{

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glClear(GL_COLOR_BUFFER_BIT);

	contsole++;
	contangolo += 5;
	contalieno += alienconst;
	gravity -= 3;
	fgravity += 8;

	t2 = 0.5 - (t);
	t1 = 0.5 - (t)+0.5;

	col_top = { 0.3,0.6,1.0, t1 - 0.3 };
	col_bottom = { 0.0 , 0.1, 1.0, t1 - 0.3 };
	if (a == 1.0) {											//se giorno
		col_top = { 0.3,0.6,1.0, 1.0 - t2 };
		col_bottom = { 0.0 , 0.1, 1.0, 1.0 - t2 };
	}

	if (contsole % (width + 150) == 0) {
		if (a == 1.0)
			a = 0.0;
		else
			a = 1.0;
	}

	if (contalieno > 0 && contalieno % (width + 400) == 0) {
		alienconst = -alienconst;
		randhigh = float(rand() % 25 + 71) / 100;
		gravity = float(height) * randhigh;
	}
	if (contalieno < -300) {
		alienconst = -alienconst;
		randhigh = float(rand() % 25 + 71) / 100;
		gravity = float(height) * randhigh;
	}


	col_top_nuvola = { nuvolar , nuvolag, nuvolab, 1.0 };
	col_bottom_nuvola = { nuvolar , nuvolag, nuvolab, 1.0 };

	col_top_nuvola2 = { nuvolar2 , nuvolag2, nuvolab2, 1.0 };
	col_bottom_nuvola2 = { nuvolar2 , nuvolag2, nuvolab2, 1.0 };




	disegna_pianoxy(0, height * 0.5, width, height * 0.5, col_bottom, col_top, Cielo);
	//Generazione del VAO del Cielo
	
	glBindVertexArray(VAO_CIELO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, vertices_Cielo * sizeof(Pointxy), &Cielo[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);





	//Disegna cielo
	Model = mat4(1.0);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_CIELO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_Cielo);
	glBindVertexArray(0);




	//Disegna prato
	Model = mat4(1.0);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_PRATO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_Prato);
	glBindVertexArray(0);



	// Disegna sole
	Model = mat4(1.0);
	
	x_normalized = width * ((contsole % (width + 150)) - 75) / (width + 150) - ((width + 150) / 2);
	t = 1.1 - abs(2 * (float(((contsole % (width + 150))) - 75) / (width + 150)) - 1);

	disegna_sole(nTriangles_sole, Sole);
	
	glBindVertexArray(VAO_SOLE);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_S);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Sole[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3((contsole % (width + 150) - 75), float(height) * 0.9 - (abs(x_normalized + 150)) / 4, 0.0));
	Model = scale(Model, vec3(30.0, 30.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_SOLE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);

	

	//Disegna Alone del sole
	Model = mat4(1.0);
	//Model = translate(Model, vec3(float(width) * 1.0, float(height) * 0.8, 0.0));
	Model = translate(Model, vec3((contsole % (width + 150) - 75), float(height) * 0.9 - (abs(x_normalized + 150)) / 4, 0.0));
	if (a == 1.0)
		Model = scale(Model, vec3(80.0, 80.0, 1.0));
	else
		Model = scale(Model, vec3(35.0, 35.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, vertices_sole / 2, vertices_sole / 2);
	glBindVertexArray(0);



	//Disegna montagne
	Model = mat4(1.0);
	Model = translate(Model, vec3(0.0, float(height) / 2, 0.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_MONTAGNE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices_montagna);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);




	// Disegna palla (ombra+palla)
	Model = mat4(1.0);
	disegna_palla(nTriangles_palla, pallar, pallag, pallab, Palla);
	//glGenVertexArrays(1, &VAO_PALLA);
	glBindVertexArray(VAO_PALLA);
	//glGenBuffers(1, &VBO_Pa);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pa);
	glBufferData(GL_ARRAY_BUFFER, vertices_palla * sizeof(Pointxy), &Palla[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	

	shadow_scale = lerp(1, 0, (float)distacco_da_terra / 255); //distacco grande -> fattore scala piccolo
	// larghezza effettiva in pixel della palla (80 diametro palla normale -- 100 palla dilatata)
	bwidth = distacco_da_terra < 0 ? lerp(80, 100, (double)abs(distacco_da_terra) / 20) : 80;
	// altezza effettiva in pixel della palla
	bheight = distacco_da_terra < 0 ? 80 + distacco_da_terra : 80;
	//Matrice per il cambiamento del sistema di riferimento per l'OMBRA della palla
	Model = translate(Model, vec3(posx - bwidth / 2 * shadow_scale, posy + 10 + 10 * (1 - shadow_scale), 0.0f));
	Model = scale(Model, vec3(float(bwidth) * shadow_scale, (50.0 * shadow_scale), 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_PALLA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, vertices_palla / 2, vertices_palla / 2);
	//Matrice per il cambiamento del sistema di riferimento per la  PALLA
	Model = mat4(1.0);
	Model = translate(Model, vec3(posx - bwidth / 2, posy + bheight + distacco_da_terra, 0.0f));
	Model = scale(Model, vec3(float(bwidth) / 2, float(bheight) / 2, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_palla / 2);
	glBindVertexArray(0);


	// Disegna nuvola1
	//1cerchio
	Model = mat4(1.0);
	



	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	
	glBindVertexArray(VAO_NUVOLA1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	contnuvola += constnuvola;

	if (contnuvola*c > width + 150 || contnuvola < -200) {
		constnuvola = -constnuvola;
	}

	
	Model = translate(Model, vec3(float(contnuvola) / float(c), float(height) * 0.9, 0.0));
	Model = scale(Model, vec3(15.0, 15.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA1);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//2cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	
	glBindVertexArray(VAO_NUVOLA1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	
	Model = translate(Model, vec3(float(contnuvola) / float(c) + 20, 7 + float(height) * 0.9, 0.0));
	Model = scale(Model, vec3(18.0, 18.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA1);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//3cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	
	glBindVertexArray(VAO_NUVOLA1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	
	Model = translate(Model, vec3(float(contnuvola) / float(c) + 44, 11 + float(height) * 0.9, 0.0));
	Model = scale(Model, vec3(21.0, 21.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA1);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//4cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	
	glBindVertexArray(VAO_NUVOLA1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	
	Model = translate(Model, vec3(float(contnuvola) / float(c) + 53, 3 + float(height) * 0.9, 0.0));
	Model = scale(Model, vec3(19.0, 19.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA1);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//5cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	
	glBindVertexArray(VAO_NUVOLA1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	
	Model = translate(Model, vec3(float(contnuvola) / float(c) + 43, -5 + float(height) * 0.9, 0.0));
	Model = scale(Model, vec3(18.0, 18.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA1);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//6cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola, col_bottom_nuvola, Nuvola1);
	
	glBindVertexArray(VAO_NUVOLA1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv1);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	
	Model = translate(Model, vec3(float(contnuvola) / float(c) + 20, -6 + float(height) * 0.9, 0.0));
	Model = scale(Model, vec3(15.0, 15.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA1);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);





	// Disegna nuvola2
	//1cerchio
	Model = mat4(1.0);
	//Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));



	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	//glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	//glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	contnuvola2 += -constnuvola2;

	if (contnuvola2 * c > width + 150 || (contnuvola2 < -300 && checkn2==1)) {
		constnuvola2 = -constnuvola2;
		checkn2 = 1;
	}

	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3(float(contnuvola2) / float(c), float(height) * 0.78, 0.0));
	Model = scale(Model, vec3(15.0, 15.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//2cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	//glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	//glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3(float(contnuvola2) / float(c) + 20, 7 + float(height) * 0.78, 0.0));
	Model = scale(Model, vec3(18.0, 18.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//3cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	//glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	//glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3(float(contnuvola2) / float(c) + 44, 11 + float(height) * 0.78, 0.0));
	Model = scale(Model, vec3(21.0, 21.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//4cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	//glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	//glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3(float(contnuvola2) / float(c) + 53, 3 + float(height) * 0.78, 0.0));
	Model = scale(Model, vec3(19.0, 19.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//5cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	//glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	//glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3(float(contnuvola2) / float(c) + 43, -5 + float(height) * 0.78, 0.0));
	Model = scale(Model, vec3(18.0, 18.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	//6cerchio
	Model = mat4(1.0);
	disegna_cerchio(nTriangles_sole, 1, col_top_nuvola2, col_bottom_nuvola2, Nuvola2);
	//glGenVertexArrays(1, &VAO_NUVOLA2);
	glBindVertexArray(VAO_NUVOLA2);
	//glGenBuffers(1, &VBO_Nuv2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nuv2);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Nuvola2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3(float(contnuvola2) / float(c) + 20, -6 + float(height) * 0.78, 0.0));
	Model = scale(Model, vec3(15.0, 15.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NUVOLA2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);



	//sparo palla
	if (fire == 1) {

		// Disegna palline
		Model = mat4(1.0);
		//Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));


		col_top_palline2 = { pallar, pallag, pallab, 1.0 };
		col_bottom_palline2 = { pallar, pallag, pallab, t };



		
		disegna_cerchio(nTriangles_sole, 1, col_top_palline2, col_bottom_palline2, Palline2);
		//glGenVertexArrays(1, &VAO_PALLINE2);
		glBindVertexArray(VAO_PALLINE2);
		//glGenBuffers(1, &VBO_Pn2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Pn2);
		glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Palline2[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
		Model = translate(Model, vec3(xproiett - 80 / 2, yproiett + fgravity + 155, 0.0));
		Model = scale(Model, vec3(5.0, 5.0, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		glBindVertexArray(VAO_PALLINE2);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


		if (((xproiett - 80 / 2) - (float(contnuvola) / c + 30) < 65.0 && (xproiett - 80 / 2) - (float(contnuvola) / c + 30) > -65.0) &&
			(yproiett + fgravity + 155) - (float(height) * 0.9) < 35.0 && (yproiett + fgravity + 155) - (float(height) * 0.9) > -35.0) {
			nuvolar = pallar;
			nuvolag = pallag;
			nuvolab = pallab;
			fire = 0;
		}
		else if (((xproiett - 80 / 2) - (float(contnuvola2) / c + 30) < 65.0 && (xproiett - 80 / 2) - (float(contnuvola2) / c + 30) > -65.0) &&
			(yproiett + fgravity + 155) - (float(height) * 0.78) < 35.0 && (yproiett + fgravity + 155) - (float(height) * 0.78) > -35.0) {
			nuvolar2 = pallar;
			nuvolag2 = pallag;
			nuvolab2 = pallab;
			fire = 0;
		}
		else if (yproiett + fgravity + 155 > height) {
			fire = 0;
		}
	}



	// Disegna Sopra Navicella
	Model = mat4(1.0);
	//Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));

	col_top = { 0.6, 0.6, 0.6, 0.9 };
	col_bottom = { 0.5, 0.2, 0.3, 0.9 };

	disegna_navicella(nTriangles_sole, 0.4, 0.4, col_top, col_bottom, Navicella);
	//glGenVertexArrays(1, &VAO_NAVICELLA);
	glBindVertexArray(VAO_NAVICELLA);
	//glGenBuffers(1, &VBO_N);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Navicella[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	
	Model = translate(Model, vec3((contalieno % (width + 400) - 75), float(height) * randhigh + 4 + 15 * (sin(((contsole)) / 20.0)) + 5, 0.0));
	Model = scale(Model, vec3(20.0, 20.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NAVICELLA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	// Disegna Navicella
	Model = mat4(1.0);
	//Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));
	


	col_top = { 0.3, 0.0, 0.8, 0.9 };
	col_bottom = { 0.15, 0.15, 0.15, 1.0 };

	disegna_navicella(nTriangles_sole, 1.0, 0.3, col_top, col_bottom, Navicella);
	//glGenVertexArrays(1, &VAO_NAVICELLA);
	glBindVertexArray(VAO_NAVICELLA);
	//glGenBuffers(1, &VBO_N);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Navicella[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
	Model = translate(Model, vec3((contalieno % (width + 400) - 75), float(height) * randhigh + 15 * (sin(((contsole)) / 20.0)) + 5, 0.0));
	Model = scale(Model, vec3(20.0, 20.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_NAVICELLA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);


	if (contsole % 160 == 0) {
		checktime = 1;
		spawnpallinax = contalieno % (width + 400) - 75;
		spawnpallinay = float(height) * randhigh + 4 + 15 * (sin(((contsole)) / 20.0)) + 5;
		gravity = 0;
		rt = float(rand() % 101) / 100;
		gt = float(rand() % 101) / 100;
		bt = float(rand() % 101) / 100;

		rb = (rand() % 101) / 100;
		gb = (rand() % 101) / 100;
		bb = (rand() % 101) / 100;
	}





	//Disegna Pala Eolica
	glBindVertexArray(VAO_PalaEolica);
	for (int i = 1; i < 7; i++) /* Disegna 6 istanze di Pala Eolica (sostegno+pale) */
	{
		Model = mat4(1.0);
		Model = translate(Model, vec3(float(width) * 0.15 * i, float(height) * 0.62, 0.0));
		Model = scale(Model, vec3(3.0, 100.0, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		glDrawArrays(GL_TRIANGLES, vertices_PalaEolica - 6, 6);
		Model = mat4(1.0);
		Model = translate(Model, vec3(float(width) * 0.15 * i, float(height) * 0.62, 0.0));
		Model = scale(Model, vec3(40.0, 40.0, 1.0));
		Model = rotate(Model, radians(contangolo), vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, vertices_PalaEolica - 6); // Starting from vertex 0; total vertices
	}
	glBindVertexArray(0);


	if (checktime == 1) {
		// Disegna palline
		Model = mat4(1.0);
		//Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));


		col_top_palline = { rt, gt, bt, 1.0 };
		col_bottom_palline = { rt, gt, bt, t };

		disegna_cerchio(nTriangles_sole, 1, col_top_palline, col_bottom_palline, Palline);
		//glGenVertexArrays(1, &VAO_PALLINE);
		glBindVertexArray(VAO_PALLINE);
		//glGenBuffers(1, &VBO_Pn);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Pn);
		glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Palline[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		//Model = translate(Model, vec3(contsole%width, float(height)*0.7 + ((contsole % width)^2)/3, 0.0));
		Model = translate(Model, vec3(spawnpallinax, spawnpallinay + gravity, 0.0));
		Model = scale(Model, vec3(5.0, 5.0, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		glBindVertexArray(VAO_PALLINE);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);

		if ((spawnpallinax - (posx - bwidth / 2) < 35.0 && spawnpallinax - (posx - bwidth / 2) > -35.0) &&
			(spawnpallinay + gravity) - (posy + bheight + distacco_da_terra) < 35.0 && (spawnpallinay + gravity) - (posy + bheight + distacco_da_terra) > -35.0) {
			checktime = 2;
			pallar = rt;
			pallag = gt;
			pallab = bt;
		}

	}


	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab.2 - Gabriele Raciti");
	glutDisplayFunc(drawScene);
	//Evento tastiera tasto premuto
	glutKeyboardFunc(keyboardPressedEvent);
	//Evento tastiera tasto rilasciato (per fermare lo spostamento a dx e sx mantenendo il rimbalzo sul posto)
	glutKeyboardUpFunc(keyboardReleasedEvent);

	//gestione animazione
	glutTimerFunc(66, update, 0);
	//glutTimerFunc(1000, updateSole, 0);
	glewExperimental = GL_TRUE;
	glewInit();


	initShader();
	init();


	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
}