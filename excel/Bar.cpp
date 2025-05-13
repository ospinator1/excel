#include "Bar.h"

Bar::Bar() {
    positionP1 = { 0, 0 };
    positionP2 = { 0, 0 };
}

void Bar::AddToBar(int checkerIndex, int player) {
    if (player == 1) {
        player1Checkers.push_back(checkerIndex);
    }
    else {
        player2Checkers.push_back(checkerIndex);
    }
}

void Bar::RemoveFromBar(int checkerIndex, int player) {
    auto& checkers = (player == 1) ? player1Checkers : player2Checkers;
    auto it = std::find(checkers.begin(), checkers.end(), checkerIndex);
    if (it != checkers.end()) {
        checkers.erase(it);
    }
}

int Bar::GetBarCount(int player) const {
    return (player == 1) ? player1Checkers.size() : player2Checkers.size();
}

bool Bar::Contains(int checkerIndex, int player) const {
    const auto& checkers = (player == 1) ? player1Checkers : player2Checkers;
    return std::find(checkers.begin(), checkers.end(), checkerIndex) != checkers.end();
}

POINT Bar::GetPosition(int player) const {
    return (player == 1) ? positionP1 : positionP2;
}

void Bar::SetPosition(int player, POINT position) {
    if (player == 1) {
        positionP1 = position;
    }
    else {
        positionP2 = position;
    }
}