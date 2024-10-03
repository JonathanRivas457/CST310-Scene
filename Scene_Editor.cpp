#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

// For convenience
using json = nlohmann::json;
GLfloat lightPos[] = {0.0f, 5.0f, 5.0f, 1.0f}; // Light position
int moveLighting = 0;

class Cube {
private:
    float posX, posY, posZ; // Position of the cube
    float rotX, rotY, rotZ; // Rotation angles of the cube
    float size;             // Size of the cube
    float width, height, length; // Dimensions of the cube
    float redVal, greenVal, blueVal; // Color of the cube
    std::string name; // Name of the cube


public:
    // constructor for the cube
    Cube(float x = 0.0f, float y = 0.0f, float z = -5.0f, 
         float w = 1.0f, float h = 1.0f, float l = 1.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f)
        : posX(x), posY(y), posZ(z), rotX(0.0f), rotY(0.0f), rotZ(0.0f), 
          width(w), height(h), length(l), redVal(red), greenVal(green), blueVal(blue) {}
    
    // Parameterized constructor (takes position, rotation, and dimensions)
    Cube(float x, float y, float z, 
         float rx, float ry, float rz, 
         float w, float h, float l, const std::string& cubeName, float red, float green, float blue) 
        : posX(x), posY(y), posZ(z), // Set position
          rotX(rx), rotY(ry), rotZ(rz), // Set rotation
          width(w), height(h), length(l), // Set dimensions
          redVal(red/255), greenVal(green/255), blueVal(blue/255), // Set color
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

    std::array<float, 3> getColor() {
        return {redVal, greenVal, blueVal};
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
    // Set material properties for shading
    GLfloat material_diffuse[] = {redVal, greenVal, blueVal, 1.0}; // Diffuse color
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);

    // GLfloat material_specular[] = {1.0, 1.0, 1.0, 1.0}; // Specular color
    // glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);

    GLfloat shininess[] = {50.0}; // Shininess coefficient
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glPushMatrix(); // Save current transformation state
    glTranslatef(posX, posY, posZ);  // Move cube to its position
    glRotatef(rotX, 1.0f, 0.0f, 0.0f); // Rotate cube around X axis
    glRotatef(rotY, 0.0f, 1.0f, 0.0f); // Rotate cube around Y axis
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f); // Rotate cube around Z axis
    glScalef(width, height, length); // Scale the cube

    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal facing in the positive Z direction
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal facing in the negative Z direction
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f); // Normal facing in the negative X direction
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f); // Normal facing in the positive X direction
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Top face
    glNormal3f(0.0f, -1.0f, 0.0f); // Normal facing in the negative Y direction
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Bottom face
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal facing in the positive Y direction
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glPopMatrix(); // Restore the previous transformation state
}

};

int currObject;
json jsonData;
Cube* objectToMove;
std::vector<Cube*> cubes;
int change_all = 0;

void drawLightIndicator() {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);  // Set color for the light indicator (e.g., yellow)
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);  // Move to light position

    // Draw a small sphere
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 2.0f, 10, 10);  // Radius of 0.1, with 10 slices and stacks
    gluDeleteQuadric(quad);
    
    glPopMatrix();
}


void init() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set light properties
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat ambient_light[] = {0.2, 0.2, 0.2, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    GLfloat diffuse_light[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

    GLfloat specular_light[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (Cube* cube : cubes) {
        cube->draw();
    }

    drawLightIndicator();
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
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(0.0f, 5.0f, 0.0f);
                }
            } 
            
            else {
                objectToMove->rotate(0.0f, 5.0f, 0.0f);
            }
            break;

        case GLUT_KEY_LEFT:
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(0.0f, -5.0f, 0.0f);
                }
            }
            else{
            objectToMove->rotate(0.0f, -5.0f, 0.0f);
            }
            break;

        case GLUT_KEY_UP:
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(5.0f, 0.0f, 0.0f);
                }
            }
            else{
            objectToMove->rotate(5.0f, 0.0f, 0.0f);
            }
            break;
            
        case GLUT_KEY_DOWN:
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(-5.0f, 0.0f, 0.0f);
                }
            }
            else{
            objectToMove->rotate(-5.0f, 0.0f, 0.0f);
            }
            
    }
    glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'c':
            change_all = !change_all; // Toggle change_all
            break;

        case 'w':
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->translate(0.0f, 0.0f, 0.1f);
                }
            } else if (moveLighting) {
                lightPos[2] += 0.1f; // Move light in the z direction
                std::cout << "Lighting Position: (" << lightPos[0] << ", " << lightPos[1] << ", " << lightPos[2] << ")" << std::endl; // Print the position
            } else {
                objectToMove->translate(0.0f, 0.0f, 0.1f);
            }
            break;

        case 's':
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->translate(0.0f, 0.0f, -0.1f);
                }
            } else if (moveLighting) {
                lightPos[2] -= 0.1f; // Move light in the negative z direction
                std::cout << "Lighting Position: (" << lightPos[0] << ", " << lightPos[1] << ", " << lightPos[2] << ")" << std::endl; // Print the position
            } else {
                objectToMove->translate(0.0f, 0.0f, -0.1f);
            }
            break;

        case 'a':
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->translate(-0.1f, 0.0f, 0.0f);
                }
            } else if (moveLighting) {
                lightPos[0] -= 0.1f; // Move light in the negative x direction
                std::cout << "Lighting Position: (" << lightPos[0] << ", " << lightPos[1] << ", " << lightPos[2] << ")" << std::endl; // Print the position
            } else {
                objectToMove->translate(-0.1f, 0.0f, 0.0f);
            }
            break;

        case 'd':
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->translate(0.1f, 0.0f, 0.0f);
                }
            } else if (moveLighting) {
                lightPos[0] += 0.1f; // Move light in the x direction
                std::cout << "Lighting Position: (" << lightPos[0] << ", " << lightPos[1] << ", " << lightPos[2] << ")" << std::endl; // Print the position
            } else {
                objectToMove->translate(0.1f, 0.0f, 0.0f);
            }
            break;

        case 'q':
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->translate(0.0f, 0.1f, 0.0f);
                }
            } else if (moveLighting) {
                lightPos[1] += 0.1f; // Move light in the y direction
                std::cout << "Lighting Position: (" << lightPos[0] << ", " << lightPos[1] << ", " << lightPos[2] << ")" << std::endl; // Print the position
            } else {
                objectToMove->translate(0.0f, 0.1f, 0.0f);
            }
            break;

        case 'e':
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->translate(0.0f, -0.1f, 0.0f);
                }
            } else if (moveLighting) {
                lightPos[1] -= 0.1f; // Move light in the negative y direction
                std::cout << "Lighting Position: (" << lightPos[0] << ", " << lightPos[1] << ", " << lightPos[2] << ")" << std::endl; // Print the position
            } else {
                objectToMove->translate(0.0f, -0.1f, 0.0f);
            }
            break;

        case 'l':
            objectToMove->log();
            break;

        case 'u':
            moveLighting = !moveLighting; // Toggle moveLighting
            std::cout << (moveLighting ? "Lighting ON" : "Lighting OFF") << std::endl;
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
            currObject = (currObject < cubes.size() - 1) ? currObject + 1 : 0; // Cycle through cubes
            objectToMove = cubes[currObject];
            break;

        case ';':
        {
            std::string cubeName = "Cube" + std::to_string(cubes.size());
            cubes.emplace_back(new Cube(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, cubeName, 0.0, 0.0, 0.0));
            break;
        }

        case '.':
            for (Cube* cube : cubes) {
                jsonData[cube->getName()]["Type"] = "Cube";
                jsonData[cube->getName()]["Position"] = cube->getPosition();
                jsonData[cube->getName()]["Rotation"] = cube->getRotation();
                jsonData[cube->getName()]["Dimensions"] = cube->getDimmensions();
                jsonData[cube->getName()]["Color"] = cube->getColor();
            }
            std::string jsonString = jsonData.dump(4);  // 4 spaces for indentation
            std::cout << "JSON Output:\n" << jsonString << std::endl;

            std::ofstream file("sceneObjects.json");
            if (file.is_open()) {
                file << jsonString;
                file.close();
                std::cout << "JSON file saved successfully." << std::endl;
            } else {
                std::cerr << "Failed to open JSON file for writing." << std::endl;
            }
            break; // Ensure break here
    }
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos); // Update light position in OpenGL
    glutPostRedisplay(); // Request a redraw
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
        auto color = value["Color"];

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
            float red = color[0];
            float green = color[1];
            float blue = color[2];
            cubes.push_back(new Cube(x, y, z, rx, ry, rz, w, h, l, key, red, green, blue));
        }
    }

    // set object to move
    objectToMove = cubes[currObject];

    // Create window and start rednering loop
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 600);
    glutCreateWindow("Cube with Class");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);  // Register special keys handler
    glutKeyboardFunc(normalKeys);  // Register normal keys handler

    glutMainLoop();
    return 0;
}
