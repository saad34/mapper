#include <algorithm>
#include <random>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"
#include "LatLon.h"
#include "OSMDatabase.h"

SUITE(distances) {

    struct PointsOnMap {
        const LatLon BloorAtBathurst;
        const LatLon HarbordAtBathurst;
        const std::string intersection1;
        const std::string streetName1;
        const std::string streetName2;
        PointsOnMap() :
            BloorAtBathurst(43.665133, -79.411218),
            HarbordAtBathurst(43.661516, -79.409759),
            intersection1("Bloor Street West & Bathurst Street"),
            streetName1("Yonge Street"),
            streetName2("College Street"){
        }

    };
    
    TEST_FIXTURE(PointsOnMap, distanceLatLon){
        double expected = 420;
        double actual = find_distance_between_two_points(BloorAtBathurst, HarbordAtBathurst);
        CHECK(relative_error(expected, actual) < 0.05);
        
        expected = 245.2059793921245;
        actual = find_distance_between_two_points(LatLon(43.64992, -79.38075000000001), LatLon(43.64916, -79.37788999999999));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 223.8238412168463;
        actual = find_distance_between_two_points(LatLon(43.67026, -79.38678), LatLon(43.66972, -79.38946));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 4857.506545485718;
        actual = find_distance_between_two_points(LatLon(43.68814, -79.39412), LatLon(43.64528, -79.38253));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 1006.653578635471;
        actual = find_distance_between_two_points(LatLon(43.65044, -79.37841), LatLon(43.65909, -79.38209000000001));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 2501.190986258448;
        actual = find_distance_between_two_points(LatLon(43.67103, -79.38312999999999), LatLon(43.64864, -79.38024));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 1304.44114716316;
        actual = find_distance_between_two_points(LatLon(43.65481, -79.38848), LatLon(43.66408, -79.39841));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 1287.545927657807;
        actual = find_distance_between_two_points(LatLon(43.66081, -79.38583), LatLon(43.64987, -79.39106));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 1469.49560721438;
        actual = find_distance_between_two_points(LatLon(43.66493, -79.38455), LatLon(43.65187, -79.38179));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 2784.653422257717;
        actual = find_distance_between_two_points(LatLon(43.6687, -79.39413), LatLon(43.64398, -79.38865));
        CHECK(relative_error(expected, actual) < 0.05);
    }
    
    TEST_FIXTURE(PointsOnMap, streetLength){
        double distCollege = find_street_length(streetName2);
        CHECK(distCollege > 4500 && distCollege < 5500);
        
        double expected = 40036.39898647602;
        double actual = find_street_length("Dufferin Street");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 35008.54471229232;
        actual = find_street_length("Weston Road");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 28319.65301328929;
        actual = find_street_length("Sheppard Avenue East");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 25872.81391299298;
        actual = find_street_length("Finch Avenue West");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 46733.03225770919;
        actual = find_street_length("Steeles Avenue East");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 13660.50099305937;
        actual = find_street_length("William R. Allen Road");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 23926.28816337195;
        actual = find_street_length("Lake Shore Boulevard West");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 23680.64178126276;
        actual = find_street_length("Erin Mills Parkway");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 14991.16402220647;
        actual = find_street_length("Morningside Avenue");
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 27614.30731837841;
        actual = find_street_length("McCowan Road");
        CHECK(relative_error(expected, actual) < 0.05);
    }
    
    TEST_FIXTURE(PointsOnMap, PointOfInterest){
        UNITTEST_TIME_CONSTRAINT(20);
        string poi = find_closest_point_of_interest(BloorAtBathurst);
        CHECK_EQUAL("Church St & Bloor St E", find_closest_point_of_interest(LatLon(43.67103, -79.38312999999999)));
        CHECK_EQUAL("BMO Bank of Montreal", find_closest_point_of_interest(LatLon(43.64864, -79.38024)));
        CHECK_EQUAL("St. Patrick", find_closest_point_of_interest(LatLon(43.65481, -79.38848)));
        CHECK_EQUAL("Joseph L. Rotman Library", find_closest_point_of_interest(LatLon(43.66408, -79.39841)));
        CHECK_EQUAL("Timothy's World Coffee", find_closest_point_of_interest(LatLon(43.66081, -79.38583)));
        CHECK_EQUAL("Starbucks Coffee", find_closest_point_of_interest(LatLon(43.64987, -79.39106)));
        CHECK_EQUAL("Not Just Noodles", find_closest_point_of_interest(LatLon(43.66493, -79.38455)));
        CHECK_EQUAL("Starbucks Coffee", find_closest_point_of_interest(LatLon(43.65187, -79.38179)));
        CHECK_EQUAL("Second Cup", find_closest_point_of_interest(LatLon(43.6687, -79.39413)));
        CHECK_EQUAL("CN Tower/Ripley's Aquarium Canada", find_closest_point_of_interest(LatLon(43.64398, -79.38865)));
        CHECK_EQUAL("La Prep", find_closest_point_of_interest(LatLon(43.64638, -79.38408)));
    }   
}