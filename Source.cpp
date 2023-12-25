#include "Angel.h"
#include "Sphere.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef Angel::vec4  color4;
typedef Angel::vec3  point3;
typedef Angel::vec4  point4;

float Camera_Speed = 0.5f, yaw = -90.0f;

const int Number_of_Spheres = 11;
GLfloat Earth_Size = 0.1;

Sphere Spheres[Number_of_Spheres] = {
    Sphere(0.3f), //Sun 
    Sphere(Earth_Size * 1 / 3), //Mercury
    Sphere(Earth_Size - 0.02),  //Venus
    Sphere(Earth_Size), //Earth
    Sphere(Earth_Size * 1 / 2), //Mars
    Sphere(Earth_Size * 3), //Jupiter
    Sphere(Earth_Size * 2), //Saturn
    Sphere(Earth_Size * 1.5), //Uranus
    Sphere((Earth_Size * 1.5) - 0.02), //Neptune
    Sphere(0.20f), //Sky Box
    Sphere((Earth_Size * 0.5) - 0.02) //Space Ship
};

GLuint Program, Vao, Vbos[Number_of_Spheres], Ibos[Number_of_Spheres], Flags[Number_of_Spheres] = {}, vPosition, vNormal, vTexture, Projection_Attrib,
Color_Attrib, Theta, Translation_Attrib, Inverse_Translation_Attrib, Model_Attrib, View_Attrib, Textures[Number_of_Spheres];

glm::mat4 Translate_Matrix, Inverse_Translate_Matrix;

GLfloat Thetas[Number_of_Spheres][3] = {};


GLfloat Speed[Number_of_Spheres] = { 
    0.5, // Sun
    2, //Mercury
    1.8, //Venus
    1.6, //Earth
    1.4, //Mars
    1.2, //Jupiter
    1.0, //Saturn
    0.8, //Uranus
    0.6, //Neptune
    0.0, //Sky Box
    0.0 //Space Ship
};

glm::mat4 Model, View, Projection;
glm::vec3 Camera_Pos, Camera_Target, Camera_Direction, Camera_Right, Camera_Up, Camera_Front, Camera_Pos2, Camera_Front2, Camera_Up2;

point3 Vertices[4] = {
 point3(-50, -1, -50),
 point3(-50, -1, 50),
 point3(50, -1, 50),
 point3(50, -1, -50),
};

point3 points[6] = {
Vertices[1], Vertices[0], Vertices[3], Vertices[1], Vertices[3], Vertices[2]
};

color4 Vertex_Colors[Number_of_Spheres] = {
    color4(0.4, 0.2, 0.0, 1.0),  // Sun (yellow-orange)
    color4(0.3, 0.15, 0.0, 1.0),  // Mercury (reddish-brown)
    color4(0.4, 0.4, 0.4, 1.0),  // Venus (gray)
    color4(0.05, 0.05, 0.2, 1.0),  // Earth (blue)
    color4(0.4, 0.2, 0.0, 1.0),  // Mars (reddish-brown)
    color4(0.05, 0.05, 0.1, 1.0),  // Jupiter (pale yellow)
    color4(0.15, 0.15, 0.15, 1.0),  // Saturn (pale gold)
    color4(0.05, 0.1, 0.2, 1.0),  // Uranus (pale blue)
    color4(0.2, 0.35, 0.5, 1.0),   // Neptune (deep blue)
    color4(0.0, 0.0, 0.0, 1.0),   // sky Box (Blue)
    color4(1.0, 0.0, 0.0, 1.0)   // Space Ship (Red)
};


int Width, Height, Channels;

unsigned char* Data_1 = stbi_load("Images/Sun.jpg", &Width, &Height, &Channels, 0),
* Data_2 = stbi_load("Images/Mercury.jpg", &Width, &Height, &Channels, 0), 
* Data_3 = stbi_load("Images/Venus.jpg", &Width, &Height, &Channels, 0),
* Data_4 = stbi_load("Images/Earth.jpg", &Width, &Height, &Channels, 0),
* Data_5 = stbi_load("Images/Mars.jpg", &Width, &Height, &Channels, 0),
* Data_6 = stbi_load("Images/Jupiter.jpg", &Width, &Height, &Channels, 0),
* Data_7 = stbi_load("Images/Saturn.jpg", &Width, &Height, &Channels, 0),
* Data_8 = stbi_load("Images/Uranus.jpg", &Width, &Height, &Channels, 0),
* Data_9 = stbi_load("Images/Neptune.jpg", &Width, &Height, &Channels, 0),
*Data_10 = stbi_load("Images/Stars.jpg", &Width, &Height, &Channels, 0),
*Data_11 = stbi_load("Images/Neptune.jpg", &Width, &Height, &Channels, 0);


unsigned char* Data_Array[] = { Data_1, Data_2, Data_3, Data_4, Data_5, Data_6, Data_7, Data_8, Data_9, Data_10, Data_11 };

void initLight()
{
    point4 Light_Direction_1(-1.0, 0.0, 0.0, 0.0);
    point4 Light_Direction_2(1.0, 0.0, 0.0, 0.0);


    color4 Light_Ambient(0.5f, 0.5f, 0.5f, 1.0);
    color4 Light_Diffuse(1.0, 1.0, 0.0, 1.0);
    color4 light_specular(1.0, 1.0, 1.0, 1.0);

    color4 Material_Ambient(0.5, 0.5, 0.5, 1.0);
    color4 Material_Diffuse(1.0, 1.0, 1.0, 1.0);
    color4 Material_Specular(1.0, 1.0, 1.0, 1.0);

    float Material_Shininess = 100.0;

    glUniform4fv(glGetUniformLocation(Program, "lights[0].ambient"), 1, Light_Ambient);
    glUniform4fv(glGetUniformLocation(Program, "lights[0].diffuse"), 1, Light_Diffuse);
    glUniform4fv(glGetUniformLocation(Program, "lights[0].specular"), 1, light_specular);
    glUniform4fv(glGetUniformLocation(Program, "lights[0].direction"), 1, Light_Direction_1);

    glUniform4fv(glGetUniformLocation(Program, "lights[1].ambient"), 1, Light_Ambient);
    glUniform4fv(glGetUniformLocation(Program, "lights[1].diffuse"), 1, Light_Diffuse);
    glUniform4fv(glGetUniformLocation(Program, "lights[1].specular"), 1, light_specular);
    glUniform4fv(glGetUniformLocation(Program, "lights[1].direction"), 1, Light_Direction_2);

    glUniform4fv(glGetUniformLocation(Program, "material.ambient"), 1, Material_Ambient);
    glUniform4fv(glGetUniformLocation(Program, "material.diffuse"), 1, Material_Diffuse);
    glUniform4fv(glGetUniformLocation(Program, "material.specular"), 1, Material_Specular);
    glUniform1f(glGetUniformLocation(Program, "material.shininess"), Material_Shininess);
}

void initTexture()
{
    stbi_set_flip_vertically_on_load(true);

    for (int i = 0; i < Number_of_Spheres; i++)
    {
        glGenTextures(1, &Textures[i]);
        glBindTexture(GL_TEXTURE_2D, Textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        

        if (Data_Array[i])
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data_Array[i]);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(Data_Array[i]);
    }
}

void init()
{
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    for (int i = 0; i < Number_of_Spheres; i++)
    {
        glGenBuffers(1, &Vbos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, Vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, Spheres[i].getInterleavedVertexSize(), Spheres[i].getInterleavedVertices(), GL_STATIC_DRAW);

        glGenBuffers(1, &Ibos[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibos[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Spheres[i].getIndexSize(), Spheres[i].getIndices(), GL_STATIC_DRAW);
    }

    Program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(Program);

    vPosition = glGetAttribLocation(Program, "vertexPosition");
    glEnableVertexAttribArray(vPosition);

    vNormal = glGetAttribLocation(Program, "vertexNormal");
    glEnableVertexAttribArray(vNormal);

    vTexture = glGetAttribLocation(Program, "vertexTexture");
    glEnableVertexAttribArray(vTexture);

    Translation_Attrib = glGetUniformLocation(Program, "Translate_Matrix");
    Inverse_Translation_Attrib = glGetUniformLocation(Program, "Inverse_Translate_Matrix");

    Color_Attrib = glGetUniformLocation(Program, "color");
    Theta = glGetUniformLocation(Program, "theta");

    Model_Attrib = glGetUniformLocation(Program, "model");
    View_Attrib = glGetUniformLocation(Program, "view");
    Projection_Attrib = glGetUniformLocation(Program, "projection");
    Color_Attrib = glGetUniformLocation(Program, "color");
    Camera_Pos = glm::vec3(0.0f, 0.0f, 2.0f);
    Camera_Target = glm::vec3(0.0f, 0.0f, 0.0f);
    Camera_Direction = glm::normalize(Camera_Pos - Camera_Target);
    Camera_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Direction));
    Camera_Up = glm::cross(Camera_Direction, Camera_Right);
    Camera_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.0f);
    glUniformMatrix4fv(Projection_Attrib, 1, GL_FALSE, glm::value_ptr(Projection));

    Camera_Pos2 = Camera_Pos;
    Camera_Front2 = Camera_Front;
    Camera_Up2 = Camera_Up;

    initLight();
    initTexture();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < Number_of_Spheres; i++)
    {
        glUniform3fv(Theta, 1, Thetas[i]);

        if (i == Number_of_Spheres - 2)
        {
            View = glm::lookAt(Camera_Pos2, Camera_Pos2 + Camera_Front2, Camera_Up2);
            Translate_Matrix = glm::mat4(1.0f);
            Translate_Matrix = glm::translate(Translate_Matrix, glm::vec3(0, 0, 0));
            Translate_Matrix = glm::scale(Translate_Matrix, glm::vec3(100, 100, 100));

            Inverse_Translate_Matrix = glm::mat4(1.0f);
            Inverse_Translate_Matrix = glm::translate(Inverse_Translate_Matrix, glm::vec3(0, 0, 0));
        }
        else if (i == Number_of_Spheres - 1)
        {
            View = glm::lookAt(Camera_Pos2, Camera_Pos2 + Camera_Front2, Camera_Up2);
            Translate_Matrix = glm::mat4(1.0f);
            Translate_Matrix = glm::translate(Translate_Matrix, glm::vec3(0, 0, -1));
            Translate_Matrix = glm::scale(Translate_Matrix, glm::vec3(10, 10, 60));

            Inverse_Translate_Matrix = glm::mat4(1.0f);
            Inverse_Translate_Matrix = glm::translate(Inverse_Translate_Matrix, glm::vec3(0, 0, 0));
        }
        else
        {
            Translate_Matrix = glm::mat4(1.0f);
            Translate_Matrix = glm::translate(Translate_Matrix, glm::vec3(0, 0, 0));

            Inverse_Translate_Matrix = glm::mat4(1.0f);
            Inverse_Translate_Matrix = glm::translate(Inverse_Translate_Matrix, glm::vec3(0, 0, i * 1.1));
            View = glm::lookAt(Camera_Pos, Camera_Pos + Camera_Front, Camera_Up);
        }

        glUniformMatrix4fv(View_Attrib, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(Translation_Attrib, 1, GL_FALSE, glm::value_ptr(Translate_Matrix));
        glUniformMatrix4fv(Inverse_Translation_Attrib, 1, GL_FALSE, glm::value_ptr(Inverse_Translate_Matrix));

        glBindBuffer(GL_ARRAY_BUFFER, Vbos[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibos[i]);
        glBindTexture(GL_TEXTURE_2D, Textures[i]);

        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(0, -1.5f, -4));
        glUniformMatrix4fv(Model_Attrib, 1, GL_FALSE, glm::value_ptr(Model));

        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, Spheres[i].getInterleavedStride(), BUFFER_OFFSET(0));
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, false, Spheres[i].getInterleavedStride(), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(vTexture, 2, GL_FLOAT, false, Spheres[i].getInterleavedStride(), (void*)(6 * sizeof(float)));

        glUniform4fv(Color_Attrib, 1, Vertex_Colors[i]);

        glDrawElements(GL_TRIANGLES, Spheres[i].getIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }

    glutSwapBuffers();
}

void idle()
{
    for (int i = 0; i < Number_of_Spheres; i++)
    {
        Thetas[i][1] += Speed[i];

        if (Thetas[i][1] > 360.0)
        {
            Thetas[i][1] -= 360;
        }
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 033:
        exit(EXIT_SUCCESS);
        break;
    case 'r':
        Camera_Pos += Camera_Speed * Camera_Up;
        break;
    case 'f':
        Camera_Pos -= Camera_Speed * Camera_Up;
        break;
    case 'w':
        Camera_Pos += Camera_Speed * Camera_Front;
        break;
    case 's':
        Camera_Pos -= Camera_Speed * Camera_Front;
        break;
    case 'a':
        Camera_Pos -= glm::normalize(glm::cross(Camera_Front, Camera_Up)) *
            Camera_Speed;
        break;
    case 'd':
        Camera_Pos += glm::normalize(glm::cross(Camera_Front, Camera_Up)) *
            Camera_Speed;
        break;
    case 'q':
        yaw -= 1.0f;
        Camera_Direction.x = glm::cos(glm::radians(yaw));
        Camera_Direction.z = glm::sin(glm::radians(yaw));
        Camera_Front = glm::normalize(Camera_Direction);
        break;
    case 'e':
        yaw += 1.0f;
        Camera_Direction.x = glm::cos(glm::radians(yaw));
        Camera_Direction.z = glm::sin(glm::radians(yaw));
        Camera_Front = glm::normalize(Camera_Direction);
        break;
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Title");

    glewInit();

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
