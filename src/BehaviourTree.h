//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_BEHAVIOURTREE_H
#define HALITESUPERSMARTBOT_BEHAVIOURTREE_H


#include <vector>
#include <memory>

namespace BehaviourTree {

    template<typename T> using Ptr = std::shared_ptr<T>;

    enum class State {
        Success,
        Failure,
        Running
    };


    class Node {
    public:
        virtual State Evaluate() const = 0;
    };

    using NodePtr = Ptr<Node>;


    class MultiChildrenNode : public Node {
    protected:
        std::vector<NodePtr> mChildren;
    public:
        void AddChild(const NodePtr& node);
    };


    class Decorator : public Node {
    protected:
        NodePtr mpChild;
    public:
        void SetChild(const NodePtr& node);
    };




    // Base Nodes //////////////////////////////////////////////

    // OR
    class Selector : public MultiChildrenNode {

    public:
        State Evaluate() const override;

    };


    // AND
    class Sequencer : public MultiChildrenNode {

    public:
        State Evaluate() const override;

    };


    // Decorators
    class Succeeder : public Decorator {
    public:
        State Evaluate() const override;
    };

    class Failer : public Decorator {
    public:
        State Evaluate() const override;
    };

    class Inverter : public Decorator {
    public:
        State Evaluate() const override;
    };


    // Helpers

    using OrNode = Selector;
    using AndNode = Sequencer;

    template<typename T>
    Ptr<T> Create() {
        static_assert(std::is_base_of_v<Node, T>, "This function is meant for nodes");
        return std::make_shared<T>();
    }

}


#endif //HALITESUPERSMARTBOT_BEHAVIOURTREE_H
