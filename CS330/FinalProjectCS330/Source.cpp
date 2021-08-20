#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // Image loading Utility functions

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Milestone3.5"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbos[2];     // Handles for the vertex buffer objects
        GLuint nVertices;
        GLuint nIndices;    // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    // Texture
    GLuint gTextureBox;
    GLuint gTexturePlant;
    GLuint gTexturePot;
    GLuint gTextureFloor;
    GLuint gWall;
    GLuint gEntCenter;
    GLuint gTV;

    // Shader program
    GLuint gProgramId;

    // camera
    Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;
}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);


/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1
    layout(location = 2) in vec2 textureCoordinate;

    out vec4 vertexColor; // variable to transfer color data to the fragment shader
    out vec2 vertexTextureCoordinate;

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexColor = color; // references incoming color data
    vertexTextureCoordinate = textureCoordinate; // references incoming texture data
}
);


/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,
    in vec4 vertexColor; // Variable to hold incoming color data from vertex shader
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform sampler2D uTexture;
    uniform vec2 uvScale;

void main()
{
    //fragmentColor = vec4(vertexColor);
    fragmentColor = texture(uTexture, vertexTextureCoordinate);
}
);

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}

int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    // Load texture
    const char* texFilename = "resources/textures/box2.jpg";
    if (!UCreateTexture(texFilename, gTextureBox))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
 
    texFilename = "resources/textures/plant.png";
    if (!UCreateTexture(texFilename, gTexturePlant))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    texFilename = "resources/textures/planter.png";
    if (!UCreateTexture(texFilename, gTexturePot))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "resources/textures/floor.png";
    if (!UCreateTexture(texFilename, gTextureFloor))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "resources/textures/wall.jpg";
    if (!UCreateTexture(texFilename, gWall))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "resources/textures/tv.jpg";
    if (!UCreateTexture(texFilename, gTV))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramId);
  
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureBox"), 0);

    // We set the texture as texture unit 1
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePlant"), 1);

    // We set the texture as texture unit 2
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePot"), 2);

    // We set the texture as texture unit 3
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureFloor"), 3);

    // We set the texture as texture unit 4
    glUniform1i(glGetUniformLocation(gProgramId, "uWall"), 4);

    // We set the texture as texture unit 4
    glUniform1i(glGetUniformLocation(gProgramId, "uTV"), 5);


    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gMesh);

    // Release texture
    UDestroyTexture(gTextureBox);
    UDestroyTexture(gTexturePlant);
    UDestroyTexture(gTexturePot);
    UDestroyTexture(gEntCenter);
    UDestroyTexture(gTextureFloor);
    UDestroyTexture(gWall);

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;
    bool orthographic = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        orthographic = !orthographic;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


// Functioned called to render a frame
void URender()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. Scales the object by 2
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
    // 2. Rotates shape by 15 degrees in the x axis
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
    // 3. Place object at the origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a orthographic projection

    bool orthographic = false;
    glm::mat4 projection;
    if (orthographic)
    {
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //switch camera to orthographic view
        //cout << "orthographic" << endl;
    }
    else {
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
        //cout << "perspective" << endl;
    }

    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.vao);

    // bind textures on corresponding texture units
     
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePot);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, 84); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePlant);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 84, 120); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureBox);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 204, 36); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureFloor);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 240, 6); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gWall);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 246, 6); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTV);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 252, 180); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureBox);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 432, 6); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTV);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 438, 6); // Draws the triangle

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureBox);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 444, 72); // Draws the triangle

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}


// Implements the UCreateMesh function
void UCreateMesh(GLMesh& mesh)
{
    // Position and Color data
    GLfloat verts[] = {

        // Vertex Positions        // Colors (r,g,b,a)         //Texture Coord
        

        //Pot Top

        0.25f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //0
        -0.5f,  0.5f,  0.25f,       1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //6
        -0.25f,  0.5f,  0.5f,       1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //7

        0.5f,  0.5f,  0.25f,        1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //1
        0.25f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //0
        -0.5f,  0.5f,  0.25f,       1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //6

        -0.5f,  0.5f, -0.25f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //5
        -0.5f,  0.5f,  0.25f,       1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //6
        0.5f,  0.5f,  0.25f,        1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //1

        0.5f,  0.5f,  0.25f,        1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //1
        -0.5f,  0.5f, -0.25f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //5
        0.5f,  0.5f,  -0.25f,       0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //2  

        -0.5f,  0.5f, -0.25f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //5
        0.5f,  0.5f,  -0.25f,       0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //2
        -0.25f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //4

        0.5f,  0.5f,  -0.25f,       0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //2
        -0.25f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //4
        0.25f,  0.5f, -0.5f,        0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //3

        //Pot Base

        0.125f,  -0.25f,  0.25f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //8
        -0.25f,  -0.25f,  0.125f,   0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //14
        -0.125f,  -0.25f,  0.25f,   0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //15

        0.125f,  -0.25f,  0.25f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //8
        -0.25f,  -0.25f,  0.125f,   0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //14
        0.25f,  -0.25f,  0.125f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //9

        -0.25f,  -0.25f,  0.125f,   0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //14
        0.25f,  -0.25f,  0.125f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //9
        -0.25f,  -0.25f, -0.125f,   0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //13

        0.25f,  -0.25f,  0.125f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //9
        -0.25f,  -0.25f, -0.125f,   0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //13
        0.25f,  -0.25f,  -0.125f,   1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //10

        -0.25f,  -0.25f, -0.125f,   0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //13
        0.25f,  -0.25f,  -0.125f,   1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //10
        -0.125f,  -0.25f, -0.25f,   1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //12

        0.25f,  -0.25f,  -0.125f,   1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //10
        -0.125f,  -0.25f, -0.25f,   1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //12
        0.125f,  -0.25f, -0.25f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //11

        //Sides of Pot

        -0.25f,  0.5f,  0.5f,       1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //7
        0.125f,  -0.25f,  0.25f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //8
        0.25f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //0
        -0.25f,  0.5f,  0.5f,       1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //7
        0.125f,  -0.25f,  0.25f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //8
        -0.125f,  -0.25f,  0.25f,   0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //15

        0.125f,  -0.25f,  0.25f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //8
        0.25f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //0
        0.25f,  -0.25f,  0.125f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //9
        0.5f,  0.5f,  0.25f,        1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //1
        0.25f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 0.0f,  //0
        0.25f,  -0.25f,  0.125f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //9

        0.25f,  -0.25f,  0.125f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //9
        0.5f,  0.5f,  0.25f,        1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //1
        0.25f,  -0.25f,  -0.125f,   1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //10
        0.5f,  0.5f,  0.25f,        1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //1
        0.25f,  -0.25f,  -0.125f,   1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //10
        0.5f,  0.5f,  -0.25f,       0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //2

        0.5f,  0.5f,  -0.25f,       0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //2
        0.25f,  0.5f, -0.5f,        0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //3
        0.125f,  -0.25f, -0.25f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //11
        0.25f,  -0.25f,  -0.125f,   1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //10
        0.125f,  -0.25f, -0.25f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //11
        0.5f,  0.5f,  -0.25f,       0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,  //2

        0.25f,  0.5f, -0.5f,        0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //3
        -0.25f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.0f,  //4
        -0.125f,  -0.25f, -0.25f,   1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //12
        0.25f,  0.5f, -0.5f,        0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //3
        0.125f,  -0.25f, -0.25f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //11
        -0.125f,  -0.25f, -0.25f,   1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //12

        -0.25f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //4
        -0.5f,  0.5f, -0.25f,       1.0f, 0.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //5
        -0.25f,  -0.25f, -0.125f,   0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //13
        -0.25f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.0f,  //4
        -0.125f,  -0.25f, -0.25f,   1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //12
        -0.25f,  -0.25f, -0.125f,   0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //13

        -0.5f,  0.5f, -0.25f,       1.0f, 0.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //5
        -0.5f,  0.5f,  0.25f,       1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 0.0f,  //6
        -0.25f,  -0.25f,  0.125f,   0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //14
        -0.5f,  0.5f, -0.25f,       1.0f, 0.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //5
        -0.25f,  -0.25f, -0.125f,   0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //13
        -0.25f,  -0.25f,  0.125f,   0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //14

        -0.5f,  0.5f,  0.25f,       1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 0.0f,  //6
        -0.25f,  0.5f,  0.5f,       1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //7
        -0.125f,  -0.25f,  0.25f,   0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //15
        -0.5f,  0.5f,  0.25f,       1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 0.0f,  //6
        -0.25f,  -0.25f,  0.125f,   0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //14
        -0.125f,  -0.25f,  0.25f,   0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //15 - 84


        //Plant

        //Spike 1

         0.0f,  1.5f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //16
        -0.07f,  0.5f, 0.07f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //17
        -0.07f,  0.5f, -0.07f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //18

        0.07f,  0.5f, 0.07f,        1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //19
        -0.07f,  0.5f, 0.07f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //17
         0.0f,  1.5f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //16

         0.0f,  1.5f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //16
        -0.07f,  0.5f, -0.07f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //18
        0.07f,  0.5f, 0.07f,        1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,  //19

        -0.07f,  0.5f, 0.07f,       0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //17
        -0.07f,  0.5f, -0.07f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //18
        0.07f,  0.5f, 0.07f,        1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 0.0f,  //19

        //Spike 2

        -0.30f,  1.25f,  -0.30f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //20
        -0.37f,  0.5f,  -0.27f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //21
        -0.37f,  0.5f,  -0.37f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //22

        -0.27f,  0.5f, -0.27f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //23
        -0.37f,  0.5f,  -0.27f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //21
        -0.37f,  0.5f,  -0.37f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //22

        -0.30f,  1.25f,  -0.30f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //20
        -0.37f,  0.5f,  -0.37f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //22
        -0.27f,  0.5f, -0.27f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //23

        -0.30f,  1.25f,  -0.30f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //20
        -0.37f,  0.5f,  -0.27f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //21
        -0.27f,  0.5f, -0.27f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //23

        //Spike 3

        -0.20f,  1.4f,  -0.20f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //24
         0.27f,  0.5f,  0.17f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //25
         0.27f,  0.5f,  0.27f,      1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //26

         0.27f,  0.5f,  0.17f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //25
         0.27f,  0.5f,  0.27f,      1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //26
         0.17f,  0.5f, 0.17f,       1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //27

        -0.20f,  1.4f,  -0.20f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //24
         0.27f,  0.5f,  0.17f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //25
         0.17f,  0.5f, 0.17f,       1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //27

         0.27f,  0.5f,  0.27f,      1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //26
         0.17f,  0.5f, 0.17f,       1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //27
        -0.20f,  1.4f,  -0.20f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //24

        //Spike 4

         0.10f,  1.6f,  0.10f,      1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //28
        -0.27f,  0.5f,  0.17f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //29
        -0.27f,  0.5f,  0.27f,      1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //30

        -0.27f,  0.5f,  0.17f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //29
        -0.27f,  0.5f,  0.27f,      1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //30
        -0.17f,  0.5f, 0.17f,       1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //31

         0.10f,  1.6f,  0.10f,      1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //28
        -0.27f,  0.5f,  0.17f,      1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //29
        -0.17f,  0.5f, 0.17f,       1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //31

         0.10f,  1.6f,  0.10f,      1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //28
        -0.27f,  0.5f,  0.27f,      1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //30
        -0.17f,  0.5f, 0.17f,       1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //31

        //Spike 5

        0.10f,  1.6f,  -0.10f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //32
        0.27f,  0.5f,  -0.17f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //33
        0.27f,  0.5f,  -0.27f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //34

        0.27f,  0.5f,  -0.17f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //33
        0.27f,  0.5f,  -0.27f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //34
        0.17f,  0.5f, -0.17f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //35

        0.10f,  1.6f,  -0.10f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //32
        0.27f,  0.5f,  -0.17f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //33
        0.17f,  0.5f, -0.17f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //35

        0.10f,  1.6f,  -0.10f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //32
        0.27f,  0.5f,  -0.27f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //34
        0.17f,  0.5f, -0.17f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //35

        //Spike 6

         0.20f,  1.4f,  -0.20f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        0.20f,  0.5f,  0.20f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        0.20f,  0.5f,  0.30f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38

        0.20f,  0.5f,  0.20f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        0.20f,  0.5f,  0.30f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        0.10f,  0.5f, 0.20f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

         0.20f,  1.4f,  -0.20f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        0.20f,  0.5f,  0.20f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        0.10f,  0.5f, 0.20f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

         0.20f,  1.4f,  -0.20f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        0.20f,  0.5f,  0.30f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        0.10f,  0.5f, 0.20f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        //Spike 7

        -0.30f,  1.7f, -0.30f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //40
        -0.27f,  0.5f,  -0.17f,    1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //41
        -0.27f,  0.5f,  -0.27f,    1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //42

        -0.27f,  0.5f,  -0.17f,    1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //41
        -0.27f,  0.5f,  -0.27f,    1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //42
        -0.17f,  0.5f, -0.17f,     1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //43
     
        -0.30f,  1.7f, -0.30f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //40
        -0.27f,  0.5f,  -0.17f,    1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //41
        -0.17f,  0.5f, -0.17f,     1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //43

        -0.30f,  1.7f, -0.30f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //40
        -0.27f,  0.5f,  -0.27f,    1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //42
        -0.17f,  0.5f, -0.17f,     1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //43

        //Spike 8

        -0.10f,  1.3f,  -0.20f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //44
        0.30f,  0.5f,  -0.30f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //45
        0.30f,  0.5f,  -0.40f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //46

        0.30f,  0.5f,  -0.30f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //45
        0.30f,  0.5f,  -0.40f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //46
        0.20f,  0.5f, -0.30f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //47

        -0.10f,  1.3f,  -0.20f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //44
        0.30f,  0.5f,  -0.40f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //46
        0.20f,  0.5f, -0.30f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //47

        -0.10f,  1.3f,  -0.20f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //44
        0.30f,  0.5f,  -0.30f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //45
        0.20f,  0.5f, -0.30f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //47

        //Spike 9

        0.30f,  1.8f,  -0.10f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //48
        0.37f,  0.5f,  -0.07f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //49
        0.37f,  0.5f,  -0.17f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //50

        0.37f,  0.5f,  -0.07f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //49
        0.37f,  0.5f,  -0.17f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //50
        0.27f,  0.5f, -0.07f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //51

        0.30f,  1.8f,  -0.10f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //48
        0.37f,  0.5f,  -0.07f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //49
        0.27f,  0.5f, -0.07f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //51

        0.30f,  1.8f,  -0.10f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //48
        0.37f,  0.5f,  -0.17f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //50
        0.27f,  0.5f, -0.07f,      1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //51

        //Spike 10

        -0.20f,  1.5f,  -0.10f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //52
        -0.47f,  0.5f,  0.17f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //53
        -0.47f,  0.5f,  0.27f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //54

        -0.47f,  0.5f,  0.17f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //53
        -0.47f,  0.5f,  0.27f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //54
        -0.37f,  0.5f,  0.17f,     1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //55

        -0.20f,  1.5f,  -0.10f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //52
        -0.47f,  0.5f,  0.27f,     1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //54
        -0.37f,  0.5f,  0.17f,     1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //55

        -0.20f,  1.5f,  -0.10f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //52
        -0.47f,  0.5f,  0.17f,     1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //53
        -0.37f,  0.5f,  0.17f,     1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //55 - 120

       
        //Cube Shelf     

        1.0f,  -0.25f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.0f,  -0.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        1.0f,  -0.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -1.0f,  -0.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        1.0f,  -0.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.0f,  -0.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        1.0f,  -0.25f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.0f,  -0.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        1.0f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -1.0f,  -0.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        1.0f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.0f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        1.0f,  -0.25f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        1.0f,  -0.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        1.0f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        1.0f,  -0.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        1.0f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        1.0f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        1.0f,  -0.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.0f,  -0.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        1.0f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.0f,  -0.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        1.0f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -1.0f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -1.0f,  -0.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.0f,  -0.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.0f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -1.0f,  -0.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -1.0f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -1.0f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        1.0f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.0f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        1.0f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.0f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        1.0f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -1.0f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36

        //Floor

         1.0f, -1.75f, 2.5f,      0.0f, 1.0f, 1.0f, 1.0f,       1.0f, 0.0f,  //64
        -10.0f, -1.75f, 2.5f,     0.0f, 0.0f, 1.0f, 1.0f,       0.0f, 0.0f,  //65
         1.0f, -1.75f, -1.0f,     1.0f, 0.0f, 1.0f, 1.0f,       0.0f, 1.0f,  //66

        -10.0f, -1.75f, 2.5f,     0.0f, 0.0f, 1.0f, 1.0f,       0.0f, 1.0f,  //65
         1.0f, -1.75f, -1.0f,     1.0f, 0.0f, 1.0f, 1.0f,       0.0f, 0.0f,  //66
        -10.0f, -1.75f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,       1.0f, 0.0f,  //67 - 6

        //wall

         1.0f, -1.75f, -1.0f,     1.0f, 0.0f, 1.0f, 1.0f,       1.0f, 0.0f,  //66
        -10.0f, -1.75f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,       0.0f, 0.0f,  //67
         1.0f, 5.0f, -1.0f,       0.5f, 0.25f, 0.5f, 1.0f,      0.0f, 1.0f,  //68

        -10.0f, -1.75f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,       0.0f, 1.0f,  //67
         1.0f, 5.0f, -1.0f,       0.5f, 0.25f, 0.5f, 1.0f,      0.0f, 0.0f,  //68
        -10.0f, 5.0f, -1.0f,      0.5f, 0.25f, 0.5f, 1.0f,      1.0f, 0.0f,  //69 - 6
 
                                                                                   
         //entertainment stand

        -1.5f,  1.25f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.5f,  1.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.5f,  1.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -8.5f,  1.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.5f,  1.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.5f,  1.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        -1.5f,  1.25f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.5f,  1.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.5f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -8.5f,  1.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.5f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -1.5f,  1.25f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.5f,  1.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.5f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -1.5f,  1.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.5f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.5f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.5f,  1.25f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.5f,  1.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.5f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.5f,  1.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.5f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -8.5f,  1.25f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.5f,  1.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -8.5f,  1.25f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        -1.5f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -1.5f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -1.5f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36
                                                                                   
                                                                                  
        // TV Stand Legs

         // Front left
        -8.3f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        -8.3f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -8.3f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.3f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.3f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -8.3f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.3f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.3f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.3f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.3f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.3f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -8.5f,  -1.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -8.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -8.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -8.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -8.5f,  -1.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        -8.3f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -8.3f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -8.3f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -8.5f,  -1.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36


            //Back Left
        -8.3f,  -0.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -8.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        -8.3f,  -0.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -8.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.3f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -8.3f,  -0.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -8.3f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.3f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -8.3f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.3f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.3f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.3f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.3f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.3f,  -1.75f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -8.5f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -8.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -8.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -8.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -8.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -8.5f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        -8.3f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -8.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -8.3f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -8.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -8.3f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -8.5f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36


          // Front right
        -1.7f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -1.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        -1.7f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -1.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -1.7f,  -0.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.7f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.7f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -1.7f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.7f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.7f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.7f,  -0.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.7f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.7f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -1.5f,  -1.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -1.5f,  -0.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -1.5f,  -0.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -1.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -1.5f,  -1.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        -1.7f,  -1.75f,  1.0f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -1.7f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.5f,  -1.75f,  1.0f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -1.7f,  -1.75f,  0.9f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -1.5f,  -1.75f,  0.9f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36


            //Back Left
        -1.7f,  -0.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -1.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        -1.7f,  -0.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -1.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.7f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -1.7f,  -0.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -1.7f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.7f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -1.7f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.7f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.7f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.7f,  -0.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -1.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.7f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.7f,  -1.75f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -1.5f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -1.5f,  -0.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -1.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -1.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -1.5f,  -0.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -1.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -1.5f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        -1.7f,  -1.75f,  -0.9f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -1.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -1.7f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -1.5f,  -1.75f,  -0.9f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -1.7f,  -1.75f,  -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -1.5f,  -1.75f,  -1.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36

        //TV

        -2.5f,  4.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -7.5f,  4.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -2.5f,  4.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58

        -7.5f,  4.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -2.5f,  4.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -7.5f,  4.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59

        -2.5f,  4.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -7.5f,  4.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -2.5f,  1.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -7.5f,  4.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -2.5f,  1.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -7.5f,  1.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -2.5f,  4.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //56
        -2.5f,  4.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -2.5f,  1.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60

        -2.5f,  4.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -2.5f,  1.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -2.5f,  1.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -2.5f,  4.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //58
        -7.5f,  4.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -2.5f,  1.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -7.5f,  4.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -2.5f,  1.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -7.5f,  1.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63

        -7.5f,  4.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //57
        -7.5f,  4.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //59
        -7.5f,  1.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61

        -7.5f,  4.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //59
        -7.5f,  1.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //61
        -7.5f,  1.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //63

        -2.5f,  1.75f,  0.2f,     0.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //60
        -7.5f,  1.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -2.5f,  1.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62

        -7.5f,  1.75f,  0.2f,    0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //61
        -2.5f,  1.75f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f,  //62
        -7.5f,  1.75f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f,  //63 - 36



        //TV Stand

            //Left Leg Bottom
        -5.9f,  1.4f,  -0.6f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -6.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -6.0f,  1.25f,  0.90f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38

        -6.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -6.0f,  1.25f,  0.90f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -5.85f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -5.9f,  1.4f,  -0.6f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -6.0f,  1.25f,  -0.70f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -5.85f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -5.9f,  1.4f,  -0.6f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -6.0f,  1.25f,  0.9f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -5.85f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39 - 12


                //Left Leg Top
        -5.9f,  2.5f,  -0.0f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -6.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -6.0f,  1.25f,  -0.50f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38

        -6.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -6.0f,  1.25f,  -0.50f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -5.85f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -5.9f, 2.5f,  -0.0f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -6.0f,  1.25f,  -0.70f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -5.85f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -5.9f, 2.5f,  -0.0f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -6.0f,  1.25f,  -0.5f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -5.85f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39 - 12



            //Right Leg Bottom
        -4.1f,  1.4f,  -0.6f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -4.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -4.0f,  1.25f,  0.90f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38

        -4.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -4.0f,  1.25f,  0.90f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -4.15f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -4.1f,  1.4f,  -0.6f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -4.0f,  1.25f,  -0.70f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -4.15f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -4.1f,  1.4f,  -0.6f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -4.0f,  1.25f,  0.9f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -4.15f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39 - 12


        //Right Leg Top
        -4.1f,  2.5f,  -0.0f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -4.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -4.0f,  1.25f,  -0.50f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38

        -4.0f,  1.25f,  -0.7f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -4.0f,  1.25f,  -0.50f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -4.15f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -4.1f, 2.5f,  -0.0f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -4.0f,  1.25f,  -0.70f,       1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f,  //37
        -4.15f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39

        -4.1f, 2.5f,  -0.0f,     1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f,  //36
        -4.0f,  1.25f,  -0.5f,       1.0f, 0.5f, 1.0f, 1.0f,     0.0f, 1.0f,  //38
        -4.15f,  1.25f, -0.70f,        1.0f, 0.5f, 0.0f, 1.0f,     0.0f, 1.0f,  //39 - 12


    };


    



        
    

   

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerColor = 4;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    //mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerColor)));
    glEnableVertexAttribArray(2);
}


void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}

// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}