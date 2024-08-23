#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 20
#define PORT_NUM 9090


 /*The client should log all sent requests and received responses to a log file.
   The log should include a timestamp, the type of operation, and the result. */

struct logins {

	char time[20];
	char sent[10],receive[10];
	char op; 
};

int check_data(const char *str);
int check_symbol(char ch);

int main()
{
    int sock_fd,len;
    struct sockaddr_in address;
    char DATA[SIZE] = {0};

    sock_fd = socket(AF_INET, SOCK_STREAM, 0); //domain ipv4(AF_INET)/ipv6(AF_INET6)

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");// localhost adress
    address.sin_port = htons(PORT_NUM); //choose number between 1024-49151 number
    len = sizeof(address);

    if(connect(sock_fd,(struct sockaddr *)&address,len) < 0) {
        perror("fail to coonnect ....\n");
        exit(1);
    }
	/*The client should allow the user to input arithmetic expressions (e.g., 5 + 3, 12 / 4) and send them to the server.
	Display the result received from the server to the user.
	Handle input validation to ensure only valid arithmetic expressions are sent. */

    	printf("enter input in this expressions 1+2 or 5-2 ... \n");
    	fgets(DATA,10,stdin);

	if(check_data(DATA)!=0){
		printf("please check your enter expression ...");
		exit(1);
	}

	send(sock_fd,DATA,sizeof(DATA),0);
	printf("data sent ... \n");
	sleep(2);
	char IPaddrss[100]={0};
	strcpy(IPaddrss,inet_ntoa(address.sin_addr));
	send(sock_fd,"127.0.0.1",10,0);
	printf("ip sent ...\n");	

	char buffer[100]={0};
	recv(sock_fd,buffer,100,0);
	printf("received ... \n");

	if(strcmp(buffer,"re-enter")==0)
	{
	   printf("enter expression failed please try again ... \n");
	   exit(1);
	}

	printf("result:%s\n",buffer);
	
	const char *dummy=NULL;
	dummy=DATA;

	while(*dummy!=0)
	{

	   if(check_symbol(*dummy))
		break;
	   else
		   dummy++;

	}
	
	struct logins *loginsnode=malloc(sizeof(struct logins));

	time_t ticks=time(NULL);
        strcpy(loginsnode->time,ctime(&ticks));

	strcpy(loginsnode->sent,DATA);
	strcpy(loginsnode->receive,buffer);
	loginsnode->op=*dummy;
	
	
    printf("exit ... \n");
    free(loginsnode);
    close(sock_fd);

    return 0;
}

int check_data(const char *str)
{
	int flag=0;
	while(*str!=NULL)
	{
		if(((*str >= '0')&&(*str <= '9'))||(*str == ' ')){
			str++;
			continue;
		}
		else if(check_symbol(*str))
		{
			flag++;
			str++;
			continue;
		}
		else
			return 0;
	}
	if(flag>=2)
		return 0; // because of if user enter more then one operand
	return 1;
}

int check_symbol(char ch)
{
	switch(ch)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
		case '%':return 1;
		default :return 0;
	}
}

/*void create_node(struct logins **head,char *str_send,char* str_receive,char ch)
{
	struct logins *newnode=(struct logins *)malloc(sizeof(struct logins));

	newnode->link=NULL;

	newnode->op=ch;
	strcpy(newnode->send,str_send);
        strcpy(newnode->receive,str_receive);
         time_t ticks=time(NULL);
        strcpy(newnode->time,ctime(&ticks));

	if(*head==NULL){
		*head=newnode;
	}

	else
	{
		struct logins *travesal=*head;

		while(travesal->link!=NULL)
		travesal=travesal->link;

		travesal->link=newnode;
	}
}*/

