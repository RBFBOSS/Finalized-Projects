#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <sqlite3.h> //gcc sv.c -o libcode.so -shared -fPIC

void function1() 
{
	printf("hi\n");
}

__attribute__ ((visibility ("default"))) void entry_point() 
{
	function1();
}

/* portul folosit */
#define PORT 2901

/* codul de eroare returnat de anumite apeluri */
extern int errno;



char buffer[2024];

char useri[100][100];

int useri_size = 0;

char sql[2024];

int maxi;

int rc;

int rn;

int aux;

int cr;

int tg;

sqlite3 *db; //baza de date

char *mesaj_eroare = 0;

int sd;		//descriptorul de socket 

int ok;

int nr_useri = 0;

char user[100];

char user1[100];

char user2[100];

char list[2024];

char message[1024];

char title[100];

int nr_prieteni1, nr_prieteni2;

int i=0;              // contor pentru evidenta pthread_t

int put_fd_in_tg(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "USER_FD"))
    {
      tg = atoi(argv[i]);
    }
  }
  return 0;
}

int notif(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  char target[100];
  char linie[2024];
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "PARTICIPANT"))
    {
      strcpy(target, argv[i]);
    }
  }

  tg = -1;

  snprintf(sql, 1024, "select * from logs where name like '%s';", target);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, put_fd_in_tg, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
    sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }

  strcpy(linie, "Ati fost adaugat intr-o conversatie cu ");
  strcat(linie, list);
  strcat(linie, "\n");
  if(tg != -1)
  {
    if (send (tg, linie, strlen(linie), 0) <= 0)
    {
      printf("[Thread %d] ",i);
      perror ("[Thread]Eroare la write() catre client.\n");
    }
  }
  return 0;
}

int make_list(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "PARTICIPANT"))
    {
      strcat(list, argv[i]);
      strcat(list, " ");
    }
  }
  return 0;
}

void clients_in_chat(int room)
{
  strcpy(list, "");

  snprintf(sql, 1024, "select * from chats where ID like '%d';", room);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, make_list, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
        sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
}

void notificare_join_chat(int room)
{
  clients_in_chat(room);

  snprintf(sql, 1024, "select * from chats where ID like '%d';", room);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, notif, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error11: %s\n", mesaj_eroare);
        sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  return;
}

int macar_o_valoare(void *useless, int argc, char **argv, char **Coloane) 
{
  ok = 1;
  return 0;
}

int get_max_id(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "ID"))
    {
      int r = atoi(argv[i]);
      if(maxi < r)
        maxi = r;
    }
  }
  return 0;
}

int get_id(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "ID"))
    {
      aux = atoi(argv[i]);
    }
  }
  return 0;
}

bool logged_in(int cl)
{
  snprintf(sql, 1024, "select 1 from logs where user_FD like '%d';", cl);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  ok = 0;

  rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

  printf("Ok = %d\n", ok);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error11: %s\n", mesaj_eroare);
        sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  return ok;
}

int get_name(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "NAME"))
    {
      strcpy(user, argv[i]);
    }
  }
  return 0;
}

void get_user(char this_user[100], int cl)
{
  strcpy(user, "");

  snprintf(sql, 1024, "select * from logs where user_fd=%d;", cl);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, get_name, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
    sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  strcpy(this_user, user);
  return;
}

int callback(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    printf("%s = %s\n", Coloane[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int cati_useri(void *useless, int argc, char **argv, char **Coloane) 
{
  nr_useri++;
  return 0;
}

int in_chat(int cl)
{
  get_user(user1, cl);

  snprintf(sql, 1024, "select * from chats where participant like '%s';", user);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  aux = 0;

  rc = sqlite3_exec(db, sql, get_id, 0, &mesaj_eroare);

  printf("ID = %d\n", aux);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
        sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  return aux;
}

int broadcast(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  char target[100];
  char linie[2024];
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "PARTICIPANT"))
    {
      strcpy(target, argv[i]);
    }
  }

  snprintf(sql, 1024, "select * from logs where name like '%s';", target);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, put_fd_in_tg, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
    sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }

  get_user(user, cr);

  if(cr != tg)
  {
    strcpy(linie, user);
    strcat(linie, ": ");
    strcat(linie, buffer);
    if (send (tg, linie, strlen(linie), 0) <= 0)
    {
      printf("[Thread %d] ",i);
      perror ("[Thread]Eroare la write() catre client.\n");
    }
  }
  return 0;
}

void f_broadcast(int cl, int room_nr)
{
  snprintf(sql, 1024, "select * from chats where ID like '%d';", room_nr);

  printf("SQL: %s\n", sql);

  mesaj_eroare = 0;

  cr = cl;

  rc = sqlite3_exec(db, sql, broadcast, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
    sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
}

int if_admin(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "ADMIN"))
    {
      if(!strcmp(argv[i], "1"))
        ok = 1;
      else ok = 0;
    }
  }
  return 0;
}

bool is_admin(char name[100])
{
  snprintf(sql, 2024, "select * from users where name like '%s';", name);

  printf("SQL: %s\n", sql);

  ok = 0;

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, if_admin, 0, &mesaj_eroare);

  printf("Ok = %d\n", ok);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
     sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  return ok;
}

int if_public(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "PUBLIC"))
    {
      if(!strcmp(argv[i], "1"))
        ok = 1;
      else ok = 0;
    }
  }
  return 0;
}

bool is_public(char name[100])
{
  snprintf(sql, 2024, "select * from users where name like '%s';", name);

  printf("SQL: %s\n", sql);

  ok = 0;

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, if_public, 0, &mesaj_eroare);

  printf("Ok = %d\n", ok);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
     sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  return ok;
}

int if_close_friend(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "CATEGORY"))
    {
      if(!strcmp(argv[i], "2"))
        ok = 1;
      else ok = 0;
    }
  }
  return 0;
}

bool is_close_friend(char owner[100], char friend[100])
{
  snprintf(sql, 2024, "select * from friends where owner like '%s' and friend like '%s';", owner, friend);

  printf("SQL: %s\n", sql);

  ok = 0;

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, if_close_friend, 0, &mesaj_eroare);

  printf("Ok = %d\n", ok);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
     sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }
  return ok;
}

int has_private_profile(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "PUBLIC"))
    {
      if(!strcmp(argv[i], "0"))
        ok = 1;
      else ok = 0;
    }
  }
  return 0;
}

int print_posts(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(strcmp(Coloane[i], "OWNER"))
    {
      strcat(buffer, Coloane[i]);
      strcat(buffer, ": ");
      strcat(buffer, argv[i]);
      strcat(buffer, "\n");
    }
  }
  strcat(buffer, "\n");
  return 0;
}

void view_posts(char target[100], bool permision) //permision = 0 => nu sunt prieteni
{
  if(permision == 1)
  {
    snprintf(sql, 1024, "select * from posts where owner like '%s';", target);

    printf("SQL: %s\n", sql);

    rc = sqlite3_exec(db, sql, print_posts, 0, &mesaj_eroare);

    if(rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
      sqlite3_free(mesaj_eroare);
    } 
    else 
    {
      fprintf(stdout, "Operation completed successfully\n");
    }
  }
  else
  {
    snprintf(sql, 1024, "select * from users where name like '%s';", target);

    printf("SQL: %s\n", sql);

    ok = 0;

    rc = sqlite3_exec(db, sql, has_private_profile, 0, &mesaj_eroare);

    if(rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
      sqlite3_free(mesaj_eroare);
    } 
    else 
    {
      fprintf(stdout, "Operation completed successfully\n");
    }
    if(ok == 1)
      snprintf(buffer, 2024, "%s are profilul privat\n", target);
    else
    {
      snprintf(sql, 1024, "select * from posts where owner like '%s' and public=1;", target);

      printf("SQL: %s\n", sql);

      rc = sqlite3_exec(db, sql, print_posts, 0, &mesaj_eroare);

      if(rc != SQLITE_OK )
      {
        fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
        sqlite3_free(mesaj_eroare);
      } 
      else 
      {
        fprintf(stdout, "Operation completed successfully\n");
      }
    }
  }
}

int broadcast_news(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  char target[100];
  char linie[2024];
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "NAME") || !strcmp(Coloane[i], "FRIEND"))
    {
      strcpy(target, argv[i]);
    }
  }

  snprintf(sql, 1024, "select * from logs where name like '%s';", target);

  printf("SQL: %s\n", sql);

  tg = -1;

  mesaj_eroare = 0;

  rc = sqlite3_exec(db, sql, put_fd_in_tg, 0, &mesaj_eroare);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
    sqlite3_free(mesaj_eroare);
  } 
  else 
  {
    fprintf(stdout, "Operation completed successfully\n");
  }

  if(tg != -1)
  {
    snprintf(linie, 2024, "===STIRE DE LA %s: %s===\n%s\n", user1, title, message);
    if (send (tg, linie, strlen(linie), 0) <= 0)
    {
      printf("[Thread %d] ",i);
      perror ("[Thread]Eroare la write() catre client.\n");
    }
  }
  return 0;
}

void n_broadcast(char owner[100], int permision, char titlu[100], char continut[1024])
{
  strcpy(title, titlu);
  strcpy(user2, owner);
  strcpy(message, continut);

  if(permision == 0)
  {
    snprintf(sql, 1024, "select * from logs where name not like '%s';", owner);

    printf("SQL: %s\n", sql);

    mesaj_eroare = 0;

    rc = sqlite3_exec(db, sql, broadcast_news, 0, &mesaj_eroare);

    if(rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
      sqlite3_free(mesaj_eroare);
    } 
    else 
    {
      fprintf(stdout, "Operation completed successfully\n");
    }
  }
  else if(permision == 1)
  {
    snprintf(sql, 1024, "select * from friends where owner like '%s';", owner);

    printf("SQL: %s\n", sql);

    mesaj_eroare = 0;

    rc = sqlite3_exec(db, sql, broadcast_news, 0, &mesaj_eroare);

    if(rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
      sqlite3_free(mesaj_eroare);
    } 
    else 
    {
      fprintf(stdout, "Operation completed successfully\n");
    }
  }
  else
  {
    snprintf(sql, 1024, "select * from friends where owner like '%s' and category=2;", owner);

    printf("SQL: %s\n", sql);

    mesaj_eroare = 0;

    rc = sqlite3_exec(db, sql, broadcast_news, 0, &mesaj_eroare);

    if(rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
      sqlite3_free(mesaj_eroare);
    } 
    else 
    {
      fprintf(stdout, "Operation completed successfully\n");
    }
  }
}

int display_chat(void *useless, int argc, char **argv, char **Coloane) 
{
  int i;
  for(i = 0; i < argc; i++)
  {
    if(!strcmp(Coloane[i], "PARTICIPANT"))
    {
      strcat(buffer, argv[i]);
      strcat(buffer, ": ");
    }
    else if(!strcmp(Coloane[i], "CONTENT"))
    {
      strcat(buffer, argv[i]);
      strcat(buffer, "\n\n");
    }
  }
  return 0;
}

bool raspunde(int cl, char this_user[100])
{
  int lungime_text;
  if (lungime_text = recv (cl, buffer,sizeof(buffer), 0) <= 0)
  {
    printf("[Thread %d]\n",i);
    perror ("Eroare la read() de la client.\n");
  }

  // Deschidem baza de date
  rc = sqlite3_open("proiect.db", &db);

  if( rc ) 
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return(0);
  } 
  else 
  {
    fprintf(stdout, "Opened database successfully\n");
  }

  fflush (stdout);

  if(!strcmp(buffer, ".help"))
  {
    strcpy(buffer, "===LISTA DE COMENZI ACCEPTATE===\n\n");
    strcat(buffer, "register <Nume> <Parola> <Confirmare Parola>\n");
    strcat(buffer, "login <Nume> <Parola>\n");
    strcat(buffer, "view_posts <Persoana>\n");
    strcat(buffer, "exit\n");
    strcat(buffer, "exit1 (inchide serverul. A se folosi doar in circumstante aprobate de admini)\n");
    strcat(buffer, "\n===Pentru a rula urmatoarele comenzi, trebuie sa fii logat===\n\n");
    strcat(buffer, "logout\n");
    strcat(buffer, "get_admin <Parola_ca_sa_fii_admin>\n");
    strcat(buffer, "rm_admin \n");
    strcat(buffer, "rm_user (trebuie sa fii admin) \n");
    strcat(buffer, "set_public_profile\n");
    strcat(buffer, "set_private_profile\n");
    strcat(buffer, "make_post <Title> <Content> <0/1 (privat/public)>\n");
    strcat(buffer, "view_my_posts\n");
    strcat(buffer, "set_public_post <Numele postarii>\n");
    strcat(buffer, "set_private_post <Numele postarii>\n");
    strcat(buffer, "add_friend <Persoana>\n");
    strcat(buffer, "rm_friend <Persoana>\n");
    strcat(buffer, "add_close_friend <Persoana>\n");
    strcat(buffer, "rm_close_friend <Persoana>\n");
    strcat(buffer, "chat_with <Persoana> <Persoana> ... <Persoana>\n");
    strcat(buffer, "leave (pentru a parasi un chat)\n");
    strcat(buffer, "make_news <Title> <Content> <0/1/2 (toti/ prieteni / prieteni apropiati)>\n");
    /* returnam mesajul clientului */
    if (send (cl, buffer, strlen(buffer), 0) <= 0)
    {
      printf("[Thread %d] ",i);
      perror ("[Thread]Eroare la write() catre client.\n");
    // continue;		/* continuam sa ascultam */
    }
    sqlite3_close(db);
    return 1;
  }
  else
  {
    //verificam daca clientul este intr-o camera de chat
    rn = in_chat(cl);
    if(rn != 0)
    {
      get_user(this_user, cl);

      //identificare comanda de la client

      if(!strcmp(buffer, "exit"))
      {
        strcpy(buffer, "Ati iesit din conversatie\n");

        //scoatem clientul din chat
        snprintf(sql, 1024, "delete from chats where participant like '%s';", this_user);

        printf("SQL: %s\n", sql);

        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

        if(rc != SQLITE_OK )
        {
          fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }

        if (send (cl, buffer, strlen(buffer), 0) <= 0)
        {
          printf("[Thread %d] ",i);
          perror ("[Thread]Eroare la write() catre client.\n");
        // continue;		/* continuam sa ascultam */
        }

        snprintf(sql, 1024, "delete from logs where user_fd like '%d';", cl);

        printf("SQL: %s\n", sql);

        ok = 0;

        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

        printf("Ok = %d\n", ok);

        if(rc != SQLITE_OK )
        {
          fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }

        return 0;
      }

      if(!strcmp(buffer, "leave"))
      {
        strcpy(buffer, "Ati iesit din conversatie\n");

        //scoatem clientul din chat
        snprintf(sql, 1024, "delete from chats where participant like '%s';", this_user);

        printf("SQL: %s\n", sql);

        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

        if(rc != SQLITE_OK )
        {
          fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }
        
        if (send (cl, buffer, strlen(buffer), 0) <= 0)
        {
          printf("[Thread %d] ",i);
          perror ("[Thread]Eroare la write() catre client.\n");
        // continue;		/* continuam sa ascultam */
        }
      }
      else
      {
        //trimitem mesajul clientului la ceilalti participanti activi si il salvam in conversatie

        snprintf(sql, 3024, "insert into messages (id, participant, content) values (%d, '%s', '%s');", rn, this_user, buffer);
        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

        if(rc != SQLITE_OK )
        {
           fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }

        f_broadcast(cl, rn);
      }
      sqlite3_close(db);
      return 1;
    }
    else
    {
      //identificare comanda de la client
      char command[1024];

      int j;
      for(j = 0; buffer[j] != ' ' && buffer[j] != 0 && buffer[j] != '\n'; j++)
        command[j] = buffer[j];

      command[j] = 0;

      if(!strcmp(buffer, "exit"))
      {
        
        snprintf(sql, 1024, "delete from logs where user_fd like '%d';", cl);

        printf("SQL: %s\n", sql);

        ok = 0;

        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

        printf("Ok = %d\n", ok);

        if(rc != SQLITE_OK )
        {
          fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }

        return 0;
      }

      else if(!strcmp(buffer, "exit1"))
      {
        snprintf(sql, 1024, "delete from logs where 1=1;");

        printf("SQL: %s\n", sql);

        ok = 0;

        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

        printf("Ok = %d\n", ok);

        if(rc != SQLITE_OK )
        {
          fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }

        sqlite3_close(db);  
        shutdown(sd, SHUT_RDWR);
        exit(0);
      }
      
      else if(!strcmp(buffer, "logout"))
      {
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          strcpy(buffer, "Delogare realizata cu succes\n");
          snprintf(sql, 1024, "delete from logs where user_fd like '%d';", cl);

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
        }
      }

      else if(!strcmp(buffer, "view_my_posts"))
      {
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          get_user(this_user, cl);
          strcpy(buffer, "Postarile tale:\n");
          view_posts(this_user, 1);
        }
      }

      else if(!strcmp(command, "register"))
      {
        if(logged_in(cl) == 1)
          strcpy(buffer, "Sunteti deja logat\n");
        else
        {
          //stocare nume dorit
          
          char nume[100];
          int u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          {
            nume[u++] = buffer[j];
            if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
              nume[u-1] = nume[u-1] + 32;
          }
          nume[u] = 0;

          //verificam daca numele este deja in baza de date

          strcpy(sql, "select * from users where name like '");
          strcat(sql, nume);
          strcat(sql, "';");

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }

          if(ok == 1)
            strcpy(buffer, "Numele nu este disponibil\n");

          else
          {
            //stocare parola dorita

            char parola[1024];
            u = 0;
            for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
              parola[u++] = buffer[j];
            parola[u] = 0;

            u = 0;
            bool corect = 1;
            for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
              if(parola[u++] != buffer[j])
                corect = 0;

            if(corect == 0)
              strcpy(buffer, "Parola incorecta\n");
            else
            {
              strcpy(sql, "insert into users (name, admin, public, password)");
              strcat(sql, "values ('");
              strcat(sql, nume);
              strcat(sql, "', 0, 0,'");
              strcat(sql, parola);
              strcat(sql, "');");
              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);
              strcpy(buffer, "Cont creat cu succes\n");

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
              fprintf(stdout, "Operation completed successfully\n");
              }
            }
          }
        }
      }
              
      else if(!strcmp(command, "login"))
      {
        if(logged_in(cl) == 1)
          strcpy(buffer, "Sunteti deja logat\n");
        else
        {
          //stocare nume dorit
          
          char nume[100];
          int u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          {
            nume[u++] = buffer[j];
            if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
              nume[u-1] = nume[u-1] + 32;
          }
          nume[u] = 0;

          //verificam daca numele este deja in baza de date

          strcpy(sql, "select * from users where name like '");
          strcat(sql, nume);
          strcat(sql, "';");

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }

          if(ok == 0)
            strcpy(buffer, "Cont neexistent\n");
          else
          {
            //stocare parola introdusa

            char parola[1024];
            u = 0;
            for(j = j+1; buffer[j] != 0; j++)
              parola[u++] = buffer[j];
            parola[u] = 0;

            //verificare parola
            strcpy(sql, "select * from users where name like '");
            strcat(sql, nume);
            strcat(sql, "' and password like '");
            strcat(sql, parola);
            strcat(sql, "';");

            printf("SQL: %s\n", sql);

            ok = 0;

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }

            if(ok == 0)
              strcpy(buffer, "Parola gresita\n");
            else
            {
              ok = 0;
              strcpy(sql, "select 1 from logs where name like '");
              strcat(sql, nume);
              strcat(sql, "';");
              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

              if(ok == 1)
                strcpy(buffer, "Cineva este deja logat pe acest cont\n");
              else
              {
                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                fprintf(stdout, "Operation completed successfully\n");
                }

                strcpy(sql, "select 1 from logs;");
                mesaj_eroare = 0;

                nr_useri = 0;

                rc = sqlite3_exec(db, sql, cati_useri, 0, &mesaj_eroare);

                strcpy(sql, "insert into logs (name, user_fd)");
                strcat(sql, "values ('");
                strcat(sql, nume);
                strcat(sql, "', ");
                char numar[100];
                snprintf (numar, sizeof(numar), "%d",cl);

                strcat(sql, numar);
                strcat(sql, ");");
                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);
                strcpy(buffer, "Logare realizata cu succes\n");

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                fprintf(stdout, "Operation completed successfully\n");
                }
              }
            }
          }
        }
      }

      else if(!strcmp(command, "add_friend"))
      {
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          char nume[100];
          int u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          {
            nume[u++] = buffer[j];
            if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
              nume[u-1] = nume[u-1] + 32;
          }
          nume[u] = 0;

          strcpy(sql, "select 1 from users where name like '");
          strcat(sql, nume);
          strcat(sql, "';");

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
          
          if(ok == 0)
          {
            strcpy(buffer, "Utilizator inexistent\n");
          }
          else
          {
            get_user(this_user, cl);
            if(!strcmp(this_user, nume))
              strcpy(buffer, "Nu te poti imprieteni cu tine\n");
            else
            {
              strcpy(sql, "select 1 from friend_requests where name like '");
              strcat(sql, nume);
              strcat(sql, "' and request_from like '");
              strcat(sql, this_user);
              strcat(sql, "';");

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              if(ok == 1)
              {
                strcpy(buffer, "Cererea de prietenie a fost trimisa deja\n");
              }
              else
              {
                //verificam daca deja suntem prieteni cu celalat user
                snprintf(sql, 1024, "select * from friends where owner like '%s' and friend like '%s';", this_user, nume);

                ok = 0;

                printf("SQL: %s\n", sql);

                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                  fprintf(stdout, "Operation completed successfully\n");
                }

                if(ok == 1)
                  snprintf(buffer, 2024, "Deja esti prieteni cu %s\n", nume);
                else
                {
                  //verificam daca celalalt user a trimis o cerere de prietenie catre noi
                  snprintf(sql, 1024, "select * from friend_requests where name like '%s' and request_from like '%s';", this_user, nume);

                  ok = 0;

                  printf("SQL: %s\n", sql);

                  mesaj_eroare = 0;

                  rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

                  if(rc != SQLITE_OK )
                  {
                    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                    sqlite3_free(mesaj_eroare);
                  } 
                  else 
                  {
                    fprintf(stdout, "Operation completed successfully\n");
                  }

                  if(ok == 1)
                  {
                    strcpy(buffer, "Cerere de prietenie acceptata\n");

                    //stergem cealalta cerere de prietenie

                    snprintf(sql, 1024, "delete from friend_requests where name like '%s' and request_from like '%s';", this_user, nume);

                    printf("SQL: %s\n", sql);

                    mesaj_eroare = 0;

                    rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                    if(rc != SQLITE_OK )
                    {
                      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                      sqlite3_free(mesaj_eroare);
                    } 
                    else 
                    {
                      fprintf(stdout, "Operation completed successfully\n");
                    }

                    snprintf(sql, 1024, "insert into friends (OWNER, FRIEND, CATEGORY) values ('%s', '%s', 1);"
                    , this_user, nume);

                    printf("SQL: %s\n", sql);

                    mesaj_eroare = 0;

                    rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                    if(rc != SQLITE_OK )
                    {
                      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                      sqlite3_free(mesaj_eroare);
                    } 
                    else 
                    {
                      fprintf(stdout, "Operation completed successfully\n");
                    }

                    snprintf(sql, 1024, "insert into friends (OWNER, FRIEND, CATEGORY) values ('%s', '%s', 1);"
                    , nume, this_user);

                    printf("SQL: %s\n", sql);

                    mesaj_eroare = 0;

                    rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                    if(rc != SQLITE_OK )
                    {
                      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                      sqlite3_free(mesaj_eroare);
                    } 
                    else 
                    {
                      fprintf(stdout, "Operation completed successfully\n");
                    }

                  }
                  else
                  {

                    strcpy(sql, "insert into friend_requests (NAME, REQUEST_FROM) VALUES('");
                    strcat(sql, nume);
                    strcat(sql, "', '");
                    strcat(sql, user);
                    strcat(sql, "');");

                    printf("SQL: %s\n", sql);

                    ok = 0;

                    mesaj_eroare = 0;

                    rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                    printf("Ok = %d\n", ok);

                    if(rc != SQLITE_OK )
                    {
                      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                      sqlite3_free(mesaj_eroare);
                    } 
                    else 
                    {
                      fprintf(stdout, "Operation completed successfully\n");
                    }

                    strcpy(buffer, "Cerere de prietenie trimisa cu succes\n");
                  }
                }
              }
            }
          }
        }
      }

      else if(!strcmp(command, "rm_friend"))
      {
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          get_user(this_user, cl);

          char nume[100];
          int u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          {
            nume[u++] = buffer[j];
            if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
              nume[u-1] = nume[u-1] + 32;
          }
          nume[u] = 0;

          snprintf(sql, 2024, "select * from friends where owner like '%s' and friend like '%s';", this_user, nume);

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
          
          if(ok == 0)
          {
            strcpy(buffer, "Utilizator inexistent in lista de prieteni\n");
          }
          else
          {
            strcpy(buffer, "Utilizator eliminat din lista de prieteni\n");

            snprintf(sql, 2024, "delete from friends where owner like '%s' and friend like '%s';", this_user, nume);

            printf("SQL: %s\n", sql);

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }

            snprintf(sql, 2024, "delete from friends where owner like '%s' and friend like '%s';", nume, this_user);

            printf("SQL: %s\n", sql);

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
          }
        }
      }

      else if(!strcmp(command, "chat_with"))
      {
        //verificam daca utilizatorul este logat
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          int cp = j;
          get_user(this_user, cl);
          char text[2024];
          strcpy(text, buffer);
          useri_size = 1;
          strcpy(useri[0], this_user);
          bool eroare = 0;

          //verificam daca userii sunt valizi

          while(text[j] != 0)
          {
            char nume[100];
            int u = 0;
            for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
            {
              nume[u++] = buffer[j];
              if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
                nume[u-1] = nume[u-1] + 32;
            }
            nume[u] = 0;

            strcpy(useri[useri_size], nume);
            useri_size++;

            //verificam daca suntem clientul tinta

            get_user(this_user, cl);

            if(!strcmp(this_user, nume))
            {
              strcpy(buffer, "Nu poti discuta cu tine insuti\n");
              eroare = 1;
              break;
            }
            else
            {
              //verificam daca suntem prieteni cu clientul tinta

              snprintf(sql, 1024, "select 1 from friends where owner like '%s' and friend like '%s';", nume, this_user);

              ok = 0;

              printf("SQL: %s\n", sql);

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              if(ok == 0)
              {
                snprintf(buffer, 2024, "Nu sunteti prieten cu %s, sau utilizatorul %s nu exista\n", nume, nume);
                eroare = 1;
                break;
              }
              else
              {
                //verificam daca clientul tinta a fost mentionat deja in lista noastra

                ok = 0;
                for(int k = 0; k < useri_size - 1; k++)
                {
                  if(!strcmp(nume, useri[k]))
                    ok = 1;
                }

                if(ok == 1)
                {
                  snprintf(buffer, 2024, "Nu puteti mentiona un utilizator de doua ori\n");
                  eroare = 1;
                  break;
                }
              }
            }
          }

          if(eroare == 0)
          {
            if(useri_size == 1)
              strcpy(buffer, "Nu poti crea o conversatie doar cu tine");
            else
            {
              snprintf(sql, 1024, "select ID from rooms r1 where size=%d and participant in ('%s'", useri_size, useri[0]);

              for(int k = 1; k < useri_size; k++)
              {
                strcat(sql, ", '");
                strcat(sql, useri[k]);
                strcat(sql, "'");
              }
              strcat(sql, ") and not exists (select * from rooms r2 where participant not in ('");

              strcat(sql, useri[0]);
              strcat(sql, "'");

              for(int k = 1; k < useri_size; k++)
              {
                strcat(sql, ", '");
                strcat(sql, useri[k]);
                strcat(sql, "'");
              }

              strcat(sql, ") and r1.ID=r2.ID);");

              aux = 0;

              printf("SQL: %s\n", sql);

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, get_id, 0, &mesaj_eroare);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }
              if(aux != 0)
              {
                //intram in conversatia deja existenta
                strcpy(buffer, "");
                snprintf(sql, 1024, "insert into chats values (%d, '%s');", aux, this_user);

                printf("SQL: %s\n", sql);

                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                  fprintf(stdout, "Operation completed successfully\n");
                }

                //afisam toate mesajele existente in conversatie

                snprintf(sql, 1024, "select * from messages where id=%d;", aux);

                printf("SQL: %s\n", sql);

                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, display_chat, 0, &mesaj_eroare);

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                  fprintf(stdout, "Operation completed successfully\n");
                }
              }
              else
              {
                //cream un room
                //obtinem un nr pt conversatie valid

                snprintf(sql, 1024, "select * from rooms;");

                maxi = 0;

                printf("SQL: %s\n", sql);

                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, get_max_id, 0, &mesaj_eroare);

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                  fprintf(stdout, "Operation completed successfully\n");
                }

                int conv_id = maxi+1;

                //adaugam clientul in conversatie si in room

                snprintf(sql, 1024, "insert into rooms (id, participant, size) values (%d, '%s', %d);", conv_id, this_user, useri_size);

                printf("SQL: %s\n", sql);

                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                  fprintf(stdout, "Operation completed successfully\n");
                }

                snprintf(sql, 1024, "insert into chats values (%d, '%s');", conv_id, this_user);

                printf("SQL: %s\n", sql);

                mesaj_eroare = 0;

                rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                if(rc != SQLITE_OK )
                {
                  fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                  sqlite3_free(mesaj_eroare);
                } 
                else 
                {
                  fprintf(stdout, "Operation completed successfully\n");
                }

                //cream room-ul si adaugam clientii activi in chat

                for(int l = 1; l < useri_size; l++)
                {
                  //adaugam in room
                  snprintf(sql, 1024, "insert into rooms (id, participant, size) values (%d, '%s', %d);", conv_id, useri[l], useri_size);

                  printf("SQL: %s\n", sql);

                  mesaj_eroare = 0;

                  rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                  if(rc != SQLITE_OK )
                  {
                    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                    sqlite3_free(mesaj_eroare);
                  } 
                  else 
                  {
                    fprintf(stdout, "Operation completed successfully\n");
                  }

                  snprintf(sql, 1024, "select 1 from chats where participant like '%s';", useri[l]);

                  ok = 0;

                  printf("SQL: %s\n", sql);

                  mesaj_eroare = 0;

                  rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

                  if(rc != SQLITE_OK )
                  {
                    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                    sqlite3_free(mesaj_eroare);
                  } 
                  else 
                  {
                    fprintf(stdout, "Operation completed successfully\n");
                  }

                  int ok1 = ok;

                  ok = 0;

                  snprintf(sql, 1024, "select 1 from logs where name like '%s';", useri[l]);

                  printf("SQL: %s\n", sql);

                  mesaj_eroare = 0;

                  rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

                  if(rc != SQLITE_OK )
                  {
                    fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                    sqlite3_free(mesaj_eroare);
                  } 
                  else 
                  {
                    fprintf(stdout, "Operation completed successfully\n");
                  }

                  int ok2 = ok;

                  if(ok1 == 0 && ok2 == 1)
                  {
                    //adaugam clientul tinta in conversatie si ii trimitem o notificare cu acest lucru
                    //adaugam

                    snprintf(sql, 1024, "insert into chats values (%d, '%s');", conv_id, useri[l]);

                    printf("SQL: %s\n", sql);

                    mesaj_eroare = 0;

                    rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

                    if(rc != SQLITE_OK )
                    {
                      fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                      sqlite3_free(mesaj_eroare);
                    } 
                    else 
                    {
                      fprintf(stdout, "Operation completed successfully\n");
                    }
                  }
                }
                strcpy(buffer, "");
                notificare_join_chat(conv_id);
              }
            }
          }
        }
      }

      else if(!strcmp(command, "get_admin"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          if(is_admin(this_user))
          {
            strcpy(buffer, "Sunteti deja admin\n");
          }
          else
          {
            char parola[100];
            int u = 0;
            for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
              parola[u++] = buffer[j];
            parola[u] = 0;
            
            if(strcmp(parola, "123"))
              strcpy(buffer, "Parola gresita");
            else
            {
              strcpy(buffer, "Permisiune de admin obtinuta cu succes\n");
              snprintf(sql, 2024, "update users set admin=1 where name like '%s';", this_user);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }
            }
          }
        }
      }

      else if(!strcmp(command, "rm_user"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          if(!is_admin(this_user))
          {
            strcpy(buffer, "Nu sunteti admin\n");
          }
          else
          {
            char nume[100];
            int u = 0;
            for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
            {
              nume[u++] = buffer[j];
              if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
                nume[u-1] = nume[u-1] + 32;
            }
            nume[u] = 0;

            snprintf(sql, 2024, "select 1 from users where name like '%s';", nume);

            printf("SQL: %s\n", sql);

            ok = 0;

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
            
            if(ok == 0)
              strcpy(buffer, "Utilizatorul nu exista\n");
            else
            {
              strcpy(buffer, "Utilizator sters cu succes\n");
              snprintf(sql, 2024, "delete from users where name like '%s';", nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              snprintf(sql, 2024, "delete from friends where owner like '%s' or friend like '%s';", nume, nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              snprintf(sql, 2024, "delete from posts where owner like '%s';", nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              snprintf(sql, 2024, "delete from logs where name like '%s';", nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              snprintf(sql, 2024, "delete from friend_requests where name like '%s' or request_from like '%s';", nume, nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              snprintf(sql, 2024, "delete from chats where ID in (select ID from chats where participant like '%s');", nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }

              snprintf(sql, 2024, "delete from rooms where ID in (select ID from rooms where participant like '%s');", nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }
            }
          }
        }
      }

      else if(!strcmp(command, "rm_admin"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          if(!is_admin(this_user))
          {
            strcpy(buffer, "Nu sunteti admin\n");
          }
          else
          {
            strcpy(buffer, "Permisiune de admin scoasa cu succes\n");
            snprintf(sql, 2024, "update users set admin=0 where name like '%s';", this_user);

            printf("SQL: %s\n", sql);

            ok = 0;

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
          }
        }
      }

      else if(!strcmp(command, "set_public_profile"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          if(is_public(this_user))
          {
            strcpy(buffer, "Sunteti deja public\n");
          }
          else
          {
            strcpy(buffer, "Profilul tau este acum public\n");
            snprintf(sql, 2024, "update users set public=1 where name like '%s';", this_user);

            printf("SQL: %s\n", sql);

            ok = 0;

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
          }
        }
      }

      else if(!strcmp(command, "set_private_profile"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          if(!is_public(this_user))
          {
            strcpy(buffer, "Sunteti deja privat\n");
          }
          else
          {
            strcpy(buffer, "Profilul tau este acum privat\n");
            snprintf(sql, 2024, "update users set public=0 where name like '%s';", this_user);

            printf("SQL: %s\n", sql);

            ok = 0;

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
          }
        }
      }

      else if(!strcmp(command, "add_close_friend"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          char nume[100];
          int u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          {
            nume[u++] = buffer[j];
            if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
              nume[u-1] = nume[u-1] + 32;
          }
          nume[u] = 0;

          snprintf(sql, 2024, "select 1 from friends where owner like '%s' and friend like '%s';", this_user, nume);

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
          
          if(ok == 0)
          {
            strcpy(buffer, "Utilizator inexistent in lista de prieteni\n");
          }
          else
          {
            if(is_close_friend(this_user, nume))
              snprintf(buffer, 2024, "%s este deja in lista de prieteni apropiati\n", nume);
            else
            {
              snprintf(buffer, 2024, "%s a fost adaugat la lista de prieteni apropiati\n", nume);

              snprintf(sql, 2024, "update friends set category=2 where owner like '%s' and friend like '%s';", this_user, nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }
            }
          }
        }
      }

      else if(!strcmp(command, "rm_close_friend"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          char nume[100];
          int u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          {
            nume[u++] = buffer[j];
            if(nume[u - 1] >= 'A' && nume[u - 1] <= 'Z')
              nume[u-1] = nume[u-1] + 32;
          }
          nume[u] = 0;

          snprintf(sql, 2024, "select 1 from friends where owner like '%s' and friend like '%s';", this_user, nume);

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
          
          if(ok == 0)
          {
            strcpy(buffer, "Utilizator inexistent in lista de prieteni\n");
          }
          else
          {
            if(!is_close_friend(this_user, nume))
              snprintf(buffer, 2024, "%s nu este in lista de prieteni apropiati\n", nume);
            else
            {
              snprintf(buffer, 2024, "%s a fost eliminat din lista de prieteni apropiati\n", nume);

              snprintf(sql, 2024, "update friends set category=1 where owner like '%s' and friend like '%s';", this_user, nume);

              printf("SQL: %s\n", sql);

              ok = 0;

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              printf("Ok = %d\n", ok);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }
            }
          }
        }
      }

      else if(!strcmp(command, "make_post"))
      {
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {

          //stocare optiune dorita
          int u = 0;

          char optiune[1024];
          for(j = j+1; buffer [j] != ' ' && buffer[j] != 0; j++)
            optiune[u++] = buffer[j];
          optiune[u] = 0;

          //stocare titlu dorit
          
          char titlu[100];
          u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
            titlu[u++] = buffer[j];
          titlu[u] = 0;

          //stocare continut dorit

          char continut[1024];
          u = 0;
          for(j = j+1; buffer[j] != 0; j++)
            continut[u++] = buffer[j];
          continut[u] = 0;

          //verificam daca o postare cu aceeasi nume, a aceluiasi utilizator, exista deja

          get_user(this_user, cl);

          snprintf(sql, 2024, "select 1 from posts where owner like '%s' and titlu like '%s';", this_user, titlu);

          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }

          if(ok == 1)
            strcpy(buffer, "Aveti deja o postare cu acest titlu\n");
          else
          {

            int option = -1;

            if(!strcmp(optiune, "0"))
              option = 0;
            else if(!strcmp(optiune, "1"))
              option = 1;
            if(option == -1)
              snprintf(buffer, 2024, "Optiune invalida:--%s--\n", optiune);
            else
            {
              //adaugam postarea
              strcpy(buffer, "Postare creata cu succes\n");

              snprintf(sql, 2024, "insert into posts (owner, public, titlu, content) values ('%s', %d, '%s', '%s');", this_user, option, titlu, continut);

              printf("SQL: %s\n", sql);

              mesaj_eroare = 0;

              rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

              if(rc != SQLITE_OK )
              {
                fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
                sqlite3_free(mesaj_eroare);
              } 
              else 
              {
                fprintf(stdout, "Operation completed successfully\n");
              }
            }
          }
        }
      }

      else if(!strcmp(command, "view_posts"))
      {
        char nume[100];
        int u = 0;
        for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
          nume[u++] = buffer[j];
        nume[u] = 0;

        strcpy(sql, "select 1 from users where name like '");
        strcat(sql, nume);
        strcat(sql, "';");

        printf("SQL: %s\n", sql);

        ok = 0;

        mesaj_eroare = 0;

        rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

        printf("Ok = %d\n", ok);

        if(rc != SQLITE_OK )
        {
          fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
          sqlite3_free(mesaj_eroare);
        } 
        else 
        {
          fprintf(stdout, "Operation completed successfully\n");
        }
          
        if(ok == 0)
        {
          strcpy(buffer, "Utilizator inexistent\n");
        }
        else
        {
          get_user(this_user, cl);
          if(!logged_in(cl))
          {
            snprintf(buffer, 2024, "Postarile lui %s:\n", nume);
            view_posts(nume, 0);
          }
          else if(!strcmp(this_user, nume))
          {
            strcpy(buffer, "Postarile tale:\n");
            view_posts(this_user, 1);
          }
          else
          {
            snprintf(buffer, 2024, "Postarile lui %s:\n", nume);
            //verificam daca deja suntem prieteni cu celalat user
            snprintf(sql, 1024, "select * from friends where owner like '%s' and friend like '%s';", this_user, nume);

            ok = 0;

            printf("SQL: %s\n", sql);

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
            view_posts(nume, ok);
          }
        }
      }

      else if(!strcmp(command, "make_news"))
      {
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {

          //stocare optiune dorita
          int u = 0;

          char optiune[1024];
          for(j = j+1; buffer [j] != ' ' && buffer[j] != 0; j++)
            optiune[u++] = buffer[j];
          optiune[u] = 0;

          //stocare titlu dorit
          
          char titlu[100];
          u = 0;
          for(j = j+1; buffer[j] != ' ' && buffer[j] != 0; j++)
            titlu[u++] = buffer[j];
          titlu[u] = 0;

          //stocare continut dorit

          char continut[1024];
          u = 0;
          for(j = j+1; buffer[j] != 0; j++)
            continut[u++] = buffer[j];
          continut[u] = 0;

          get_user(this_user, cl);
            int option = -1;

          if(!strcmp(optiune, "0"))
            option = 0;
          else if(!strcmp(optiune, "1"))
            option = 1;
          else if(!strcmp(optiune, "2"))
            option = 2;
          if(option == -1)
            snprintf(buffer, 2024, "Optiune invalida:--%s--\n", optiune);
          else
          {
            //adaugam postarea
            strcpy(buffer, "Stire creata cu succes\n");

            n_broadcast(this_user, option, titlu, continut);
          }
        }
      }

      else if(!strcmp(command, "set_public_post"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          int u = 0;

          char nume[1024];
          for(j = j+1; buffer [j] != ' ' && buffer[j] != 0; j++)
            nume[u++] = buffer[j];
          nume[u] = 0;

          //verificam daca exista postarea respectiva
          snprintf(sql, 2024, "select 1 from posts where owner like '%s' and titlu like '%s' and public=0;", this_user, nume);
          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
          if(ok == 0)
            strcpy(buffer, "Nu aveti o postare privata cu acest nume\n");
          else
          {
            strcpy(buffer, "Postarea este acum publica\n");

            snprintf(sql, 2024, "update posts set public=1 where owner like '%s' and titlu like '%s';", this_user, nume);
            printf("SQL: %s\n", sql);

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
          }
        }
      }

      else if(!strcmp(command, "set_private_post"))
      {
        get_user(this_user, cl);
        if(logged_in(cl) == 0)
          strcpy(buffer, "Nu sunteti logat\n");
        else
        {
          int u = 0;

          char nume[1024];
          for(j = j+1; buffer [j] != ' ' && buffer[j] != 0; j++)
            nume[u++] = buffer[j];
          nume[u] = 0;

          //verificam daca exista postarea respectiva
          snprintf(sql, 2024, "select 1 from posts where owner like '%s' and titlu like '%s' and public=1;", this_user, nume);
          printf("SQL: %s\n", sql);

          ok = 0;

          mesaj_eroare = 0;

          rc = sqlite3_exec(db, sql, macar_o_valoare, 0, &mesaj_eroare);

          printf("Ok = %d\n", ok);

          if(rc != SQLITE_OK )
          {
            fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
            sqlite3_free(mesaj_eroare);
          } 
          else 
          {
            fprintf(stdout, "Operation completed successfully\n");
          }
          if(ok == 0)
            strcpy(buffer, "Nu aveti o postare publica cu acest nume\n");
          else
          {
            strcpy(buffer, "Postarea este acum privata\n");

            snprintf(sql, 2024, "update posts set public=0 where owner like '%s' and titlu like '%s';", this_user, nume);
            printf("SQL: %s\n", sql);

            mesaj_eroare = 0;

            rc = sqlite3_exec(db, sql, callback, 0, &mesaj_eroare);

            printf("Ok = %d\n", ok);

            if(rc != SQLITE_OK )
            {
              fprintf(stderr, "SQL error: %s\n", mesaj_eroare);
              sqlite3_free(mesaj_eroare);
            } 
            else 
            {
              fprintf(stdout, "Operation completed successfully\n");
            }
          }
        }
      }

      else
      {
        strcpy(buffer, "Comanda invalida\n");
      }
    }
    /* returnam mesajul clientului */
    if (send (cl, buffer, strlen(buffer), 0) <= 0)
    {
      printf("[Thread %d] ",i);
      perror ("[Thread]Eroare la write() catre client.\n");
    // continue;		/* continuam sa ascultam */
    }
    sqlite3_close(db);
    return 1;
  }
}

void *treat(void * arg)
{
  char this_user[100];
  while(1)
	{
	  fflush (stdout);
    if(!raspunde((int)arg, this_user))
      break;
  }
	/* am terminat cu acest client, inchidem conexiunea */
	close ((int)arg);
	return(NULL);	
}

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
  sqlite3 *db;
  int rc;

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror ("[server]Eroare la socket().\n");
    return errno;
  }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  {
    perror ("[server]Eroare la bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 10) == -1)
  {
    perror ("[server]Eroare la listen().\n");
    return errno;
  }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
  {
    int client;
    int length = sizeof (from);

    printf ("[server]Asteptam la portul %d...\n",PORT);
    fflush (stdout);

    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	  {
	    perror ("[server]Eroare la accept().\n");
	    continue;
	  }

    printf ("[server]S-a conectat clientul %d...\n", client);
    fflush (stdout);
    
    /* s-a realizat conexiunea, se astepta mesajul */
    i++;
    // cream thread-ul
	  pthread_create(&th[i], NULL, &treat, (void*) (int)client);
	}
  sqlite3_close(db);  

  return 0;
  
};