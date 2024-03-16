#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

char buffer[2024];

int incoming(int sd)
{
  /* citirea raspunsului dat de server */
    while(1)
    {
      fflush (stdout);
      for(int i = 0; i < 2024; i++)
        buffer[i] = 0;
      if (recv (sd, buffer,sizeof(buffer), 0) < 0)
      {
        perror ("[client]Eroare la read() de la server.\n");
        return errno;
      }
      /* afisam mesajul primit */
      printf ("%s\n", buffer);
    }
    return 0;
}

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int nr=0;
  char buf[10];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
  {
    perror ("[client]Eroare la connect().\n");
    return errno;
  }

  pthread_t id;
  pthread_create(&id, NULL, &incoming, sd);

  /* citirea mesajului */
  while(1)
  {
    char *text = NULL;
    size_t lungime_text = 0;
    char buf[2024];
    fflush (stdout);
    for(int i = 0; i < 2024; i++)
      buf[i] = 0;
    read (0, buf, sizeof(buf));
    buf[strlen(buf) - 1] = 0;
    if (send (sd, buf, sizeof(buf), 0) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    if(!strcmp(buf, "exit") || !strcmp(buf, "exit1"))
      break;
  }
  close (sd);
}