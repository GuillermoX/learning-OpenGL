//Importante que GLAD se importe antes que otras librerias que usan funciones de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Función callback para redimensionar la pantalla que se usa (e implementa) más adelante
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Función que encapsula el procesamiento del input de teclado
void processInput(GLFWwindow* window);

//Versión cutre de guardar un shader para evitar tener que implementar la lectura
//de un archivo
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main()
{
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);      //Versión de OpenGL v3.*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);      //Versión concreta v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Crear la ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "HelloTriangle", NULL, NULL);
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


//---- CREACIÓN DE LOS OBJETOS SHADERS Y COMPILACIÓN ----

    //Crear un objeto shader
    unsigned int vertexShader;
    //Aqui se crea así el objeto ya que no tiene que especializarse luego con Bind como
    //en el buffer anterior VBO
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Asociar el codigo del shader al objeto shader
    //(el segundo param es el numero de strings que pasamos como codigo fuente)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);  //Compilar el shader (lenguaje GLSL)
    //Comprobar si la compilación fue exitosa
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED:\n" << infoLog << "\n";
    }
    else
        std::cout << "SHADER::VERTEX::COMPILATION_SUCCEED\n";


    //Crear y compilar el fragment shader al igual que con el vertex shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED:\n" << infoLog << "\n";
    }
    else
        std::cout << "SHADER::FRAGMENT::COMPILATION_SUCCEED\n";


//---- LINKADO DE LOS SHADERS EN UN SHADER PROGRAM ----

    //Creación del objeto Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //Añadir los shaders compilados al shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //Comprobar que se ha linkado bien
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED:\n" << infoLog << "\n";
    }
    else
        std::cout << "SHADER_PROGRAM::LINKING_SUCEED:\n";

    //Activar el Shader Program para que a partir de ahora cualquier llamada de
    //renderizado o shading se haga siguiendo los shaders programados
    //glUseProgram(shaderProgram);  //Comentado ya que se llamará cuando se quiera renderizar

    //Eliminar los objetos de los shaders una vez se han linkado al Programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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
    float vertices[] = {        //OpenGL solo dibuja coordenadas que salgan del vertex
        -0.5f, -0.5f, 0.0f,     //shader que estén entre -1.0 y 1.0 pero como nuestro
        0.5f, -0.5f, 0.0f,      //vertex shader no hace nada entonces ya introducimos
        0.0f, 0.5f, 0.0f        //las coordenadas normalizadas (NDC Normalized Device Coords)

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  //Se activa el atributo del vertice (pasando el identificador especificado en el shader)
  glEnableVertexAttribArray(0);



//Se podrían configurar otros VAO con más VBO y configuraciones de atributos ya que es
//en el momento de renderizar donde especificamos que VAO queremos pintar


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

        glUseProgram(shaderProgram);
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
}