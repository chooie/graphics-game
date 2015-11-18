#ifndef CAMERA_H
#define CAMERA_H

#include "../maths_funcs.h"
#include <GLFW/glfw3.h> // GLFW helper library
#include "versor.h"

extern int g_gl_width;
extern int g_gl_height;

class Camera {
	private:
		static const vec4 FORWARD;
		static const vec4 RIGHT;
		static const vec4 UP;
		void recalc_axes();
		int view_mat_location;
		int proj_mat_location;
		mat4 view_mat;
		mat4 proj_mat;
		vec3 cam_pos;
		// input variables
		float near; // clipping plane
		float far; // clipping plane
		float fovy; // 67 degrees
		float aspect; // aspect ratio

		float cam_speed; // 1 unit per second
		float cam_heading_speed; // 30 degrees per second
		float cam_heading; // y-rotation in degrees
		mat4 T;
		// rotation matrix from my maths library. just holds 16 floats
		mat4 R;
		// make a quaternion representing negated initial camera orientation
		float quaternion[4];
		// keep track of some useful vectors that can be used for keyboard movement
		vec4 fwd;
		vec4 rgt;
		vec4 up;

		// control keys
		bool cam_moved;
		vec3 move;
		float cam_yaw; // y-rotation in degrees
		float cam_pitch;
		float cam_roll;
	public:
		Camera(int view_mat_location, int proj_mat_location);
		void update();
		void set_view();
		void set_proj();
		void reset_control();
		void strafe_left(double elapsed_seconds);
		void strafe_right(double elapsed_seconds);
		void strafe_down(double elapsed_seconds);
		void strafe_up(double elapsed_seconds);
		void strafe_backwards(double elapsed_seconds);
		void strafe_forwards(double elapsed_seconds);
		void yaw_left(double elapsed_seconds);
		void yaw_right(double elapsed_seconds);
		void pitch_down(double elapsed_seconds);
		void pitch_up(double elapsed_seconds);
		void roll_left(double elapsed_seconds);
		void roll_right(double elapsed_seconds);
		mat4 get_view_mat();
		mat4 get_proj_mat();
};
#endif
