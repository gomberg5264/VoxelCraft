# Network protocol
There are two types of packets. Requests and Commands. Commands are saved in a buffer to allow for features such as
client side prediction, server reconciliation and lag prediction.
We interpolate between command states to do that. 
Requests are packets that are treated like messages. They often exchange different data. A request could be
give me a list of players. Or give me the initial world, or give me this chunk.

# Pseudocode network loop
The bare minimum to for the network protocol to work

## Client
Serialize last packet with user ID
Send packet on a fixed rate

## Server
Listen for packets every frame and queue them
Extract packet information of last packet
Verify packet contents
Respond or broadcast new packet

## Client
Listen for packets every frame
While new packet have arrived
    Extract packet information
    Apply packet

# Pseudocode joining
## Client
Send join packet
Await ack

## Server
Retrieves join packet
If join conditions fails (too many clients)
    Respond with disconnect packet
Else   
    Load the player into the world.
    Broadcast new entity player packet.
    Create a join packet with that unique ID.
    Return join packet to client.

## Client
If timed out
    Tell user
Else if ack is disconnected
    Tell user that they can't connect
Else if ack is joined


# Gameplay loop
We have one server and one client class. They both have an instance of a game world. `A game world is a structure that contains all entities and chunks`. 
What makes the server and client unique is that they decide how to update the game world. For example, the client will send packages to the server it is connected to. The server will return packages. The client will then apply these.