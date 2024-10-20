#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <SOIL/SOIL.h>

GLfloat lightPos[] = {10.7f, -1.3f, -7.1f, 1.0f}; 
GLfloat lightPos0[] = {-2.7f, -1.7f, 3.5f, 1.0f};

GLuint textureFloor, textureWood, textureFridge, textureLaptop, textureTV, textureScreen, textureMattress, texturePaper, textureConsole;

GLuint depthMapFBO; // Framebuffer for shadow mapping
GLuint depthMap; // Texture to store the depth map
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

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
    GLuint textureID;


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
         int light, GLuint texID) 
        : posX(x), posY(y), posZ(z), // Set position
          rotX(rx), rotY(ry), rotZ(rz), // Set rotation
          width(w), height(h), length(l), // Set dimensions
          redVal(red/255), greenVal(green/255), blueVal(blue/255), // Set color
          name(cubeName),
          lightsource(light), // set name
          textureID(texID)
    {}

    void setTextureID(GLuint texID) {
        textureID = texID;
    }

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


    void draw() {
        // Set material properties for shading (only if light source is enabled)
        if (lightsource > 0) {
            GLfloat material_diffuse[] = {redVal, greenVal, blueVal, 1.0};  // Diffuse color
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            GLfloat shininess[] = {100.0};  // Shininess coefficient
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        }

        glPushMatrix();  // Save current transformation state
        glTranslatef(posX, posY, posZ);  // Move cube to its position
        glRotatef(rotX, 1.0f, 0.0f, 0.0f);  // Rotate cube around X axis
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);  // Rotate cube around Y axis
        glRotatef(rotZ, 0.0f, 0.0f, 1.0f);  // Rotate cube around Z axis
        glScalef(width, height, length);  // Scale the cube

        // Enable or disable texturing based on textureID
        if (textureID != 0) {
            glEnable(GL_TEXTURE_2D);  // Enable 2D texturing
            glBindTexture(GL_TEXTURE_2D, textureID);
        } else {
            glDisable(GL_TEXTURE_2D);  // Disable texturing and use color
            glColor3f(redVal, greenVal, blueVal);  // Use cube's color
        }

        glBegin(GL_QUADS);

        // Front face
        glNormal3f(0.0f, 0.0f, 1.0f);  // Normal facing forward
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        // Back face
        glNormal3f(0.0f, 0.0f, -1.0f);  // Normal facing backward
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

        // Left face
        glNormal3f(-1.0f, 0.0f, 0.0f);  // Normal facing left
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);  // Normal facing right
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

        // Top face
        glNormal3f(0.0f, 1.0f, 0.0f);  // Normal facing up
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        // Bottom face
        glNormal3f(0.0f, -1.0f, 0.0f);  // Normal facing down
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

        glEnd();

        if (textureID != 0) {
            glDisable(GL_TEXTURE_2D);  // Disable texturing after drawing if it was enabled
        }

        glPopMatrix();  // Restore the previous transformation state
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
    GLuint textureID;  // Texture ID for applying textures


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
             float red, float green, float blue, int light, GLuint texID)
        : posX(x), posY(y), posZ(z), 
          rotX(rx), rotY(ry), rotZ(rz), 
          radius(r), height(h), 
          redVal(red / 255), greenVal(green / 255), blueVal(blue / 255), 
          name(cylinderName), lightsource(light), textureID(texID) {}

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

        // Enable or disable texturing based on textureID
        if (textureID != 0) {
            glEnable(GL_TEXTURE_2D);  // Enable 2D texturing
            glBindTexture(GL_TEXTURE_2D, textureID);
        } else {
            glDisable(GL_TEXTURE_2D);  // Disable texturing and use color
            glColor3f(redVal, greenVal, blueVal);  // Use cylinder's color
        }

        // Draw the cylinder
        GLUquadric* quadric = gluNewQuadric();
        gluQuadricNormals(quadric, GLU_SMOOTH);
        gluQuadricTexture(quadric, GL_TRUE); // Apply texture if texture is enabled

        // Draw the cylinder body
        gluCylinder(quadric, radius, radius, height, 32, 32);

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

        if (textureID != 0) {
            glDisable(GL_TEXTURE_2D);  // Disable texturing after drawing if it was enabled
        }

        glPopMatrix(); // Restore the previous transformation state
        gluDeleteQuadric(quadric); // Clean up
    }
};



int currObject;
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
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    glEnable(GL_LIGHTING);    // Enable lighting
    glEnable(GL_LIGHT0);      // Enable light 0
    glEnable(GL_LIGHT1);      // Enable light 1

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



int main(int argc, char** argv) {
    currObject = 0;

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 600);
    glutCreateWindow("Room Scene (with Textures)");

    // Load textures before creating cubes
    textureFloor = SOIL_load_OGL_texture("floor.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureWood = SOIL_load_OGL_texture("wood.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureFridge = SOIL_load_OGL_texture("fridge.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureLaptop = SOIL_load_OGL_texture("laptop.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureTV = SOIL_load_OGL_texture("tv.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureScreen = SOIL_load_OGL_texture("screen.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureMattress = SOIL_load_OGL_texture("mattress.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texturePaper = SOIL_load_OGL_texture("paper.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textureConsole = SOIL_load_OGL_texture("console.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if (!textureFloor) {
        std::cerr << "Error loading textureFloor: " << SOIL_last_result() << std::endl;
    }

    if (!textureWood) {
        std::cerr << "Error loading textureWood: " << SOIL_last_result() << std::endl;
    }

    if (!textureFridge) {
        std::cerr << "Error loading textureFridge: " << SOIL_last_result() << std::endl;
    }

    if (!textureLaptop) {
        std::cerr << "Error loading textureLaptop: " << SOIL_last_result() << std::endl;
    }

    if (!textureTV) {
        std::cerr << "Error loading textureTV: " << SOIL_last_result() << std::endl;
    }

    if (!textureScreen) {
        std::cerr << "Error loading textureScreen: " << SOIL_last_result() << std::endl;
    }

    if (!textureMattress) {
        std::cerr << "Error loading textureMattress: " << SOIL_last_result() << std::endl;
    }

    if (!texturePaper) {
        std::cerr << "Error loading texturePaper: " << SOIL_last_result() << std::endl;
    }

    if (!textureConsole) {
        std::cerr << "Error loading textureConsole: " << SOIL_last_result() << std::endl;
    }

    if (textureFloor != 0) {
        cubes.push_back(new Cube(-0.30, -4.20, -17.00, 15.00, 720.00, 0.00, 13.20, 0.25, 13.15, "Floor", 200.00, 192.00, 173.00, 0, textureFloor));
    } else {
        std::cerr << "Floor texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureWood != 0) {
        cubes.push_back(new Cube(0.20, 0.50, -11.50, 375.00, -180.00, 0.00, 3.75, 0.10, 2.50, "DeskTop", 205.00, 181.00, 150.00, 1, textureWood));
        cubes.push_back(new Cube(3.00, -0.50, -9.90, 10.00, 180.00, 0.00, 0.75, 0.25, 0.70, "DeskTopDrawer", 173.00, 107.00, 75.00, 1, textureWood));
        cubes.push_back(new Cube(3.00, -1.30, -10.10, 10.00, 180.00, 0.00, 0.75, 0.30, 0.70, "DeskMiddleDrawer", 173.00, 107.00, 75.00, 1, textureWood));
        cubes.push_back(new Cube(3.00, -2.50, -10.30, 10.00, 180.00, 0.00, 0.75, 0.65, 0.70, "DeskBottomDrawer", 173.00, 107.00, 75.00, 1, textureWood));
        cubes.push_back(new Cube(-0.80, -1.60, -16.00, -175.00, 0.00, 0.00, 3.30, 2.00, 2.25, "DeskBackLeg", 173.00, 107.00, 75.00, 1, textureWood));
        cubes.push_back(new Cube(-3.50, -1.60, -10.50, 5.00, 0.00, 0.00, 0.10, 1.65, 0.95, "DeskLeftLeg", 173.00, 107.00, 75.00, 1, textureWood));
        cubes.push_back(new Cube(3.00, -1.70, -10.50, 10.00, 0.00, 0.00, 0.95, 1.65, 0.95, "DeskRightLeg", 173.00, 107.00, 75.00, 1, textureWood));
        cubes.push_back(new Cube(3.90, 0.70, -10.10, 10.00, -10.00, 0.00, 0.05, 0.25, 4.25, "BedFrameBottomeam", 195.00, 151.00, 114.00, -1, textureWood));
        cubes.push_back(new Cube(4.30, -1.20, -10.10, 10.00, -10.00, 0.00, 0.05, 0.25, 4.25, "BedFrameMiddleBeam", 195.00, 151.00, 114.00, -1, textureWood));
        cubes.push_back(new Cube(4.20, -2.50, -10.10, 10.00, -5.00, 0.00, 0.05, 0.25, 4.25, "BedFrameTopBeam", 195.00, 151.00, 114.00, -1, textureWood));
        cubes.push_back(new Cube(4.30, -1.30, -12.90, 10.00, 0.00, 0.00, 0.25, 3.05, 0.15, "BedFramePillar", 182.00, 146.00, 112.00, 0, textureWood));
    } else {
        std::cerr << "Wood texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureFridge != 0) {
        cubes.push_back(new Cube(-8.90, 1.20, -19.70, 0.00, 0.00, 0.00, 1.00, 1.65, 1.00, "FridgeBase", 141.00, 138.00, 123.00, 0, textureFridge));
        cubes.push_back(new Cube(-9.10, 0.70, -18.80, 0.00, 175.00, 0.00, 1.20, 1.15, 0.05, "FridgeTop", 255.00, 255.00, 255.00, 0, textureFridge));
        cubes.push_back(new Cube(-9.10, 2.40, -18.80, 0.00, 175.00, 0.00, 1.20, 0.40, 0.05, "FridgeBottom", 255.00, 255.00, 255.00, 0, textureFridge));

    } else {
        std::cerr << "Fridge texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureLaptop != 0) {
        cubes.push_back(new Cube(0.20, 0.40, -10.20, 15.00, -35.00, 0.00, 1.20, 0.05, 0.95, "Laptop", 150.00, 150.00, 150.00, 0, textureLaptop));
    } else {
        std::cerr << "Laptop texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureTV != 0) {
        cylinders.push_back(new Cylinder(-1.90, 1.00, -12.80, 105.00, 0.00, 0.00, 0.95, 1.00, "TVBase", 0.0, 0.0, 0.0, 0, textureTV));
        cubes.push_back(new Cube(-2.00, 1.20, -13.00, 10.00, 15.00, 0.00, 0.15, 0.45, 0.25, "TVNeck", 255.00, 255.00, 255.00, 2, textureTV));
        cubes.push_back(new Cube(-1.70, 2.40, -12.20, 10.00, 20.00, 0.00, 2.15, 1.05, 0.05, "TVFrame", 0.00, 0.00, 0.00, 0, textureTV));
    } else {
        std::cerr << "TV texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureScreen != 0) {
        cubes.push_back(new Cube(-1.60, 2.20, -11.20, 8.00, 16.00, 0.00, 1.80, 0.8, 0.05, "TVScreen", 100.00, 100.00, 100.00, 0, textureScreen));
    } else {
        std::cerr << "Screen texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureMattress != 0) {
        cubes.push_back(new Cube(5.30, 1.20, -10.10, 15.00, 5.00, 0.00, 1.00, 0.35, 1.00, "Mattress", 255.00, 255.00, 255.00, 0, textureMattress));
    } else {
        std::cerr << "Mattress texture not loaded, skipping cube creation." << std::endl;
    }

    if (texturePaper != 0) {
        cylinders.push_back(new Cylinder(-8.20, 3.60, -19.70, 85.00, 0.00, 5.00, 0.25, 0.80, "PaperRoll", 245.0, 240.0, 221.0, 0, texturePaper));
    } else {
        std::cerr << "Paper texture not loaded, skipping cube creation." << std::endl;
    }

    if (textureConsole != 0) {
        cubes.push_back(new Cube(2.80, 0.90, -11.60, 15.00, 720.00, 0.00, 1.00, 0.25, 1.00, "Xbox", 150.00, 150.00, 150.00, 0, textureConsole));
    } else {
        std::cerr << "Console texture not loaded, skipping cube creation." << std::endl;
    }


    cubes.push_back(new Cube(1.80, 1.50, -19.50, 270.00, 180.00, 0.00, 7.35, 2.55, 6.00, "ForegroundWall", 229.00, 231.00, 222.00, 0, 0));
    cubes.push_back(new Cube(-8.60, 6.40, -18.00, 0.00, 720.00, 0.00, 1.00, 0.65, 4.70, "BackBeam", 248.00, 246.00, 212.00, 2, 0));
    cubes.push_back(new Cube(-9.40, 5.00, -24.40, 0.00, 0.00, 0.00, 2.35, 4.70, 1.05, "BackCeiling", 210.00, 205.00, 168.00, 2, 0));
    cubes.push_back(new Cube(-8.60, 2.70, -21.70, 0.00, 0.00, 0.00, 1.00, 3.10, 1.00, "BackWall", 248.00, 246.00, 212.00, 2, 0));
    cubes.push_back(new Cube(-4.10, 3.00, -12.80, 720.00, -525.00, 0.00, 0.10, 6.35, 0.05, "ForegroundWallCorner", 148.00, 150.00, 139.00, 0, 0));
    cubes.push_back(new Cube(-8.40, 2.90, -18.70, 5.00, 0.00, 0.00, 0.30, 0.10, 0.20, "Plates", 0.00, 0.00, 0.00, 0, 0));

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}