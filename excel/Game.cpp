#include "Game.h"
#include "MyForm.h"
Game::Game()
{
    currentPlayer = 1;
    availableMoves = gcnew List<int>();
    moveCounter = 0;
    hasMovedFromBar = false;
    mustMoveFromBar = false;
    bearingOffPossible = false;
    highlightedPoints = gcnew List<int>();
    checkerMoveCount = gcnew List<int>();
}

void Game::StartNewGame()
{
    currentPlayer = 1;
    availableMoves->Clear();
    moveCounter = 0;
    hasMovedFromBar = false;
    mustMoveFromBar = false;
    bearingOffPossible = false;
    highlightedPoints->Clear();
    checkerMoveCount->Clear();
}

void Game::SwitchPlayer()
{
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    moveCounter = 0;
    hasMovedFromBar = false;
    availableMoves->Clear();
    highlightedPoints->Clear();
}


bool Game::CheckWinCondition(List<int>^ bearOffCheckersP1, List<int>^ bearOffCheckersP2)
{
    return (bearOffCheckersP1->Count == 15) || (bearOffCheckersP2->Count == 15);
}

void Game::CalculatePossibleMoves(int checkerIndex, bool isPlayer1, List<int>^ barCheckers, List<int>^ bearOffCheckers, List<int>^ points, List<System::Drawing::Color>^ colors)
{
    highlightedPoints->Clear();

    Color checkerColor = checkerColors[checkerIndex];
    bool isPlayer1 = (checkerColor.R < 100);
    bool isInBar = (isPlayer1 && barCheckersP1->Contains(checkerIndex)) ||
        (!isPlayer1 && barCheckersP2->Contains(checkerIndex));

    if (isInBar || mustMoveFromBar)
    {
        if (!isInBar && mustMoveFromBar) return;

        for each (int move in availableMoves)
        {
            int targetPoint = isPlayer1 ? (24 - move) : (move - 1);
            if (targetPoint >= 0 && targetPoint < 24)
            {
                int opponentCount = 0;
                Color opponentColor = isPlayer1 ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                    {
                        opponentCount++;
                    }
                }

                if (opponentCount < 2)
                {
                    highlightedPoints->Add(targetPoint);
                }
            }
        }
        return;
    }

    int currentPoint = checkerPoints[checkerIndex];

    bool allInHomeAndNoBar = CheckIfBearingOffPossible(currentPlayer);

    for each (int move in availableMoves)
    {
        int targetPoint;
        if (isPlayer1)
        {
            targetPoint = currentPoint + move;
        }
        else
        {
            targetPoint = currentPoint - move;
        }

        if (allInHomeAndNoBar)
        {
            int distanceToBearOff = isPlayer1 ? (6 - currentPoint) : (currentPoint - 17);
            if ((isPlayer1 && targetPoint >= 6) || (!isPlayer1 && targetPoint <= 17))
            {
                bool canBearOff = false;
                if (isPlayer1 && targetPoint >= 6) canBearOff = true;
                if (!isPlayer1 && targetPoint <= 17) canBearOff = true;

                if (canBearOff)
                {
                    bool furtherCheckersExist = false;
                    if ((isPlayer1 && move > distanceToBearOff) || (!isPlayer1 && move > Math::Abs(distanceToBearOff)))
                    {
                        for (int i = 0; i < checkerPoints->Count; i++)
                        {
                            bool isCurrentCheckerPlayer1 = (checkerColors[i].R < 100);
                            if (isCurrentCheckerPlayer1 == isPlayer1 && checkerPoints[i] != -1 && checkerPoints[i] != -2)
                            {
                                if (isPlayer1 && checkerPoints[i] > currentPoint && checkerPoints[i] <= 5)
                                {
                                    furtherCheckersExist = true;
                                    break;
                                }
                                if (!isPlayer1 && checkerPoints[i] < currentPoint && checkerPoints[i] >= 18)
                                {
                                    furtherCheckersExist = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (!furtherCheckersExist)
                    {
                        highlightedPoints->Add(isPlayer1 ? 24 : -1);
                    }
                }
            }
            else
            {
                CheckAndAddHighlightedPoints(targetPoint, isPlayer1);
            }
        }
        else
        {
            if (targetPoint >= 0 && targetPoint < 24)
            {
                CheckAndAddHighlightedPoints(targetPoint, isPlayer1);
            }
        }
    }
}

void Game::RollDice(array<int>^ diceValues)
{
    availableMoves->Clear();
    availableMoves->Add(diceValues[0]);
    availableMoves->Add(diceValues[1]);

    if (diceValues[0] == diceValues[1])
    {
        availableMoves->Add(diceValues[0]);
        availableMoves->Add(diceValues[1]);
    }
}

void Game::ResetDice()
{
    availableMoves->Clear();
}