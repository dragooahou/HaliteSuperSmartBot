//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_POINTOFINTEREST_H
#define HALITESUPERSMARTBOT_POINTOFINTEREST_H


#include "game.hpp"

class PointOfInterest {

public:
    // The origin is always the cell with the most halite
    hlt::Position mOrigin;
    std::vector<hlt::Position> mPositions;

    hlt::Halite mTotalHalite = 0;


private:
    PointOfInterest() = default;

public:

    // Create a point of interest from a list of cells
    // Start by the first cell of the list (the one with the most halite)
    // and search around. The search stop when a cell is under the halite treshold
    static PointOfInterest CreateFromList(hlt::GameMap &rMap, std::vector<hlt::Position> &rPositionList, int treshold);

    bool IsInside(const hlt::Position& position);
    
};


#endif //HALITESUPERSMARTBOT_POINTOFINTEREST_H
