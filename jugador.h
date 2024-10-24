float jugadorVertices[] =
{
	-0.3f, -0.3f, -0.3f,    1.0f, 1.0f, 1.0f,
	 0.3f, -0.3f, -0.3f,    1.0f, 1.0f, 1.0f,
	 0.3f,  0.3f, -0.3f,    1.0f, 1.0f, 1.0f,
	-0.3f,  0.3f, -0.3f,    1.0f, 1.0f, 1.0f,

	-0.3f, -0.3f,  -0.1f,   1.0f, 1.0f, 1.0f,
	 0.3f, -0.3f,  -0.1f,   1.0f, 1.0f, 1.0f,
	 0.3f,  0.3f,  -0.1f,   1.0f, 1.0f, 1.0f,
	-0.3f,  0.3f,  -0.1f,   1.0f, 1.0f, 1.0f
};

unsigned int indicesJugador[] = {
	0,1,2,
	0,2,3,

	4,5,6,
	4,6,7,

	7,6,2,
	7,2,3,

	4,5,1,
	4,1,0,

	4,7,0,
	0,7,3,

	5,6,1,
	1,6,2
};

vec3 posJugador[] = {
	vec3(3, 13, 0.1)
};

vec3 jugadorVel = vec3(0.0f, 0.0f, 0.0f);
float gravedad = -9.81f;