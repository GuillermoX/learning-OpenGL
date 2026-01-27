//Importante que GLAD se importe antes que otras librerias que usan funciones de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <shader/Shader.hpp>

//Función callback para redimensionar la pantalla que se usa (e implementa) más adelante
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Función que encapsula el procesamiento del input de teclado
void processInput(GLFWwindow* window);


float x_axis = 0.0f;
float y_axis = 0.0f;

int main()
{
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);      //Versión de OpenGL v3.*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);      //Versión concreta v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Crear la ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Shaders", NULL, NULL);
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




//---- CREACIÓN DE LOS DATOS Y GUARDADO EN VRAM (RAM DE GPU) -------

    /*Crear un VAO (Vertex Array Object) que guardará el VBO (Vertex Buffer Object) y
      toda la configuración asociada a los datos (atributos del shader, etc)*/
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    /*A partir de ahora en el VAO se guardará todo lo siguiente:
        - Llamadas a glEnableVertexAttribArray o glDisableVertexAttribArray
        - Configuraciones de atribs de vertices con glVertexAttribPointer
        - VBOs asociados a las llamadas a glVertexAttribPointer (aquellos VBO Binded
          con GL_ARRAY_BUFFER al llamar a la función de configuración de atributos de vertices)*/

    //Vertices a dibujar
    float vertices[] = {            
        //Coordinates           //Colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.0f, 
        0.0f, 0.5f, 0.0f,        1.0f, 0.0f, 1.0f

    };

    //Crear un buffer para los vertices que guarda los vertices en la memoria
    //de la GPU (Vertex Buffer Objects VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);      //Generar un objeto de tipo Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   //Asignar el nuevo buffer al VBO del contexto
    /*Copiar el vector de vertices al objeto VBO en memoria de GPU:
        - El último parámetro indica como la GPU guarda los datos:
            - GL_STATIC_DRAW: datos que NO cambian y se usan MUCHAS veces.
            - GL_STREAM_DRAW: datos que NO cambian y se usan POCAS veces.
            - GL_DYNAMIC_DRAW: datos que SÍ cambian y se usan MUCHAS veces.*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




//--- LINKADO DE LOS ATRIBUTOS DE LOS VERTICES ----
/* Ya tenemos los datos en la memoria de la GPU pero hace falta especificar
   cómo tiene que coger esos datos el vertex shader para interpretarlos como el
   input especificado.*/

   /*Especificar los atributos del vertex shader:
     Params:
       - Atributo que queremos configurar (marcado con "location = 0" en el vertex shader)
       - Número de elementos del atributo (en este caso 3 pq es un vec3 compuesto de 3 floats)
       - Tipo del atributo (float en este caso)
       - Indica si los datos tienen que estar normalizados (irrelevante por ahora)
       - STRIDE: indica bytes hasta el mismo atributo del siguiente vertice
                 (si pones 0 OpenGL asume que solo hay un atributo por vertice)
       - Offset del buffer: indica el offset donde comienzan los datos de vertices en el buffer
    
    (Los atributos de los vertices se extraen del VBO asignado al GL_ARRAY_BUFFER que hay
     en el momento de llamar a esta función)*/ 
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
  //Se activa el atributo del vertice (pasando el identificador especificado en el shader)
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);



//Se podrían configurar otros VAO con más VBO y configuraciones de atributos ya que es
//en el momento de renderizar donde especificamos que VAO queremos pintar


// Creación del SHADER PROGRAM con clase Shader propia
    Shader shaderProgram("../assets/shaders/shader.vert", "../assets/shaders/shader.frag");


//---- BUCLE DE EJECUCIÓN ----

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
        float timeValue = glfwGetTime();
        float darkness = (sin(timeValue)/2.0f) + 0.5f;
        //Ubicar la variable global "darkness" que usa el fragment shader
        int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "darkness");
        //Es necesario Usar el Shader Program para poder actualizar los valores de sus variables
        glUseProgram(shaderProgram.ID);
        //Actualizar la variable global del fragment shader
        glUniform1f(vertexColorLocation, darkness);

        int translationLocation = glGetUniformLocation(shaderProgram.ID, "translation");
        glUniform2f(translationLocation, x_axis, y_axis);

        glBindVertexArray(VAO);
        /*Dibuja los datos del Buffer VBO:
         Params:
          - Tipo de primitiva
          - Indice del primer vertice que queremos procesar
          - Numero de vertices a procesar*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        if((x_axis + 0.05f) < 0.5f) x_axis += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        if((x_axis -0.05f ) > -0.5f) x_axis -= 0.05f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if((y_axis + 0.05f) < 0.5f) y_axis += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        if((y_axis - 0.05f) > -0.5f) y_axis -= 0.05f;
}