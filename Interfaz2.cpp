/******************* Login 1.2 By Mosse ******************/

/************************ I N F O ************************/
/*
Programa: Login 1.2
Autor: Nahuel Mosse
Fecha de creacion: ‎Lunes, ‎3 ‎de ‎mayo ‎de ‎2021
Fecha de salida 1.0: Domingo, 9 de mayo de 2021
Fecha de salida 1.1 (Bug fix): Domingo, 9 de mayo de 2021
Fecha de salida 1.2 (Mejora de lectura de codigo)
*/

/******************* L I B R E R I A S *******************/
#include <windows.h>    // Libreria que permite crear la interfaz grafica
#include <iostream>     // Libreria basica de C++
#include <string.h>     // Libreria de las funciones str
#include <fstream>      // Libreria para el manejo de .CSV

/**************** D E F I N I C I O N E S ****************/

/********************* C O N S O L A *********************/
#define _WIN32_WINNT 0x0500     // Es necesaria esta definicion para esconder ventana de consola

/********************* B O T O N E S *********************/
#define BTM_INGRESO 1
#define BTM_REGISTRO 2
#define BTM_SALIR 3
#define EDT_INGRESO_DATOS_1 4
#define EDT_INGRESO_DATOS_2 5
#define EDT_INGRESO_DATOS_3 6
#define BTM_ENVIAR 7
#define BTM_RETROCEDER 8
#define STC_TEXTOESTATICO 9

/**** D E C L A R A C I O N   D E   V A R I A B L E S ****/

/***************** E S T R U C T U R A S *****************/
struct Opciones_menu            // Botones del menu principal
{
    HWND ingresar;
    HWND registrarse;
    HWND salir;
}Menu_principal;
struct Opciones_ingresar        // Botones del menu de ingreso
{
    HWND igresar_nombre;
    HWND ingresar_password;
    HWND enviar;
    HWND retroceder;
}Menu_ingresar;
struct Opciones_registrarse     // Botones del menu de registro
{
    HWND igresar_nombre;
    HWND ingresar_password;
    HWND ingresar_password_aux;
    HWND enviar;
    HWND retroceder;
}Menu_registrarse;
struct Opciones_extras          // Botones extras
{
    HWND texto_superior;
    HWND cerrar_sesion;
    HWND texto_final_1;
    HWND texto_final_2;
    HWND texto_final_3;
    HWND texto_final_4;
    HWND texto_final_5;
    HWND texto_final_6;
    HWND texto_final_7;
    HWND texto_final_8;
}Menu_agregados;
struct info_usuario             // Variables temporales
{
    char nombre[100];
    char nombre_aux[100];
    char password[100];
    char password_aux[100];
    char nombre_password[100];
}usuario;
int ingresando_o_registrando;   // Verificador de ingreso o registro

/**** D E C L A R A C I O N   D E   F U N C I O N E S ****/

/********** F U N C I O N   D E   W I N D O W S **********/
LRESULT CALLBACK WindowProcedure (HWND ventana, UINT mensaje, WPARAM wParam, LPARAM lParam); //Creador del interior de la ventana principal

/******** F U N C I O N E S   D E   I N G R E S O ********/
int confirmar_ingreso();                    // Confirma que el nombre y la contraseña esten registrados haciendo uso de contar_filas_csv() y ingreso_info_csv_ingreso(fila)
void ingreso_info_csv_ingreso(int fila);    // Saca un conjunto nombre;contraseña de "Nombres y contraseÃ±as.csv"

/******* F U N C I O N E S   D E   R E G I S T R O *******/
int confirmar_no_repeticion();              // Confirma que el nombre ingresado sea distinto a uno ya registrado haciendo uso de contar_filas_csv() y ingreso_info_csv_registro(fila)
void ingreso_info_csv_registro(int fila);   // Saca un nombre de "Nombres y contraseÃ±as.csv"
void registro_info_csv();                   // Registra nombre y contraseña en "Nombres y contraseÃ±as.csv"

/********** F U N C I O N E S   D E   A M B O S **********/
int contar_filas_csv();     // Cuenta las filas de "Nombres y contraseÃ±as.csv"

/***** F U N C I O N   D E   W I N D O W S   M A I N *****/
int WINAPI WinMain (HINSTANCE hInstanciaAct,HINSTANCE hInstanciaPrev,LPSTR lpCmdline,int iCmdShow)  // Funcion principal(WINAPI, no es necesario, es por convencion)
{
    /**** D E C L A R A C I O N   D E   V A R I A B L E S ****/
    WNDCLASSEX clase;   // Nombre de la clase para los estilos de ventana
    HWND ventana=NULL;  // Manejador de la ventana
    MSG mensaje;        // Mensajes internos que se envian a la aplicacion

    /****** C O N F I G U R A C I O N   D E   C L A S E ******/

    /**** E S T R U C T U R A   D E   L A   V E N T A N A ****/
    clase.hInstance = hInstanciaAct;
    clase.lpfnWndProc = WindowProcedure;        // Llama a la funcion WindowProcedure
    clase.cbSize = sizeof (WNDCLASSEX);         // Tamaño de la clase de ventana
    clase.lpszClassName = "MICLASE";            // Nombre de la clase
    clase.style = CS_HREDRAW|CS_VREDRAW;        // Estilos de ventana
    clase.hbrBackground = (HBRUSH) COLOR_MENU;  // Color del fondo de la ventana
    clase.lpszMenuName = NULL;                  // Menu superior

    /******* I C O N O S , C U R S O R  y  E X T R A S *******/
    clase.hIcon = LoadIcon (NULL, IDI_APPLICATION);     // Icono del archivo.exe
    clase.hIconSm = LoadIcon (NULL, IDI_APPLICATION);   // Icono de la ventana(incluida la barra de tareas)
    clase.hCursor = LoadCursor (NULL, IDC_ARROW);       // Skin del cursor
    clase.cbClsExtra = 0;                               // Parametro extra no utilizado
    clase.cbWndExtra = 0;                               // Parametro extra no utilizado

    /*********** R E G I S T R O   D E   C L A S E ***********/
    if (!RegisterClassEx (&clase)){                         // Registra la clase y luego, verifica que haya sucedido
        MessageBox(NULL,"Error 1","Error",MB_ICONERROR);    // Ventana emergente en caso de fallar
        return EXIT_FAILURE;                                // Retorno a la funcion WinMain
    }

    /****** C R E A C I O N   D E   L A   V E N T A N A ******/
    ventana = CreateWindowEx(   // Crea la ventana
        0,                      // Parametro no utilizado
        "MICLASE",              // Clase de la ventana
        ("Login"),              // Nombre de la ventana
        WS_OVERLAPPEDWINDOW,    // Estilos de ventana
        CW_USEDEFAULT,          // Ubicacion horizontal inicial de la ventana en pixeles
        CW_USEDEFAULT,          // Ubicacion vertical inicial de la ventana en pixeles
        216,                    // Tamaño horizontal de la ventana
        224,                    // Tamaño vertical de la ventana
        HWND_DESKTOP,           // Manejador de ventana
        NULL,                   // Menu superior
        hInstanciaAct,          // Instancia de la ventana
        NULL                    // Parametro extra no utilizado
    );

    if (ventana == NULL){                                   // Verifica que la ventana se haya creado
        MessageBox(NULL,"Error 2","Error",MB_ICONERROR);    // Ventana emergente en caso de fallar
        return EXIT_FAILURE;                                // Retorno a la funcion WinMain
    }

    ShowWindow (ventana, iCmdShow);             // Muestra la ventana
    UpdateWindow (ventana);                     // Refresca la ventana
    ShowWindow(GetConsoleWindow(), SW_HIDE );   // Esconde la consola

    while (GetMessage (&mensaje, NULL, 0, 0)){  // Mantiene la ventana abierta
        TranslateMessage(&mensaje);             // Traduce mensajes virtual-key
        DispatchMessage(&mensaje);              // Envia mensajes a WindowProcedure
    }

    return mensaje.wParam;  // Permite el cierre de la ventana atravez de PostQuitMessage(0);
}

LRESULT CALLBACK WindowProcedure (HWND ventana, UINT mensaje, WPARAM wParam, LPARAM lParam)
{
    
    static HINSTANCE Instancia;
    switch (mensaje) // Manejamos los mensajes
    {
        case WM_CREATE: //Crea botones dentro de la ventana
            Instancia = ((LPCREATESTRUCT)lParam)->hInstance;

            /******** M E N U   P R I N C I P A L ********/

            if ((Menu_agregados.texto_superior=CreateWindowEx (0,"STATIC","By Mosse",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,20,160,25,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
            {
                MessageBox(ventana,"Error 3","Error",MB_OK);
                SendMessage(ventana,WM_DESTROY,0,0);
            }
            if ((Menu_principal.ingresar=CreateWindowEx (0,"BUTTON","Ingresar",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,60,160,25,ventana,(HMENU)BTM_INGRESO,Instancia,NULL))==NULL)
            {
                MessageBox(ventana,"Error 4","Error",MB_OK);
                SendMessage(ventana,WM_DESTROY,0,0);
            }
            if ((Menu_principal.registrarse=CreateWindowEx (0,"BUTTON","Registrarse",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,100,160,25,ventana,(HMENU)BTM_REGISTRO,Instancia,NULL))==NULL)
            {
                MessageBox(ventana,"Error 5","Error",MB_OK);
                SendMessage(ventana,WM_DESTROY,0,0);
            }
            if ((Menu_principal.salir=CreateWindowEx (0,"BUTTON","Salir",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,140,160,25,ventana,(HMENU)BTM_SALIR,Instancia,NULL))==NULL)
            {
                MessageBox(ventana,"Error 6","Error",MB_OK);
                SendMessage(ventana,WM_DESTROY,0,0);
            }
        break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) //Acciones de los botones
            {
                case BTM_INGRESO:
                    ingresando_o_registrando=1;

                    /********** M E N U   I N G R E S O **********/

                    DestroyWindow(Menu_principal.ingresar);
                    DestroyWindow(Menu_principal.registrarse);
                    DestroyWindow(Menu_principal.salir);
                    SetDlgItemText(ventana,STC_TEXTOESTATICO,"Ingreso"); //Setea el texto de un STATIC
                    if ((Menu_ingresar.igresar_nombre=CreateWindowEx (0,"EDIT","Nombre",ES_AUTOHSCROLL|ES_CENTER|WS_BORDER|WS_CHILD|WS_VISIBLE,20,60,160,25,ventana,(HMENU)EDT_INGRESO_DATOS_1,Instancia,NULL))==NULL) //Te permite escribir en el
                    {
                        MessageBox(ventana,"Error 7","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_ingresar.ingresar_password=CreateWindowEx (0,"EDIT","Password",ES_AUTOHSCROLL|ES_PASSWORD|ES_CENTER|WS_BORDER|WS_CHILD|WS_VISIBLE,20,100,160,25,ventana,(HMENU)EDT_INGRESO_DATOS_2,Instancia,NULL))==NULL) //Te permite escribir en el
                    {
                        MessageBox(ventana,"Error 8","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_ingresar.enviar=CreateWindowEx (0,"BUTTON","Enviar",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,110,140,70,25,ventana,(HMENU)BTM_ENVIAR,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 9","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_ingresar.retroceder=CreateWindowEx (0,"BUTTON","Retroceder",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,140,80,25,ventana,(HMENU)BTM_RETROCEDER,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 10","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                break;
                case BTM_REGISTRO:
                    ingresando_o_registrando=0;

                    /********* M E N U   R E G I S T R O *********/

                    DestroyWindow(Menu_agregados.texto_superior);
                    DestroyWindow(Menu_principal.ingresar);
                    DestroyWindow(Menu_principal.registrarse);
                    DestroyWindow(Menu_principal.salir);
                    SetDlgItemText(ventana,STC_TEXTOESTATICO,"Registro"); //Setea el texto de un STATIC
                    if ((Menu_registrarse.igresar_nombre=CreateWindowEx (0,"EDIT","Nombre",ES_AUTOHSCROLL|ES_CENTER|WS_BORDER|WS_CHILD|WS_VISIBLE,20,20,160,25,ventana,(HMENU)EDT_INGRESO_DATOS_1,Instancia,NULL))==NULL) //Te permite escribir en el
                    {
                        MessageBox(ventana,"Error 11","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_registrarse.ingresar_password=CreateWindowEx (0,"EDIT","Password",ES_AUTOHSCROLL|ES_CENTER|WS_BORDER|WS_CHILD|WS_VISIBLE,20,60,160,25,ventana,(HMENU)EDT_INGRESO_DATOS_2,Instancia,NULL))==NULL) //Te permite escribir en el
                    {
                        MessageBox(ventana,"Error 12","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_registrarse.ingresar_password_aux=CreateWindowEx (0,"EDIT","Confirmar password",ES_AUTOHSCROLL|ES_CENTER|WS_BORDER|WS_CHILD|WS_VISIBLE,20,100,160,25,ventana,(HMENU)EDT_INGRESO_DATOS_3,Instancia,NULL))==NULL) //Te permite escribir en el
                    {
                        MessageBox(ventana,"Error 13","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_registrarse.enviar=CreateWindowEx (0,"BUTTON","Enviar",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,110,140,70,25,ventana,(HMENU)BTM_ENVIAR,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 14","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_registrarse.retroceder=CreateWindowEx (0,"BUTTON","Retroceder",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,140,80,25,ventana,(HMENU)BTM_RETROCEDER,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 15","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                break;
                case BTM_SALIR:
                    SendMessage(ventana,WM_DESTROY,0,0); //destruye la ventana
                break;

                

                case BTM_ENVIAR:

                    /************ M E N U   F I N A L ************/

                    if (ingresando_o_registrando==0)
                    {
                        if (GetDlgItemText(ventana,EDT_INGRESO_DATOS_1,usuario.nombre,100)==0) //Lee el texto introducido
                        {
                            MessageBox(ventana,"Error 16","Error",MB_OK);
                        }
                        if (GetDlgItemText(ventana,EDT_INGRESO_DATOS_2,usuario.password,100)==0) //Lee el texto introducido
                        {
                            MessageBox(ventana,"Error 17","Error",MB_OK);
                        }
                        if (GetDlgItemText(ventana,EDT_INGRESO_DATOS_3,usuario.password_aux,100)==0) //Lee el texto introducido
                        {
                            MessageBox(ventana,"Error 18","Error",MB_OK);
                        }
                        if ((strcmp(usuario.password,usuario.password_aux))!=0)
                        {
                            MessageBox(ventana,"Passwords no coinciden","Registro fallo",MB_OK);
                        }
                        else if (confirmar_no_repeticion()==0)
                        {
                            MessageBox(ventana,"Nombre ya tomado","Registro fallo",MB_OK);
                        }
                        else
                        {
                            registro_info_csv(); //registra nombre y contraseña en Nombres y contraseÃ±as.csv

                            /*** M E N U   F I N A L   R E G I S T R O ***/

                            DestroyWindow(Menu_registrarse.igresar_nombre);
                            DestroyWindow(Menu_registrarse.ingresar_password);
                            DestroyWindow(Menu_registrarse.ingresar_password_aux);
                            DestroyWindow(Menu_registrarse.enviar);
                            DestroyWindow(Menu_registrarse.retroceder);
                            DestroyWindow(Menu_agregados.texto_superior);
                            if ((Menu_agregados.texto_final_1=CreateWindowEx (0,"STATIC","",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,20,160,5,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 19","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_2=CreateWindowEx (0,"STATIC","Se ha registrado",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,25,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 20","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_3=CreateWindowEx (0,"STATIC","correctamente",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,43,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 21","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_4=CreateWindowEx (0,"STATIC","",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,61,160,5,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 22","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_5=CreateWindowEx (0,"STATIC","Gracias por utilizar",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,66,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 23","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_6=CreateWindowEx (0,"STATIC","nuestro servicio",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,84,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 24","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_7=CreateWindowEx (0,"STATIC","de Login",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,102,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 25","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_8=CreateWindowEx (0,"STATIC","",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,120,160,5,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 26","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.cerrar_sesion=CreateWindowEx (0,"BUTTON","Cerrar sesion",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,140,160,25,ventana,(HMENU)BTM_RETROCEDER,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error al ejecutar","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                        }
                    }
                    else if (ingresando_o_registrando==1)
                    {
                        if (GetDlgItemText(ventana,EDT_INGRESO_DATOS_1,usuario.nombre,100)==0) //Lee el texto introducido
                        {
                            MessageBox(ventana,"Error 27","Error",MB_OK);
                        }
                        if (GetDlgItemText(ventana,EDT_INGRESO_DATOS_2,usuario.password,100)==0) //Lee el texto introducido
                        {
                            MessageBox(ventana,"Error 28","Error",MB_OK);
                        }
                        int verificador;
                        verificador=confirmar_ingreso(); //confirma que el nombre y la contraseña esten registrados
                        if (verificador==0)
                        {
                            /**** M E N U   F I N A L   I N G R E S O ****/

                            DestroyWindow(Menu_ingresar.igresar_nombre);
                            DestroyWindow(Menu_ingresar.ingresar_password);
                            DestroyWindow(Menu_ingresar.enviar);
                            DestroyWindow(Menu_ingresar.retroceder);
                            DestroyWindow(Menu_agregados.texto_superior);
                            if ((Menu_agregados.texto_final_1=CreateWindowEx (0,"STATIC","",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,20,160,5,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 29","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_2=CreateWindowEx (0,"STATIC","Ha ingresado",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,25,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 30","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_3=CreateWindowEx (0,"STATIC","correctamente",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,43,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 31","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_4=CreateWindowEx (0,"STATIC","",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,61,160,5,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 32","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_5=CreateWindowEx (0,"STATIC","Gracias por utilizar",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,66,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 33","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_6=CreateWindowEx (0,"STATIC","nuestro servicio",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,84,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 34","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_7=CreateWindowEx (0,"STATIC","de Login",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,102,160,18,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 35","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.texto_final_8=CreateWindowEx (0,"STATIC","",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,120,160,5,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error 36","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                            if ((Menu_agregados.cerrar_sesion=CreateWindowEx (0,"BUTTON","Cerrar sesion",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,140,160,25,ventana,(HMENU)BTM_RETROCEDER,Instancia,NULL))==NULL)
                            {
                                MessageBox(ventana,"Error al ejecutar","Error",MB_OK);
                                SendMessage(ventana,WM_DESTROY,0,0);
                            }
                        }
                        else
                        {
                            MessageBox(ventana,"Nombre o password incorrectos","Ingreso fallo",MB_OK);
                            SetDlgItemText(ventana,EDT_INGRESO_DATOS_1,"Nombre"); //Setea el EDIT
                            SetDlgItemText(ventana,EDT_INGRESO_DATOS_2,"Password");
                        }
                    }
                    else
                    {
                        MessageBox(ventana,"Error 37","Error",MB_OK);
                    }
                break;
                case BTM_RETROCEDER:
                    DestroyWindow(Menu_ingresar.igresar_nombre);
                    DestroyWindow(Menu_ingresar.ingresar_password);
                    DestroyWindow(Menu_ingresar.enviar);
                    DestroyWindow(Menu_ingresar.retroceder);
                    DestroyWindow(Menu_registrarse.igresar_nombre);
                    DestroyWindow(Menu_registrarse.ingresar_password);
                    DestroyWindow(Menu_registrarse.ingresar_password_aux);
                    DestroyWindow(Menu_registrarse.enviar);
                    DestroyWindow(Menu_registrarse.retroceder);
                    DestroyWindow(Menu_agregados.texto_superior);
                    DestroyWindow(Menu_agregados.texto_final_1);
                    DestroyWindow(Menu_agregados.texto_final_2);
                    DestroyWindow(Menu_agregados.texto_final_3);
                    DestroyWindow(Menu_agregados.texto_final_4);
                    DestroyWindow(Menu_agregados.texto_final_5);
                    DestroyWindow(Menu_agregados.texto_final_6);
                    DestroyWindow(Menu_agregados.texto_final_7);
                    DestroyWindow(Menu_agregados.texto_final_8);
                    DestroyWindow(Menu_agregados.cerrar_sesion);
                    if ((Menu_agregados.texto_superior=CreateWindowEx (0,"STATIC","By Mosse",SS_CENTER|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,20,20,160,25,ventana,(HMENU)STC_TEXTOESTATICO,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 3","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_principal.ingresar=CreateWindowEx (0,"BUTTON","Ingresar",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,60,160,25,ventana,(HMENU)BTM_INGRESO,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 38","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_principal.registrarse=CreateWindowEx (0,"BUTTON","Registrarse",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,100,160,25,ventana,(HMENU)BTM_REGISTRO,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 39","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                    if ((Menu_principal.salir=CreateWindowEx (0,"BUTTON","Salir",BS_PUSHBUTTON|BS_CENTER|WS_CHILD|WS_VISIBLE,20,140,160,25,ventana,(HMENU)BTM_SALIR,Instancia,NULL))==NULL)
                    {
                        MessageBox(ventana,"Error 40","Error",MB_OK);
                        SendMessage(ventana,WM_DESTROY,0,0);
                    }
                break;
            }
        break;
        case WM_CLOSE: // Que hacer en caso de recibir el mensaje WM_CLOSE
            DestroyWindow(ventana); // Destruir la ventana
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:  // Tratamiento por defecto para mensajes que no especificamos
            return DefWindowProc (ventana,mensaje,wParam,lParam);
        break;
    }
return 0;
}

/******** I N G R E S O ********/

//confirma que el nombre y la contraseña esten registrados
int confirmar_ingreso()
{
    int len,fila=1;
    char nombre_passwordaux[100]={0};
    strcat(strcat(strcpy(nombre_passwordaux,usuario.nombre),";"),usuario.password);
    len=contar_filas_csv(); //cuenta las filas de Nombres y contraseÃ±as.csv
    while (fila<=len)
    {
        ingreso_info_csv_ingreso(fila); //saca una linea de Nombres y contraseÃ±as.csv
        fila++;
        if ((strcmp(usuario.nombre_password,nombre_passwordaux))==0)
        {
            return 0;
            break;
        }
    }
    return 1;
}

//cuenta las filas de Nombres y contraseÃ±as.csv
int contar_filas_csv()
{
    int len=0;
    std::ifstream file;
    file.open("Nombres y contraseñas.csv",std::ios::in);
        if(file.fail())
        {
            std::cout<<"error al abrir el archivo\n";
        }
        while (file.good())
        {
            if(file.get()=='\n')
            {
                len++;
            }
        }
    file.close();
    return len;
}

//saca conjunto nombre;contraseña de Nombres y contraseÃ±as.csv
void ingreso_info_csv_ingreso(int fila)
{
    int i=0;
    std::string nombre_passwordtemp;
    std::ifstream file;
    file.open("Nombres y contraseñas.csv",std::ios::in);
        if(file.fail())
        {
        std::cout<<"error al abrir el archivo\n";
        }
        while (i<fila)
        {
            getline(file,nombre_passwordtemp,'\n');
            i++;
        }
    file.close();
    strcpy(usuario.nombre_password,nombre_passwordtemp.c_str());
}

/******* R E G I S T R O *******/

//registra nombre y contraseña en Nombres y contraseÃ±as.csv
void registro_info_csv()
{
    std::ofstream file;
    file.open("Nombres y contraseñas.csv",std::ios::app);
        if(file.fail())
        {
        std::cout<<"error al abrir el archivo\n";
        }
        file<<usuario.nombre<<";";
        file<<usuario.password<<"\n";
    file.close();
}

//confirma que el nombre no este registrado
int confirmar_no_repeticion()
{
    int len,fila=2;
    char nombre_passwordaux[100]={0};
    len=contar_filas_csv(); //cuenta las filas de Nombres y contraseÃ±as.csv
    while (fila<=len)
    {
        ingreso_info_csv_registro(fila); //saca una linea de Nombres y contraseÃ±as.csv
        fila++;
        //std::cout<<usuario.nombre<<" vs "<<nombre;
        if ((strcmp(usuario.nombre_aux,usuario.nombre))==0)
        {
            return 0;
            break;
        }
    }
    return 1;
}

//saca los nombres de Nombres y contraseÃ±as.csv
void ingreso_info_csv_registro(int fila)
{
    std::string nombretemp,basura;
    int i=0;
    std::ifstream file;
    file.open("Nombres y contraseñas.csv",std::ios::in);
        if(file.fail())
        {
        std::cout<<"error al abrir el archivo\n";
        }
        while (i<fila)
        {
            getline(file,nombretemp,';');
            getline(file,basura,'\n');
            i++;
        }
    file.close();
    strcpy(usuario.nombre_aux,nombretemp.c_str());
}
