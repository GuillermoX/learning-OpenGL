cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto (esto es interno de CMake)
project(BasicLighting)

# -----------------------------------------------------------
# 1. DEFINICIÓN DE VARIABLES
# -----------------------------------------------------------

# Definimos la ruta a la carpeta común
set(COMMON_DIR "${CMAKE_SOURCE_DIR}/../Common")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Habilitar warnings de variables no usadas
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wunused -Wunused-variable -Wunused-parameter)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# -----------------------------------------------------------
# 2. DEPENDENCIAS
# -----------------------------------------------------------
find_package(glfw3 3.3 REQUIRED)

# -----------------------------------------------------------
# 3. CREACIÓN DEL EJECUTABLE
# -----------------------------------------------------------
# Usamos la variable ${EXE_NAME} en lugar de escribir el texto a mano
add_executable(${PROJECT_NAME} 
    src/main.cpp
    "${COMMON_DIR}/src/Shader.cpp"
    "${COMMON_DIR}/src/glad.c"
)

# Configurar los directorios de include para este target
target_include_directories(${PROJECT_NAME} PRIVATE
    "${COMMON_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include"
)

# -----------------------------------------------------------
# 4. LINKADO
# -----------------------------------------------------------
# También usamos la variable aquí para enlazar las librerías al target correcto
target_link_libraries(${PROJECT_NAME} glfw GL dl)
