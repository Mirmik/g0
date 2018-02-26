#include <gxx/print.h> 
#include <g0/gate/udp.h>
#include <g0/test_service.h>

#include <fcntl.h>
#include <signal.h>

#include <unistd.h>

g0::echo_service echo1("echo");
g0::udp_gate ugate(11001);

void signal_callback_handler(int sig) {
	ugate.read_handler();
}

int main() {
	int ufd = ugate.get_fd();

	fcntl(ufd, F_SETOWN, getpid());
	fcntl(ufd, F_SETSIG, SIGUSR1);
	fcntl(ufd,F_SETFL,fcntl(ufd,F_GETFL) | O_NONBLOCK | O_ASYNC); 
   	signal(SIGUSR1, signal_callback_handler);

	g0::registry_service(&ugate, 8);
	g0::registry_service(&echo1, 0xFFFF);

	while(1) {
		sleep(1);
	}
}