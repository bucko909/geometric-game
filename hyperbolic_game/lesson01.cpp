#include <stdlib.h>    // For malloc() etc.
#include <stdio.h>     // For printf(), fopen() etc.
#include <math.h>      // For sin(), cos() etc.
#include <GL/glfw.h>   // For GLFW, OpenGL and GLU
#include <iostream>
#include "point.hpp"
#include "hyperbolic.hpp"
#include "textfile.h"

using namespace std;
using namespace bucko;


//----------------------------------------------------------------------
// Draw() - Main OpenGL drawing function that is called each frame
//----------------------------------------------------------------------

HyperbolicPlane3 game_floor = HyperbolicPlane3(HyperbolicPoint3(0,0,0), 3);
HyperbolicPoint3 eye = HyperbolicPoint3(0,0,4);;
Vector3 look = Vector3(1,0,0); // Direction we're looking in
float updown = 0; // Angle up/down we're looking
float leftright = 0; // Angle left/right we're looking
Vector3 forward = Vector3(1,0,0); // Direction we consider "forward"
Vector3 up = Vector3(0,0,1);
Vector3 velocity = Vector3(0,0,0);
float accel = 0.0003;

GLuint hyp_vertex_shader_program;

#define PI 3.141592654

float a=0;

void Draw( void )
{
    int    width, height;  // Window dimensions

    // Get window size
    glfwGetWindowSize( &width, &height );

    // Make sure that height is non-zero to avoid division by zero
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    height = height < 1 ? 1 : height;

    // Set viewport
    glViewport( 0, 0, width, height );
	gluPerspective(45, 1.3333, 0.00001, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // Clear color and depht buffers
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	gluLookAt(eye, look, up);
	glBegin(GL_QUAD_STRIP);
	int i;
	for(i=1;i<=1000;i++) {
		glColor3f(0,1.0-0.001*i,0.001*i);
		glVertex3f(eye, 5*i/300.0*(sin(PI*i/20.0)),5*i/300.0*cos(PI*i/20.0),0.1+i/100.0);
		glVertex3f(eye, 6*i/300.0*(sin(PI*i/20.0)),6*i/300.0*cos(PI*i/20.0),0.1+i/100.0);
	}
	glEnd();

/*	glBegin(GL_QUAD_STRIP);
	for(i=1;i<=1000;i++) {
		glColor3f(0,1.0-0.001*i,0.001*i);
		glVertex3f_hyperbolic(eye, Point3(5*i/300.0*(sin(PI*i/20.0)),5*i/300.0*cos(PI*i/20.0),0.1+i/100.0));
		glVertex3f_hyperbolic(eye, Point3(6*i/300.0*(sin(PI*i/20.0)),6*i/300.0*cos(PI*i/20.0),0.1+i/100.0));
	}
	glEnd();*/
}


//----------------------------------------------------------------------
// main() - Program entry point
//----------------------------------------------------------------------

int main( int argc, char **argv )
{
    int    ok;             // Flag telling if the window was opened
    int    running;        // Flag telling if the program is running

    // Initialize GLFW
    glfwInit();

    // Open window
    ok = glfwOpenWindow(
        640, 480,          // Width and height of window
        8, 8, 8,           // Number of red, green, and blue bits for color buffer
        8,                 // Number of bits for alpha buffer
        24,                // Number of bits for depth buffer (Z-buffer)
        0,                 // Number of bits for stencil buffer
        GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
    );

    // If we could not open a window, exit now
    if( !ok )
    {
        glfwTerminate();
        return 0;
    }

    // Set window title
    glfwSetWindowTitle( "My OpenGL program" );

    // Enable sticky keys
    glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable( GLFW_MOUSE_CURSOR );
	glEnable(GL_DEPTH_TEST);

	glfwSetMousePos(0, 0);
    // Main rendering loop
	int i=0;
	double time_last;
    do
    {
		
        // Call our rendering function
        Draw();

        // Swap front and back buffers (we use a double buffered display)
        glfwSwapBuffers();

    	// Get current time
	    double time = glfwGetTime();
		double time_step = time - time_last;

		// Calculate facing vector
		// First, we need to know where down is
		Vector3 down = eye.directionTo(game_floor);
		printf("Down: (%f, %f, %f)\n", down.x, down.y, down.z);
		
		// Assuming left is always projected to a vector parallel to (0, -1, 0).
		Vector3 projected_left = Vector3(0, -1, 0);

		// Therefore forwards is the cross product of this and down. This should
		// face in the positive x direction.
		Vector3 unrotate_forward = ~(projected_left * down);
		//cout << "Unrotated forward vector: " << unrotate_forward << endl;
		//printf("Unrotate_forward: (%f, %f, %f)\n", unrotate_forward.x, unrotate_forward.y, unrotate_forward.z);
		
		// Now we must rotate so that forwards points in the correct direction.
		// Note that down does not point up (duh!), so we need to negate the
		// clockwise rotation.
		forward = unrotate_forward.rotateClockwise(down, -leftright);
		printf("Forward: (%f, %f, %f)\n", forward.x, forward.y, forward.z);

		// Finally, rotate into the look direction so that we are looking
		// up/down correctly. Start by getting back our concept of "left".
		Vector3 normal = down * forward;
		printf("Normal: (%f, %f, %f)\n", normal.x, normal.y, normal.z);
		look = forward.rotateClockwise(normal, updown);
		printf("Look: (%f, %f, %f)\n", look.x, look.y, look.z);
		printf("Eye: (%f, %f, %f)\n", eye.x, eye.y, eye.z);
		up = -down;
		//down = Vector3(0,0,-1);

		/*
		int i;
		for(i=0; i<1000; i++) {
			HyperbolicPoint3 a = HyperbolicPoint3(5*i/300.0, 5*i/300.0, 0.1+i/100.0);
			cout << "From " << a << ", to " << HyperbolicPoint3(0,0,4).directionTo(a) << " to " << a.directionTo(game_floor) << "." << endl;
		}// */
		
        // Check if the escape key was pressed, or if the window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
                  glfwGetWindowParam( GLFW_OPENED );

		// Find the downwards vector
		Point3 oldeye = eye;
		float length = 0.01;
		if (!(eye.z > 0.001))
			running = 0;

		// For now, we attract to the bottom of the floor, too.
		if (down.length() < 0.000099999) {
			// We dropped through, or lie on, the floor.
			printf("Dropped through the floor! (dist is %f)\n", down.length());
			eye = eye.addVector(down, 1.0 - down.length());
			velocity = Vector3(0,0,0);
			down = ~down;
		}
		if (down.length() < 1.001) {
			// Consider ourselves on the ground.
			if (glfwGetKey( GLFW_KEY_SPACE )) {
				velocity -= ~down * 0.01f;
				eye = eye.addVector(velocity);
			}
		} else {
			// Fix sticky keys.
			glfwGetKey( GLFW_KEY_SPACE );
//			velocity += ~down * accel;
//			if (velocity.length() > 0.001) eye = eye.addVector(velocity);
		}
		printf("Velocity: (%f, %f, %f)\n", velocity.x, velocity.y, velocity.z);
		printf("Eye2: (%f, %f, %f)\n", eye.x, eye.y, eye.z);
		if (glfwGetKey('W')) {
			eye = eye.addVector(forward, time_step);
		}
		if (glfwGetKey('S')) {
			eye = eye.addVector(forward, -time_step);
		}
		if (glfwGetKey('D')) {
			// TODO temporary hack
			Vector3 right = ~(down * forward);
			eye = eye.addVector(right, time_step);
			forward = ~(right * down);
		}
		if (glfwGetKey('A')) {
			// TODO temporary hack
			Vector3 right = ~(down * forward);
			eye = eye.addVector(right, -time_step);
			forward = ~(right * down);
		}
		printf("Eye3: (%f, %f, %f)\n", eye.x, eye.y, eye.z);
		int x, y;
		glfwGetMousePos( &x, &y );
		if (x != 0 || y != 0) {
			leftright += x * 0.01;
			if (leftright > 2 * PI) leftright -= 2 * PI;
			if (leftright < -2 * PI) leftright += 2 * PI;
			updown -= y * 0.01;
			if (updown > PI / 2 - 0.0001) updown = PI / 2 - 0.0001;
			if (updown < -PI / 2 + 0.0001) updown = -PI / 2 + 0.0001;
			glfwSetMousePos( x/2, y/2 );
		}
		time_last = time;
    }
    while( running );

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    return 0;
}
