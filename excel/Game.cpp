#include <vector>
#include <algorithm>
#include <cstdlib> // для rand
#include <ctime>   // для srand
#include "CheckerFactory.h"

class Game {
private:
    int currentPlayer; // 1 или 2
    std::vector<int> availableMoves; // допустимые ходы
    int movePointer; // позиция в списке ходов
    bool hasMoveFromBar;
    bool mustMoveFromBar;
    bool bearingOffPossible;

    std::vector<int> checkerPoints; // позиции шашек: -2 — снята, -1 — в баре, 0..23 — на доске
    std::vector<COLORREF> checkerColors; // цвет каждой шашки
    std::vector<int> checkerMoveCount; // сколько раз ходила каждая шашка

    // Для бара и снятых
    std::vector<int> barCheckersP1; // индексы шашек 1-го игрока на баре
    std::vector<int> barCheckersP2; // индексы шашек 2-го игрока на баре
    std::vector<int> bearOffCheckersP1; // снятые шашки 1-го
    std::vector<int> bearOffCheckersP2; // снятые шашки 2-го

    // Таймер
    int turnSeconds;
    bool isTurnTimerActive;

    // Для UI подсветки
    std::vector<int> highlightedPoints; // куда можно ходить

public:
    Game() {
        srand(static_cast<unsigned int>(time(nullptr)));
        ResetGame();
    }

    void StartNewGame() {
        ResetGame();
    }

    void SwitchPlayer() {
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        availableMoves.clear();
        movePointer = 0;
        hasMoveFromBar = false;
        mustMoveFromBar = false;
        bearingOffPossible = false;
        turnSeconds = 30;
        // тут можно запускать таймер
    }

    bool CheckMoveValidity() const {
        return !availableMoves.empty();
    }

    bool CheckWin() const {
        if (currentPlayer == 1)
            return (int)bearOffCheckersP1.size() == 15;
        else
            return (int)bearOffCheckersP2.size() == 15;
    }

    // Расчет возможных ходов для выбранной шашки
    void CalculatePossibleMoves(int checkerIndex) {
        highlightedPoints.clear();

        if (checkerIndex < 0 || checkerIndex >= (int)checkerPoints.size())
            return;

        // Определяем цвет шашки
        COLORREF cChecker = checkerColors[checkerIndex];
        COLORREF cPlayer1 = CreateColor(); // Черный
        COLORREF cPlayer2 = CreateColor(); // Белый
        bool isPlayer1Checker = (cChecker == cPlayer1);

        // Проверка, на баре или в доме
        bool isInBar = false;
        if (isPlayer1Checker) {
            auto it = std::find(barCheckersP1.begin(), barCheckersP1.end(), checkerIndex);
            isInBar = (it != barCheckersP1.end());
        }
        else {
            auto it = std::find(barCheckersP2.begin(), barCheckersP2.end(), checkerIndex);
            isInBar = (it != barCheckersP2.end());
        }

        // Проверка, есть ли у текущего игрока шашки на баре
        bool hasCheckersOnBar = false;
        if (currentPlayer == 1)
            hasCheckersOnBar = !barCheckersP1.empty();
        else
            hasCheckersOnBar = !barCheckersP2.empty();

        // Если есть шашки на баре — ходить только из бара
        if (hasCheckersOnBar)
        {
            // Обязательный ход из бара
            int currentPoint = checkerPoints[checkerIndex];

            int moveDistance = currentPlayer == 1 ? (24 - currentPoint) : (currentPoint + 1);
            int targetPoint = (currentPlayer == 1) ? (24 - moveDistance) : (moveDistance - 1);

            // Проверка, что эта шашка на баре
            if (!isInBar)
                return;

            // Проверка, что целевая точка допустима (менее 2 противников)
            int opponentCount = 0;
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != checkerColor)
                    opponentCount++;
            }
            if (opponentCount < 2)
                highlightedPoints.push_back(targetPoint);

            return;
        }

        // Шашка на доске
        int currentPoint = checkerPoints[checkerIndex];

        // Если у игрока есть шашки на баре — ходить с доски нельзя
        if (currentPlayer == 1 && !barCheckersP1.empty())
            return;
        if (currentPlayer == 2 && !barCheckersP2.empty())
            return;

        // Обработка возможных ходов
        bool allInHomeAndNoBar = CheckIfBearingOffPossible(currentPlayer);
        for (int move : availableMoves)
        {
            int targetPoint = currentPoint + (currentPlayer == 1 ? move : -move);

            if (targetPoint >= 0 && targetPoint <= 23)
            {
                // Проверка блокировки
                int opponentCount = 0;
                for (size_t i = 0; i < checkerPoints.size(); i++) {
                    if (checkerPoints[i] == targetPoint && checkerColors[i] != checkerColor)
                        opponentCount++;
                }
                if (opponentCount < 2)
                    highlightedPoints.push_back(targetPoint);
            }
            else if (allInHomeAndNoBar)
            {
                // Возможность снимать
                int distanceToBearOff = (currentPlayer == 1) ? (6 - currentPoint) : (currentPoint - 17);
                if ((currentPlayer == 1 && targetPoint >= 6) || (currentPlayer == 2 && targetPoint <= 17))
                {
                    // Проверка, есть ли шашки дальше
                    bool furtherExist = false;
                    for (size_t i = 0; i < checkerPoints.size(); i++) {
                        if (checkerColors[i] == CreateColor()) { // игрок 1
                            if (i != checkerIndex && checkerPoints[i] > currentPoint && checkerPoints[i] <= 5)
                                furtherExist = true;
                        }
                        else { // игрок 2
                            if (i != checkerIndex && checkerPoints[i] < currentPoint && checkerPoints[i] >= 18)
                                furtherExist = true;
                        }
                    }
                    if (!furtherExist)
                    {
                        // Можно снимать
                        highlightedPoints.push_back(currentPlayer == 1 ? 24 : -1);
                        continue;
                    }
                }
                // обычный ход
                CheckAndAddHighlightedPoints(targetPoint, isPlayer1Checker);
            }
        }
    }

    void CheckAndAddHighlightedPoints(int targetPoint, bool isPlayer1Checker)
    {
        int opponentCount = 0;
        for (size_t i = 0; i < checkerPoints.size(); i++) {
            if (checkerPoints[i] == targetPoint && checkerColors[i] != (isPlayer1Checker ? CreateColor() : CreateColor())) {
                opponentCount++;
            }
        }
        if (opponentCount < 2)
            highlightedPoints.push_back(targetPoint);
    }

    bool CheckIfBearingOffPossible(int player) const
    {
        // Проверка, что все шашки в доме
        for (size_t i = 0; i < checkerPoints.size(); i++) {
            if (checkerColors[i] == CreateColor()) { // игрок 1
                int pt = checkerPoints[i];
                if (pt >= 0 && pt <= 23 && !(player == 1 && pt <= 5))
                    return false;
            }
            else { // игрок 2
                int pt = checkerPoints[i];
                if (pt >= 0 && pt <= 23 && !(player == 2 && pt >= 18))
                    return false;
            }
        }
        return true;
    }

    void ResetGame() {
        checkerPoints.clear();
        checkerColors.clear();
        checkerMoveCount.clear();

        barCheckersP1.clear();
        barCheckersP2.clear();
        bearOffCheckersP1.clear();
        bearOffCheckersP2.clear();

        AddCheckersAtPoint(23, 2, 1);
        AddCheckersAtPoint(12, 5, 1);
        AddCheckersAtPoint(7, 3, 1);
        AddCheckersAtPoint(5, 5, 1);

        AddCheckersAtPoint(0, 2, 2);
        AddCheckersAtPoint(11, 5, 2);
        AddCheckersAtPoint(16, 3, 2);
        AddCheckersAtPoint(18, 5, 2);

        for (size_t i = 0; i < checkerPoints.size(); i++) {
            checkerMoveCount.push_back(0);
        }

        currentPlayer = 1;
        availableMoves.clear();
        turnSeconds = 30;
        isTurnTimerActive = false;
    }

    void AddCheckersAtPoint(int point, int count, int player)
    {
        for (int i = 0; i < count; i++) {
            checkerPoints.push_back(point);
            if (player == 1)
                checkerColors.push_back(CreateColor()); // черный
            else
                checkerColors.push_back(CreateColor()); // белый
        }
    }

    // Основные методы
    bool PlayerMakeMove(int checkerIndex, int targetPoint, int moveDistance)
    {
        if (checkerIndex < 0 || checkerIndex >= (int)checkerPoints.size())
            return false;

        // Проверка, из бара или с доски
        bool fromBar = false;
        bool bearOff = false;
        COLORREF c = checkerColors[checkerIndex];
        bool isPlayer1Checker = (c == CreateColor());

        if (isPlayer1Checker) {
            auto it = std::find(barCheckersP1.begin(), barCheckersP1.end(), checkerIndex);
            fromBar = (it != barCheckersP1.end());
        }
        else {
            auto it = std::find(barCheckersP2.begin(), barCheckersP2.end(), checkerIndex);
            fromBar = (it != barCheckersP2.end());
        }

        if (checkerPoints[checkerIndex] == -2) // уже снята
            return false;

        return MakeMove(checkerIndex, targetPoint, moveDistance, fromBar, bearOff);
    }

    bool MakeMove(int checkerIndex, int targetPoint, int moveDistance, bool fromBar, bool bearOff)
    {
        if (checkerIndex < 0 || checkerIndex >= (int)checkerPoints.size())
            return false;

        COLORREF c = checkerColors[checkerIndex];
        bool isPlayer1Checker = (c == CreateColor());

        if (fromBar)
        {
            auto& barVec = isPlayer1Checker ? barCheckersP1 : barCheckersP2;
            auto itBar = std::find(barVec.begin(), barVec.end(), checkerIndex);
            if (itBar == barVec.end())
                return false;

            int moveNeeded = currentPlayer == 1 ? (24 - checkerPoints[checkerIndex]) : (checkerPoints[checkerIndex] + 1);
            if (moveNeeded != moveDistance)
                return false;

            // Проверка блокировки
            int opponentCount = 0;
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                    opponentCount++;
            }
            if (opponentCount >= 2)
                return false;

            // Удаляем из бара
            barVec.erase(itBar);

            // Обработка взятия
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                {
                    // Бьют шашку
                    if (checkerColors[i] == CreateColor()) // игрок 1
                        barCheckersP2.push_back(i);
                    else
                        barCheckersP1.push_back(i);
                    checkerPoints[i] = -1; // в бар
                }
            }

            // Перемещаем шашку
            checkerPoints[checkerIndex] = targetPoint;
            return true;
        }
        else if (bearOff)
        {
            int currentPt = checkerPoints[checkerIndex];
            int requiredMove = (currentPlayer == 1) ? (6 - currentPt) : (currentPt - 17);
            if (moveDistance != requiredMove && moveDistance != abs(targetPoint - currentPt))
                return false;

            if (!CheckIfBearingOffPossible(currentPlayer))
                return false;

            // Снимаем
            checkerPoints[checkerIndex] = -2; // снята
            if (currentPlayer == 1)
                bearOffCheckersP1.push_back(checkerIndex);
            else
                bearOffCheckersP2.push_back(checkerIndex);
            return true;
        }
        else
        {
            // Обычный ход
            if (abs(targetPoint - checkerPoints[checkerIndex]) != moveDistance)
                return false;

            // Проверка блокировки
            int opponentCount = 0;
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                    opponentCount++;
            }
            if (opponentCount >= 2)
                return false;

            // Обработка взятия
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                {
                    if (checkerColors[i] == CreateColor()) // игрок 1
                        barCheckersP2.push_back(i);
                    else
                        barCheckersP1.push_back(i);
                    checkerPoints[i] = -1;
                }
            }
            // Перемещение
            checkerPoints[checkerIndex] = targetPoint;
            checkerMoveCount[checkerIndex]++;
            return true;
        }
    }

    void EndTurn() {
        if (CheckWin())
        {
            // Обработка победы
            return;
        }

        SwitchPlayer();

        // Автоматическая проверка снятия
        CheckAutoBearOff();

        turnSeconds = 30;
        isTurnTimerActive = false;
        availableMoves.clear();
    }

    void CheckAutoBearOff()
    {
        CheckPlayerBearOff(1);
        CheckPlayerBearOff(2);
    }

    void CheckPlayerBearOff(int player)
    {
        bool allInHome = true;
        for (size_t i = 0; i < checkerPoints.size(); i++) {
            if (checkerColors[i] == CreateColor()) { // игрок 1
                int pt = checkerPoints[i];
                if (pt >= 0 && pt <= 23 && !(player == 1 && pt <= 5))
                    allInHome = false;
            }
            else { // игрок 2
                int pt = checkerPoints[i];
                if (pt >= 0 && pt <= 23 && !(player == 2 && pt >= 18))
                    allInHome = false;
            }
        }
        if (allInHome)
        {
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerColors[i] == CreateColor()) {
                    int pt = checkerPoints[i];
                    if ((player == 1 && pt >= 0 && pt <= 5) && checkerPoints[i] != -2)
                    {
                        checkerPoints[i] = -2; // снята
                        bearOffCheckersP1.push_back(i);
                    }
                    if ((player == 2 && pt >= 18 && pt <= 23) && checkerPoints[i] != -2)
                    {
                        checkerPoints[i] = -2; // снята
                        bearOffCheckersP2.push_back(i);
                    }
                }
            }
        }
    }

    void ResetGame() {
        checkerPoints.clear();
        checkerColors.clear();
        checkerMoveCount.clear();

        barCheckersP1.clear();
        barCheckersP2.clear();
        bearOffCheckersP1.clear();
        bearOffCheckersP2.clear();

        AddCheckersAtPoint(23, 2, 1);
        AddCheckersAtPoint(12, 5, 1);
        AddCheckersAtPoint(7, 3, 1);
        AddCheckersAtPoint(5, 5, 1);

        AddCheckersAtPoint(0, 2, 2);
        AddCheckersAtPoint(11, 5, 2);
        AddCheckersAtPoint(16, 3, 2);
        AddCheckersAtPoint(18, 5, 2);

        for (size_t i = 0; i < checkerPoints.size(); i++) {
            checkerMoveCount.push_back(0);
        }

        currentPlayer = 1;
        availableMoves.clear();
        turnSeconds = 30;
        isTurnTimerActive = false;
    }

    void AddCheckersAtPoint(int point, int count, int player)
    {
        for (int i = 0; i < count; i++) {
            checkerPoints.push_back(point);
            if (player == 1)
                checkerColors.push_back(CreateColor()); // черный
            else
                checkerColors.push_back(CreateColor()); // белый
        }
    }

    COLORREF CreateColor() {
        return (currentPlayer == 1) ? RGB(50, 50, 50) : RGB(240, 240, 240);
    }

    // Вызовите для броска кубиков
    void RollDice()
    {
        int dice1 = rand() % 6 + 1;
        int dice2 = rand() % 6 + 1;
        availableMoves.clear();
        if (dice1 == dice2)
        {
            availableMoves = { dice1, dice1, dice1, dice1 };
        }
        else
        {
            availableMoves = { dice1, dice2 };
        }
    }

    void TickTimer()
    {
        if (isTurnTimerActive)
        {
            turnSeconds--;
            if (turnSeconds <= 0)
            {
                EndTurn();
            }
        }
    }

    void FinishTurn()
    {
        EndTurn();
    }
};