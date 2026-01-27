cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto (esto es interno de CMake)
project(HelloWindow)

# -----------------------------------------------------------
# 1. DEFINICIÓN DE VARIABLES
# -----------------------------------------------------------

# Definimos la ruta a la carpeta común
set(COMMON_DIR "${CMAKE_SOURCE_DIR}/../Common")

set(CMAKE_CXX_STANDARD 17)

# -----------------------------------------------------------
# 2. DEPENDENCIAS
# -----------------------------------------------------------
find_package(glfw3 3.3 REQUIRED)

include_directories(${COMMON_DIR}/include)

# -----------------------------------------------------------
# 3. CREACIÓN DEL EJECUTABLE
# -----------------------------------------------------------
# Usamos la variable ${EXE_NAME} en lugar de escribir el texto a mano
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    "${COMMON_DIR}/src/glad.c"
)

# -----------------------------------------------------------
# 4. LINKADO
# -----------------------------------------------------------
# También usamos la variable aquí para enlazar las librerías al target correcto
target_link_libraries(${PROJECT_NAME} glfw GL dl)
