#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int nth_prime(int n) {
    int count = 0, candidate = 1;
    while (count < n) {
        ++candidate;
        if (is_prime(candidate)) ++count;
    }
    return candidate;
}

int main() {
    int pipe_parent_to_child[2], pipe_child_to_parent[2];
    pipe(pipe_parent_to_child);
    pipe(pipe_child_to_parent);

    pid_t pid = fork();
    if (pid == 0) {
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        while (true) {
            int m;
            if (read(pipe_parent_to_child[0], &m, sizeof(m)) <= 0) break;
            int result = nth_prime(m);
            write(pipe_child_to_parent[1], &result, sizeof(result));
        }
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
    } else {
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
        while (true) {
            std::string input;
            std::cout << "[parent] please enter the number: ";
            std::cin >> input;
            if (input == "exit") break;
            int m = std::stoi(input);
            std::cout << "[parent] sending " << m << " to the child process\n";
            write(pipe_parent_to_child[1], &m, sizeof(m));
            std::cout << "[parent] waiting for the response from the child process\n";
            int result;
            read(pipe_child_to_parent[0], &result, sizeof(result));
            std::cout << "[parent] received calculation result of prime(" << m << ") = " << result << "...\n";
        }
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        wait(nullptr);
    }
    return 0;
}