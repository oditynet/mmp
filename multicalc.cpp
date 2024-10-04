
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8838
using namespace std;
int type=0;  //1 - server, 0 - client

string str = "1234567890";
#define N 3
char* ip[N]={"127.0.0.1","127.0.0.2","127.0.0.3"};
string ip_matrinx[N][2];
int curr_ip=0;
//#define next_ip(x) (((x) == (0)) ? (curr_ip=1) : (curr_ip=0))
int charlen=64;
void gen_random(char **tmp_gen_string) {
	char ch[charlen+1];

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    	"!@#$%^&*()_+';'\|/"
        "abcdefghijklmnopqrstuvwxyz";
    int i;
    for(i=0;i<charlen;i++)
    	ch[i]=alphanum[rand() % (sizeof(alphanum) - 1)];
    ch[i+1]='\0';
    *tmp_gen_string=&ch[0];
    //return tmp_gen_string;
}
int client(){
	int server_fd, new_socket;
	    ssize_t valread;
	    struct sockaddr_in address;
	    int opt = 1;
	    socklen_t addrlen = sizeof(address);
	    char buffer[1024] = { 0 };
	    //char* hello = "Hello from server";

	    // Creating socket file descriptor
	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	        perror("socket failed");
	        exit(EXIT_FAILURE);
	    }

	    // Forcefully attaching socket to the port 8080
	    if (setsockopt(server_fd, SOL_SOCKET,
	                   SO_REUSEADDR | SO_REUSEPORT, &opt,
	                   sizeof(opt))) {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }
	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
	    address.sin_port = htons(PORT);
	    //while(1){
	    // Forcefully attaching socket to the port 8080
	    if (bind(server_fd, (struct sockaddr*)&address,
	             sizeof(address))
	        < 0) {
	        perror("bind failed");
	        exit(EXIT_FAILURE);
	    }
	    while(1){
			if (listen(server_fd, 3) < 0) {
				perror("listen");
				exit(EXIT_FAILURE);
			}
			if ((new_socket
				 = accept(server_fd, (struct sockaddr*)&address,
						  &addrlen))
				< 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			while(1){
				valread = read(new_socket, buffer,
							   1024 - 1); // subtract 1 for the null
										  // terminator at the end
				if (valread > 0){
					//printf("string: %s\n", buffer);

					static const char alphanum[] =
						"0123456789"
						"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						"!@#$%^&*()_+';'\|/"
						"abcdefghijklmnopqrstuvwxyz";
					int j;
					for(j=0;j<charlen;j++)
						buffer[j]=alphanum[rand() % (sizeof(alphanum) - 1)];

					send(new_socket, buffer, strlen(buffer), 0);
					//free(tmp);
					printf("Send result %s\n",buffer);
					//sleep(1);
					break;
				}

				cout<<"Wait... "<<endl;
				sleep(1);
			}
	    }
	    // closing the connected socket
	    close(new_socket);
	    // closing the listening socket
	    close(server_fd);
	    return 0;
}
int server_send_str(char *ip_curr,char  *hello)
{
	   int status, client_fd, valread;
	    struct sockaddr_in serv_addr;
	    //char* hello = "get";
	    char buffer[1024] = { 0 };
	    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	        printf("\n Socket creation error \n");
	        return -1;
	    }

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(PORT);

	    // Convert IPv4 and IPv6 addresses from text to binary
	    // form
	    if (inet_pton(AF_INET, ip_curr, &serv_addr.sin_addr)
	        <= 0) {
	        printf(
	            "\nInvalid address/ Address not supported \n");
	        return -1;
	    }
	    while ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
	        //printf("\nConnection Failed . Waiting\n");
	        //return -1;
	    	usleep(50); //50ms sleep
	    }
	    send(client_fd, hello, strlen(hello), 0);
	    //printf("Hello message sent\n");
	    valread = read(client_fd, buffer,
	                   1024 - 1); // subtract 1 for the null
	                              // terminator at the end
	    printf("Return result: %s\n", buffer);
	    for (int i=0;i<N;i++){
			if (ip_matrinx[i][0] == ip_curr)
			{
				ip_matrinx[i][1] ="";
				break;
			}
		}
	    // closing the connected socket
	    close(client_fd);
	    return 0;
}
int main(int argc, char * argv[]) {
	srand((unsigned)time(NULL) * getpid());
	if (argc <= 1)
		client(); //1
	else{
		for (int i=0;i<N;i++){
			ip_matrinx[i][0] = ip[curr_ip];
			ip_matrinx[i][1]="";
			//next_ip(curr_ip);
		}
		string curr_ip;
		int wait=0;
		for (int k=0;k < str.length();k++)
		{
			while(wait==0){
				for (int i=0;i<N;i++){
					if (ip_matrinx[i][1] == "")
					{
						ip_matrinx[i][1] = "w";
						curr_ip = ip_matrinx[i][0] ;
						wait=1;
						break;
					}
				}
			}
			string ch=str.substr(k,1);
			//cout<<"Client "<<curr_ip<<" not work"<<endl;
			//cout<<ch<<" send "<<ip[curr_ip]<<endl;
			server_send_str((char*)curr_ip.c_str(),(char*) ch.data());
			wait=0;
			//next_ip(curr_ip);
		}
	}
	return 0;
}
