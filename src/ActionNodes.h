//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_ACTIONNODES_H
#define HALITESUPERSMARTBOT_ACTIONNODES_H

#include "BehaviourTree.h"
#include "game.hpp"

namespace BehaviourTree {
    namespace ActionNodes {


        class ShipActionNode : public Node {
        protected:
            hlt::EntityId mShipID;
        public:
            explicit ShipActionNode(int mShipId);
        };



        class Spawn : public Node {
        public:
            State Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const override;
            BT_GETNAME_OVERRIDE
        };



        class MakeDropoff : public ShipActionNode {
        public:
            explicit MakeDropoff(int mShipId);
            State Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const override;
            BT_GETNAME_OVERRIDE
        };


        class StayStill : public ShipActionNode {
        public:
            explicit StayStill(int mShipId);
            State Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const override;
            BT_GETNAME_OVERRIDE
        };


        class MoveTowardPosition : public ShipActionNode {
            hlt::Position mTargetPosition;
        public:
            MoveTowardPosition(int mShipId, const hlt::Position& mpTargetPosition);
            State Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const override;
            BT_GETNAME_OVERRIDE
        };

        class MoveTowardPositionPtr : public ShipActionNode {
            hlt::Position* mpTargetPosition;
        public:
            MoveTowardPositionPtr(int mShipId, hlt::Position* mpTargetPosition);
            State Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const override;
            BT_GETNAME_OVERRIDE
        };


    }
}


#endif //HALITESUPERSMARTBOT_ACTIONNODES_H
