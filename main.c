//
//  main.c
//  clientLast
//
//  Created by Duclos Lucas on 22/04/2019.
//  Copyright © 2019 Duclos Lucas. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <wiringPi.h>

int VD = 25; //roue droite tourne
int VG = 22; //roue gauche tourne
int RDA = 29; //roue droite avance
int RDR = 28; //roue droite recule
int RGA = 23; //roue gauche avance
int RGR = 26; //roue gauche recule

int ctrl (char *buffer){
    
    if (wiringPiSetup () == -1)
        return 1;
    pinMode (VD, OUTPUT);
    pinMode (VG, OUTPUT);
    pinMode (RDA, OUTPUT);
    pinMode (RDR, OUTPUT);
    pinMode (RGA, OUTPUT);
    pinMode (RGR, OUTPUT);
    
    digitalWrite (VD, HIGH);
    digitalWrite (VG, HIGH);
    digitalWrite (RDA, LOW);
    digitalWrite (RDR, LOW);
    digitalWrite (RGA, LOW);
    digitalWrite (RGR, LOW);
    
    if (strcmp(buffer, "ar") == 0) { //Groot s'arrete
        printf("Groot s'arrete\n");
        digitalWrite (RDA, LOW);
        digitalWrite (RDR, LOW);
        digitalWrite (RGA, LOW);
        digitalWrite (RGR, LOW);
    } else if (strcmp(buffer, "av") == 0) { //Groot avance
        printf("Groot s'avance\n");
        digitalWrite (RDA, HIGH);
        digitalWrite (RDR, LOW);
        digitalWrite (RGA, HIGH);
        digitalWrite (RGR, LOW);
    } else if (strcmp(buffer,"rc") == 0) { //Groot recule
        printf("Groot recule\n");
        digitalWrite (RDA, LOW);
        digitalWrite (RDA, LOW);
        digitalWrite (RDR, HIGH);
        digitalWrite (RGA, LOW);
        digitalWrite (RGR, HIGH);
    } else if (strcmp(buffer,"td") == 0) { //Groot tourne a droite
        printf("Groot tourne a droite\n");
        digitalWrite (RDA, LOW);
        digitalWrite (RDR, HIGH);
        digitalWrite (RGA, HIGH);
        digitalWrite (RGR, LOW);
    } else if (strcmp(buffer,"tg") == 0) { //Groot tourne a gauche
        printf("Groot tourne a gauche\n");
        digitalWrite (RDA, HIGH);
        digitalWrite (RDR, LOW);
        digitalWrite (RGA, LOW);
        digitalWrite (RGR, HIGH);
    } else {
        printf("j'ai pas compris ce que voulait dire : %s\n",buffer);
    }
    
    return 0;
    
    
    char *substr(char *src,int pos,int len) {
        char *dest=NULL;
        if (len>0) {
            /* allocation et mise à zéro */
            dest = calloc(len+1, 1);
            /* vérification de la réussite de l'allocation*/
            if(NULL != dest) {
                strncat(dest,src+pos,len);
            }
        }
        return dest;
    }
    void error(const char *msg)
    {
        perror(msg);
        exit(0);
    }
    
    int main(int argc, char *argv[])
    {
        //Init client socket
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[255];
        
        //init servor socket
        int sockfd2, newsockfd2, portno2;
        char buffer1[255];
        socklen_t clilen2;
        struct sockaddr_in serv_addr2, cli_addr2;
        int n2;
        
        
        
        
        //Check arguments
        if (argc < 2) {
            fprintf(stderr,"1ERROR, no port provided\n");
            exit(1);
        }
        int argv2 = atoi(argv[2])+1;
        
        
        if (argc < 3) {
            fprintf(stderr,"usage %s hostname port\n", argv[0]);
            exit(0);
        }
        
        
        //Socket Client
        portno = atoi(argv[2]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");
        server = gethostbyname(argv[1]);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");
        int condition=0;
        
        
        
        
        
        
        //Socket Servor
        sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd2 < 0)
            error("2ERROR opening socket");
        bzero((char *) &serv_addr2, sizeof(serv_addr2));
        portno2 = argv2;
        serv_addr2.sin_family = AF_INET;
        serv_addr2.sin_addr.s_addr = INADDR_ANY;
        serv_addr2.sin_port = htons(portno2);
        
        if (bind(sockfd2, (struct sockaddr *) &serv_addr2,
                 sizeof(serv_addr2)) < 0)
            error("3ERROR on binding");
        printf("En attente de connexion");
        listen(sockfd2,5);
        clilen2 = sizeof(cli_addr2);
        newsockfd2 = accept(sockfd2,
                            (struct sockaddr *) &cli_addr2,
                            &clilen2);
        // if (newsockfd2 < 0)
        //  error("4ERROR on accept");
        
        
        
        
        //The Loop
        while (condition==0) {
            bzero(buffer1, 255);
            // n2 = write(newsockfd2,buffer,16);
            //if (n < 0) error("ERROR writing to socket");
            n2 = read(newsockfd2,buffer1, 2);
            //printf("Here is the message: %s\n", buffer1);
            
            if (n < 0) error("5ERROR reading from socket");
            
            char trans = substr(buffer1, 0, 2);
             ctrl(trans);
            // n2 = write(newsockfd2,"I got your message",18);
            //  if (n < 0) error("6ERROR writing to socket");
            
            bzero(buffer1, 255);
            /*      if ((strcmp(buffer, "ar") == 0) || (strcmp(buffer, "av") == 0) || (strcmp(buffer, "tg") == 0) || (strcmp(buffer, "td") == 0) || (strcmp(buffer, "ar") == 0)) {*/
            sleep(1);
            
            bzero(buffer, 255);
            printf("transmitting %s\n ",buffer);
            // bzero(buffer,256);
            // fgets(buffer,255,stdin);
            n = write(sockfd,trans,strlen(buffer1));
            // if (n < 0)
            //      error("ERROR writing to socket");
            //   bzero(buffer,256);
            //  n = read(sockfd,buffer1,255);
            //if (n < 0) error("5ERROR reading from socket");
            
            
            
            
            
            
            
            //  if (n < 0)
            //     error("ERROR reading from socket");
            //     printf("%s\n",buffer);
            
        }
        close(sockfd);
        close(sockfd2);
        return 0;
        
    }
    
