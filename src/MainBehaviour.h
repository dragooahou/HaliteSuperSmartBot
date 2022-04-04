//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_MAINBEHAVIOUR_H
#define HALITESUPERSMARTBOT_MAINBEHAVIOUR_H

#include <map>

#include "game.hpp"
#include "BehaviourTree.h"
#include "PointOfInterest.h"
#include "src/AiNodes/ExploitPosition.h"

// The global behaviour of the AI
class MainBehaviour {

    hlt::Game* mpGame;

    bt::NodePtr tree;

    // All cell with this amount of halite or more are inside a poi
    int mPoiTreshold = 450;

    // All poi with an origin cell with this amout of halite or more is considered interesting
    int mInterrestingPoiTreshold = 750;

    std::vector<PointOfInterest> mPoiList;

    // List of behaviour for each ship
    std::map<hlt::EntityId, bt::Ptr<ExploitPosition>> mShipBeahviours;

public:

    // Number of ship to send per poi
    static constexpr int SHIP_PER_POI = 5;

    // The current poi idx to visit
    int mPoiToVisit = 0;

    // The number of ship sent on tge current poi
    int mShipCountOnCurrPoi = 0;

    explicit MainBehaviour(hlt::Game *mpGame);

    // Evaluate the main behaviour and each ship behaviour
    std::vector<hlt::Command> Evaluate();

private:

    // Create the poi list
    void FindPointsOfInterrest();

    // Select the most interesting poi to visit
    void SelectMostInterrestingPoi();

};


#endif //HALITESUPERSMARTBOT_MAINBEHAVIOUR_H
