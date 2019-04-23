//
//  main.c
//  servor
//
//  Created by Duclos Lucas on 22/04/2019.
//  Copyright © 2019 Duclos Lucas. All rights reserved.
//

#include <stdio.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wiringPi.h>

int VD = 25; //roue droite tourne
int VG = 22; //roue gauche tourne
int RDA = 29; //roue droite avance
int RDR = 28; //roue droite recule
int RGA = 23; //roue gauche avance
int RGR = 26; //roue gauche recule

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

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
 }

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    char buffer[255];
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"1ERROR, no port provided\n");
        exit(1);
    }
    
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("2ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("3ERROR on binding");
    int condition = 0;
        printf("En attente de connexion");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("4ERROR on accept");
    while(condition == 0){
        bzero(buffer,255);
       // n = write(newsockfd,"You are connected",16);
        n = read(newsockfd,buffer,255);
        /*     if (n < 0) error("5ERROR reading from socket");*/
        printf("Here is the message: %s\n",buffer);
        ctrl(buffer);
       // n = write(newsockfd,"I got your message",18);
        if (n < 0) error("6ERROR writing to socket");
        if (strcmp(buffer,"close") == 0){
            condition = 1;
            printf("on quitte");
        }
        
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
