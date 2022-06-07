/*****
 * test_config.cc
 * 
 * See LICENSE for details.
 *
 *****/

#include "external/catch.hpp"

#include "bridge_sim.h"
#include "config.h"

TEST_CASE("basic", "[basic]")
{
    const std::string contents = R"(
bridges:
  -
    length: 100
    people:
      -
        name: A
        speed: 100
      -
        name: B
        speed: 50
      -
        name: C
        speed: 20
      -
        name: D
        speed: 10
  -
    length: 250
    people:
      -
        name: E
        speed: 2.5
  -
    length: 150
    people:
      -
        name: F
        speed: 25
      -
        name: G
        speed: 15

)";

    BridgeSim::State state = BridgeSim::loadInitialState(contents);

    REQUIRE( state.m_bridges.size() == 3 );
    REQUIRE( state.m_bridges[0].m_id == 0 );
    REQUIRE( state.m_bridges[0].m_length == 100 );
    REQUIRE( state.m_bridges[1].m_id == 1 );
    REQUIRE( state.m_bridges[1].m_length == 250 );
    REQUIRE( state.m_bridges[2].m_id == 2 );
    REQUIRE( state.m_bridges[2].m_length == 150 );

    REQUIRE( state.m_persons.size() == 7 );

    // The persons are sorted by speed, so the order
    // will be different from the yaml order
    REQUIRE( state.m_persons[0].m_id == 0 );
    REQUIRE( state.m_persons[0].m_name == "E" );
    REQUIRE( state.m_persons[0].m_speed == 2.5 );
    REQUIRE( state.m_persons[0].m_cost.size() == 3 );
    REQUIRE( state.m_persons[0].m_cost[0] == 40 );
    REQUIRE( state.m_persons[0].m_cost[1] == 100 );
    REQUIRE( state.m_persons[0].m_cost[2] == 60 );

    REQUIRE( state.m_persons[1].m_id == 1 );
    REQUIRE( state.m_persons[1].m_name == "D" );
    REQUIRE( state.m_persons[1].m_speed == 10 );
    REQUIRE( state.m_persons[1].m_cost.size() == 3 );
    REQUIRE( state.m_persons[1].m_cost[0] == 10 );
    REQUIRE( state.m_persons[1].m_cost[1] == 25 );
    REQUIRE( state.m_persons[1].m_cost[2] == 15 );

    REQUIRE( state.m_persons[2].m_id == 2 );
    REQUIRE( state.m_persons[2].m_name == "G" );
    REQUIRE( state.m_persons[2].m_speed == 15 );
    REQUIRE( state.m_persons[2].m_cost.size() == 3 );

    REQUIRE( state.m_persons[3].m_id == 3 );
    REQUIRE( state.m_persons[3].m_name == "C" );
    REQUIRE( state.m_persons[3].m_speed == 20 );
    REQUIRE( state.m_persons[3].m_cost.size() == 3 );
    REQUIRE( state.m_persons[3].m_cost[0] == 5 );
    REQUIRE( state.m_persons[3].m_cost[1] == 12.5 );
    REQUIRE( state.m_persons[3].m_cost[2] == 7.5 );

    REQUIRE( state.m_persons[4].m_id == 4 );
    REQUIRE( state.m_persons[4].m_name == "F" );
    REQUIRE( state.m_persons[4].m_speed == 25 );
    REQUIRE( state.m_persons[4].m_cost.size() == 3 );
    REQUIRE( state.m_persons[4].m_cost[0] == 4 );
    REQUIRE( state.m_persons[4].m_cost[1] == 10 );
    REQUIRE( state.m_persons[4].m_cost[2] == 6 );

    REQUIRE( state.m_persons[5].m_id == 5 );
    REQUIRE( state.m_persons[5].m_name == "B" );
    REQUIRE( state.m_persons[5].m_speed == 50 );
    REQUIRE( state.m_persons[5].m_cost.size() == 3 );
    REQUIRE( state.m_persons[5].m_cost[0] == 2 );
    REQUIRE( state.m_persons[5].m_cost[1] == 5 );
    REQUIRE( state.m_persons[5].m_cost[2] == 3 );

    REQUIRE( state.m_persons[6].m_id == 6 );
    REQUIRE( state.m_persons[6].m_name == "A" );
    REQUIRE( state.m_persons[6].m_speed == 100 );
    REQUIRE( state.m_persons[6].m_cost.size() == 3 );
    REQUIRE( state.m_persons[6].m_cost[0] == 1 );
    REQUIRE( state.m_persons[6].m_cost[1] == 2.5 );
    REQUIRE( state.m_persons[6].m_cost[2] == 1.5 );

}
