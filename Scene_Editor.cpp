#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <array>
#include <string>

// For convenience
using json = nlohmann::json;

GLfloat lightPos[] = {10.7f, -1.3f, -7.1f, 1.0f}; 
GLfloat lightPos0[] = {-2.7f, -1.7f, 3.5f, 1.0f};


// Create a 2D array to store both light positions
GLfloat lightPositions[2][4] = {
    {-2.7f, -2.1f, 3.3f, 1.0f},  // lightPos
    {10.7f, -0.9f, -6.8f, 1.0f}   // lightPos0
};

GLfloat lightRotations[2][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}
};


int light_to_move = 0;
int moveLighting = 0;
int rotate = 0;

class Cube {
private:
    float posX, posY, posZ; // Position of the cube
    float rotX, rotY, rotZ; // Rotation angles of the cube
    float size;             // Size of the cube
    float width, height, length; // Dimensions of the cube
    float redVal, greenVal, blueVal; // Color of the cube
    int lightsource;
    std::string name; // Name of the cube


public:
    // constructor for the cube
    Cube(float x = 0.0f, float y = 0.0f, float z = -5.0f, 
         float w = 1.0f, float h = 1.0f, float l = 1.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f,
         int lighting = 0)
        : posX(x), posY(y), posZ(z), rotX(0.0f), rotY(0.0f), rotZ(0.0f), 
          width(w), height(h), length(l), redVal(red), greenVal(green), blueVal(blue), lightsource(lighting) {}
    
    // Parameterized constructor (takes position, rotation, and dimensions)
    Cube(float x, float y, float z, 
         float rx, float ry, float rz, 
         float w, float h, float l, const std::string& cubeName, float red, float green, float blue,
         int light) 
        : posX(x), posY(y), posZ(z), // Set position
          rotX(rx), rotY(ry), rotZ(rz), // Set rotation
          width(w), height(h), length(l), // Set dimensions
          redVal(red/255), greenVal(green/255), blueVal(blue/255), // Set color
          name(cubeName),
          lightsource(light) // set name
    {}

    // Methods to modify the cube's position and rotation
    void setPosition(float x, float y, float z) {
        posX = x; posY = y; posZ = z;
    }

    // getLight Source
    int getLightSource(){
        return lightsource;
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
        return {redVal * 255, greenVal * 255, blueVal * 255};
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
    if (lightsource > 0){
        // Set material properties for shading

        GLfloat material_diffuse[] = {redVal, greenVal, blueVal, 1.0}; // Diffuse color
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);

        GLfloat shininess[] = {100.0}; // Shininess coefficient
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }

    glPushMatrix(); // Save current transformation state
    glTranslatef(posX, posY, posZ);  // Move cube to its position
    glRotatef(rotX, 1.0f, 0.0f, 0.0f); // Rotate cube around X axis
    glRotatef(rotY, 0.0f, 1.0f, 0.0f); // Rotate cube around Y axis
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f); // Rotate cube around Z axis
    glScalef(width, height, length); // Scale the cube

    glBegin(GL_QUADS);

    if (lightsource == -1) {
        // Front face
        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        // Back face
        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Left face
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        // Right face
        glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Top face
        glColor3f(1.0f, 0.0f, 1.0f);  // Magenta
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        // Bottom face
        glColor3f(0.0f, 1.0f, 1.0f);  // Cyan
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
    } else {
        // Draw as before if lightsource != 0
        // Front face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        // Back face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Left face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Top face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        // Bottom face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
    }

    glEnd();
    glPopMatrix(); // Restore the previous transformation state
}

};


class Cylinder {
private:
    float posX, posY, posZ; // Position of the cylinder
    float rotX, rotY, rotZ; // Rotation angles of the cylinder
    float radius, height;   // Dimensions of the cylinder
    float redVal, greenVal, blueVal; // Color of the cylinder
    int lightsource;
    std::string name; // Name of the cylinder

public:
    // Constructor for the cylinder
    Cylinder(float x = 0.0f, float y = 0.0f, float z = -5.0f, 
             float r = 1.0f, float h = 1.0f, float red = 1.0f, 
             float green = 1.0f, float blue = 1.0f, int lighting = 0)
        : posX(x), posY(y), posZ(z), rotX(0.0f), rotY(0.0f), rotZ(0.0f), 
          radius(r), height(h), redVal(red), greenVal(green), blueVal(blue), lightsource(lighting) {}

    // Parameterized constructor (takes position, rotation, and dimensions)
    Cylinder(float x, float y, float z, 
             float rx, float ry, float rz, 
             float r, float h, const std::string& cylinderName, 
             float red, float green, float blue, int light)
        : posX(x), posY(y), posZ(z), 
          rotX(rx), rotY(ry), rotZ(rz), 
          radius(r), height(h), 
          redVal(red / 255), greenVal(green / 255), blueVal(blue / 255), 
          name(cylinderName), lightsource(light) {}

    // Methods to modify the cylinder's position and rotation
    void setPosition(float x, float y, float z) {
        posX = x; posY = y; posZ = z;
    }

    // Get light source
    int getLightSource() {
        return lightsource;
    }

    // Method to modify the cylinder's dimensions
    void setDimensions(float r, float h) {
        radius = r; height = h;
    }

    void log() {
        std::cout << posX << " " << posY << " " << posZ << std::endl;
        std::cout << rotX << " " << rotY << " " << rotZ << std::endl;
        std::cout << radius << " " << height << std::endl;
    }

    std::array<float, 3> getPosition() {
        return {posX, posY, posZ};
    }

    std::array<float, 3> getRotation() {
        return {rotX, rotY, rotZ};
    }

    std::array<float, 2> getDimensions() {
        return {radius, height};
    }

    std::array<float, 3> getColor() {
        return {redVal * 255, greenVal * 255, blueVal * 255};
    }

    std::string getName() {
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

    void resize(float dr, float dh) {
        radius += dr; height += dh;
    }

    // Method to draw the cylinder
    void draw() {
    if (lightsource > 0) {
        // Set material properties for shading
        GLfloat material_diffuse[] = {redVal, greenVal, blueVal, 1.0}; // Diffuse color
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        GLfloat shininess[] = {100.0}; // Shininess coefficient
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }

    glPushMatrix(); // Save current transformation state
    glTranslatef(posX, posY, posZ); // Move cylinder to its position
    glRotatef(rotX, 1.0f, 0.0f, 0.0f); // Rotate cylinder around X axis
    glRotatef(rotY, 0.0f, 1.0f, 0.0f); // Rotate cylinder around Y axis
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f); // Rotate cylinder around Z axis

    // Draw the cylinder
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE); // If you want to apply textures
    
    // Draw the cylinder body
    glColor3f(redVal, greenVal, blueVal);
    gluCylinder(quadric, radius, radius, height, 32, 32); // Draw the cylinder

    // Draw the top cap
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, height); // Move to the top of the cylinder
    gluDisk(quadric, 0.0f, radius, 32, 1); // Draw the top cap
    glPopMatrix();

    // Optionally, draw the bottom cap
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f); // Move to the bottom of the cylinder
    gluDisk(quadric, 0.0f, radius, 32, 1); // Draw the bottom cap
    glPopMatrix();

    glPopMatrix(); // Restore the previous transformation state
    gluDeleteQuadric(quadric); // Clean up
}

};



int currObject;
json jsonData;
Cube* objectToMove;
std::vector<Cube*> cubes;

int currCylinder;
int manipCylinders;
std::vector<Cylinder*> cylinders;
Cylinder* currentCylinder;


int change_all = 0;

void drawLightIndicator() {
    glPushMatrix();  // Save current transformation state

    // Apply the same rotation as the light
    glRotatef(lightRotations[light_to_move][0], 1.0f, 0.0f, 0.0f);  // Rotate around X
    glRotatef(lightRotations[light_to_move][1], 0.0f, 1.0f, 0.0f);  // Rotate around Y
    glRotatef(lightRotations[light_to_move][2], 0.0f, 0.0f, 1.0f);  // Rotate around Z

    // Move to the light position
    glTranslatef(lightPositions[light_to_move][0], lightPositions[light_to_move][1], lightPositions[light_to_move][2]);

    // Draw the cube, starting with different colors for each face
    glBegin(GL_QUADS);

    // Front face (direction of light)
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow to indicate light emanation
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back face
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Top face
    glColor3f(0.0f, 1.0f, 0.0f);  // Green
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Bottom face
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face
    glColor3f(1.0f, 0.5f, 0.0f);  // Orange
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left face
    glColor3f(0.5f, 0.0f, 1.0f);  // Purple
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();

    glPopMatrix();  // Restore the transformation state
}



void init() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Setup light 0
    glPushMatrix(); // Save current matrix
    glRotatef(lightRotations[0][0], 1.0f, 0.0f, 0.0f);
    glRotatef(lightRotations[0][1], 0.0f, 1.0f, 0.0f);
    glRotatef(lightRotations[0][2], 0.0f, 0.0f, 1.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositions[0]);
    glPopMatrix(); // Restore matrix

    GLfloat ambient_light[] = {0.2, 0.2, 0.2, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    GLfloat diffuse_light[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

    GLfloat specular_light[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    // Setup light 1
    glPushMatrix(); // Save current matrix
    glRotatef(lightRotations[1][0], 1.0f, 0.0f, 0.0f);
    glRotatef(lightRotations[1][1], 0.0f, 1.0f, 0.0f);
    glRotatef(lightRotations[1][2], 0.0f, 0.0f, 1.0f);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPositions[1]);
    glPopMatrix(); // Restore matrix

    GLfloat ambient_light1[] = {0.2, 0.2, 0.2, 1.0};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);

    GLfloat diffuse_light1[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);

    GLfloat specular_light1[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply rotation to the lights before rendering cubes
    // Light 0 Rotation
    glPushMatrix(); // Save the current matrix state
    glRotatef(lightRotations[0][0], 1.0f, 0.0f, 0.0f); // Rotate around X
    glRotatef(lightRotations[0][1], 0.0f, 1.0f, 0.0f); // Rotate around Y
    glRotatef(lightRotations[0][2], 0.0f, 0.0f, 1.0f); // Rotate around Z
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositions[0]); // Apply the light position after rotation
    glPopMatrix(); // Restore the previous matrix state

    // Light 1 Rotation
    glPushMatrix();
    glRotatef(lightRotations[1][0], 1.0f, 0.0f, 0.0f); // Rotate around X
    glRotatef(lightRotations[1][1], 0.0f, 1.0f, 0.0f); // Rotate around Y
    glRotatef(lightRotations[1][2], 0.0f, 0.0f, 1.0f); // Rotate around Z
    glLightfv(GL_LIGHT1, GL_POSITION, lightPositions[1]); // Apply the light position after rotation
    glPopMatrix();
    
    for (Cube* cube : cubes) {
        
        // Enable light based on the cube's light source
        if (cube->getLightSource() == 1) {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            std::cout << "Color: " 
          << (cube->getColor()[0]) << ", " 
          << (cube->getColor()[1]) << ", " 
          << (cube->getColor()[2]) << std::endl;
        } else if (cube->getLightSource() == 2) {
            glEnable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            
        } else if (cube->getLightSource() == 0) {
            // If the cube doesn't use any light, disable both lights
            // Disable lighting for this cube
            glDisable(GL_LIGHTING);
            // Set the color directly for rendering without lighting
            glColor3f(cube->getColor()[0]/255, cube->getColor()[1]/255, cube->getColor()[2]/255);
        }

        // Draw the cube
        cube->draw();
    }

    for (Cylinder* cylinder: cylinders) {
        // Enable light based on the cube's light source
        if (cylinder->getLightSource() == 1) {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            std::cout << "Color: " 
          << (cylinder->getColor()[0]) << ", " 
          << (cylinder->getColor()[1]) << ", " 
          << (cylinder->getColor()[2]) << std::endl;
        } else if (cylinder->getLightSource() == 2) {
            glEnable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            
        } else if (cylinder->getLightSource() == 0) {
            // If the cylinder doesn't use any light, disable both lights
            // Disable lighting for this cylinder
            glDisable(GL_LIGHTING);
            // Set the color directly for rendering without lighting
            glColor3f(cylinder->getColor()[0]/255, cylinder->getColor()[1]/255, cylinder->getColor()[2]/255);
        }

        // Draw the cylinder
        cylinder->draw();
    }

    // Ensure lights are disabled after drawing
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
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

            else if (moveLighting == 1) {
                lightRotations[light_to_move][0] += 5.0f;
            }
            
            else {
                objectToMove->rotate(-5.0f, 0.0f, 0.0f);
            }
            break;

        case GLUT_KEY_LEFT:
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(0.0f, -5.0f, 0.0f);
                }
            }

            else if (moveLighting == 1) {
                lightRotations[light_to_move][0] -= 5.0f;
            }

            else{
            objectToMove->rotate(5.0f, 0.0f, 0.0f);
            }
            break;

        case GLUT_KEY_UP:
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(5.0f, 0.0f, 0.0f);
                }
            }

            else if (moveLighting == 1) {
                lightRotations[light_to_move][2] += 5.0f;
            }

            else{
            objectToMove->rotate(0.0f, 0.0f, 5.0f);
            }
            break;
            
        case GLUT_KEY_DOWN:
            if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(0.0f, 5.0f, 0.0f);
                }
            }

            else if (moveLighting == 1) {
                lightRotations[light_to_move][1] -= 5.0f;
            }

            else{
            objectToMove->rotate(0.0f, 5.0f, 0.0f);
            }
            
    }
    glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'c':
            change_all = !change_all; // Toggle change_all
            break;

        case 'r':
            rotate = !rotate; // Toggle rotation mode
            std::cout << (rotate ? "Rotation mode ON" : "Rotation mode OFF") << std::endl;
            break;

        case 'w':
            if (rotate) {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->rotate(5.0f, 0.0f, 0.0f); // Rotate around the x-axis
                        }
                    } else {
                        currentCylinder->rotate(5.0f, 0.0f, 0.0f); // Rotate current cylinder around the x-axis
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->rotate(10.0f, 0.0f, 0.0f); // Rotate around the x-axis
                        }
                    } else {
                        objectToMove->rotate(5.0f, 0.0f, 0.0f); // Rotate current object around the x-axis
                    }
                }
            } else {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->translate(0.0f, 0.0f, 0.1f);
                        }
                    } else {
                        currentCylinder->translate(0.0f, 0.0f, 0.1f); // Translate current cylinder
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->translate(0.0f, 0.0f, 0.1f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][2] += 0.1f; // Move light in the z direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        objectToMove->translate(0.0f, 0.0f, 0.1f);
                    }
                }
            }
            break;

        case 's':
            if (rotate) {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->rotate(-10.0f, 0.0f, 0.0f); // Rotate around the x-axis
                        }
                    } else {
                        currentCylinder->rotate(-5.0f, 0.0f, 0.0f); // Rotate current cylinder around the x-axis
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->rotate(-10.0f, 0.0f, 0.0f); // Rotate around the x-axis
                        }
                    } else {
                        objectToMove->rotate(-5.0f, 0.0f, 0.0f); // Rotate current object around the x-axis
                    }
                }
            } else {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->translate(0.0f, 0.0f, -0.1f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][2] -= 0.1f; // Move light in the negative z direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        currentCylinder->translate(0.0f, 0.0f, -0.1f); // Translate current cylinder
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->translate(0.0f, 0.0f, -0.1f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][2] -= 0.1f; // Move light in the negative z direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        objectToMove->translate(0.0f, 0.0f, -0.1f);
                    }
                }
            }
            break;

        case 'a':
            if (rotate) {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->rotate(0.0f, 10.0f, 0.0f); // Rotate around the y-axis
                        }
                    } else {
                        currentCylinder->rotate(0.0f, 5.0f, 0.0f); // Rotate current cylinder around the y-axis
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->rotate(0.0f, 10.0f, 0.0f); // Rotate around the y-axis
                        }
                    } else {
                        objectToMove->rotate(0.0f, 5.0f, 0.0f); // Rotate current object around the y-axis
                    }
                }
            } else {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->translate(-0.1f, 0.0f, 0.0f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][0] -= 0.1f; // Move light in the negative x direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        currentCylinder->translate(-0.1f, 0.0f, 0.0f); // Translate current cylinder
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->translate(-0.1f, 0.0f, 0.0f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][0] -= 0.1f; // Move light in the negative x direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        objectToMove->translate(-0.1f, 0.0f, 0.0f);
                    }
                }
            }
            break;

        case 'd':
            if (rotate) {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->rotate(0.0f, -5.0f, 0.0f); // Rotate around the y-axis
                        }
                    } else {
                        currentCylinder->rotate(0.0f, -5.0f, 0.0f); // Rotate current cylinder around the y-axis
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->rotate(0.0f, -5.0f, 0.0f); // Rotate around the y-axis
                        }
                    } else {
                        objectToMove->rotate(0.0f, -5.0f, 0.0f); // Rotate current object around the y-axis
                    }
                }
            } else {
                if (manipCylinders && currentCylinder) {
                    if (change_all) {
                        for (Cylinder* cylinder : cylinders) {
                            cylinder->translate(0.1f, 0.0f, 0.0f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][0] += 0.1f; // Move light in the x direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        currentCylinder->translate(0.1f, 0.0f, 0.0f); // Translate current cylinder
                    }
                } else {
                    if (change_all) {
                        for (Cube* cube : cubes) {
                            cube->translate(0.1f, 0.0f, 0.0f);
                        }
                    } else if (moveLighting) {
                        lightPositions[light_to_move][0] += 0.1f; // Move light in the x direction
                        std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
                    } else {
                        objectToMove->translate(0.1f, 0.0f, 0.0f);
                    }
                }
            }
            break;

        case 'q':
            if (rotate) {
                if (manipCylinders && currentCylinder) {
                    // Rotate the current cylinder around the z-axis
                    currentCylinder->rotate(0.0f, 0.0f, 5.0f); 
            } else if (change_all) {
                for (Cube* cube : cubes) {
                    cube->rotate(0.0f, 0.0f, 10.0f); // Rotate around the z-axis
                }
            } 
            else {
            objectToMove->rotate(0.0f, 0.0f, 5.0f); // Rotate around the z-axis
            }
            }
            else {
                if (manipCylinders && currentCylinder) {
                    currentCylinder->translate(0.0f, 0.1f, 0.0f); // Translate current cylinder
        }      
                else if (change_all) {
            for (Cube* cube : cubes) {
                        cube->translate(0.0f, 0.1f, 0.0f);
                }
        } else if (moveLighting) {
            lightPositions[light_to_move][1] += 0.1f; // Move light in the y direction
            std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
        } else {
            objectToMove->translate(0.0f, 0.1f, 0.0f);
        }
    }
    break;

case 'e':
    if (rotate) {
        if (manipCylinders && currentCylinder) {
            // Rotate the current cylinder around the z-axis
            currentCylinder->rotate(0.0f, 0.0f, -5.0f);
        } else if (change_all) {
            for (Cube* cube : cubes) {
                cube->rotate(0.0f, 0.0f, -10.0f); // Rotate around the z-axis
            }
        } else {
            objectToMove->rotate(0.0f, 0.0f, -5.0f); // Rotate around the z-axis
        }
    } else {
        if (manipCylinders && currentCylinder) {
            currentCylinder->translate(0.0f, -0.1f, 0.0f); // Translate current cylinder
        } else if (change_all) {
            for (Cube* cube : cubes) {
                cube->translate(0.0f, -0.1f, 0.0f);
            }
        } else if (moveLighting) {
            lightPositions[light_to_move][1] -= 0.1f; // Move light in the negative y direction
            std::cout << "Lighting Position: (" << lightPositions[light_to_move][0] << ", " << lightPositions[light_to_move][1] << ", " << lightPositions[light_to_move][2] << ")" << std::endl;
        } else {
            objectToMove->translate(0.0f, -0.1f, 0.0f);
        }
    }
    break;


        case 'l':
            objectToMove->log();
            break;

        case 'u':
            moveLighting = !moveLighting; // Toggle moveLighting
            std::cout << (moveLighting ? "Lighting ON" : "Lighting OFF") << std::endl;
            break;

        case 'n':
            manipCylinders = !manipCylinders; // Toggle manipCylinders
            std::cout << (manipCylinders ? "Cylinders ON" : "Cylinders OFF") << std::endl;
            break;

        case '1':
            if (manipCylinders && currentCylinder) {
                currentCylinder->resize(0.05f, 0.0f);
                break;
            }
            else{
                objectToMove->resize(0.05f, 0.0f, 0.0f);
                break;
            }

        case '2':
            if (manipCylinders && currentCylinder) {
                currentCylinder->resize(-0.05f, 0.0f);
                break;
            }
            else{
                objectToMove->resize(-0.05f, 0.0f, 0.0f);
                break;
            }

        case '3':
            if (manipCylinders && currentCylinder) {
                currentCylinder->resize(0.0f, 0.05f);
                break;
            }

            else{
                objectToMove->resize(0.0f, 0.05f, 0.0f);
                break;
            }

        case '4':
            if (manipCylinders && currentCylinder) {
                currentCylinder->resize(0.0f, -0.05f);
                break;
            }

            else{
                objectToMove->resize(0.0f, -0.05f, 0.0f);
                break;
            }

        case '5':
            objectToMove->resize(0.0f, 0.0f, 0.05f);
            break;

        case '6':
            objectToMove->resize(0.0f, 0.0f, -0.1f);
            break;

        case '/':
            if (manipCylinders && currentCylinder) {
                currCylinder = (currCylinder < cylinders.size() - 1) ? currCylinder + 1 : 0; // Cycle through cylinders
                currentCylinder = cylinders[currCylinder];
                break;
            }

            else{
                currObject = (currObject < cubes.size() - 1) ? currObject + 1 : 0; // Cycle through cubes
                objectToMove = cubes[currObject];
                break;
            }

        case ';':
        {
            if (manipCylinders && currentCylinder) {
                std::string name = "Cylinder" + std::to_string(cylinders.size());
                cylinders.emplace_back(new Cylinder(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, name, 0.0, 0.0, 0.0, -1));
                break;
            }
            std::string cubeName = "Cube" + std::to_string(cubes.size());
            cubes.emplace_back(new Cube(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, cubeName, 0.0, 0.0, 0.0, -1));
            break;
        }

        case '.':

            for (Cylinder* cylinder : cylinders) {
                jsonData[cylinder->getName()]["Type"] = "Cylinder";
                jsonData[cylinder->getName()]["Position"] = cylinder->getPosition();
                jsonData[cylinder->getName()]["Rotation"] = cylinder->getRotation();
                jsonData[cylinder->getName()]["Dimensions"] = cylinder->getDimensions();
                jsonData[cylinder->getName()]["Color"] = cylinder->getColor();
                jsonData[cylinder->getName()]["LightSource"] = cylinder->getLightSource();
            }
            for (Cube* cube : cubes) {
                jsonData[cube->getName()]["Type"] = "Cube";
                jsonData[cube->getName()]["Position"] = cube->getPosition();
                jsonData[cube->getName()]["Rotation"] = cube->getRotation();
                jsonData[cube->getName()]["Dimensions"] = cube->getDimmensions();
                jsonData[cube->getName()]["Color"] = cube->getColor();
                jsonData[cube->getName()]["LightSource"] = cube->getLightSource();
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

    if (light_to_move == 0) {
        glLightfv(GL_LIGHT0, GL_POSITION, lightPositions[light_to_move]); // Update light position in OpenGL
    }

    else if (light_to_move == 1) {
        glLightfv(GL_LIGHT1, GL_POSITION, lightPositions[light_to_move]); // Update light position in OpenGL
    }
    
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
        int lightSource = value["LightSource"];

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
            cubes.push_back(new Cube(x, y, z, rx, ry, rz, w, h, l, key, red, green, blue, lightSource));
        }

        if (type == "Cylinder") {
            float x = position[0];
            float y = position[1];
            float z = position[2];
            float rx = rotation[0];
            float ry = rotation[1];
            float rz = rotation[2];
            float r = dimensions[0];
            float h = dimensions[1];
            float red = color[0];
            float green = color[1];
            float blue = color[2];
            cylinders.push_back(new Cylinder(x, y, z, rx, ry, rz, r, h, key, red, green, blue, lightSource));
        }
    }

    // set object to move
    objectToMove = cubes[currObject];
    currentCylinder = cylinders[currCylinder];

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