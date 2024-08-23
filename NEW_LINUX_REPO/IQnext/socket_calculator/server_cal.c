#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PORT_NUM 9090
#define SIZE 20;

/* The server should log all incoming requests and outgoing responses to a log file.
The log should include a timestamp, the client IP address, the received expression, and the computed result. */

struct logins {

	char time[20];
	char send[10],receive[10];
	char clientip[10];
	struct logins *link;
};

void math_calculation(int num1,int num2,char *str,char ch);
void create_node(struct logins **head,char *str_send,char* str_receive,char *clientadress);
int check_data(const char *str);
int check_symbol(char ch);
long int own_atoi( char *str);

void freenode(struct logins *head)
{
	struct logins *travel=head;

	while(travel->link!=0)
	{
		head=head->link;
		free(travel);
		travel=head;
	}
	free(head);
}
int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    char receive_buffer[100]={0},send_buffer[100]={0},receive_bufferip[100]={0};
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT_NUM);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    listen(server_sockfd, 5);

    struct logins *head;
    while(1) {

        printf("server waiting\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
	
	if(client_sockfd<0)
	{
		printf("fail to accept ... \n");
		exit(1);
	}
	printf("accepted ... \n");
        /* The server should receive arithmetic expressions from clients, process them, and send back the result.
 	The server should be capable of handling multiple clients simultaneously.*/

	recv(client_sockfd,receive_buffer,1024,0);
	printf("receive expression:%s\n",receive_buffer);
		
	recv(client_sockfd,receive_bufferip,1024,0);
	printf("client ip adress :%s\n",receive_bufferip);

	char receive_buffert[100]={0};
	strcpy(receive_buffert,receive_buffer);
	//printf("%s",receive_buffert);
	if(check_data(receive_buffer)!=0)
	{
		printf("problem in receive data ... \n");
		send(client_sockfd,"re-enter",10,0);
		exit(1);
	}

	int num1,num2;
	printf("extracitng\n");

//	printf("%d\n",sizeof(receive_buffert));
	int i=0;
	while(receive_buffert[i]!=0){
	if(check_symbol(receive_buffert[i])){
		break;}
	i++;
	}
	char oprand=receive_buffert[i];
	//char oprand='+';
	num1=atoi(receive_buffert);
	getchar();
	printf("%d\n",num1);
	num2=atoi(receive_buffert+(++i));
	//num1=23;num2=45;
	printf("%d\n",num2);
	math_calculation(num1,num2,send_buffer,oprand);
	if(strcmp(send_buffer,"error")){
	
	send(client_sockfd,send_buffer,10,0);
	printf("send data:%s\n",send_buffer);
	}
	else
	{
		send(client_sockfd,"re-enter",10,0);
		printf("error in math_cal ... \n");
		exit(1);
	}
	getchar();
	create_node(&head,send_buffer,receive_buffer,receive_bufferip);// logins
	printf("close client ... \n");
	freenode(head);
	//free(head);
        close(client_sockfd);
	
    }
}

void math_calculation(int num1,int num2,char *str,char ch)
{

	long int num=1;
	switch(ch)
        {
                case '+':snprintf(str,20,"%d",num1+num2);break;
                case '-':snprintf(str,20,"%d",num1-num2);break;
                case '*':snprintf(str,20,"%d",num1*num2);break;
                case '/':if(num2==0){
				 sprintf(str,"%s","error");
				 break;
			 }
			 else
			 snprintf(str,20,"%.2f",(float)num1/num2);break;
                case '^':
			 while(num2>0)
			 {
				 num2--;
				 num*=num1;
			 }
			 snprintf(str,20,"%ld",num);break;
                case '%':if(num2==0){
				 sprintf(str,"%s","error");
				 break;
			 }
			 else
			 snprintf(str,20,"%d",num1%num2);break;
                default :sprintf(str,"%s","error");break;
        }

	
}

void create_node(struct logins **head,char *str_send,char* str_receive,char *clientadress)
{
	struct logins *newnode=(struct logins *)malloc(sizeof(struct logins));

	newnode->link=NULL;

	strcpy(newnode->send,str_send);
        strcpy(newnode->receive,str_receive);
         time_t ticks=time(NULL);
        strcpy(newnode->time,ctime(&ticks));
	strcpy(newnode->clientip,clientadress);
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
}

long int own_atoi(char *str)
{
	 int num=0;
        while(*str!=NULL)
        {
                if((*str >= '0')&&(*str <= '9')){
                       num=num*10+(*str-'0');
		       	str++;
                       
                }
		else if(check_symbol(*str))
		{
			return num;

		}
	}
		return num;

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
