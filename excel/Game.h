#pragma once
#include <Windows.h>

ref class Game
{
private:
    int currentPlayer; // ����� �������� ������ (1 ��� 2)
    List<int>^ availableMoves; // ������ ��������� ����� ��� �������� ������
    int moveCounter; // ������� ����� � ������� ������
    bool hasMovedFromBar; // ����, �����������, ��� �� ������ ��� � ����
    bool mustMoveFromBar; // ����, �����������, ����� �� ����������� ������ ��� � ����
    bool bearingOffPossible; // ����, �����������, �������� �� �������� �����
    List<int>^ highlightedPoints; // ������ �����, ������� ������ ���� ����������
    List<int>^ checkerMoveCount; // ������ ���������� ����� ��� ������ �����

public:
    Game(); // ����������� ������ Game
    void StartNewGame(); // �������� ����� ����
    void SwitchPlayer(); // ����������� �������� ������
    bool CheckWinCondition(List<int>^ bearOffCheckersP1, List<int>^ bearOffCheckersP2); // ��������� ������� ������ ��� ����� �������
    void CalculatePossibleMoves(int checkerIndex, bool isPlayer1, List<int>^ barCheckers, List<int>^ bearOffCheckers, List<int>^ points, List<System::Drawing::Color>^ colors); // ��������� ��������� ���� ��� ��������� �����
    void RollDice(array<int>^ diceValues); // ������� ������
    void ResetDice(); // ���������� �������� �������

    property int CurrentPlayer {
        int get() {
            return currentPlayer;
        }
    } // �������� ��� ������� � ������ �������� ������
    property List<int>^ AvailableMoves {
        List<int>^ get() {
            return availableMoves;
        }
    } // �������� ��� ������� � ������ ��������� �����
    property int MoveCounter { int get() {
        return moveCounter;
    }
    } // �������� ��� ������� � �������� �����
    property bool HasMovedFromBar { bool get() {
        return hasMovedFromBar;
    }
    set(bool value) {
        hasMovedFromBar = value;
    }
    } // �������� ��� ������� � �����, ������������, ��� �� ������ ��� � ����
    property bool MustMoveFromBar {
        bool get() {
            return mustMoveFromBar;
        } set(bool value) {
            mustMoveFromBar = value;
        }
    } // �������� ��� ������� � �����, ������������, ����� �� ����������� ������ ��� � ����
    property bool BearingOffPossible {
        bool get() {
            return bearingOffPossible;
        } set(bool value) {
            bearingOffPossible = value;
        }
    } // �������� ��� ������� � �����, ������������, �������� �� �������� �����
    property List<int>^ HighlightedPoints {
        List<int>^ get() {
            return highlightedPoints;
        }
    } // �������� ��� ������� � ������ �����, ������� ������ ���� ����������
    property List<int>^ CheckerMoveCount {
        List<int>^ get() {
            return checkerMoveCount;
        }
    } // �������� ��� ������� � ������ ���������� ����� ��� ������ �����
};