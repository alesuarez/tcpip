//Servidor
#include <stdlib.h> //Libreria standar de c
#include <stdio.h>	//Libreria standar de in/out
#include <string.h> //Libreria para trabajar con string de caracteres
#include <sys/types.h>	//Libreria de tipos de datos usado en system calls
#include <sys/socket.h> //Libreria con estructuras necesitadas para sockets
#include <netinet/in.h> //Libreria necesaria con constantes y estructuras para internet
#include <arpa/inet.h> //Libreria para transformar tipos de datos
#include <netdb.h> //Define struct hostent
#include <unistd.h> //Close
#include <errno.h> //Detector de errores
#include <time.h> //Libreria para trabajar con el tiempo

#define PORT_INITIAL 1100
#define MAX_PORT_LENGHT 20
#define SOCKET_ERROR -1
#define ERROR_SYSTEM_CODE 1
#define SUCESS_SYSTEM_CODE 0
#define ASCII_BASE 32
static int getPort();
static int socketCreation();
static void welcomeMessageCreation(int);
static void initServer(struct sockaddr_in *, int);
static int bindSocket(struct sockaddr_in *, int);
static void listenSocket(int);
static int connectSocket(int, struct sockaddr_in *);

void toUpper(char *s); //Funcion para pasar todo a mayuscula

void error(const char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char const *argv[]) {

	int clientSocket;
	int serverSocket;
	int port;
	int clientPort;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char serverResponse[256];
	char clientResponse[100];
	char ip[15];

	port = getPort();

	welcomeMessageCreation(port);

	serverSocket = socketCreation(port);

	initServer(&server, port);

	if (bindSocket(&server, serverSocket) == SOCKET_ERROR) {
		printf("Error al ejecutar el Bind\n");
		return ERROR_SYSTEM_CODE;
	}

	listenSocket(serverSocket);

	if(connectSocket(serverSocket, client) == SOCKET_ERROR) {
	    printf("Error al conectar con el host\n");
	    close(serverSocket);
	    return ERROR_SYSTEM_CODE;
	} else {
		printf("Exito al conectar\n");
	}

	//DATOS DEL CLIENTE//////////////////////////////////////////////////////////////
	//showDataClient();
	strcpy(ip, inet_ntoa(client.sin_addr));
	printf("IP DEL CLIENTE: %s \n", ip);
	clientPort = ntohs(client.sin_port);
	printf("PUERTO DEL CLIENTE: %d \n\n", clientPort);

	//LIMPIEZA DE BUFFERS
	//bzero(serverResponse, 256); no lo pondria
	//bzero(clientResponse, 100);

	//ENVIO DE MENSAJE
	strcpy(serverResponse, "BIENVENIDO AL SERVIDOR! POR FAVOR INTRODUZCA UNA CADENA DE NO MAS DE 100 CARACTERES\n");

	//writeSocket()
	if (write(clientSocket, serverResponse, strlen(serverResponse)) < 0) {
		printf("FALLO AL ENVIAR EL MENSAJE\n\n");
		return 1;
	}

	printf("PETICION DE MENSAJE ENVIADA\n\n");

	// readSocket
	//ESPERANDO RESPUESTA DEL CLIENTE
	if (read(clientSocket, &clientResponse, sizeof(clientResponse)) < 0) {
		printf("ERROR AL RECIBIR \n");
		return 1;
	}
	printf("CLIENTE: %s \n", clientResponse);


	// sendMessagge()
	//PREPARANDO MENSAJE DE RESPUESTA
	toUpper(clientResponse);
	//bzero(serverResponse, 256);
	strcpy(serverResponse, clientResponse);

	//ENVIO DE MENSAJE
	if (write(clientSocket, serverResponse, strlen(serverResponse)) < 0) {
		printf("FALLO AL ENVIAR MENSAJE\n");
	}
	printf("MENSAJE MODIFICADO ENVIADO\n\n");
	printf("FIN DEL SERVICIO\n");
	//
	//CIERRO EL SOCKET
	close(serverSocket);
	return SUCESS_SYSTEM_CODE;
}

static int getPort() {
	return (rand() % MAX_PORT_LENGHT) + PORT_INITIAL;
}

static int socketCreation() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == SOCKET_ERROR) {
		printf("Error al crear el socket\n");
	}

	return serverSocket;
}

static void welcomeMessageCreation(int port) {
	printf("Bienvenido al servidor \n");
	printf("Puerto: %d \n", port);
}

static void initServer(struct sockaddr_in * server, int port) {
	bzero((char *) &server, sizeof(server));
	server->sin_addr.s_addr = INADDR_ANY;
	server->sin_family = AF_INET;
	server->sin_port = htons(port);
}

static int  bindSocket(struct sockaddr_in * server, int serverSocket) {
	 return bind(serverSocket, (struct sockaddr *) &server, sizeof(server));
}

static void  listenSocket(int socket) {
	listen(socket, 3);
	printf("Esperando por clientes...\n\n");
}

static int connectSocket(int serverSocket, struct sockaddr_in * client) {
	return connect(serverSocket,(struct sockaddr *)&client, sizeof(client));
}

void toUpper(char * s) {
  	//cambiar a toUpper
	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] >= 'a' && s[i] <= 'z') {
			s[i] = s[i] - ASCII_BASE;
		}
	}
}

