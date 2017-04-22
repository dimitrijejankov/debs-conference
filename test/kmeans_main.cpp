//Created by ss107 on 4/21/17.

#include "kmeans/kmeans.h"
#include "kmeans/circular_queue.h"
#include <fstream>
#include <sstream>

int main() {

    // Parameters for KMeans:
    std::cout << "KMeans Testing!\n";

    // Variable Parameter:
    int numClusters = 5;

    // Other Fixed Parameters:
    int windowSize = 10;
    int maximumIterations = 50;
    double clusteringPrecision = 0.00001;
    int smallerWindowSize = 5;
    double thresholdProbability = 0.005;


    std::ifstream file("31.txt");
    std::string   line;
    circular_queue * cq = new  circular_queue(windowSize);

    int i = 0;

    while(std::getline(file, line)) {
        std::stringstream   lineStream(line);
        std::string         data;


        while(std::getline(lineStream, data, ';')) {
            //std::cout << data << std::endl;
        }
        //std::cout <<  << std::endl;

        sscanf(data.c_str(), "%lf", &cq->next_point().x);
        cq->point_inserted();

        if (cq->is_full()) {
            //cq->display();
            // Create kmeans object:
            kmeans * k = new kmeans(windowSize, maximumIterations, clusteringPrecision, smallerWindowSize, thresholdProbability);
            bool hasAnomalies = k->perform_all_calculation(cq, numClusters);

            i++;

            if (hasAnomalies) {
                std::cout << "Anomaly!" << i << std::endl;
                printf("%.19lf \n", k->get_result_threshold());
                //std::cout << k->get_result_threshold() << std::endl;
                cq->display();
            }
        }



    }






    return 0;
}