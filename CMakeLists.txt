cmake_minimum_required(VERSION 3.16)

project(VoxelCraft)

add_subdirectory(thirdParty/Shinobu-Engine/Shinobu)
add_subdirectory(thirdParty/enet)

set(JUST_INSTALL_CEREAL ON)
add_subdirectory(thirdParty/cereal)

add_executable(Application
    src/Entry.cpp
    
    src/Net/Packet.h

    # src/Common/GameLayer.cpp
    # 
    # src/Client/Primitive.cpp
    # src/Common/Player.cpp  
    # src/Client/PlayerInput.cpp
    # src/Net/Client.cpp 
    # src/Net/Server.cpp  
    # src/Common/ServerLayer.cpp
    )

target_include_directories(Application PUBLIC
    src
)

target_link_libraries(Application PUBLIC Shinobu enet cereal)

if (EXISTS ${CMAKE_SOURCE_DIR}/thirdParty/Shinobu-Engine/Shinobu/res)
    add_custom_target(CopyEngineAssets
                    ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/thirdParty/Shinobu-Engine/Shinobu/res
                    $<TARGET_FILE_DIR:Application>/res
                    COMMENT "Copying Shinobu resources to output dir"
    )
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/res)
    add_custom_target(CopyGameAssets
                    ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/res 
                    $<TARGET_FILE_DIR:Application>/res
                    COMMENT "Copying Shinobu resources to output dir"
    )
endif()