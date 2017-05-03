//Created by ss107 on 4/21/17.


#include <metadata_parser.h>
#include <kmeans/kmeans.h>
#include <iomanip>

int main() {

    metadata_parser mp;

    kmeans km(mp.get_window_size(), mp.get_max_clustering_iterations(), mp.get_clustering_precision(), mp.get_transitions_amount(), mp.get_threshold());

    circular_queue cq(10);

    double values[] = { -0.00, 0.00, 0.01, -0.00, -0.00, -0.05, -0.04, 0.02, -0.02, -0.01};

    for(int i = 0; i < 10; i++) {
        cq.next_point().x = values[i];
        cq.point_inserted();
    }

    bool x = km.perform_all_calculation(&cq, 3);

    printf("%lf\n", km.get_result_threshold());

    return 0;
}