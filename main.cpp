#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"

struct vec2
{
    float x, y;
};
struct vec3
{
    float x, y, z;
};

struct Vertex
{
    vec2 position;
    vec3 color; // Add color attribute
};

struct Application
{
    GLShader m_basicProgram;
    GLuint VAO, VBO, EBO;

    void Initialize()
    {
        m_basicProgram.LoadVertexShader("basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        m_basicProgram.Create();

        // Generate and bind the Vertex Array Object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        const std::vector<Vertex> vertices =
        {
            {{0.5f, 0.5f}, {1.f, 0.f, 0.f}},   // top right
            {{0.5f, -0.5f}, {0.f, 1.f, 0.f}},  // bottom right
            {{-0.5f, -0.5f}, {0.f, 0.f, 1.f}}, // bottom left
            {{-0.5f, 0.5f}, {1.f, 1.f, 0.f}}   // top left
        };
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Generate and bind the Element Buffer Object
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        const std::vector<unsigned int> indices = 
        {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        uint32_t program = m_basicProgram.GetProgram();

        // Configure the position attribute
        const int POSITION = glGetAttribLocation(program, "a_position");
        if (POSITION < 0)
            std::cout << "erreur de programme" << std::endl;
        glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(POSITION);

        // Configure the color attribute
        const int loc_color = glGetAttribLocation(program, "a_color");
        if (loc_color < 0)
            std::cout << "erreur de programme" << std::endl;
        glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
        glEnableVertexAttribArray(loc_color);

        // Unbind the VAO to avoid accidental modifications
        glBindVertexArray(0);
    }

    void Terminate()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
        m_basicProgram.Destroy();
    }

    void Render()
    {
        glClearColor(1.f, 1.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        uint32_t program = m_basicProgram.GetProgram();
        glUseProgram(program);

        double time = glfwGetTime();
        int timeLocation = glGetUniformLocation(program, "u_Time");
        glUniform1f(timeLocation, static_cast<float>(time));

        // Bind the VAO
        glBindVertexArray(VAO);

        // Draw the rectangle using the indices in the EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Unbind the VAO
        glBindVertexArray(0);
    }
};

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Exemple OpenGL 2.+", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cout << "Erreur d'initialisation de GLEW" << std::endl;
        return -1;
    }

    Application app;
    app.Initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        app.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    app.Terminate();

    glfwTerminate();
    return 0;
}
