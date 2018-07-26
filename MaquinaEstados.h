/**
*@file MaquinaEstados.h
*@author Jose Manuel Terres
*@date 6 February 2018
*@brief Finite State Machine declarations
*/

#ifndef MaquinaEstados_H
#define MaquinaEstados_H

#ifdef __cplusplus
extern "C"{
#endif

#define MAX_ESTADOS 4
#define MAX_EVENTOS 4
	
/**
*@typedef enum Estado
*@brief Possible states enumeration
*/
typedef enum{
	Estado_ERR = -1, /// Dummy error
	Estado_0 = 0,		 /// IDLE
	Estado_1 = 1,		 /// Positioning
	Estado_2 = 2, 	 /// Experiment
	Estado_3 = 3,		 /// Error
	Estado_4 = 4		 /// Test
}	Estado;

/**
*@typedef enum Evento
*@brief Possible events enumeration
*/
typedef enum{
	Evento_ERR = -1, /// Error
	Evento_Null = 0, /// Nothing happened
	Evento_StartStop = 1, /// Start/Stop command recv
	Evento_TimeOut = 2, /// Experiment time end
	Evento_Position = 3, /// Laser position
	Evento_Error = 4, /// Error
	Evento_Test = 5		/// Test rcv
} Evento;

// Declaración de funciones prototipos y variables
/**@brief Creates a data type: pointer to the function */
typedef void(*Accion)();
/**@brief Evaluates the state with the event */
void Eval_Estado(Evento Ev);

// Function declaration
void Accion_s0_e0(void);
void Accion_s0_e1(void);
void Accion_s0_e2(void);
void Accion_s0_e3(void);
void Accion_s0_e4(void);
void Accion_s0_e5(void);

void Accion_s1_e0(void);
void Accion_s1_e1(void);
void Accion_s1_e2(void);
void Accion_s1_e3(void);
void Accion_s1_e4(void);
void Accion_s1_e5(void);

void Accion_s2_e0(void);
void Accion_s2_e1(void);
void Accion_s2_e2(void);
void Accion_s2_e3(void);
void Accion_s2_e4(void);
void Accion_s2_e5(void);

void Accion_s3_e0(void);
void Accion_s3_e1(void);
void Accion_s3_e2(void);
void Accion_s3_e3(void);
void Accion_s3_e4(void);
void Accion_s3_e5(void);

void Accion_s4_e0(void);
void Accion_s4_e1(void);
void Accion_s4_e2(void);
void Accion_s4_e3(void);
void Accion_s4_e4(void);
void Accion_s4_e5(void);

/**
*@typedef struct Planifica_Estado
*@brief Structure declaration for the State-Event matrix
*/
typedef struct{
	Estado Next_Estado; // Enumera el siguiente estado
	Accion Accion_hacer; // Puntero a la función a realizar
} Planifica_Estado;

// Definición de la tabla Estado-Evento de la MFE
/**@brief State-Event matrix definition*/
Planifica_Estado TABLA_ESTADOS_EV[MAX_ESTADOS][MAX_EVENTOS] = {
	{{Estado_0,Accion_s0_e0},{Estado_0,Accion_s0_e1},{Estado_0,Accion_s0_e2},{Estado_0,Accion_s0_e3},{Estado_0,Accion_s0_e4},{Estado_0, Accion_s0_e5}},
	{{Estado_1,Accion_s1_e0},{Estado_1,Accion_s1_e1},{Estado_1,Accion_s1_e2},{Estado_1,Accion_s1_e3},{Estado_1,Accion_s1_e4},{Estado_1, Accion_s1_e5}},
	{{Estado_2,Accion_s2_e0},{Estado_0,Accion_s2_e1},{Estado_2,Accion_s2_e2},{Estado_2,Accion_s2_e3},{Estado_2,Accion_s2_e4},{Estado_2, Accion_s2_e5}},
	{{Estado_3,Accion_s3_e0},{Estado_3,Accion_s3_e1},{Estado_3,Accion_s3_e2},{Estado_3,Accion_s3_e3},{Estado_3,Accion_s3_e4},{Estado_3, Accion_s3_e5}},
	{{Estado_4,Accion_s4_e0},{Estado_4,Accion_s4_e1},{Estado_4,Accion_s4_e2},{Estado_4,Accion_s4_e3},{Estado_4,Accion_s4_e4},{Estado_4, Accion_s4_e5}},
};

/**@brief Declare and initialize the first state of the FSM*/
Estado Estado_Actual = Estado_0; // Estado actual (tipo enumeración)
/**@brief Declare and initialize last state of the FSM*/
Estado Estado_Anterior = Estado_ERR;
/**@brief Declare and initialize the first event of the FSM*/
Evento Ev_ocurrido = Evento_Null; // Evento ocurrido (tipo enumeración)
/**@brief Declare and initialize the pointer to the first function*/
//Accion Accion_exe = Accion_s0_e0; // Puntero a funcion inicializado en la 0
/**@brief Declare and initialize the structure and its initial values*/
Planifica_Estado Eval_Plan_Estado = {Estado_0,Accion_s0_e0}; // Define la estructura e inicializa en 0,0

#ifdef __cplusplus
}
#endif
#endif