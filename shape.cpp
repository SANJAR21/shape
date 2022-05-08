

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 5);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TwoPentagons", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // build and compile our shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // outputs the color orange
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // outputs the color yellow
    unsigned int shaderProgramOrange = glCreateProgram(); // the first shader program
    unsigned int shaderProgramYellow = glCreateProgram(); // the second shader program
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);
    // link the first program object
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    // link the second program object through a different fragment shader 
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // set up vertex data 
    float firstPentagon[] = {
        -0.8f, -0.3f, 0.0f,
        -0.2f, -0.3f, 0.0f,
        -0.2f, 0.3f, 0.0f,
        -0.5f, 0.6f, 0.0f,
        -0.8f, 0.3f, 0.0f
    };
    float secondPentagon = {
        -0.8f, -0.3f, 0.0f,
        -0.2f, -0.3f, 0.0f,
        -0.2f, 0.3f, 0.0f,
        -0.5f, 0.6f, 0.0f,
        -0.8f, 0.3f, 0.0f
    };
    
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); 
    glGenBuffers(2, VBOs);
    // first shape setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstPentagon), firstPentagon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 5, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // second shape setup
    glBindVertexArray(VAOs[1]);	// a different VAO 
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondPentagon), secondPentagon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 5, GL_FLOAT, GL_FALSE, 0, (void*)0); 
    glEnableVertexAttribArray(0);

    // rendering the loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the shape we first use the vertex and orange fragment shader from the first program
        glUseProgram(shaderProgramOrange);
        // first shape using from first VAO
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 5);	// outputs an orange shape
        // second shape from the second VAO
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);	// outputs a yellow shape

        // glfw: swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: terminate
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
