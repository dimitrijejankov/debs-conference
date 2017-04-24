//Created by ss107 on 4/21/17.


#include <metadata_parser.h>
#include <kmeans/kmeans.h>
#include <iomanip>

int main() {

    metadata_parser mp;

    kmeans km(mp.get_window_size(), mp.get_max_clustering_iterations(), mp.get_clustering_precision(), mp.get_transitions_amount(), mp.get_threshold());

    circular_queue cq(10);

    double values[] = {2.97, 3, 2.94, 2.89, 3.02, 2.95, 2.94, 3.05, 3.05, 2.89};

    //double values[] = {2.9699999999999998, 3, 2.9399999999999999, 2.8900000000000001, 3.02, 2.9500000000000002, 2.9399999999999999, 3.0499999999999998, 3.0499999999999998, 2.8900000000000001};

    /*
    double values[] = {2.9399999999999999,
                       3.0499999999999998,
                       3.0499999999999998,
                       2.8900000000000001,
                       2.9699999999999998,
                       3,
                       2.9399999999999999,
                       2.8900000000000001,
                       3.02,
                       2.9500000000000002};
   */

    for(int i = 0; i < 10; i++) {
        cq.next_point().x = values[i];
        cq.point_inserted();
    }

    cq.next = 0;

    bool x = km.perform_all_calculation(&cq, 4);

    printf("%lf\n", km.get_result_threshold());

    return 0;
}