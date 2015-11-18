#include "Camera.h"
#include "versor.h"

const vec4 Camera::FORWARD = vec4(0.0f, 0.0f, -1.0f, 0.0f);
const vec4 Camera::RIGHT = vec4(1.0f, 0.0f, 0.0f, 0.0f);
const vec4 Camera::UP = vec4(0.0f, 1.0f, 0.0f, 0.0f);

Camera::Camera(int view_mat_location, int proj_mat_location) {
	this->view_mat_location = view_mat_location;
	this->proj_mat_location = proj_mat_location;
	cam_pos = vec3 (0.0f, 0.0f, 5.0f);
	near = 0.1f; // clipping plane
	far = 100.0f; // clipping plane
	fovy = 67.0f; // 67 degrees
	// aspect ratio
	aspect = (float)g_gl_width / (float)g_gl_height;
	proj_mat = perspective (fovy, aspect, near, far);
	cam_speed = 5.0f; // 1 unit per second
	cam_heading_speed = 100.0f; // 30 degrees per second
	cam_heading = 0.0f; // y-rotation in degrees
	T = translate (
		identity_mat4 (), vec3 (-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2])
	);
	create_versor (quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
	// convert the quaternion to a rotation matrix (just an array of 16 floats)
	quat_to_mat4 (R.m, quaternion);
	// combine the inverse rotation and transformation to make a view matrix
	view_mat = R * T;

	fwd = FORWARD;
	rgt = RIGHT;
	up = UP;

	set_view();
	set_proj();

	reset_control();
}

void Camera::recalc_axes() {
	quat_to_mat4 (R.m, quaternion);
	fwd = R * FORWARD;
	rgt = R * RIGHT;
	up = R * UP;
}

void Camera::update() {

	// update view matrix
	if (cam_moved) {
		quat_to_mat4 (R.m, quaternion);

		// checking for fp errors
		//	printf ("dot fwd . up %f\n", dot (fwd, up));
		//	printf ("dot rgt . up %f\n", dot (rgt, up));
		//	printf ("dot fwd . rgt\n %f", dot (fwd, rgt));

		cam_pos += vec3 (fwd) * -move.v[2];
		cam_pos += vec3 (up) * move.v[1];
		cam_pos += vec3 (rgt) * move.v[0];
		T = translate (identity_mat4 (), vec3 (cam_pos));

		view_mat = inverse (R) * inverse (T);
		set_view();
	}
}


void Camera::set_view() {
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
}

void Camera::set_proj() {
	// Maybe here add some camera controls
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat.m);
}

void Camera::reset_control() {
	// control keys
	cam_moved = false;
	move = vec3 (0.0, 0.0, 0.0);
	cam_yaw = 0.0f; // y-rotation in degrees
	cam_pitch = 0.0f;
	cam_roll = 0.0;
}

void Camera::strafe_left(double elapsed_seconds) {
	move.v[0] -= cam_speed * elapsed_seconds;
	cam_moved = true;
}

void Camera::strafe_right(double elapsed_seconds) {
	move.v[0] += cam_speed * elapsed_seconds;
	cam_moved = true;
}

void Camera::strafe_down(double elapsed_seconds) {
	move.v[1] -= cam_speed * elapsed_seconds;
	cam_moved = true;
}

void Camera::strafe_up(double elapsed_seconds) {
	move.v[1] += cam_speed * elapsed_seconds;
	cam_moved = true;
}

void Camera::strafe_backwards(double elapsed_seconds) {
	move.v[2] -= cam_speed * elapsed_seconds;
	cam_moved = true;
}

void Camera::strafe_forwards(double elapsed_seconds) {
	move.v[2] += cam_speed * elapsed_seconds;
	cam_moved = true;
}

void Camera::yaw_left(double elapsed_seconds) {
	cam_yaw += cam_heading_speed * elapsed_seconds;
	cam_moved = true;

	// create a quaternion representing change in heading (the yaw)
	float q_yaw[16];
	create_versor (q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
	// add yaw rotation to the camera's current orientation
	mult_quat_quat (quaternion, q_yaw, quaternion);

	// recalc axes to suit new orientation
	recalc_axes();
}

void Camera::yaw_right(double elapsed_seconds) {
	cam_yaw -= cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_yaw[16];
	create_versor (q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
	mult_quat_quat (quaternion, q_yaw, quaternion);

	// recalc axes to suit new orientation
	recalc_axes();
}

void Camera::pitch_down(double elapsed_seconds) {
	cam_pitch -= cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_pitch[16];
	create_versor (
		q_pitch, cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]
	);
	mult_quat_quat (quaternion, q_pitch, quaternion);

	// recalc axes to suit new orientation
	recalc_axes();
}

void Camera::pitch_up(double elapsed_seconds) {
	cam_pitch += cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_pitch[16];
	create_versor (
		q_pitch, cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]
	);
	mult_quat_quat (quaternion, q_pitch, quaternion);

	// recalc axes to suit new orientation
	recalc_axes();
}

void Camera::roll_left(double elapsed_seconds) {
	cam_roll -= cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_roll[16];
	create_versor (q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	mult_quat_quat (quaternion, q_roll, quaternion);

	// recalc axes to suit new orientation
	recalc_axes();
}

void Camera::roll_right(double elapsed_seconds) {
	cam_roll += cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_roll[16];
	create_versor (q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	mult_quat_quat (quaternion, q_roll, quaternion);

	// recalc axes to suit new orientation
	recalc_axes();
}

mat4 Camera::get_view_mat() {
	return view_mat;
}

mat4 Camera::get_proj_mat() {
	return proj_mat;
}
