#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define Y_MAX 10
#define X_MAX 10

int success;
char infoLog[512];
GLboolean wireframe = GL_FALSE;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"        
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSourceOrange = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\0";

typedef struct color
{
    float r, g, b, a;
}color;

typedef struct point
{
    float x, y, z;
}point;


typedef struct triangle
{
    point p1, p2, p3;
}triangle;


color clearColor = {0, 0, 0, 0};

float vertices1[] = {
     -0.8f, -0.4f, 0.0f,
     -0.4f,  0.4f, 0.0f,
      0.0f, -0.4f, 0.0f,    
};

float vertices2[] = {
      0.0f, -0.4f, 0.0f,
      0.4f,  0.4f, 0.0f,
      0.8f, -0.4f, 0.0f     
};



//Callback para resize da tela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    static int previous = GLFW_RELEASE;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if(previous == GLFW_PRESS)
        {
            return;
        }
        else
        {
            previous = GLFW_PRESS;
        }

        GLboolean previousWireframe = wireframe;
        wireframe ^= GL_TRUE;
        if(previousWireframe != wireframe)
        {
            if(wireframe == GL_TRUE)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            if(wireframe == GL_FALSE)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        previous = GLFW_RELEASE;
    }

}

GLfloat normalizeRGB(float value)
{
    return (value / 255.0);
}

void setColor(color *col, int r, int g, int b, int a)
{
    col->r = normalizeRGB(r);
    col->g = normalizeRGB(g);
    col->b = normalizeRGB(b);
    col->a = normalizeRGB(a);

}

int initContext()
{
    if(!glfwInit())
    {   
        return -1;
    };

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return 0;
}

GLFWwindow *initWindow(int width, int height, char* name)
{
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);    if(!window)
    if(window == NULL)
    {
        std::cout << "FAILED TO CREATE GLFW WINDOW" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
        return NULL;
    }
    return window;
}

unsigned int createVertexShader()
{
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: Vertex Shader compilation failed\n" << infoLog << std::endl;
    }

    return vertexShader;
}

unsigned int createFragmentShader(const char *src)
{

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &src, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR: Fragment Shader compilation failed\n" << infoLog << std::endl;
    }
    return fragmentShader;

}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR: Shader Program creation failed\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

int main(void)
{
    if(initContext() != 0)
    {
        return -1;
    }

    GLFWwindow *window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL");
    glViewport(0, 0, WINDOW_WIDTH ,WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
// VERTEX ARRAY BIND =================================================
    //Vertex Buffer
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);      //Preenche buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //Organiza buffer
    glEnableVertexAttribArray(0);                                                   //Habilita buffer
    // -- Vertex Buffer --
// VERTEX ARRAY BIND =================================================

    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
// VERTEX ARRAY BIND =================================================
    //Vertex Buffer
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);      //Preenche buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //Organiza buffer
    glEnableVertexAttribArray(0);                                                   //Habilita buffer
    // -- Vertex Buffer --
// VERTEX ARRAY BIND =================================================
    
    //Vertex Shaders
    unsigned int vertexShader = createVertexShader();
    // -- Vertex Shaders --
    
    //Fragment Shaders
    unsigned int fragmentShaderOrange = createFragmentShader(fragmentShaderSourceOrange);
    unsigned int fragmentShaderYellow = createFragmentShader(fragmentShaderSourceYellow);
    // -- Fragment Shaders --

    //Shader Program
    unsigned int shaderProgramOrange = createShaderProgram(vertexShader, fragmentShaderOrange);
    unsigned int shaderProgramYellow = createShaderProgram(vertexShader, fragmentShaderYellow);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOrange);  
    glDeleteShader(fragmentShaderYellow);  
    // -- Shader Program --

    while(!glfwWindowShouldClose(window))
    {
        //Input
        processInput(window);  
        // ----
        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);   
        glUseProgram(shaderProgramYellow); 
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);    
        //--Rendering--

        //Events
        glfwSwapBuffers(window);
        glfwPollEvents();
        //--Events--
    }

    glfwTerminate();
    
    return 0;
}