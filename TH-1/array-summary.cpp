#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <numeric>

void partial_sum(const std::vector<int>& array, size_t start, size_t end, long long& result) {
    result = std::accumulate(array.begin() + start, array.begin() + end, 0LL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "use: " << argv[0] << " <array_size> <num_threads>" << std::endl;
        return EXIT_FAILURE;
    }

    size_t N = std::stoul(argv[1]);
    size_t M = std::stoul(argv[2]);

    if (N <= 1000000 || M == 0) {
        std::cerr << "arr size must be greater than 1.000.000 and number of threads must be positive" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<int> array(N);
    for (size_t i = 0; i < N; ++i) {
        array[i] = rand() % 100;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    long long total_sum = std::accumulate(array.begin(), array.end(), 0LL);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_no_threads = end_time - start_time;

    start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(M, 0);

    size_t chunk_size = N / M;
    for (size_t i = 0; i < M; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == M - 1) ? N : start + chunk_size;
        threads.emplace_back(partial_sum, std::cref(array), start, end, std::ref(partial_sums[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    long long threaded_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_with_threads = end_time - start_time;

    std::cout << "without threads" << duration_no_threads.count() << " seconds" << std::endl;
    std::cout << "time with " << M << " threads: " << duration_with_threads.count() << " seconds" << std::endl;

    if (total_sum != threaded_sum) {
        std::cerr << "sums dont match" << std::endl;
    } else {
        std::cout << "sums match" << std::endl;
    }

    return EXIT_SUCCESS;
}