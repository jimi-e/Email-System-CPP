#include "server_net.h"
#include "mail.h"
#include "identify.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define pth_num 50
#define SERVERIP "192.168.43.38"
#define SERVERPORT 5000

server_base server;
Mail mail;
Identify identify;

//typedef void (*signal_handler)(int);
typedef struct pthread_block{
	bool used;
	int sockfd;
	pthread_t p;
} spth;

spth pth[pth_num];
sem_t available_pth;
int sock;

void *client_process(void* args);

void signal_handler_fun(int sig) {
	close(sock);
	exit(sig);
}

int main(){
    sockaddr_in addr;
	sock = server.run_sock(SERVERIP, SERVERPORT, addr);
	signal(SIGINT, signal_handler_fun);
	sem_init(&available_pth, 0, pth_num);
	///
	int addr_size = sizeof(addr), p_pth;
	bzero(&addr, addr_size);
	while(1){
		int new_sock = accept(sock,(sockaddr *)&addr, (socklen_t*)&addr_size);
		if(new_sock<0){ printf("accept err\n"); exit(1);}
		sem_wait(&available_pth);
		p_pth = 0;
		while (pth[p_pth].used) p_pth++;
		pth[p_pth].used = true;
		pth[p_pth].sockfd = new_sock;
		if(pthread_create(&pth[p_pth].p, NULL, client_process, (void*)&p_pth)!=0) {printf("pth err!\n"); exit(1);}
	}
	///
	close(sock);
	return 0;
}

void *client_process(void *args){
	int p_pth = *((int *)args);
	int sock = pth[p_pth].sockfd;
	std::cout<<"accept pthread "<<p_pth<<std::endl;
    std::string recv_buf;
	while(1){
        server.receive_str(sock, recv_buf);
		std::cout<<"pthread"<<p_pth<<":  "<<recv_buf<<std::endl;
        if(recv_buf=="gettext"){
            if(mail.send_text(sock)==2) break;
        }else if(recv_buf=="getlist"){
            if(mail.send_list(sock)==-1) break;
        }else if(recv_buf=="login"){
			if(identify.check_user(sock)==2) break;
		}else if(recv_buf=="register"){
			if(identify.register_user(sock)==2) break;
		}else if(recv_buf=="sendmail"){
			if(mail.receive_mail(sock)==2) break;
		}else if(recv_buf=="deletemail"){
			if(mail.delete_mail(sock)==2) break;
		}else if(recv_buf=="getattach"){
			if(mail.send_attach(sock)==-1) break;
		}
        if(recv_buf=="close") break;
    }
	std::cout<<"close pthread "<<p_pth<<std::endl<<std::endl;
	close(sock);
	pth[p_pth].used = false;
	sem_post(&available_pth);
	pthread_exit(NULL);
}
