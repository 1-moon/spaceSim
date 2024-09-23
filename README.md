# spaceSim - Coursework 2

The space simulation project for the Coursework 2.

The code is based on the OpenGL/3D exercises (G2 and forward). If necessary,
refer to the exercises for information on specific implementation..

Structure:

- assets/
  Runtime assets, such as textures, models and _shaders_. Essentially,
  anything that is loaded at runtime goes in here. Feel free to create
  additional subdirectories. Avoid loading data outside of assets/ (I might
  sandbox your application to prevent this.)

- main/
  Main project source code. This is where the bulk of your project will go.

- support/
  Support functions, as presented in the exercises. You should not change the
  code in here.

- vmlib/
  Math library. Not all of the functions are implemented yet, so you will
  need to provide some implementations yourself. You may add additional
  operators/functions as you see fit.

- third*party/
  Third party code. See third_party.md for additional information. If you add
  any third party code, you must place it here and document it in the
  third_party.md file and your report. IMPORTANT: if you want to use
  additional third party libraries, you will need to get permission from me
  first \_by email*. See Coursework 2 brief for details.

  Any custom third party libraries _must_ be built from source code using the
  premake build system (you can not use e.g. CMake or similar). That is, they
  may not add additional custom build steps that the user must perform when
  compiling your project. You must not include or rely on pre-compiled
  objects, libraries (shared or static) or executables.
