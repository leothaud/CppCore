add_subdirectory(compiler)
add_subdirectory(ast)
add_subdirectory(core)
add_subdirectory(gssa)

addExecutable(main main.cpp
  cidmolCore
  cidmolAst
  cidmolFrontend
  cidmolGssa
)