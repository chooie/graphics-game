#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

/* create a unit quaternion q from an angle in degrees a, and an axis x,y,z */
void create_versor (float* q, float a, float x, float y, float z) {
	float rad = ONE_DEG_IN_RAD * a;
	q[0] = cosf (rad / 2.0f);
	q[1] = sinf (rad / 2.0f) * x;
	q[2] = sinf (rad / 2.0f) * y;
	q[3] = sinf (rad / 2.0f) * z;
}

void create_camera() {
	// input variables
	float near = 0.1f; // clipping plane
	float far = 100.0f; // clipping plane
	float fovy = 67.0f; // 67 degrees
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	proj_mat = perspective (fovy, aspect, near, far);

	float cam_speed = 5.0f; // 1 unit per second
	float cam_heading_speed = 100.0f; // 30 degrees per second
	float cam_heading = 0.0f; // y-rotation in degrees
	mat4 T = translate (
		identity_mat4 (), vec3 (-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2])
	);
	// rotation matrix from my maths library. just holds 16 floats
	mat4 R;
	// make a quaternion representing negated initial camera orientation
	float quaternion[4];
	create_versor (quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
	// convert the quaternion to a rotation matrix (just an array of 16 floats)
	quat_to_mat4 (R.m, quaternion);
	// combine the inverse rotation and transformation to make a view matrix
	view_mat = R * T;
	// keep track of some useful vectors that can be used for keyboard movement
	vec4 fwd (0.0f, 0.0f, -1.0f, 0.0f);
	vec4 rgt (1.0f, 0.0f, 0.0f, 0.0f);
	vec4 up (0.0f, 1.0f, 0.0f, 0.0f);
}
