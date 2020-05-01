#include "EngineMain.h"
// #include "TextureLoader.h"
#include "Texture2D.h"


HelloGL::HelloGL(int argc, char* argv[])
{

	InitGL(argc, argv);
	InitLighting();
	InitFog();

	InitObjects();
	glutMainLoop();
}


// Initialise all of the settings required for use of OpenGL in the project
void HelloGL::InitGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Simple OpenGL Program");


	glutKeyboardFunc(GLUTCallbacks::keyboard);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	glutDisplayFunc(GLUTCallbacks::Display);

	
	//set Up Lighting
	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);*/

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);


	// Set the viewport to be the entire window
	glViewport(0, 0, 800, 800);

	// Set the correct perspective.
	gluPerspective(90, 1, 0.1, 10000);


	//revert back to modelview
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMAL_ARRAY);
	

	glCullFace(GL_BACK);
}

void HelloGL::InitLighting()
{
	//lightPosition = new Vector3();
	//lightPosition->X = 0.0;
	//lightPosition->Y = 0.0;
	//lightPosition->Z = 3.0;
	//lightPosition->Z = 0.0;

	//lightData = new Lighting();
	//lightData->ambient.X = 0.8;
	//lightData->ambient.Y = 0.8;
	//lightData->ambient.Z = 0.8;
	////lightData->ambient.w = 1.0;

	//lightData->diffuse.X = 0.8;
	//lightData->diffuse.Y = 0.8;
	//lightData->diffuse.Z = 0.8;
	////lightData->diffuse.w = 1.0;

	//lightData->specular.X = 0.8;
	//lightData->specular.Y = 0.8;
	//lightData->specular.Z = 0.8;
	////lightData->speclar.w = 1.0;

}
void HelloGL::InitFog() {
	//Gof initiation and rendering
	fogFilter = 0;
	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glFogi(GL_FOG_MODE, fogMode[fogFilter]); // Sets fog mode
	glFogfv(GL_FOG_COLOR, fogColor); // Sets fog color
	glFogf(GL_FOG_DENSITY, 0.01f); // Sets fog density
	glHint(GL_FOG_HINT, GL_DONT_CARE); // Fog hint 
	glFogf(GL_FOG_START, 1.0f); // Fog starting depth
	glFogf(GL_FOG_END, 2.0f); // Fog end depth
	glEnable(GL_FOG);

}
void HelloGL::GenerateAsteroidField()
{
	//Create a dynamic array of asteroids
	asteroidArray = new Asteroid * [asteroidCount];
	

	for (int i = 0; i < asteroidCount; i++)
	{
		asteroidArray[i] = new Asteroid();
		asteroidArray[i]->LoadModel(std::string("Objects/astroid.obj"));
	}

}

void HelloGL::InitObjects()
{
	camera = new Camera();
	cameraPosition.X = 0.0f, cameraPosition.Y = 0.0f, cameraPosition.Z = -50.0f;

	//mLight = new Lighting();

	spaceFighter = new SpaceFigher(Vector3(0.0f, 0.0f, 0.0f));
	spaceFighter->LoadModel(std::string("Objects/ShipFighterModel.obj"));
	

	freighter = new Freighter(Vector3(40.0f, 50.0f, -2000.0f));
	freighter->LoadModel(std::string("Objects/Freighter.obj"));

	asteroidCount = 20;
	GenerateAsteroidField();

	camera->eye.Z = 1.0f;
	camera->up.Y = 1.0f;

}

void HelloGL::Display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clears the scene

	for (size_t i = 0; i < asteroidCount; i++)
	{
		asteroidArray[i]->Render();
	}

	spaceFighter->Render();
	freighter->Render();
	

	glutSwapBuffers();

	

	glFlush();
}


void HelloGL::Keyboard(unsigned char key, int x, int y)
{
	spaceFighter->CheckKeyInputs(key, x, y);

}

void HelloGL::Update() {

	cameraTarget = spaceFighter->GetPosition();

	glLoadIdentity();
	gluLookAt(camera->eye.X, camera->eye.Y, camera->eye.Z, camera->center.X, camera->center.Y, camera->center.Z, camera->up.X, camera->up.Y, camera->up.Z);
	glTranslatef(-cameraTarget.X, -cameraTarget.Y - 15, -cameraTarget.Z - 40); // Keeps the camera in the same point behind the spacecraft

	


	rotation.X += 0.0f;
	rotation.Y += 1.0f;
	rotation.Z += 0.0f;
	//
	//glLightfv(GL_LIGHT0, GL_AMBIENT, &(mLight->ambient.X));
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, &(mLight->diffuse.X));
	//glLightfv(GL_LIGHT0, GL_SPECULAR, &(mLight->specular.X));

	spaceFighter->Update();
	freighter->Update();

	for (size_t i = 0; i < asteroidCount; i++)
	{
		asteroidArray[i]->Update();
	}
	

	glutPostRedisplay();

}





HelloGL::~HelloGL()
{
	delete camera;
}
