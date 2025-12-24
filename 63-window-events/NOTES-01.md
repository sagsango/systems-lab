# Xwayland is a modern display-server which replaced old X11 server
Your App
  |
  | XCopyArea()
  v
Xwayland (X server)
  |
  | Wayland surface
  v
Mutter compositor
  |
  | GPU composition (EGL / OpenGL)
  v
Kernel DRM (i915 / amdgpu / nvidia)
  |
  v
Display


