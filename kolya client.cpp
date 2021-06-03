#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <stdlib.h>


using namespace std;

void print(char* word);
void print_answer(char* word);
bool win_condition(char* word);

int main(int argc, char* argv)
{
	WSAData wsadata;
	WORD Dllversion = MAKEWORD(2, 1);
	if (WSAStartup(Dllversion, &wsadata) != 0) {
		cout << "the libis not downloaded" << endl;
		exit(-1);
	}
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	int sizeofaddr = sizeof(addr);

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		cout << "can not connect to server" << endl;
		return 1;
	}
	cout << "connection complete" << endl;


	char msg[BUFSIZ];
	char answer[BUFSIZ];
	char otvet[BUFSIZ];

	char letter[5];
	int win = 0;
	cout << "Your annotation is:" << endl;

	recv(Connection, msg, sizeof(msg), NULL);

	print(msg);
	cout << endl;
	cout << "Your word is:" << endl;

	recv(Connection, answer, sizeof(answer), NULL);
	strcpy(otvet, answer);
	print_answer(answer);
	while (true) {
		cout << "Enter a letter, or Stop to EXIT the game:You have " <<10-win<<" tries left" << endl;

		fgets(letter, sizeof(letter), stdin);
		if (win == 10 || strcmp(letter, "Stop") == 0) {

			cout << "You lost, Game Over" << endl;
			break;
		}
		send(Connection, letter, 5, NULL);

		recv(Connection, answer, sizeof(answer), NULL);
		if (strcmp(otvet, answer) == 0) {

			win++;
		}
		strcpy(otvet, answer);

		//проверка на победу
		print_answer(answer);

		if (win_condition(answer)) {

			cout << "You win, congratulations!" << endl;
			break;
		}



	}
	//send(Connection, msg, sizeof(), NULL);

	//recv(Connection, sizeof(), NULL);

	return 0;
}

void print(char* word) {

	for (int i = 0; i < BUFSIZ; i++) {

		if (word[i] == '\0') {
			break;
		}
		cout << word[i];
	}
}

void print_answer(char* word) {

	for (int i = 0; i < BUFSIZ; i++) {

		if (word[i] == '\0') {
			break;
		}
		cout << word[i] << ' ';

	}
	cout << endl;
}

bool win_condition(char* word) {

	for (int i = 0; i < BUFSIZ; i++) {

		if (word[i] == '\0') {
			break;
		}
		if (word[i] == '_') {
			return false;
		}
	}
	return true;
}