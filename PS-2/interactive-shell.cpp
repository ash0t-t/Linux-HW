#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

void execute_command(const std::vector<std::string>& args, bool silent) {
    if (args.empty()) return;

    std::vector<char*> argv;
    for (const auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fail on fork" << std::endl;
        return;
    } else if (pid == 0) {
        if (silent) {
            std::string log_file = std::to_string(getpid()) + ".log";
            int fd = open(log_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                std::cerr << "failed on log file" << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        std::string current_path = getenv("PATH");
        std::string new_path = "PATH=." + current_path;
        putenv(const_cast<char*>(new_path.c_str()));

        execvp(argv[0], argv.data());
        std::cerr << "failes on command: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            std::cout << "exit with " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "fail on exit" << std::endl;
        }
    }
}

int main() {
    std::string input;
    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;
        if (input == "exit") {
            std::cout << "exiting shell" << std::endl;
            break;
        }

        std::istringstream iss(input);
        std::vector<std::string> args;
        std::string word;
        while (iss >> word) {
            args.push_back(word);
        }

        bool silent = false;
        if (!args.empty() && args[0] == "silent") {
            silent = true;
            args.erase(args.begin());
        }

        execute_command(args, silent);
    }

    return EXIT_SUCCESS;
}