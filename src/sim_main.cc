/*****
 * sim_main.cc
 * 
 * See LICENSE for details.
 *
 *****/

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "bridge_sim.h"
#include "config.h"
#include "strategy.h"

/*************************
 * Helper functions (to read in file)
 *************************/

bool fileExists(const char *filename)
{
    std::ifstream f(filename);
    return f.good();
}

//
// Loads the contents of the yaml file into the std::string
//
size_t fileGetContents(const char *filename, std::string *v)
{
    ::FILE *fp = ::fopen(filename, "rb");
    //C4_CHECK_MSG(fp != nullptr, "could not open file");
    ::fseek(fp, 0, SEEK_END);
    long sz = ::ftell(fp);
    v->resize(static_cast<typename std::string::size_type>(sz));
    if(sz)
    {
        ::rewind(fp);
        size_t ret = ::fread(&(*v)[0], 1, v->size(), fp);
        assert(ret == (size_t)sz);
        //C4_CHECK(ret == (size_t)sz);
    }
    ::fclose(fp);
    return v->size();
}


int main(int argc, char *argv[])
{
    //$ TODO: add command-line parameter to specify strategy

    // There is only one command-line parameter, the name
    // of the input YAML file.
    if (argc != 2 || strlen(argv[1]) == 0)
    {
        std::cerr << "ERROR: Incorrect command-line parameters" << std::endl
                  << "Expected usage: sim <filename>" << std::endl;
        return 1;
    }
    std::cout << "Loading data from: " << argv[1] << std::endl;
    if (!fileExists(argv[1]))
    {
        std::cerr << "ERROR: could not find/access the file at: "
                  << argv[1] << std::endl;
        return 1;
    }

    // Load file data into a string
    std::string  yamlData;
    BridgeSim::State initialState;
    BridgeSim::Statistics stats;

    fileGetContents(argv[1], &yamlData);

    initialState = BridgeSim::loadInitialState(yamlData);

    // Run the simulation with a heuristic
    std::unique_ptr<BridgeSim::IStrategy> strategy;
    strategy = std::make_unique<BridgeSim::LocalStrategy>();

    strategy->initialize(initialState);
    while (!strategy->is_finished())
    {
        auto move = strategy->generateNextMove();
        strategy->execute(move);

#if 0
        std::cout << "move ";
        if (move.m_person1 == BridgeSim::PERSON_NONE)
            std::cout << "-";
        else
            std::cout << initialState.m_persons[move.m_person1].m_name;
        std::cout << " ";
        if (move.m_person2 == BridgeSim::PERSON_NONE)
            std::cout << "-";
        else
            std::cout << initialState.m_persons[move.m_person2].m_name;

        std::cout << "  from:" << move.m_from << " to:" << move.m_to
                  << "  time:" << strategy->elapsedTime()
                  << std::endl;
#endif
    }
    stats = strategy->results();

    std::cout << "Results:" << std::endl
              << "  Total time: " << stats.m_totalTimeElapsed << std::endl;
    for (int i=0; i<static_cast<int>(stats.m_timeBridgeCompleted.size()); i++)
    {
        std::cout << "  Bridge[" << i << "] : "
                  << stats.m_timeBridgeCompleted[i] << std::endl;
    }
    return 0;
}
