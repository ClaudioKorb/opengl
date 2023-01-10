#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define Y_MAX 10
#define X_MAX 10

int success;
char infoLog[512];
GLboolean wireframe = GL_FALSE;

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

    Shader shaderOrange("../shaders/vertex.vs", "../shaders/orange.fs");
    Shader shaderYellow("../shaders/vertex.vs", "../shaders/yellow.fs");

    while(!glfwWindowShouldClose(window))
    {
        //Input
        processInput(window);  
        // ----
        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderOrange.ID);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);   
        glUseProgram(shaderYellow.ID);
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