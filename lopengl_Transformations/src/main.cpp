//Importante que GLAD se importe antes que otras librerias que usan funciones de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <shader/Shader.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>*/

//Función callback para redimensionar la pantalla que se usa (e implementa) más adelante
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Función que encapsula el procesamiento del input de teclado
void processInput(GLFWwindow* window);


float x_axis = 0.0f;
float y_axis = 0.0f;

float tex_interpol = 0.5f;

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
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // top left 
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


    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
  //Se activa el atributo del vertice (pasando el identificador especificado en el shader)
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


//--- TEXTURAS ----

    //Crear objeto textura
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);    //Bindeamos para configurar después textura

    /*Configurar el comportamiento de la textura en caso de salirse de que la
      coordenada se salga del rango entre 0 y 1:
        - 1er param: Tipo de textura (2D/3D)
        - 2ndo param: Coordenada a configurar de la textura -> 2D(s,t) / 3D (s,t,r)
        - 3er param: Tipo de comportamiento:
            - GL_REPEAT: Si se sale el rango se repite la textura (ignora la parte entera)
            - GL_MIRRORED_REPEAT: Lo mismo que repeat pero se invierte
            - GL_CLAMP_TO_EDGE: Se alarga la texutura con la repitiendo la última línea de pixeles
            - GL_CLAMP_TO_BORDER: Las coords fuera del rango se pintan con un color que elije el usuario*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //--- FILTRADO DE TEXTURA ----
    //(Para el caso de querer el tipo GL_CLAMP_TO_BORDER)
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    /*Configurar el tipo de filtrado de textura:
        - 1er: Tipo textura (2D/3D)
        - 2o: Segun si la imagen original se tiene que escalar más grande o más pequeña
            (magnifying -> MAG / minifying -> MIN)
        - 3er: Tipo de filtrado:
            - GL_NEAREST: Se coge el pixel (o textel) donde apunte la coordenada de textura
            - GL_LINEAR: Se hace interpolación de todos los pixeles cercanos a la coord*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //--- MIPMAPS ---
    /*Són texturas que incluyen dentro de una misma imagen versiones reducidas a la mitad
      recursivamente de una textura asi los objetos pequeños usan la versión más ligera
      y con menos pixeles ahorrando trabajo a la GPU
      (Más adelante generamos el MIPMAP)*/

    /*Substituye la configuración anterior para la reducción de textura para usar mipmap
      (IMPORTANTE): Solo usar mipmap para reducción de textura ya que con Magnificación da error
          - 3er: Tipo de interpolación
              - GL_NEAREST_MIPMAP_NEAREST: Coge el mipmap más adecuado para el tamaño de pixel
                  y usa Nearest para el pixel en si de la textura (como antes)
              - GL_LINEAR_MIPMAP_NEAREST: Mipmap más adecuado para tamaño pixel y usa
                  interpolación Linear para coger el pixel correcto.
              - GL_NEAREST_MIPMAP_LINEAR: Interpola entre los dos mipmaps mas adecuados
                  y usa Nearest para interpolación del pixel en sí.
              - GL_LINEAR_MIPMAP_LINEAR: Interpola entre dos mimaps adecuados y usa
                  interpolación Linear del pixel en sí.*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


    int width, height, nrChannels;
    /*Usamos la libreria stb_image para cargar de un formato de imagen a un buffer:
      -1er param: Path a la imagen
      -2o y 3er param: Valores de retorno ancho y algo (Se usarán para generar textura)
      -4o param: Valor del número de canales de color
    */
    unsigned char *data = stbi_load("../assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        /*Generar la textura 2D en el objeto bindeado actualemente a GL_TEXTURE_2D:
            -1: Target de la operación 
            -2: Nivel del mipmap por si quieres configurarlo manualmente
            -3: Formato de color en el que guardar la textura (RGB en este caso)
            -4,5: Dimensiones de la textura resultante (los mismos que tamaño de la imagen)
            -6: Siempre 0 (Cosas legathy)
            -7: Formato de color de la imagen original (RGB en este caso)
            -8: Tipo de datos por color de la imagen resultante (en este caso BYTES)
            -9: Buffer de los datos de la imagen ya cargada
        */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //Generar un Mipmap a partir de la imagen ya guardada en el objeto Texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //Liberamos el buffer de la imagen ya que ya está guardada en la memoria de GPU
    stbi_image_free(data);


    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    data = stbi_load("../assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data)
    {
        /*Generar la textura 2D en el objeto bindeado actualemente a GL_TEXTURE_2D:
            -1: Target de la operación 
            -2: Nivel del mipmap por si quieres configurarlo manualmente
            -3: Formato de color en el que guardar la textura (RGB en este caso)
            -4,5: Dimensiones de la textura resultante (los mismos que tamaño de la imagen)
            -6: Siempre 0 (Cosas legathy)
            -7: Formato de color de la imagen original (RGB en este caso)
            -8: Tipo de datos por color de la imagen resultante (en este caso BYTES)
            -9: Buffer de los datos de la imagen ya cargada
        */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //Generar un Mipmap a partir de la imagen ya guardada en el objeto Texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //Liberamos el buffer de la imagen ya que ya está guardada en la memoria de GPU
    stbi_image_free(data);



//--- TRANSFORMACIONES ---



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

        //Cosas personalizadas para oscuridad y movimiento
        float timeValue = glfwGetTime();
        float darkness = (sin(timeValue)/2.0f) + 0.5f;
        //Ubicar la variable global "darkness" que usa el fragment shader
        int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "darkness");
        //Es necesario Usar el Shader Program para poder actualizar los valores de sus variables
        glUseProgram(shaderProgram.ID);
        //Actualizar la variable global del fragment shader
        glUniform1f(vertexColorLocation, darkness);

        //Actualizar valor de interpolación
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "tex_interpol"), tex_interpol);

        //Indicar que texturas usar en el fragment shader
        glActiveTexture(GL_TEXTURE0);   //(Opcional apra sampler 0)
        glBindTexture(GL_TEXTURE_2D, texture);      //Asignar la textura 0 al fragment shader
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);      //Asignar la textura 1 al fragment shader
        //Indicar en el uniform del fragment shader a que textura corresponde cada sampler
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture2"), 1);


        //Matrices de transformación
        glm::mat4 trans = glm::mat4(1.0f);
        //Aplicamos la matriz rotación a la matriz identidad (ya que solo queremos la translacion)
        //Tercer param es el vector que marca el eje sobre el que rotar
        trans = glm::translate(trans, glm::vec3(x_axis, y_axis, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        unsigned int transformLocation = glGetUniformLocation(shaderProgram.ID, "transform");
        /*Pasar la matriz al vertex shader:
            - 2: Número de matrices a pasar
            - 3: Indica si queremos transposar la matriz (cambiar filas por columnas)
            - 4: La propia matriz (hace falta pasarla a un formato que OpenGL entienda)
            */
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

        //Pintado del fondo
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);               

        //Bindeo y pintado de los triangulos
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //Second container
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-x_axis, -y_axis, 0.0f));
        float scale = (sin(glfwGetTime()) + 1)/2;
        trans = glm::scale(trans, glm::vec3(scale, scale, 1.0f));
        transformLocation = glGetUniformLocation(shaderProgram.ID, "transform");
        /*Pasar la matriz al vertex shader:
            - 2: Número de matrices a pasar
            - 3: Indica si queremos transposar la matriz (cambiar filas por columnas)
            - 4: La propia matriz (hace falta pasarla a un formato que OpenGL entienda)
            */
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);    /* Intercambia los buffers 2D del back por el front
                                        (la pantalla solo muestra los pixeles del front)*/
        glfwPollEvents();           /*Chequea si ha ocurrido algun evento (teclado,
                                      raton, redimensionamiento) y llama a la función
                                      asignada al evento (como en nuestro caso para
                                      el redimensionamiento)*/
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO); 

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

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        if(tex_interpol < 1.0f) tex_interpol += 0.01f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        if(tex_interpol > 0.0f) tex_interpol -= 0.01f;
}