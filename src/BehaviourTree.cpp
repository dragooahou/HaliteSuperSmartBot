//
// Created by etien on 02/02/2022.
//

#include "cassert"

#include "BehaviourTree.h"

void BehaviourTree::MultiChildrenNode::AddChild(const BehaviourTree::NodePtr &node) {
    mChildren.push_back(node);
}

BehaviourTree::State BehaviourTree::Selector::Evaluate() const {
    assert(!mChildren.empty());

    for (const auto& child : mChildren) {

        auto result = child->Evaluate();
        if(result != State::Failure) {
            return result;
        }

    }

    return State::Failure;
}


BehaviourTree::State BehaviourTree::Sequencer::Evaluate() const {
    assert(!mChildren.empty());

    for (const auto& child : mChildren) {

        auto result = child->Evaluate();
        if(result != State::Success) {
            return result;
        }

    }

    return BehaviourTree::State::Success;
}

void BehaviourTree::Decorator::SetChild(const BehaviourTree::NodePtr &node) {
    mpChild = node;
}

BehaviourTree::State BehaviourTree::Succeeder::Evaluate() const {
    assert(mpChild);
    return mpChild->Evaluate() == State::Running ? State::Running : State::Success;
}

BehaviourTree::State BehaviourTree::Failer::Evaluate() const {
    assert(mpChild);
    return mpChild->Evaluate() == State::Running ? State::Running : State::Failure;
}

BehaviourTree::State BehaviourTree::Inverter::Evaluate() const {
    assert(mpChild);
    auto result = mpChild->Evaluate();
    if(result == State::Running) return State::Running;
    return result == State::Success ? State::Failure : State::Success;
}