//Created by ss107 on 4/21/17.

#include <metadata_parser.h>
#include <kmeans/kmeans.h>
#include <chrono>

#define NUMBER_OF_WINDOWS 20
#define WINDOW_SIZE 1000
#define NUMBER_OF_THREADS 4

int main() {

    metadata_parser mp;

    kmeans *kms[NUMBER_OF_THREADS];

    for(int i = 0; i < NUMBER_OF_THREADS; ++i) {
        kms[i] = new kmeans(WINDOW_SIZE, mp.get_max_clustering_iterations(), mp.get_clustering_precision(), mp.get_transitions_amount(), mp.get_threshold());
    }

    circular_queue* queues[NUMBER_OF_WINDOWS];

    for(int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        queues[i] = new circular_queue(WINDOW_SIZE);

        for(int j = 0; j < WINDOW_SIZE; j++) {
            queues[i]->next_point().x = i + j;
            queues[i]->point_inserted();
        }
    }

    bool x = true;

    auto begin = std::chrono::high_resolution_clock::now();

#pragma omp parallel for ordered schedule(dynamic) num_threads(4)
    for(int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        x = x ^ kms[i]->perform_all_calculation(queues[i], 70);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration << "ns." << std::endl;


    for(int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        delete queues[i];
    }

    for(int i = 0; i < NUMBER_OF_THREADS; ++i) {
        delete kms[i];
    }

    return 0;
}