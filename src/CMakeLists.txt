add_subdirectory(core)
add_subdirectory(ast)

addExecutable(main main.cpp
  cidmolCore
  cidmolAst
)