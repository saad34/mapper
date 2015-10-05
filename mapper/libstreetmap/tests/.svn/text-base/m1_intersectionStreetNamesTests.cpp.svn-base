#include <algorithm>
#include <random>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"

TEST(intersectionIDStreetNamesFunctionality) {
        
    std::vector<std::string> result;
    std::vector<std::string> expected_result;

    expected_result = {"Eglinton Avenue East", "Eglinton Avenue West", "Yonge Street"};
    result = find_intersection_street_names(1648);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Bay Street", "Dundas Street West"};
    result = find_intersection_street_names(23162);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Bay Street", "Wellington Street West"};
    result = find_intersection_street_names(58114);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Dundas Street East", "Dundas Street West", "Yonge Street"};
    result = find_intersection_street_names(2298);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Carlton Street", "College Street", "Yonge Street"};
    result = find_intersection_street_names(3023);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Queen Street East", "Queen Street West", "Yonge Street"};
    result = find_intersection_street_names(2522);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Adelaide Street West", "Bay Street"};
    result = find_intersection_street_names(40201);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"King Street East", "King Street West", "Yonge Street"};
    result = find_intersection_street_names(10538);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Bloor Street East", "Bloor Street West", "Yonge Street"};
    result = find_intersection_street_names(1896);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);

    expected_result = {"Bay Street", "Bloor Street West"};
    result = find_intersection_street_names(27814);
    std::sort(result.begin(), result.end());
    CHECK_EQUAL(expected_result, result);
}
