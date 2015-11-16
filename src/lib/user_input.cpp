void handle_user_input(
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
}
