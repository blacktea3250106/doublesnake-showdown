#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread





pthread_t thread_id;

//the thread function
void *connection_handler(void *);
int rank_score[50];
int rank_snake[50];
int rank_index;
int main(int argc )
{
    
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8877 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
	//pthread_t thread_id;
	
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
    
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    //close(sock);
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    int client_result;
    int score;
    
    
    //Send some messages to the client
    /*message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));*/
     
   /* message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));*/
     
    //Receive a message from client
    
       
             
          while(1)  {
            read(sock,&client_result,1);
            read(sock,&score,1);          
            
            
            
            if(client_result != 0){
                if(rank_index > 50){
                    rank_index = 49;
                }
                rank_score[rank_index] = score;
                rank_snake[rank_index] = client_result;

                for(int i = 0;i<50;i++){
                    for(int j = i+1;j<50;j++){
                        if(rank_score[i] < rank_score[j]){
                            int temp = rank_score[i];
                            rank_score[i] = rank_score[j];
                            rank_score[j] = temp;

                            temp = rank_snake[i];
                            rank_snake[i] = rank_snake[j];
                            rank_snake[j] = temp;
                        }
                    }
                }
                printf("--------------------\n");
                for(int i = 0;i<5;i++){
                    if(rank_snake[i] == 0){
                        printf("rank%d: None\n",i+1);
                    }else{
                        printf("rank%d: snake%d score:%d\n",i+1,rank_snake[i],rank_score[i]);
                    }
                    
                }
                printf("--------------------\n");
                client_result = 0;
                rank_index+=1;
                //printf("index:%d\n",rank_index);
            }
            //printf("----------\nwinner is snake%d score:%d\n----------\n",i,client_result,score);
            //client_result = 0;
            
          }
            
        close(sock);
        
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    exit(0);
    return 0;
    
}
