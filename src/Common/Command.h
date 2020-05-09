#pragma once

// We include this header because we need to include Cereal archives before registration
#include "Net/Packet.h"
//#include "Common/Entity/Entity.h"

#include <memory>
#include <vector>

/**
 * The command is a very important class in this project
 * It allows us to save and defer an operation. Since it is an object
 * we can even send it to the server. 
 * It also allows us to store the commands which allows us to implement things
 * such as client side prediction, server reconciliation (we can store the command
 * with time stamp data). 
 *
 * The plan right now is to send commands in a packet, convert the packets to the right command 
 * and then replicate them. By storing the commands, we can do a bunch of optimizations
 * to make the multiplayer experience appear seamless. 
 */
class Command
{
public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

class Entity;

class EntityCommand
{
public:
    unsigned entityID;

    virtual void Execute(std::vector<std::unique_ptr<Entity>>& entities) = 0;
    virtual void Undo(std::vector<std::unique_ptr<Entity>>& entities) = 0;

    template <typename T>
    void serialize(T& archive)
    {
        archive(entityID);
    }
};

///**
// * The command buffer is something that will probably be moved to the net code.
// * The design that I'm thinking of right now is that the user submits commands to the buffer.
// * On a set interval, we will flush this buffer to the server. I do this to save on bandwitdh tho
// * it may be redundant. 
// *
// * We are also only are concerned with the last command of any type but we need to store older commands
// * so that we can reconciliate our client prediction with the actual server state.
// */
//template <typename CommandType>
//class CommandBuffer
//{
//public:
//    void Push(CommandType command);
//
//    // Send the last command to the server
//    void Flush();
//
//
//
//private:
//    
//    std::vector<std::pair<unsigned, CommandType>> m_commands;
//};