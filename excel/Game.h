#pragma once
#include <vector>
#include "Board.h"
#include "Dice.h"
#include "Bar.h"
#include "Timer.h"
#include "CheckerFactory.h"
class Game {
private:
    int currentPlayer;
    std::vector<int> availableMoves;
    int movePointer;
    bool hasMoveFromBar;
    bool mustMoveFromBar;
    bool bearingOffPossible;
    Board board;
    Dice dice;
    Bar bar;
    Timer timer;
    std::vector<int> highlightedPoints;
    std::vector<int> checkerMoveCount;

public:
    Game(int boardWidth, int boardHeight, int triangleWidth);
    void StartNewGame();
    void SwitchPlayer();
    bool CheckMoveValidity() const;
    bool CheckWinCondition() const;
    void CalculatePossibleMoves(int checkerIndex);
    void CheckForAutomaticBearOff(int player);
    bool CheckIfBearingOffPossible(int player) const;
    void CheckForWin();
    void ResetGame();

    // Getters
    int GetCurrentPlayer() const;
    const std::vector<int>& GetAvailableMoves() const;
    const std::vector<int>& GetHighlightedPoints() const;
    const Bar& GetBar() const;
    const Dice& GetDice() const;
    const Timer& GetTimer() const;

    // Setters
    void SetCurrentPlayer(int player);
    void SetMustMoveFromBar(bool value);
};