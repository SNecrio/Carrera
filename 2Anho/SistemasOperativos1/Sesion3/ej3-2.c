#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

static void gestion(int sig) {
    if (sig == SIGUSR1) {
        printf("Proceso %d: SIGUSR1 recibida.\n", getpid());
    } else if (sig == SIGUSR2) {
        printf("Proceso %d: SIGUSR2 recibida.\n", getpid());
    }
}

int main() {

  int padre;
  int hijo;

  if (signal(SIGUSR1, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");
  if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");
    padre=getpid();
  if ((hijo = fork()) == 0) { /*Trabajo del hijo*/
    kill(padre,SIGUSR1); /*Envia señal al padre*/
    for (;;); /*Espera una señal del padre indefinidamente*/
  }
  else {
    sleep(1); 
    
    kill(hijo, SIGUSR2);
    printf("Padre envía señal SIGUSR2 al hijo.\n");

    sleep(1);

    kill(hijo, SIGTERM);
    printf("Padre envía señal SIGTERM al hijo.\n");

    wait(NULL);
    printf("El proceso hijo ha terminado.\n");
  }
}