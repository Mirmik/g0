#include <g0/service.h> 
#include <g0/test_service.h>
#include <g0/gate/udp.h>

#include <fcntl.h>
#include <signal.h>

#include <unistd.h>

g0::test_service test0("test");
g0::udp_gate ugate(11002);

void signal_callback_handler(int sig) {
	ugate.read_handler();
	exit(1);
}

int main() {
	int ufd = ugate.get_fd();

	fcntl(ufd, F_SETOWN, getpid());
	fcntl(ufd, F_SETSIG, SIGUSR1);
	fcntl(ufd,F_SETFL,fcntl(ufd,F_GETFL) | O_NONBLOCK | O_ASYNC); 
   	signal(SIGUSR1, signal_callback_handler);

	g0::registry_service(&test0, 6);
	g0::registry_service(&ugate, 7);

	g0::id_t ugate_id = ugate.add("127.0.0.1", 11001);

	g0::message* msg = new g0::message;
	
	g0::id_t raddr[] = {7, ugate_id, 0xFFFF};
	g0::message_init(msg, raddr, 6, "helloworld2", 11);

	g0::transport(msg, test0.id);

	while(1) {}
}