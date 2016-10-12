

#ifdef WIN32
#include <windows.h>
#endif

#include <GLUT/GLUT.h>
#include <math.h>

// Defines maximum stack capacity.
#define STACK_CAP 16
// Defines pi for converting angles.
#define PI 3.14159265359

// Structure for the matrix stack, top specifies current top position on the stack, initially zero (which means one matrix in the stack)
struct matrix_stack
{
    GLdouble m[STACK_CAP][16];
    int top;
};

// Define a macro for retrieving current matrix from top of current stack.
#define current_matrix (current_stack->m[current_stack->top])

// Identity matrix constant.
const GLdouble identity[16] =
{1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1};

// the model view matrix stack.
struct matrix_stack model_view = {{{0}}, 0};
// the projection matrix stack.
struct matrix_stack projection = {{{0}}, 0};
// The current stack pointer that specifies the matrix mode.
struct matrix_stack *current_stack = &model_view;

//HELPER FUNCTIONS
GLdouble deg_to_rad(GLdouble degrees) {
    return (degrees*PI/180.0);
}


// Multiplies current matrix with matrix b, put the result in current matrix.
// current = current * b
void matrix_multiply(const GLdouble *b)
{
    GLdouble total, current, b_curr = 0.0;
    int j, count = 0;
    //first loop to iterate through current_matrix by row
    for (int i = 0; i < STACK_CAP; ++i) {
        current = current_matrix[i];
        //second loop to iterate through b by column
        for (j; j < STACK_CAP; j += 4) {
            b_curr = b[j];
        }
        total += current * b_curr;
        if (i != 0 && i % 4 == 0) {
            current_matrix[count++] = total;
            total = 0;
        }
        j++;
    }
    
}

// Calculates cross product of b and c, put the result in a
// a = b x c
void cross_product(GLdouble *ax, GLdouble *ay, GLdouble *az,
    GLdouble bx, GLdouble by, GLdouble bz,
    GLdouble cx, GLdouble cy, GLdouble cz)
{
    *ax = (by*cz - bz*cy);
    *ay = (bz*cx - bx*cz);
    *az = (bx*cy - by*cx);
}

// Normalizes vector (x, y, z).
void normalize(GLdouble *x, GLdouble *y, GLdouble *z)
{
    GLdouble magnitude = sqrt((*x)*(*x) + (*y)*(*y) + (*z)*(*z)); //calculate magnitude of vector (x, y, z)
    //do normalization
    *x = (*x)/magnitude;
    *y = (*y)/magnitude;
    *z = (*z)/magnitude;
}

// Switches matrix mode by changing the current stack pointer.
void I_my_glMatrixMode(GLenum mode)
{
    if (mode == GL_MODELVIEW) {
        current_stack = &model_view;
    } else if (mode == GL_PROJECTION) {
        current_stack = &projection;
    }
}

// Overwrites current matrix with identity matrix.
void I_my_glLoadIdentity(void)
{
    //TEST THIS (current_matrix is 2d 16x16?)
    for (int i = 0; i < STACK_CAP; ++i) {
        current_matrix[i] = identity[i];
    }
}

// Pushes current matrix onto current stack, report error if the stack is already full.
void I_my_glPushMatrix(void)
{
    if (current_stack->top == 15) {
        throw GL_STACK_OVERFLOW;
    } else {
        current_stack->top++;
        for (int i = 0; i < STACK_CAP; ++i) {
            current_stack->m[current_stack->top][i] = current_matrix[i];
        }
    }
}

// Pops current matrix from current stack, report error if the stack has only one matrix left.
void I_my_glPopMatrix(void)
{
    if (current_stack->top == 0) {
        throw GL_STACK_UNDERFLOW;
    } else {
        current_stack->top--;
    }
}

// Overwrites currentmatrix with m.
void I_my_glLoadMatrixf(const GLfloat *m)
{
    //CHECK: do we need to switch matrix modes depending pn float/double?
    for (int i = 0; i < STACK_CAP; ++i) {
        current_matrix[i] = (GLdouble)m[i];
    }
}

void I_my_glLoadMatrixd(const GLdouble *m)
{
    for (int i = 0; i < STACK_CAP; ++i) {
        current_matrix[i] = m[i];
    }
}

void I_my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
    //Constructs the translation matrix
    GLdouble translate[16];
    GLdouble t_vector[3] = {x, y, z};
    int count = 0; //to increment the translate vector array
    for (int i = 0; i < STACK_CAP; ++i) {
        translate[i] = identity[i];
        if (i-3%4 == 0 && count <= 3) { //CHECK if it iterates through last column of translate matrix
            translate[i] = t_vector[count++];
        }
    }
    
    //multiplies current_matrix with translation matrix
    matrix_multiply(translate);
}

void I_my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}



// Remember to normalize vector (x, y, z), and to convert angle from degree to radius before calling sin and cos.
void I_my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    normalize(&x, &y, &z);
    angle = deg_to_rad(angle);
    
}

void I_my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void I_my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
    
}

void I_my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glScaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// Copies current matrix to m.
void I_my_glGetMatrixf(GLfloat *m)
{
    // ...
}

void I_my_glGetMatrixd(GLdouble *m)
{
    // ...
}

// Remember to normalize vectors.
void I_my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
    GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    // ...
}

void I_my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
    GLdouble top, GLdouble zNear, GLdouble zFar)
{
    // ...
}

// Based on the inputs, calculate left, right, bottom, top, and call I_my_glFrustum accordingly
// remember to convert fovy from degree to radius before calling tan
void I_my_gluPerspective(GLdouble fovy, GLdouble aspect, 
    GLdouble zNear, GLdouble zFar)
{
    // ...
}


