#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // to read and sleep
#include <netinet/in.h>
#include "libs/erproc.h"

int main() {
    int fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34545);

    Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    Connect(fd, (struct sockaddr *) &adr, sizeof adr);

    size_t choicelen = 0;
    char *choice = malloc(sizeof(char) * 256);
    size_t len = 0;
    char *smessage = malloc(sizeof(char) * 256);
    char *rmessage = malloc(sizeof(char) * 256);

    ssize_t nread;
    int quit = 0;
    while (!quit) {
        printf("<=>Welcome to LiteSH<=>\n"
               "1 create process\n"
               "2 create background process\n"
               "3 send signal\n"
               "4 receive signal\n"
               "5 help\n"
               "6 exit\n\n");
        printf("Enter key: ");
        //Input choice
        getline(&choice, &choicelen, stdin);
        choice[strlen(choice) - 1] = '\0';

        if (!strcmp(choice, "1") || !strcmp(choice, "2")) { //1 create process
            //Send choice                                   //2 create background process
            send(fd, choice, strlen(choice), 0);
            //Recv "Enter name of process:"
            //  or "Enter name of silent process:"
            nread = recv(fd, rmessage, 31, 0);
            if (nread == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);
            //Input name of process
            getline(&smessage, &len, stdin);
            smessage[strlen(smessage) - 1] = '\0';

            //Send name to server
            send(fd, smessage, strlen(smessage), 0);
            //Recv "Procces closed"
            //  or "Procces created"
            nread = recv(fd, rmessage, 17, 0);
            if (nread == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);

        } else if (!strcmp(choice, "3")) { //3 send signal
            //Send choice
            send(fd, choice, strlen(choice), 0);
            //Recv "Enter PID:"
            nread = recv(fd, rmessage, 12, 0);
            if (nread == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);
            //Input PID of process
            getline(&smessage, &len, stdin);
            smessage[strlen(smessage) - 1] = '\0';

            //Send PID to server
            send(fd, smessage, strlen(smessage), 0);
            //Recv "Enter signal:"
            nread = recv(fd, rmessage, 15, 0);
            if (nread == -1) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);

            //Input singal
            getline(&smessage, &len, stdin);
            smessage[strlen(smessage) - 1] = '\0';
            //Send signal
            send(fd, smessage, strlen(smessage), 0);
            //Recv "Signal sent"
            nread = recv(fd, rmessage, 13, 0);
            if (nread == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);

        } else if (!strcmp(choice, "4")) { //4 receive signal
            //Send choice
            send(fd, choice, strlen(choice), 0);
            //Recv "Enter signal:"
            nread = recv(fd, rmessage, 15, 0);
            if (nread == -1) {
                perror("Recv failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);
            //Input singal
            getline(&smessage, &len, stdin);
            smessage[strlen(smessage) - 1] = '\0';

            //Send signal
            send(fd, smessage, strlen(smessage), 0);
            //Recv "Process recieve signal"
            nread = read(fd, rmessage, 24);
            if (nread == -1) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0) {
                printf("END OF FILE occured\n");
            }
            //Print recv message
            write(STDOUT_FILENO, rmessage, nread);

        } else if (!strcmp(choice, "5")) { //5 help
            printf("\033[2J");
            printf("\033[0;0f");
            printf("/This programm can send and receive signals to/from processes\n"
                   "/and create processes.\n"
                   ".To create process enter 1, next type the name of process\n"
                   " programm create process with this command.\n"
                   " (Enter name of programm on your computer)\n"
                   ".To create background process enter 2, type name\n"
                   " programm do the same things as create ordionary process,\n"
                   " but created process will running on another thread.\n"
                   " (Enter name of programm on your computer)\n"
                   ".To send signal enter 3, type Process ID, and number of\n"
                   " signal.\n"
                   "     PID: 777777\n"
                   "    Signal: 15)\n"
                   ".To receive signal enter 4, type number of signal, when\n"
                   " process receive this signal, you will see message -\n"
                   " \"Signal - {number of signal} received successfully\"\n"
                   ".To get help enter 5.\n"
                   ".To exit enter 6.\n"
                   "Authors: Besedin Iakov, Melnikov Ivan.\n");
            printf("Press Enter...\n");
            getchar();

        } else if (!strcmp(choice, "6")) { //6 exit
            //Send choice
            write(fd, choice, strlen(choice));
            quit++;

        } else {
            printf("Wrong key\n");
            continue;
        }
    }

    free(choice);
    free(rmessage);
    free(smessage);

    close(fd);

    return 0;
}
