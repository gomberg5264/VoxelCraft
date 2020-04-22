#pragma once

/**
 * The server is where the actual game logic is executed in.
 * Even when you play single player, you still host the server on 
 * your machine while connecting to yourself. 
 *
 * The server is designed as a client model server with client prediction.
 */
class Server
{
public:

    struct Config
    {
        sf::IpAddress address;
        unsigned short port;
    };

    struct User
    {
        std::string name;
        sf::IpAddress address;
        unsigned short port;
    };

    /**
     * Host the server with the given config
     * Make sure that the engine has been constructed before
     * (which should happen automatically)
     */
    bool Run(Config config);

private:
    Config m_config;
};