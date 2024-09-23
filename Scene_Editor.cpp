#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

// For convenience
using json = nlohmann::json;

class Cube {
private:
    float posX, posY, posZ; // Position of the cube
    float rotX, rotY, rotZ; // Rotation angles of the cube
    float size;             // Size of the cube
    float width, height, length; // Dimensions of the cube
    std::string name; // Name of the cube


public:
    // constructor for the cube
    Cube(float x = 0.0f, float y = 0.0f, float z = -5.0f, 
         float w = 1.0f, float h = 1.0f, float l = 1.0f)
        : posX(x), posY(y), posZ(z), rotX(0.0f), rotY(0.0f), rotZ(0.0f), 
          width(w), height(h), length(l) {}
    
    // Parameterized constructor (takes position, rotation, and dimensions)
    Cube(float x, float y, float z, 
         float rx, float ry, float rz, 
         float w, float h, float l, const std::string& cubeName) 
        : posX(x), posY(y), posZ(z), // Set position
          rotX(rx), rotY(ry), rotZ(rz), // Set rotation
          width(w), height(h), length(l), // Set dimensions
          name(cubeName) // set name
    {}

    // Methods to modify the cube's position and rotation
    void setPosition(float x, float y, float z) {
        posX = x; posY = y; posZ = z;
    }

    // Method to modify the cube's position and rotation
    void setDimmensions(float w, float h, float l) {
        width = w; height = h; length = l;
    }

    void log() {
        std::cout << posX << " " << posY << " " << posZ << std::endl;
        std::cout << rotX << " " << rotY << " " << rotZ << std::endl;
        std::cout << width << " " << height << " " << length << std::endl;
    }

    std::array<float, 3> getPosition() {
        return {posX, posY, posZ};
    }

    std::array<float, 3> getRotation() {
        return {rotX, rotY, rotZ};
    }

    std::array<float, 3> getDimmensions() {
        return {width, height, length};
    }

    std::string getName(){
        return name;
    }

    void setRotation(float x, float y, float z) {
        rotX = x; rotY = y; rotZ = z;
    }

    void translate(float dx, float dy, float dz) {
        posX += dx; posY += dy; posZ += dz;
    }

    void rotate(float dx, float dy, float dz) {
        rotX += dx; rotY += dy; rotZ += dz;
    }
    
    void resize(float dl, float dh, float dw)
    {
        width += dl; height += dh; length += dw;
    }


    // Method to draw the cube
    void draw() {
        glPushMatrix(); // Save current transformation state
        glTranslatef(posX, posY, posZ);  // Move cube to its position
        glRotatef(rotX, 1.0f, 0.0f, 0.0f); // Rotate cube around X axis
        glRotatef(rotY, 0.0f, 1.0f, 0.0f); // Rotate cube around Y axis
        glRotatef(rotZ, 0.0f, 0.0f, 1.0f); // Rotate cube around Z axis
        glScalef(width, height, length);      // Scale the cube
        
        // Draw a simple cube using quads
        glBegin(GL_QUADS);
    
        // Front face (z = 1.0)
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        // Back face (z = -1.0)
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Left face (x = -1.0)
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        // Right face (x = 1.0)
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Top face (y = 1.0)
        glColor3f(0.0f, 1.0f, 1.0f); // Cyan
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        // Bottom face (y = -1.0)
        glColor3f(1.0f, 0.0f, 1.0f); // Magenta
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);

        glEnd();

        glPopMatrix(); // Restore transformation state
    }
};

int currObject;
json jsonData;
Cube* objectToMove;
std::vector<Cube*> cubes;

void init() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (Cube* cube : cubes) {
        cube->draw();
    }
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            objectToMove->rotate(0.0f, 5.0f, 0.0f);
            break;
        case GLUT_KEY_LEFT:
            objectToMove->rotate(0.0f, -5.0f, 0.0f);
            break;
        case GLUT_KEY_UP:
            objectToMove->rotate(-5.0f, 0.0f, 0.0f);
            break;
        case GLUT_KEY_DOWN:
            objectToMove->rotate(5.0f, 0.0f, 0.0f);
            break;
            
    }
    glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            objectToMove->translate(0.0f, 0.0f, 0.1f);
            break;
        case 's':
            objectToMove->translate(0.0f, 0.0f, -0.1f);
            break;
        case 'a':
            objectToMove->translate(-0.1f, 0.0f, 0.0f);
            break;
        case 'd':
            objectToMove->translate(0.1f, 0.0f, 0.0f);
            break;
        case 'q':
            objectToMove->translate(0.0f, 0.1f, 0.0f);
            break;
        case 'e':
            objectToMove->translate(0.0f, -0.1f, 0.0f);
            break;
        case 'l':
            objectToMove->log();
            break;
        
        case '1':
            objectToMove->resize(0.05f, 0.0f, 0.0f);
            break;
        
        case '2': 
            objectToMove->resize(-0.05f, 0.0f, 0.0f);
            break;
        
        case '3':
            objectToMove->resize(0.0f, 0.05f, 0.0f);
            break;
        
        case '4':
            objectToMove->resize(0.0f, -0.05f, 0.0f);
            break;
        
        case '5':
            objectToMove->resize(0.0f, 0.0f, 0.05f);
            break;
        
        case '6':
            objectToMove->resize(0.0f, 0.0f, -0.1f);
            break;
        
        case '/':
            if (currObject < cubes.size() - 1) {
                currObject++;
            } else
            {
                currObject = 0;
            }
            objectToMove = cubes[currObject];
            break;

        case ';':
        {
            std::string cubeName = "Cube" + std::to_string(cubes.size());
            cubes.emplace_back(new Cube(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, cubeName));
            break;
        }

        case '.':
            for (Cube* cube : cubes) {
                jsonData[cube->getName()]["Type"] = "Cube";
                jsonData[cube->getName()]["Position"] = cube->getPosition();
                jsonData[cube->getName()]["Rotation"] = cube->getRotation();
                jsonData[cube->getName()]["Dimensions"] = cube->getDimmensions();
            }
            std::string jsonString = jsonData.dump(4);  // 4 spaces for indentation
            std::cout << "JSON Output:\n" << jsonString << std::endl;

            std::ofstream file("sceneObjects.json");
            if (file.is_open()) {
                file << jsonString;
                file.close();
                std::cout << "JSON file saved successfully." << std::endl;
            } 
            else {
                std::cerr << "Failed to open JSON file for writing." << std::endl;
            }
            break;  // Ensure break here
    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    currObject = 0;

    // Load the JSON data from a file
    std::ifstream file("sceneObjects.json");
    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }

    json jsonData;
    file >> jsonData;
    file.close();

    // Iterate through each object, create the objects and add them to the cubes vector
    for (const auto& [key, value] : jsonData.items()) 
    {
        std::cout << "Object: " << key << "\n";

        // Access individual properties
        std::string type = value["Type"];
        auto position = value["Position"];
        auto rotation = value["Rotation"];
        auto dimensions = value["Dimensions"];

        // Create the object
        if (type == "Cube") {
            float x = position[0];
            float y = position[1];
            float z = position[2];
            float rx = rotation[0];
            float ry = rotation[1];
            float rz = rotation[2];
            float w = dimensions[0];
            float h = dimensions[1];
            float l = dimensions[2];
            cubes.push_back(new Cube(x, y, z, rx, ry, rz, w, h, l, key));
        }
    }

    // set object to move
    objectToMove = cubes[currObject];

    // Create window and start rednering loop
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube with Class");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);  // Register special keys handler
    glutKeyboardFunc(normalKeys);  // Register normal keys handler

    glutMainLoop();
    return 0;
}
