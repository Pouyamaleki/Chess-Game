cmake_minimum_required(VERSION 3.10)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Board
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Color
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Game
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/GameStorage
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/GamePosition
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Move
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Piece
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Player
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Position
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Rule
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Event
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

file(GLOB_RECURSE SOURCES
    Src/Game/Game.cpp
    Src/Board/Board.cpp
    Src/Piece/Piece.cpp
    Src/Piece/King.cpp
    Src/Piece/Queen.cpp
    Src/Piece/Rook.cpp
    Src/Piece/Bishop.cpp
    Src/Piece/Knight.cpp
    Src/Piece/Pawn.cpp
    Src/Piece/ArmoredQueen.cpp
    Src/Piece/SpecialPawn.cpp
    Src/Piece/Joker.cpp
    Src/Piece/Spy.cpp
    Src/Player/Player.cpp
    Src/Player/GetPlayer.cpp
    Src/Player/Watcher.cpp
    Src/Move/Move.cpp
    Src/GamePosition/GamePosition.cpp
    Src/GameStorage/GameStorage.cpp
    Src/Game/GameModeManager.cpp
    Src/Rule/Rules.cpp
    Src/Event/TimeBomb.cpp
    Src/Event/SeasonEvent.cpp
    Src/Event/MassacreAbillity.cpp
    Src/Event/Mission.cpp
    Src/main/main.cpp
    Src/Event/EventManager.cpp 
)

add_executable(Chess ${SOURCES})
