#pragma once

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