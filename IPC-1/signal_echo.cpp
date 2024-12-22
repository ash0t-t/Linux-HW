#define _XOPEN_SOURCE 700 

#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ucontext.h>

void signal_handler(int signal, siginfo_t *info, void *context) {
    if (signal == SIGUSR1) {
        pid_t sender_pid = info->si_pid;  
        uid_t sender_uid = info->si_uid; 
        struct passwd *pwd = getpwuid(sender_uid); 

        std::string sender_name = pwd ? pwd->pw_name : "unknown";

#if defined(__arm64__) || defined(__aarch64__)
        ucontext_t *uc = static_cast<ucontext_t *>(context);
        auto &regs = uc->uc_mcontext->__ss;  

        auto pc = regs.__pc; 
        auto x0 = regs.__x[0]; 
        auto x1 = regs.__x[1];

        std::cout << "seceived a SIGUSR1 signal from process: " << sender_pid
                  << " executed by:  " << sender_uid << " (" << sender_name << ").\n";
        std::cout << "state of the context: PC = 0x" << std::hex << pc
                  << ", X0 = 0x" << x0 << ", X1 = 0x" << x1 << ".\n";
#else
        std::cerr << "signal handling for this architecture is not done\n";
#endif
    }
}

int main() {
    pid_t pid = getpid();
    std::cout << "pid: " << pid << std::endl;

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signal_handler;
    sigaction(SIGUSR1, &sa, nullptr);

    while (true) {
        std::cout << "waiting for SIGUSR1 signal..." << std::endl;
        sleep(10);  
    }

    return 0;
}