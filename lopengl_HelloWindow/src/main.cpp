//Importante que GLAD se importe antes que otras librerias que usan funciones de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Función callback para redimensionar la pantalla que se usa (e implementa) más adelante
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Función que encapsula el procesamiento del input de teclado
void processInput(GLFWwindow* window);

int main()
{
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);      //Versión de OpenGL v3.*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);      //Versión concreta v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Crear la ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWindow", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);     //Indicar que el contexto de la ventana
                                        //es el contexto del thread actual


    /*Inicializar GLAD antes de llamar a cualquier función de OpenGL:
        - Le pasamos a GLAD la función que carga las direcciones de los punteros
            a las funciones de OpenGL del Sistema Operativo específico.
        - GLFW nos proporciona una función específica para el SO en el que compilamos. 
    */
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Indicar el ViewPort a OpenGL
    glViewport(0, 0, 800, 600);     /* - Los dos primeros params indican la ubicación
                                         de la esquina inferior izquierda que se consi-
                                         derará.
                                       - Los dos últimos params son el tamaño de la
                                         pantalla que coincide con los indicados al
                                         crear la ventana con GLFW 

                                    !!Importante: Las coordenadas en OpenGL van de
                                     (-1 a 1) = (0, 800) en este caso. */


    /*Registrar la función de redimensionamiento para que GLFW la llame cada vez
      que se redimensiona la ventana */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //Entrar en el bucle de renderizado (cada iteración del bucle es un frame)
    while(!glfwWindowShouldClose(window))
    {
        //Función propia que encapsula el procesamiento del teclado y acciones de este
        processInput(window);

        //Funciones de renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);       //State-Setting function de OpenGL
        glClear(GL_COLOR_BUFFER_BIT);               /*State-Using function de OpenGL:
                                                        Llena el color buffer del color
                                                        configurado en glClearColor*/

        glfwSwapBuffers(window);    /* Intercambia los buffers 2D del back por el front
                                        (la pantalla solo muestra los pixeles del front)*/
        glfwPollEvents();           /*Chequea si ha ocurrido algun evento (teclado,
                                      raton, redimensionamiento) y llama a la función
                                      asignada al evento (como en nuestro caso para
                                      el redimensionamiento)*/
    }

    //Eliminar los recursos de GLFW
    glfwTerminate();
    return 0;



}


/*Implementación función callback que se llama por GLFW cada vez que se
  redimensiona la pantalla */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


//Función que encapsula el procesamiento del input de teclado
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}