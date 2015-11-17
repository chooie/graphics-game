#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

/* convert a unit quaternion q to a 4x4 matrix m */
void quat_to_mat4 (float* m, float* q) {
	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];
	m[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	m[1] = 2.0f * x * y + 2.0f * w * z;
	m[2] = 2.0f * x * z - 2.0f * w * y;
	m[3] = 0.0f;
	m[4] = 2.0f * x * y - 2.0f * w * z;
	m[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	m[6] = 2.0f * y * z + 2.0f * w * x;
	m[7] = 0.0f;
	m[8] = 2.0f * x * z + 2.0f * w * y;
	m[9] = 2.0f * y * z - 2.0f * w * x;
	m[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

/* create a unit quaternion q from an angle in degrees a, and an axis x,y,z */
void create_versor (float* q, float a, float x, float y, float z) {
	float rad = ONE_DEG_IN_RAD * a;
	q[0] = cosf (rad / 2.0f);
	q[1] = sinf (rad / 2.0f) * x;
	q[2] = sinf (rad / 2.0f) * y;
	q[3] = sinf (rad / 2.0f) * z;
}

/* normalise a quaternion in case it got a bit mangled */
void normalise_quat (float* q) {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if (fabs (1.0f - sum) < thresh) {
		return;
	}
	float mag = sqrt (sum);
	for (int i = 0; i < 4; i++) {
		q[i] = q[i] / mag;
	}
}

/* multiply quaternions to get another one. result=R*S */
void mult_quat_quat (float* result, float* r, float* s) {
	result[0] = s[0] * r[0] - s[1] * r[1] -
		s[2] * r[2] - s[3] * r[3];
	result[1] = s[0] * r[1] + s[1] * r[0] -
		s[2] * r[3] + s[3] * r[2];
	result[2] = s[0] * r[2] + s[1] * r[3] +
		s[2] * r[0] - s[3] * r[1];
	result[3] = s[0] * r[3] - s[1] * r[2] +
		s[2] * r[1] + s[3] * r[0];
	// re-normalise in case of mangling
	normalise_quat (result);
}

class Camera {
	private:
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

Camera::Camera(int vm_location, int pm_location) {
	view_mat_location = vm_location;
	proj_mat_location = pm_location;
	cam_pos = vec3 (0.0f, 0.0f, 5.0f);
	near = 0.1f; // clipping plane
	far = 100.0f; // clipping plane
	fovy = 67.0f; // 67 degrees
	aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
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
	// keep track of some useful vectors that can be used for keyboard movement
	fwd = vec4 (0.0f, 0.0f, -1.0f, 0.0f);
	rgt = vec4 (1.0f, 0.0f, 0.0f, 0.0f);
	up = vec4 (0.0f, 1.0f, 0.0f, 0.0f);

	set_view();
	set_proj();

	reset_control();
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
	quat_to_mat4 (R.m, quaternion);
	fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
	up = R * vec4 (0.0, 1.0, 0.0, 0.0);
}

void Camera::yaw_right(double elapsed_seconds) {
	cam_yaw -= cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_yaw[16];
	create_versor (q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
	mult_quat_quat (quaternion, q_yaw, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4 (R.m, quaternion);
	fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
	up = R * vec4 (0.0, 1.0, 0.0, 0.0);
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
	quat_to_mat4 (R.m, quaternion);
	fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
	up = R * vec4 (0.0, 1.0, 0.0, 0.0);
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
	quat_to_mat4 (R.m, quaternion);
	fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
	up = R * vec4 (0.0, 1.0, 0.0, 0.0);
}

void Camera::roll_left(double elapsed_seconds) {
	cam_roll -= cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_roll[16];
	create_versor (q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	mult_quat_quat (quaternion, q_roll, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4 (R.m, quaternion);
	fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
	up = R * vec4 (0.0, 1.0, 0.0, 0.0);
}

void Camera::roll_right(double elapsed_seconds) {
	cam_roll += cam_heading_speed * elapsed_seconds;
	cam_moved = true;
	float q_roll[16];
	create_versor (q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	mult_quat_quat (quaternion, q_roll, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4 (R.m, quaternion);
	fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
	up = R * vec4 (0.0, 1.0, 0.0, 0.0);
}

mat4 Camera::get_view_mat() {
	return view_mat;
}

mat4 Camera::get_proj_mat() {
	return proj_mat;
}
