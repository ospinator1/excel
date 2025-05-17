#pragma once
#include <Windows.h>

ref class Game
{
private:
    int currentPlayer; // Номер текущего игрока (1 или 2)
    List<int>^ availableMoves; // Список доступных ходов для текущего игрока
    int moveCounter; // Счетчик ходов в текущем раунде
    bool hasMovedFromBar; // Флаг, указывающий, был ли сделан ход с бара
    bool mustMoveFromBar; // Флаг, указывающий, нужно ли обязательно делать ход с бара
    bool bearingOffPossible; // Флаг, указывающий, возможно ли выводить шашки
    List<int>^ highlightedPoints; // Список точек, которые должны быть подсвечены
    List<int>^ checkerMoveCount; // Список количества ходов для каждой шашки

public:
    Game(); // Конструктор класса Game
    void StartNewGame(); // Начинает новую игру
    void SwitchPlayer(); // Переключает текущего игрока
    bool CheckWinCondition(List<int>^ bearOffCheckersP1, List<int>^ bearOffCheckersP2); // Проверяет условие победы для обоих игроков
    void CalculatePossibleMoves(int checkerIndex, bool isPlayer1, List<int>^ barCheckers, List<int>^ bearOffCheckers, List<int>^ points, List<System::Drawing::Color>^ colors); // Вычисляет возможные ходы для выбранной шашки
    void RollDice(array<int>^ diceValues); // Бросает кубики
    void ResetDice(); // Сбрасывает значения кубиков

    property int CurrentPlayer {
        int get() {
            return currentPlayer;
        }
    } // Свойство для доступа к номеру текущего игрока
    property List<int>^ AvailableMoves {
        List<int>^ get() {
            return availableMoves;
        }
    } // Свойство для доступа к списку доступных ходов
    property int MoveCounter { int get() {
        return moveCounter;
    }
    } // Свойство для доступа к счетчику ходов
    property bool HasMovedFromBar { bool get() {
        return hasMovedFromBar;
    }
    set(bool value) {
        hasMovedFromBar = value;
    }
    } // Свойство для доступа к флагу, указывающему, был ли сделан ход с бара
    property bool MustMoveFromBar {
        bool get() {
            return mustMoveFromBar;
        } set(bool value) {
            mustMoveFromBar = value;
        }
    } // Свойство для доступа к флагу, указывающему, нужно ли обязательно делать ход с бара
    property bool BearingOffPossible {
        bool get() {
            return bearingOffPossible;
        } set(bool value) {
            bearingOffPossible = value;
        }
    } // Свойство для доступа к флагу, указывающему, возможно ли выводить шашки
    property List<int>^ HighlightedPoints {
        List<int>^ get() {
            return highlightedPoints;
        }
    } // Свойство для доступа к списку точек, которые должны быть подсвечены
    property List<int>^ CheckerMoveCount {
        List<int>^ get() {
            return checkerMoveCount;
        }
    } // Свойство для доступа к списку количества ходов для каждой шашки
};