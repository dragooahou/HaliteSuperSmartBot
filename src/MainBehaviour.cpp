//
// Created by etien on 02/02/2022.
//

#include <algorithm>
#include <sstream>

#include "InputNode.h"
#include "ActionNodes.h"

#include "MainBehaviour.h"

bool CompareCellByAmount(const hlt::MapCell& lhs, const hlt::MapCell& rhs) {
    return lhs.halite > rhs.halite;
}


MainBehaviour::MainBehaviour(hlt::Game *mpGame) : mpGame(mpGame) {

    // Build tree
    auto treeRoot = bt::Create<bt::OrNode>();
    {
        auto andNode = bt::Create<bt::AndNode>();
        {

            andNode->AddChild(
                        bt::Create<bt::InputNode>([](hlt::Game& rGame) { return rGame.me->halite >= 3000; })
                    );

            andNode->AddChild(
                    bt::Create<bt::InputNode>([](hlt::Game& rGame) { return rGame.me->ships.size() < 2; })
            );

            andNode->AddChild(
                    bt::Create<bt::InputNode>([](hlt::Game& rGame) { return !rGame.game_map->at(rGame.me->shipyard)->is_occupied(); })
            );

            andNode->AddChild(
                    bt::Create<bt::ActionNodes::Spawn>()
            );

        }
        treeRoot->AddChild(andNode);
    }
    tree = treeRoot;

    FindPointsOfInterrest();
}

std::vector<hlt::Command> MainBehaviour::Evaluate() {
    std::vector<hlt::Command> commandQueue;
    tree->Evaluate(*mpGame,commandQueue);

    for (const auto & pair : mpGame->me->ships) {
        const hlt::EntityId& shipId = pair.first;
        std::shared_ptr<hlt::Ship> ship = pair.second;

        if(mShipBeahviours.find(shipId) == mShipBeahviours.end()) {
            mShipBeahviours[shipId] = bt::Create<ExploitPoi>(shipId, &mPoiList[mPoiToVisit]);
        }
    }

    std::vector<hlt::EntityId> destroyedShips;

    for (auto& pair : mShipBeahviours) {
        const hlt::EntityId& shipId = pair.first;
        bt::Ptr<ExploitPoi> shipTree = pair.second;

        if(mpGame->me->has_ship(shipId)) {
            shipTree->Evaluate(*mpGame, commandQueue);
        }
        else {
            destroyedShips.push_back(shipId);
        }
    }

    for(const auto& shipId : destroyedShips) {
        mShipBeahviours.erase(shipId);
    }
    
    return commandQueue;
}

void MainBehaviour::FindPointsOfInterrest() {

    // Create a list of cell
    std::vector<hlt::MapCell> cells;
    cells.reserve(64*64);
    for(const auto& cellColumn : mpGame->game_map->cells) {
        for(const hlt::MapCell& cell : cellColumn) {
            cells.push_back(cell);
        }
    }

    // Sort it by halite amount
    std::sort(cells.begin(), cells.end(), CompareCellByAmount);

    // Convert to position list
    std::vector<hlt::Position> positions;
    positions.reserve(cells.size());
    for(const auto& cell : cells) {
        positions.push_back(cell.position);
    }

    // Build the points of interest
    while (mpGame->game_map->at(*positions.begin())->halite > mPoiTreshold) {
        mPoiList.push_back(PointOfInterest::CreateFromList(*mpGame->game_map, positions, mPoiTreshold));
    }

    /*
    std::stringstream msg;
    msg << "Found " << mPoiList.size() << " POI : \n";
    for (const auto& poi : mPoiList) {
        msg << "\t" << poi.mOrigin.to_string() << "\n";
    }
    hlt::log::log(msg.str());
    */
}
