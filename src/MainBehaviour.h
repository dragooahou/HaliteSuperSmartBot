//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_MAINBEHAVIOUR_H
#define HALITESUPERSMARTBOT_MAINBEHAVIOUR_H

#include <map>

#include "game.hpp"
#include "BehaviourTree.h"
#include "PointOfInterest.h"
#include "src/AiNodes/ExploitPoi.h"

class MainBehaviour {

    hlt::Game* mpGame;

    bt::NodePtr tree;

    int mPoiTreshold = 450;
    int mInterrestingPoiTreshold = 750;

    std::vector<PointOfInterest> mPoiList; // Poi are sorted by distance

    std::map<hlt::EntityId, bt::Ptr<ExploitPosition>> mShipBeahviours;

public:
    static constexpr int SHIP_PER_POI = 5;
    static constexpr int MAX_SHIP_COUNT = 30;

    int mPoiToVisit = 0;
    int mShipCountOnCurrPoi = 0;

    explicit MainBehaviour(hlt::Game *mpGame);

    std::vector<hlt::Command> Evaluate();

private:

    void FindPointsOfInterrest();
    void SelectMostInterrestingPoi();

};


#endif //HALITESUPERSMARTBOT_MAINBEHAVIOUR_H
