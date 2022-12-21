#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define Y_MAX 10
#define X_MAX 10

int success;
char infoLog[512];
GLboolean wireframe = GL_FALSE;

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
     -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
      0.0f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f

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
    
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);   //Organiza buffer
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));   //Organiza buffer
    glEnableVertexAttribArray(1);                                                   //Habilita buffer
                                               //Habilita buffer
    // -- Vertex Buffer --
// VERTEX ARRAY BIND =================================================
    
    Shader ourShader("../shaders/shader.vs", "../shaders/shader.fs");

    while(!glfwWindowShouldClose(window))
    {
        //Input
        processInput(window);  
        // ----
        //Rendering
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        ourShader.use();
        ourShader.set4Float("someUniform",1.0f, greenValue, 1.0f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
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