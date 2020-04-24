/**
 * The gameplay system sends commands to each other.
 * If you were to use this as an engine, the gameplay folder
 * would be were you would create your game. 
 * 
 * By using commands, we can pass it as an event. That is usefull
 * because it means that we can update the model from multiple places.
 * In our case, we send commands from the GameLayer and based on user input.
 */
#pragma once
#include <glm/glm.hpp>


enum class CommandType
{
    AddChunk, RemoveChunk
};

#define COMMAND_TYPE(type)                                              \
    static CommandType StaticType() { return type; }                    \
    virtual CommandType Type() override final { return StaticType(); }  \

struct Command
{
    virtual CommandType Type() = 0;
};

class CommandDispatcher
{
public:
    CommandDispatcher(Command& command) : m_command(command) {}

    template <typename T, typename F>
    inline bool Dispatch(const F& fn)
    {
        static_assert(std::is_base_of<Command, T>::value, "T is not derived from Command");
        if(m_command.Type() == T::StaticType)
        {
            func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Command& m_command;
};

struct AddChunkCommand : public Command
{
    AddChunkCommand(const glm::vec3& pos) : pos(pos) {}
    glm::vec3 pos;
    
    COMMAND_TYPE(CommandType::AddChunk)
};

struct RemoveChunkCommand : public Command
{
    RemoveChunkCommand(const glm::vec3& pos) : pos(pos) {}
    glm::vec3 pos;

    COMMAND_TYPE(CommandType::RemoveChunk)
};