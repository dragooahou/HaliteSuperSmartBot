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

    int mPoiTreshold = 400;
    std::vector<PointOfInterest> mPoiList;

    std::map<hlt::EntityId, bt::Ptr<ExploitPoi>> mShipBeahviours;

public:
    unsigned int mPoiToVisit = 0;

    explicit MainBehaviour(hlt::Game *mpGame);

    std::vector<hlt::Command> Evaluate();

private:

    void FindPointsOfInterrest();

};


#endif //HALITESUPERSMARTBOT_MAINBEHAVIOUR_H
