#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <sys/wait.h>
#include <unistd.h>

void do_command(char** argv) {
    auto start_time = std::chrono::high_resolution_clock::now();
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(argv[0], argv);
        std::cerr << "fail on execution" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;

        if (WIFEXITED(status)) {
            std::cout << "completed with " << WEXITSTATUS(status) 
                      << " exit code and took: " << duration.count() << " seconds" 
                      << std::endl;
        } else {
            std::cerr << "error on exit" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "use like: " << argv[0] << " <command> [arguments...]" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<char*> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    args.push_back(nullptr);

    do_command(args.data());
    return EXIT_SUCCESS;
}