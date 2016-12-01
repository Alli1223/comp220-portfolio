#include "stdafx.h"
#include "World.h"

void showErrorMessage(const char* message, const char* title)
{
	// Note: this is specific to Windows, and would need redefining to work on Mac or Linux
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

bool compileShader(GLuint shaderId, const std::string& shaderFileName)
{
	// Read the source code from the file
	std::string shaderSource;
	std::ifstream sourceStream(shaderFileName, std::ios::in);
	if (sourceStream.is_open())
	{
		std::stringstream buffer;
		buffer << sourceStream.rdbuf();
		shaderSource = buffer.str();
		sourceStream.close();
	}
	else
	{
		showErrorMessage(shaderFileName.c_str(), "File not found");
		return false;
	}

	// Compile the shader
	const char* sourcePointer = shaderSource.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	// Check the results of compilation
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1)
	{
		// Display the compilation log
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, errorMessage.data());
		showErrorMessage(errorMessage.data(), shaderFileName.c_str());
	}

	return (result != GL_FALSE);
}

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {

	// Create the shaders
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShaderId, vertex_file_path);

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShaderId, fragment_file_path);

	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Check the program
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, errorMessage.data());
		showErrorMessage(errorMessage.data(), "glLinkProgram error");
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

GLuint loadTexture(const std::string& fileName)
{
	SDL_Surface* textureSurface = IMG_Load(fileName.c_str());

	if (textureSurface == nullptr)
	{
		showErrorMessage(SDL_GetError(), "IMG_Load failed");
		return 0;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int format;
	if (textureSurface->format->BytesPerPixel == 3)
	{
		format = GL_RGB;
	}
	else if (textureSurface->format->BytesPerPixel == 4)
	{
		format = GL_RGBA;
	}
	else
	{
		showErrorMessage("Invalid pixel format", ":(");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, textureSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(textureSurface);
	return textureId;
}

World::World()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		showErrorMessage(SDL_GetError(), "SDL_Init failed");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("Portfolio Project - Alli", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		showErrorMessage(SDL_GetError(), "SDL_CreateWindow failed");
	}

	glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		showErrorMessage(SDL_GetError(), "SDL_GL_CreateContext failed");
	}

	if (glewInit() != GLEW_OK)
	{
		showErrorMessage("glewInit failed", ":(");
	}
}


World::~World()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void World::createWorld()
{
	// Load textures
	GLuint grassTexture = loadTexture("GrassTex.png");
	GLuint mountainTexture = loadTexture("MountainTexture.png");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create an instance of the objects
	Mesh grassMesh;
	Mesh mountainMesh;
	Terrain terrain;
	Player_Movement playerMovement;

	/////// Generate the terrain ///////////////
	terrain.generateTerrain(grassMesh, mountainMesh);
	grassMesh.createBuffers();
	mountainMesh.createBuffers();


	// Variables to be used in the shader
	GLuint programID = loadShaders("vertex.glsl", "fragment.glsl");
	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");
	GLuint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");
	GLuint eyeDirectionLocation = glGetUniformLocation(programID, "eyeDirection");
	GLuint specularIntensity = glGetUniformLocation(programID, "specularIntensity");
	GLuint LightColor = glGetUniformLocation(programID, "LightColor");
	GLuint ObjectColor = glGetUniformLocation(programID, "ObjectColor");
	GLuint LightPower = glGetUniformLocation(programID, "LightPower");
	GLuint distance = glGetUniformLocation(programID, "distance");
	GLuint LightPos = glGetUniformLocation(programID, "LightPos");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	// Vec4 of players start pos
	glm::vec4 playerPosition(PlayerStartX, PlayerStartY, PlayerStartZ, 1);


	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);

	bool running = true;
	while (running)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}

		// Function that moves the player
		playerMovement.playerMove(playerPosition);

		// Gets the value of player look for functions later in this class
		glm::vec4 playerLook = playerMovement.GetPlayerLook();

		// Random variable that changes over time (for testing)
		float varyingPower = sin(SDL_GetTicks() / 1000.0f);

		// Set the background to sky blue
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		// Calculate the view matrix
		glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));

		// Calculate the projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 6000.0f);

		// Transform matrix
		glm::mat4 transform;

		// Uncomment to apply rotation and translation to the terrain
		//transform = glm::translate(transform, glm::vec3(varyingPower * 10, 0, 0));
		//transform = glm::rotate(transform, glm::radians(-90.0f), glm::vec3(1, 0, 0)); 

		// Calcualte the MVP matrix and pass it in
		glm::mat4 mvp = projection * view * transform;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		// Set player height to the floor height
		if (playerPosition.x > 0 && playerPosition.x < terrain.getTerrainWidth() && playerPosition.z > 0 && playerPosition.z < terrain.getTerrainDepth())
			playerPosition.y = terrain.getHeight(playerPosition.x, playerPosition.z) + playerheight;

		////////////// Lighting Variables /////////////////
		// Changes specular value and light power
		float specularIntensityVal = 10000.0f;
		float lightPower = 1.0f;

		// Changes the colour of the light
		glm::vec3 lightColour(1, 1, 1);

		// The grounds colour Variable for offset if needed
		glm::vec3 colour = glm::vec3(0.5, 0.5, 0.5);
		glm::vec3 objectColour(colour.r, colour.g, colour.b);

		// The position of the light
		glm::vec3 lightPos(playerPosition.x, playerPosition.y, playerPosition.z);

		// Passing in the values to the fragment shader
		glUniform3f(lightDirectionLocation, 100, -5, 1);
		glUniform3f(eyeDirectionLocation, playerPosition.x, playerPosition.y, playerPosition.z);
		glUniform1f(LightPower, lightPower);
		glUniform3f(LightColor, lightColour.r, lightColour.g, lightColour.b);
		glUniform3f(ObjectColor, objectColour.r, objectColour.g, objectColour.b);
		glUniform1f(specularIntensity, specularIntensityVal);
		glUniform3f(LightPos, lightPos.x, lightPos.y, lightPos.z);

		// Bind Textures
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		grassMesh.draw();

		glBindTexture(GL_TEXTURE_2D, mountainTexture);
		mountainMesh.draw();
		SDL_GL_SwapWindow(window);
	}

}

