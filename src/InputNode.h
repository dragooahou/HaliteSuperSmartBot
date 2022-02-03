//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_INPUTNODE_H
#define HALITESUPERSMARTBOT_INPUTNODE_H

#include <functional>

#include "BehaviourTree.h"
#include "game.hpp"

namespace BehaviourTree {

    class InputNode : public Node {
        std::function<bool(hlt::Game&)> mFunction;
    public:
        explicit InputNode(std::function<bool(hlt::Game&)> mFunction);
        State Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const override;
        BT_GETNAME_OVERRIDE
    };
}



#endif //HALITESUPERSMARTBOT_INPUTNODE_H
