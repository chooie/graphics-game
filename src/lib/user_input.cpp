/*void handle_user_input(
  float cam_pos[], double& elapsed_seconds,
  float& cam_speed, float& cam_yaw, float& cam_yaw_speed,
  int view_mat_location
) {
  // control keys
  bool cam_moved = false;
  if (glfwGetKey (g_window, GLFW_KEY_A)) {
    cam_pos[0] -= cam_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_D)) {
    cam_pos[0] += cam_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_PAGE_UP)) {
    cam_pos[1] += cam_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_PAGE_DOWN)) {
    cam_pos[1] -= cam_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_W)) {
    cam_pos[2] -= cam_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_S)) {
    cam_pos[2] += cam_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
    cam_yaw += cam_yaw_speed * elapsed_seconds;
    cam_moved = true;
  }
  if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
    cam_yaw -= cam_yaw_speed * elapsed_seconds;
    cam_moved = true;
  }
  // update view matrix
  if (cam_moved) {
    // cam translation
    mat4 T = translate (
      identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])
    );
    mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw); //
    mat4 view_mat = R * T;
    glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
  }


  if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose (g_window, 1);
  }
}*/

void handle_user_input(
  Camera &cam, float elapsed_seconds, int view_mat_location
) {
  // control keys
	bool cam_moved = false;
	vec3 move (0.0, 0.0, 0.0);
	float cam_yaw = 0.0f; // y-rotation in degrees
	float cam_pitch = 0.0f;
	float cam_roll = 0.0;
	if (glfwGetKey (g_window, GLFW_KEY_A)) {
		move.v[0] -= cam.cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey (g_window, GLFW_KEY_D)) {
		move.v[0] += cam.cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey (g_window, GLFW_KEY_Q)) {
		move.v[1] += cam.cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey (g_window, GLFW_KEY_E)) {
		move.v[1] -= cam.cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey (g_window, GLFW_KEY_W)) {
		move.v[2] -= cam.cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey (g_window, GLFW_KEY_S)) {
		move.v[2] += cam.cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
		cam_yaw += cam.cam_heading_speed * elapsed_seconds;
		cam_moved = true;

		// create a quaternion representing change in heading (the yaw)
		float q_yaw[16];
		create_versor (q_yaw, cam_yaw, cam.up.v[0], cam.up.v[1], cam.up.v[2]);
		// add yaw rotation to the camera's current orientation
		mult_quat_quat (cam.quaternion, q_yaw, cam.quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4 (cam.R.m, cam.quaternion);
		cam.fwd = cam.R * vec4 (0.0, 0.0, -1.0, 0.0);
		cam.rgt = cam.R * vec4 (1.0, 0.0, 0.0, 0.0);
		cam.up = cam.R * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
		cam_yaw -= cam.cam_heading_speed * elapsed_seconds;
		cam_moved = true;
		float q_yaw[16];
		create_versor (q_yaw, cam_yaw, cam.up.v[0], cam.up.v[1], cam.up.v[2]);
		mult_quat_quat (cam.quaternion, q_yaw, cam.quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4 (cam.R.m, cam.quaternion);
		cam.fwd = cam.R * vec4 (0.0, 0.0, -1.0, 0.0);
		cam.rgt = cam.R * vec4 (1.0, 0.0, 0.0, 0.0);
		cam.up = cam.R * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (g_window, GLFW_KEY_UP)) {
		cam_pitch += cam.cam_heading_speed * elapsed_seconds;
		cam_moved = true;
		float q_pitch[16];
		create_versor (
      q_pitch, cam_pitch, cam.rgt.v[0], cam.rgt.v[1], cam.rgt.v[2]
    );
		mult_quat_quat (cam.quaternion, q_pitch, cam.quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4 (cam.R.m, cam.quaternion);
		cam.fwd = cam.R * vec4 (0.0, 0.0, -1.0, 0.0);
		cam.rgt = cam.R * vec4 (1.0, 0.0, 0.0, 0.0);
		cam.up = cam.R * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (g_window, GLFW_KEY_DOWN)) {
		cam_pitch -= cam.cam_heading_speed * elapsed_seconds;
		cam_moved = true;
		float q_pitch[16];
		create_versor (
      q_pitch, cam_pitch, cam.rgt.v[0], cam.rgt.v[1], cam.rgt.v[2]
    );
		mult_quat_quat (cam.quaternion, q_pitch, cam.quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4 (cam.R.m, cam.quaternion);
		cam.fwd = cam.R * vec4 (0.0, 0.0, -1.0, 0.0);
		cam.rgt = cam.R * vec4 (1.0, 0.0, 0.0, 0.0);
		cam.up = cam.R * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (g_window, GLFW_KEY_Z)) {
		cam_roll -= cam.cam_heading_speed * elapsed_seconds;
		cam_moved = true;
		float q_roll[16];
		create_versor (q_roll, cam_roll, cam.fwd.v[0], cam.fwd.v[1], cam.fwd.v[2]);
		mult_quat_quat (cam.quaternion, q_roll, cam.quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4 (cam.R.m, cam.quaternion);
		cam.fwd = cam.R * vec4 (0.0, 0.0, -1.0, 0.0);
		cam.rgt = cam.R * vec4 (1.0, 0.0, 0.0, 0.0);
		cam.up = cam.R * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (g_window, GLFW_KEY_C)) {
		cam_roll += cam.cam_heading_speed * elapsed_seconds;
		cam_moved = true;
		float q_roll[16];
		create_versor (q_roll, cam_roll, cam.fwd.v[0], cam.fwd.v[1], cam.fwd.v[2]);
		mult_quat_quat (cam.quaternion, q_roll, cam.quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4 (cam.R.m, cam.quaternion);
		cam.fwd = cam.R * vec4 (0.0, 0.0, -1.0, 0.0);
		cam.rgt = cam.R * vec4 (1.0, 0.0, 0.0, 0.0);
		cam.up = cam.R * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	// update view matrix
	if (cam_moved) {
		quat_to_mat4 (cam.R.m, cam.quaternion);

		// checking for fp errors
		//	printf ("dot fwd . up %f\n", dot (fwd, up));
		//	printf ("dot rgt . up %f\n", dot (rgt, up));
		//	printf ("dot fwd . rgt\n %f", dot (fwd, rgt));

		cam.cam_pos += vec3 (cam.fwd) * -move.v[2];
		cam.cam_pos += vec3 (cam.up) * move.v[1];
		cam.cam_pos += vec3 (cam.rgt) * move.v[0];
		mat4 T = translate (identity_mat4 (), vec3 (cam.cam_pos));

		cam.view_mat = inverse (cam.R) * inverse (T);
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, cam.view_mat.m);
	}


	if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose (g_window, 1);
	}
}
