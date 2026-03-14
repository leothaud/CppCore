add_subdirectory(core)
add_subdirectory(ast)
add_subdirectory(compiler)

addExecutable(main main.cpp
  cidmolCore
  cidmolAst
  cidmolFrontend
)