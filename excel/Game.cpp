#include <vector>
#include <algorithm>
#include <cstdlib> // ��� rand
#include <ctime>   // ��� srand
#include "CheckerFactory.h"

class Game {
private:
    int currentPlayer; // 1 ��� 2
    std::vector<int> availableMoves; // ���������� ����
    int movePointer; // ������� � ������ �����
    bool hasMoveFromBar;
    bool mustMoveFromBar;
    bool bearingOffPossible;

    std::vector<int> checkerPoints; // ������� �����: -2 � �����, -1 � � ����, 0..23 � �� �����
    std::vector<COLORREF> checkerColors; // ���� ������ �����
    std::vector<int> checkerMoveCount; // ������� ��� ������ ������ �����

    // ��� ���� � ������
    std::vector<int> barCheckersP1; // ������� ����� 1-�� ������ �� ����
    std::vector<int> barCheckersP2; // ������� ����� 2-�� ������ �� ����
    std::vector<int> bearOffCheckersP1; // ������ ����� 1-��
    std::vector<int> bearOffCheckersP2; // ������ ����� 2-��

    // ������
    int turnSeconds;
    bool isTurnTimerActive;

    // ��� UI ���������
    std::vector<int> highlightedPoints; // ���� ����� ������

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
        // ��� ����� ��������� ������
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

    // ������ ��������� ����� ��� ��������� �����
    void CalculatePossibleMoves(int checkerIndex) {
        highlightedPoints.clear();

        if (checkerIndex < 0 || checkerIndex >= (int)checkerPoints.size())
            return;

        // ���������� ���� �����
        COLORREF cChecker = checkerColors[checkerIndex];
        COLORREF cPlayer1 = CreateColor(); // ������
        COLORREF cPlayer2 = CreateColor(); // �����
        bool isPlayer1Checker = (cChecker == cPlayer1);

        // ��������, �� ���� ��� � ����
        bool isInBar = false;
        if (isPlayer1Checker) {
            auto it = std::find(barCheckersP1.begin(), barCheckersP1.end(), checkerIndex);
            isInBar = (it != barCheckersP1.end());
        }
        else {
            auto it = std::find(barCheckersP2.begin(), barCheckersP2.end(), checkerIndex);
            isInBar = (it != barCheckersP2.end());
        }

        // ��������, ���� �� � �������� ������ ����� �� ����
        bool hasCheckersOnBar = false;
        if (currentPlayer == 1)
            hasCheckersOnBar = !barCheckersP1.empty();
        else
            hasCheckersOnBar = !barCheckersP2.empty();

        // ���� ���� ����� �� ���� � ������ ������ �� ����
        if (hasCheckersOnBar)
        {
            // ������������ ��� �� ����
            int currentPoint = checkerPoints[checkerIndex];

            int moveDistance = currentPlayer == 1 ? (24 - currentPoint) : (currentPoint + 1);
            int targetPoint = (currentPlayer == 1) ? (24 - moveDistance) : (moveDistance - 1);

            // ��������, ��� ��� ����� �� ����
            if (!isInBar)
                return;

            // ��������, ��� ������� ����� ��������� (����� 2 �����������)
            int opponentCount = 0;
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != checkerColor)
                    opponentCount++;
            }
            if (opponentCount < 2)
                highlightedPoints.push_back(targetPoint);

            return;
        }

        // ����� �� �����
        int currentPoint = checkerPoints[checkerIndex];

        // ���� � ������ ���� ����� �� ���� � ������ � ����� ������
        if (currentPlayer == 1 && !barCheckersP1.empty())
            return;
        if (currentPlayer == 2 && !barCheckersP2.empty())
            return;

        // ��������� ��������� �����
        bool allInHomeAndNoBar = CheckIfBearingOffPossible(currentPlayer);
        for (int move : availableMoves)
        {
            int targetPoint = currentPoint + (currentPlayer == 1 ? move : -move);

            if (targetPoint >= 0 && targetPoint <= 23)
            {
                // �������� ����������
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
                // ����������� �������
                int distanceToBearOff = (currentPlayer == 1) ? (6 - currentPoint) : (currentPoint - 17);
                if ((currentPlayer == 1 && targetPoint >= 6) || (currentPlayer == 2 && targetPoint <= 17))
                {
                    // ��������, ���� �� ����� ������
                    bool furtherExist = false;
                    for (size_t i = 0; i < checkerPoints.size(); i++) {
                        if (checkerColors[i] == CreateColor()) { // ����� 1
                            if (i != checkerIndex && checkerPoints[i] > currentPoint && checkerPoints[i] <= 5)
                                furtherExist = true;
                        }
                        else { // ����� 2
                            if (i != checkerIndex && checkerPoints[i] < currentPoint && checkerPoints[i] >= 18)
                                furtherExist = true;
                        }
                    }
                    if (!furtherExist)
                    {
                        // ����� �������
                        highlightedPoints.push_back(currentPlayer == 1 ? 24 : -1);
                        continue;
                    }
                }
                // ������� ���
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
        // ��������, ��� ��� ����� � ����
        for (size_t i = 0; i < checkerPoints.size(); i++) {
            if (checkerColors[i] == CreateColor()) { // ����� 1
                int pt = checkerPoints[i];
                if (pt >= 0 && pt <= 23 && !(player == 1 && pt <= 5))
                    return false;
            }
            else { // ����� 2
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
                checkerColors.push_back(CreateColor()); // ������
            else
                checkerColors.push_back(CreateColor()); // �����
        }
    }

    // �������� ������
    bool PlayerMakeMove(int checkerIndex, int targetPoint, int moveDistance)
    {
        if (checkerIndex < 0 || checkerIndex >= (int)checkerPoints.size())
            return false;

        // ��������, �� ���� ��� � �����
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

        if (checkerPoints[checkerIndex] == -2) // ��� �����
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

            // �������� ����������
            int opponentCount = 0;
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                    opponentCount++;
            }
            if (opponentCount >= 2)
                return false;

            // ������� �� ����
            barVec.erase(itBar);

            // ��������� ������
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                {
                    // ���� �����
                    if (checkerColors[i] == CreateColor()) // ����� 1
                        barCheckersP2.push_back(i);
                    else
                        barCheckersP1.push_back(i);
                    checkerPoints[i] = -1; // � ���
                }
            }

            // ���������� �����
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

            // �������
            checkerPoints[checkerIndex] = -2; // �����
            if (currentPlayer == 1)
                bearOffCheckersP1.push_back(checkerIndex);
            else
                bearOffCheckersP2.push_back(checkerIndex);
            return true;
        }
        else
        {
            // ������� ���
            if (abs(targetPoint - checkerPoints[checkerIndex]) != moveDistance)
                return false;

            // �������� ����������
            int opponentCount = 0;
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                    opponentCount++;
            }
            if (opponentCount >= 2)
                return false;

            // ��������� ������
            for (size_t i = 0; i < checkerPoints.size(); i++) {
                if (checkerPoints[i] == targetPoint && checkerColors[i] != c)
                {
                    if (checkerColors[i] == CreateColor()) // ����� 1
                        barCheckersP2.push_back(i);
                    else
                        barCheckersP1.push_back(i);
                    checkerPoints[i] = -1;
                }
            }
            // �����������
            checkerPoints[checkerIndex] = targetPoint;
            checkerMoveCount[checkerIndex]++;
            return true;
        }
    }

    void EndTurn() {
        if (CheckWin())
        {
            // ��������� ������
            return;
        }

        SwitchPlayer();

        // �������������� �������� ������
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
            if (checkerColors[i] == CreateColor()) { // ����� 1
                int pt = checkerPoints[i];
                if (pt >= 0 && pt <= 23 && !(player == 1 && pt <= 5))
                    allInHome = false;
            }
            else { // ����� 2
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
                        checkerPoints[i] = -2; // �����
                        bearOffCheckersP1.push_back(i);
                    }
                    if ((player == 2 && pt >= 18 && pt <= 23) && checkerPoints[i] != -2)
                    {
                        checkerPoints[i] = -2; // �����
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
                checkerColors.push_back(CreateColor()); // ������
            else
                checkerColors.push_back(CreateColor()); // �����
        }
    }

    COLORREF CreateColor() {
        return (currentPlayer == 1) ? RGB(50, 50, 50) : RGB(240, 240, 240);
    }

    // �������� ��� ������ �������
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