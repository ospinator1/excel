#include <Windows.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

// Добавляем using для таймера
using namespace System::Windows::Forms;

// Singleton Pattern for Game Rules
ref class GameRules
{
private:
    static GameRules^ instance;
    String^ rulesText;

    GameRules()
    {
        rulesText = "Правила игры в Нарды:\n\n";
        rulesText += "Цель игры: Переместить все свои шашки в свой дом и затем 'снять' их с доски.\n\n";
        rulesText += "Начальная расстановка: Каждая сторона имеет 15 шашек. Игрок 1 (черные) начинает с 2 шашек на 24-м поле, 5 на 13-м, 3 на 8-м и 5 на 6-м. Игрок 2 (белые) начинает с 2 шашек на 1-м поле, 5 на 12-м, 3 на 17-м и 5 на 19-м.\n\n";
        rulesText += "Ход: Игроки по очереди бросают два кубика и перемещают шашки на количество полей, выпавшее на каждом кубике. Можно использовать оба значения для одной шашки или переместить две разные шашки.\n\n";
        rulesText += "Дубль: Если на кубиках выпадает одинаковое число (дубль), игрок может сделать четыре хода на это число.\n\n";
        rulesText += "Занятое поле: Шашку можно переместить на поле, если на нем нет шашек противника, или есть только одна шашка противника (блокада).\n\n";
        rulesText += "Взятие: Если шашка попадает на поле, занятое одной шашкой противника, шашка противника 'бьется' и помещается на 'бар'.\n\n";
        rulesText += "Бар: Шашки на баре должны быть введены обратно на доску с начальных полей противника (для игрока 1 - поля 1-6, для игрока 2 - поля 19-24). Для этого нужно выбросить число на кубике, соответствующее свободному полю в доме противника. Пока на баре есть шашки, игрок обязан ходить только ими.\n\n";
        rulesText += "Дом: Дом игрока 1 - поля 1-6, дом игрока 2 - поля 19-24.\n\n";
        rulesText += "Снятие шашек: Когда все шашки игрока находятся в его доме, он может начать 'снимать' их с доски. Для этого нужно выбросить на кубике число, равное расстоянию от шашки до края доски. Если выпавшее число больше необходимого, можно снять шашку с самого дальнего поля, на котором есть шашка.\n\n";
        rulesText += "Победа: Побеждает игрок, который первым снимет все свои шашки с доски.";
    }

public:
    static GameRules^ GetInstance()
    {
        if (instance == nullptr)
        {
            instance = gcnew GameRules();
        }
        return instance;
    }

    String^ GetRules()
    {
        return rulesText;
    }
};

// Abstract Factory Pattern for Checker Creation
ref class CheckerFactory abstract
{
public:
    virtual Color CreateColor() abstract;
};

ref class Player1CheckerFactory : public CheckerFactory
{
public:
    virtual Color CreateColor() override
    {
        return Color::FromArgb(50, 50, 50); // Dark color for Player 1
    }
};

ref class Player2CheckerFactory : public CheckerFactory
{
public:
    virtual Color CreateColor() override
    {
        return Color::FromArgb(240, 240, 240); // Light color for Player 2
    }
};

public ref class BackgammonForm : public Form
{
private:
    List<Point>^ checkerPositions;
    List<Color>^ checkerColors;
    List<int>^ checkerPoints;
    int checkerRadius;

    Random^ random;

    // Game state
    int currentPlayer;
    int player1Dice1, player1Dice2;
    int player2Dice1, player2Dice2;
    List<int>^ availableMoves;
    List<int>^ highlightedPoints;
    int selectedCheckerIndex;
    Point initialCheckerPosition;
    bool hasMovedFromBar;
    bool mustMoveFromBar;  // Новый флаг: обязан ходить из бара
    bool bearingOffPossible; // Флаг: возможно ли снятие шашек

    // UI элементы
    Button^ rollDiceButton;
    Button^ endTurnButtonP1;
    Button^ endTurnButtonP2;
    Button^ showRulesButton;

    // Таймеры для общего времени и времени хода
 
    Label^ player1TimerLabel;
    Label^ player2TimerLabel;
    Timer^ turnTimer;
    int totalSeconds;
    int turnSeconds;
    bool isTurnTimerActive;

    // Board dimensions
    int boardWidth;
    int boardHeight;
    int triangleWidth;
    int triangleHeight;
    int boardOffsetX;

    // Move-related variables
    int moveCounter;
    List<int>^ checkerMoveCount;

    // Bar позиции
    List<int>^ barCheckersP1;
    List<int>^ barCheckersP2;
    Point barPositionP1;
    Point barPositionP2;

    // Bear off позиции
    Point bearOffPositionP1;
    Point bearOffPositionP2;
    List<int>^ bearOffCheckersP1;
    List<int>^ bearOffCheckersP2;

    // Внутренние константы
    const int VERTICAL_OFFSET = 50;
    const int UI_VERTICAL_OFFSET = 100;

public:
    property Point BarPositionP1
    {
        Point get() { return barPositionP1; }
        void set(Point value) { barPositionP1 = value; }
    }

    property Point BarPositionP2
    {
        Point get() { return barPositionP2; }
        void set(Point value) { barPositionP2 = value; }
    }

    property Point BearOffPositionP1
    {
        Point get() { return bearOffPositionP1; }
        void set(Point value) { bearOffPositionP1 = value; }
    }

    property Point BearOffPositionP2
    {
        Point get() { return bearOffPositionP2; }
        void set(Point value) { bearOffPositionP2 = value; }
    }

public:
    BackgammonForm()
    {
        this->Text = "Backgammon";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1200, 750);
        this->BackColor = Color::SkyBlue;

        checkerRadius = 20;
        boardWidth = 700;
        boardHeight = 500;
        triangleWidth = boardWidth / 12;
        triangleHeight = boardHeight / 2;
        boardOffsetX = (this->ClientSize.Width - boardWidth) / 2;

        checkerPositions = gcnew List<Point>();
        checkerColors = gcnew List<Color>();
        checkerPoints = gcnew List<int>();
        availableMoves = gcnew List<int>();
        highlightedPoints = gcnew List<int>();
        selectedCheckerIndex = -1;
        random = gcnew Random();
        currentPlayer = 1;
        player1Dice1 = player1Dice2 = 0;
        player2Dice1 = player2Dice2 = 0;
        moveCounter = 0;
        hasMovedFromBar = false;
        mustMoveFromBar = false;
        bearingOffPossible = false;

        checkerMoveCount = gcnew List<int>();
        barCheckersP1 = gcnew List<int>();
        barCheckersP2 = gcnew List<int>();
        bearOffCheckersP1 = gcnew List<int>();
        bearOffCheckersP2 = gcnew List<int>();

        this->BarPositionP1 = Point(boardOffsetX + boardWidth - checkerRadius, boardHeight / 2 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET + 50);
        this->BarPositionP2 = Point(boardOffsetX + boardWidth + checkerRadius, boardHeight / 2 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET + 50);

        this->BearOffPositionP1 = Point(boardOffsetX + boardWidth * 2 + 100, boardHeight + VERTICAL_OFFSET + UI_VERTICAL_OFFSET - 50);
        this->BearOffPositionP2 = Point(boardOffsetX + boardWidth * 2 + 100, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET + 50);

        InitializeUI();
        InitializeCheckers();

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            checkerMoveCount->Add(0);
        }

  

        turnTimer = gcnew Timer();
        turnTimer->Interval = 1000;
        turnTimer->Tick += gcnew EventHandler(this, &BackgammonForm::TurnTimer_Tick);
        turnSeconds = 30; // время хода
        isTurnTimerActive = false;

        InitializeTimerLabels();

   
        this->MouseDown += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &BackgammonForm::Form_Paint);
    }

private:
    void InitializeUI()
    {
        rollDiceButton = gcnew Button();
        rollDiceButton->Text = "Бросить кубики";
        rollDiceButton->Size = System::Drawing::Size(120, 40);
        rollDiceButton->Location = System::Drawing::Point(
            this->ClientSize.Width / 2 - rollDiceButton->Width / 2 + 350,
            this->ClientSize.Height - UI_VERTICAL_OFFSET + 100);
        rollDiceButton->BackColor = Color::LightSkyBlue;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        rollDiceButton->Click += gcnew EventHandler(this, &BackgammonForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

        endTurnButtonP1 = gcnew Button();
        endTurnButtonP1->Text = "Завершить ход";
        endTurnButtonP1->Size = System::Drawing::Size(100, 40);
        endTurnButtonP1->Location = System::Drawing::Point(
            this->ClientSize.Width - 1100,
            this->ClientSize.Height - UI_VERTICAL_OFFSET);
        endTurnButtonP1->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP1->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP1->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP1_Click);
        endTurnButtonP1->Enabled = false;
        this->Controls->Add(endTurnButtonP1);

        endTurnButtonP2 = gcnew Button();
        endTurnButtonP2->Text = "Завершить ход";
        endTurnButtonP2->Size = System::Drawing::Size(100, 40);
        endTurnButtonP2->Location = System::Drawing::Point(
            this->ClientSize.Width + 500,
            this->ClientSize.Height - UI_VERTICAL_OFFSET);
        endTurnButtonP2->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP2->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP2->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP2_Click);
        endTurnButtonP2->Enabled = false;
        this->Controls->Add(endTurnButtonP2);

        showRulesButton = gcnew Button();
        showRulesButton->Text = "Правила игры";
        showRulesButton->Size = System::Drawing::Size(120, 40);
        showRulesButton->Location = System::Drawing::Point(10, 10);
        showRulesButton->BackColor = Color::LightGreen;
        showRulesButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        showRulesButton->Click += gcnew EventHandler(this, &BackgammonForm::ShowRulesButton_Click);
        this->Controls->Add(showRulesButton);
    }

    void InitializeTimerLabels()
    {
      
        player1TimerLabel = gcnew Label();
        player1TimerLabel->Font = gcnew  System::Drawing::Font("Arial", 10, FontStyle::Bold);
        player1TimerLabel->ForeColor = Color::Black;
        player1TimerLabel->Location = Point(100, 180 + UI_VERTICAL_OFFSET);
        player1TimerLabel->Size = System::Drawing::Size(100, 20);
        player1TimerLabel->Text = "Время: 30 сек";
        this->Controls->Add(player1TimerLabel);

        player2TimerLabel = gcnew Label();
        player2TimerLabel->Font = gcnew  System::Drawing::Font("Arial", 10, FontStyle::Bold);
        player2TimerLabel->ForeColor = Color::Black;
        player2TimerLabel->Location = Point(this->ClientSize.Width + 500, 180 + UI_VERTICAL_OFFSET);
        player2TimerLabel->Size = System::Drawing::Size(100, 20);
        player2TimerLabel->Text = "Время: 30 сек";
        this->Controls->Add(player2TimerLabel);
    }
    void AddCheckersForPoint(int pointIndex, int count, CheckerFactory^ factory)
    {
        Color checkerColor = factory->CreateColor();

        for (int i = 0; i < count; i++)
        {
            checkerPositions->Add(GetCheckerPositionOnPoint(pointIndex, GetPointCount(pointIndex, checkerColor)));
            checkerColors->Add(checkerColor);
            checkerPoints->Add(pointIndex);
            checkerMoveCount->Add(0);
        }
    }
    Point GetPointPosition(int pointIndex)
    {
        int x, y;

        if (pointIndex < 12)
        {
            x = boardOffsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
            y = boardHeight - 30 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET;
        }
        else
        {
            x = boardOffsetX + (pointIndex - 12) * triangleWidth * 2 + triangleWidth;
            y = 70 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET;
        }

        return Point(x, y);
    }

    Point GetCheckerPositionOnPoint(int pointIndex, int stackIndex)
    {
        Point basePosition = GetPointPosition(pointIndex);
        int direction = (pointIndex < 12) ? -1 : 1;
        int spacing = 20;
        return Point(basePosition.X, basePosition.Y + (stackIndex * spacing * direction));
    }

    int GetPointCount(int pointIndex, Color color)
    {
        int count = 0;
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerPoints[i] == pointIndex && checkerColors[i] == color)
            {
                count++;
            }
        }
        return count;
    }
    void InitializeCheckers()
    {
        checkerPositions->Clear();
        checkerColors->Clear();
        checkerPoints->Clear();
        checkerMoveCount->Clear();
        barCheckersP1->Clear();
        barCheckersP2->Clear();
        bearOffCheckersP1->Clear();
        bearOffCheckersP2->Clear();

        // Player 1 (dark) checkers using Abstract Factory
        CheckerFactory^ player1Factory = gcnew Player1CheckerFactory();
        AddCheckersForPoint(23, 2, player1Factory);
        AddCheckersForPoint(12, 5, player1Factory);
        AddCheckersForPoint(7, 3, player1Factory);
        AddCheckersForPoint(5, 5, player1Factory);

        // Player 2 (light) checkers using Abstract Factory
        CheckerFactory^ player2Factory = gcnew Player2CheckerFactory();
        AddCheckersForPoint(0, 2, player2Factory);
        AddCheckersForPoint(11, 5, player2Factory);
        AddCheckersForPoint(16, 3, player2Factory);
        AddCheckersForPoint(18, 5, player2Factory);
    }
    
    void TurnTimer_Tick(Object^ sender, EventArgs^ e)
    {
        turnSeconds--;
        UpdateTurnTimerDisplay();

        if (turnSeconds <= 0)
        {
            turnTimer->Stop();
            isTurnTimerActive = false;
            MessageBox::Show("Время на ход истекло!", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            ForceTurnEnd();
        }
    }

    void UpdateTurnTimerDisplay()
    {
        if (currentPlayer == 1)
            player1TimerLabel->Text = String::Format("Время: {0} сек", turnSeconds);
        else
            player2TimerLabel->Text = String::Format("Время: {0} сек", turnSeconds);
    }

    void StartTurnTimer()
    {
        turnSeconds = 30;
        UpdateTurnTimerDisplay();
        turnTimer->Start();
        isTurnTimerActive = true;
    }

    void StopTurnTimer()
    {
        turnTimer->Stop();
        isTurnTimerActive = false;
        UpdateTurnTimerDisplay();
    }

    void ForceTurnEnd()
    {
        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        if (currentPlayer == 1)
        {
            ResetDiceAndEnableNextPlayer(2, (barCheckersP2->Count > 0));
        }
        else
        {
            ResetDiceAndEnableNextPlayer(1, (barCheckersP1->Count > 0));
        }
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        if (isTurnTimerActive)
            return;

        if (moveCounter >= 2 && availableMoves->Count > 0)
        {
            MessageBox::Show("Вы уже переместили две шашки. Завершите свой ход.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        availableMoves->Clear();

        if (currentPlayer == 1)
        {
            player1Dice1 = random->Next(1, 7);
            player1Dice2 = random->Next(1, 7);
            if (player1Dice1 == player1Dice2)
            {
                availableMoves->Add(player1Dice1);
                availableMoves->Add(player1Dice2);
                availableMoves->Add(player1Dice1);
                availableMoves->Add(player1Dice2);
            }
            else
            {
                availableMoves->Add(player1Dice1);
                availableMoves->Add(player1Dice2);
            }
            endTurnButtonP1->Enabled = true;
        }
        else
        {
            player2Dice1 = random->Next(1, 7);
            player2Dice2 = random->Next(1, 7);
            if (player2Dice1 == player2Dice2)
            {
                availableMoves->Add(player2Dice1);
                availableMoves->Add(player2Dice2);
                availableMoves->Add(player2Dice1);
                availableMoves->Add(player2Dice2);
            }
            else
            {
                availableMoves->Add(player2Dice1);
                availableMoves->Add(player2Dice2);
            }
            endTurnButtonP2->Enabled = true;
        }

        mustMoveFromBar = (currentPlayer == 1 && barCheckersP1->Count > 0) ||
            (currentPlayer == 2 && barCheckersP2->Count > 0);

        if (mustMoveFromBar)
        {
            MessageBox::Show("Вы должны сначала переместить шашку из бара!", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        this->Text = String::Format("Backgammon - Ход игрока {0} | Ходы: {1} и {2}",
            currentPlayer, availableMoves[0], availableMoves[1]);

        if (availableMoves->Count > 2)
        {
            this->Text += " (Дубль!)";
        }

        // Запускаем таймер
        StartTurnTimer();

        this->Invalidate();
    }

    void EndTurnButtonP1_Click(Object^ sender, EventArgs^ e)
    {
        StopTurnTimer();
        bool nextPlayerHasBar = (barCheckersP2->Count > 0);
        ResetDiceAndEnableNextPlayer(2, nextPlayerHasBar);
        CheckForAutomaticBearOff(1);
    }

    void EndTurnButtonP2_Click(Object^ sender, EventArgs^ e)
    {
        StopTurnTimer();
        bool nextPlayerHasBar = (barCheckersP1->Count > 0);
        ResetDiceAndEnableNextPlayer(1, nextPlayerHasBar);
        CheckForAutomaticBearOff(2);
    }

    void ResetDiceAndEnableNextPlayer(int nextPlayer, bool nextPlayerHasBar)
    {
        // Останавливаем таймер
        StopTurnTimer();

        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        endTurnButtonP1->Enabled = false;
        endTurnButtonP2->Enabled = false;
        hasMovedFromBar = false;
        mustMoveFromBar = nextPlayerHasBar;

        if (nextPlayer == 1) {
            player1Dice1 = player1Dice2 = 0;
        }
        else {
            player2Dice1 = player2Dice2 = 0;
        }
        // Запускаем таймер
        if (nextPlayer == 1)
        {
            player1TimerLabel->ForeColor = Color::Red;
            player2TimerLabel->ForeColor = Color::Black;
        }
        else
        {
            player2TimerLabel->ForeColor = Color::Red;
            player1TimerLabel->ForeColor = Color::Black;
        }
        rollDiceButton->Enabled = true;
        currentPlayer = nextPlayer;
        moveCounter = 0;
        for (int i = 0; i < checkerMoveCount->Count; i++)
        {
            checkerMoveCount[i] = 0;
        }
        this->Text = String::Format("Backgammon - Ход игрока {0}", currentPlayer);
        this->Invalidate();
    }

    void ShowRulesButton_Click(Object^ sender, EventArgs^ e)
    {
        GameRules^ rules = GameRules::GetInstance();
        MessageBox::Show(rules->GetRules(), "Правила игры в Нарды", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void CalculatePossibleMoves(int checkerIndex)
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

    void CheckAndAddHighlightedPoints(int targetPoint, bool isPlayer1)
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

    void DrawBoard(Graphics^ g)
    {
        // Цвет доски — насыщенный бежевый
        Color boardColor = Color::FromArgb(210, 180, 140);
        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET, boardWidth * 2, boardHeight);
        g->DrawRectangle(Pens::Black, boardOffsetX, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET, boardWidth * 2, boardHeight);

        // Рисуем точки
        for (int i = 0; i < 12; i++)
        {
            DrawPoint(g, i, true);
            DrawPoint(g, i, false);
        }

        // Номера точек
        DrawPointNumbers(g);

        // Надпись "БАР"
        g->DrawString("БАР", gcnew Drawing::Font("Arial", 12, FontStyle::Bold),
            Brushes::Black, PointF(barPositionP1.X - 20, barPositionP1.Y - 50));
    }

    void DrawPoint(Graphics^ g, int pointIndex, bool isTop)
    {
        int x = boardOffsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;
        int y = (isTop ? 50 : 50 + triangleHeight) + VERTICAL_OFFSET + UI_VERTICAL_OFFSET;

        array<Point>^ triangle = gcnew array<Point>(3);
        if (isTop)
        {
            triangle[0] = Point(x - triangleWidth, y);
            triangle[1] = Point(x, y + triangleHeight);
            triangle[2] = Point(x + triangleWidth, y);
        }
        else
        {
            triangle[0] = Point(x - triangleWidth, y + triangleHeight);
            triangle[1] = Point(x, y);
            triangle[2] = Point(x + triangleWidth, y + triangleHeight);
        }

        // Цвета для треугольников — насыщенные
        Color darkColor = Color::FromArgb(139, 69, 19);   // коричневый
        Color lightColor = Color::FromArgb(245, 222, 179); // светлый бежевый

        // Чередование цветов для более красивого эффекта
        Brush^ pointBrush = ((pointIndex % 2 == 0) ^ isTop) ? gcnew SolidBrush(darkColor) : gcnew SolidBrush(lightColor);
        g->FillPolygon(pointBrush, triangle);
        g->DrawPolygon(Pens::Black, triangle);
    }

    void DrawPointNumbers(Graphics^ g)
    {
        Drawing::Font^ font = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);
        Brush^ brush = Brushes::Black;

        for (int i = 0; i < 12; i++)
        {
            int upperNumber = 13 + i;
            int upperX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;

            // Верхние номера — чуть смещены, чтобы не мешать треугольникам
            g->DrawString(upperNumber.ToString(), font, brush, PointF(upperX, 50 - 25 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET));

            int lowerNumber = 12 - i;
            int lowerX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;

            // Нижние номера
            g->DrawString(lowerNumber.ToString(), font, brush, PointF(lowerX, 50 + boardHeight + 10 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET));
        }
    }

    void DrawDice(Graphics^ g, int x, int y, int value, Color dotColor)
    {
        // Основной цвет кубика
        Color diceColor = Color::FromArgb(255, 255, 255); // белый
        Pen^ borderPen = gcnew Pen(Color::DarkSlateGray, 2);
        SolidBrush^ fillBrush = gcnew SolidBrush(diceColor);
        SolidBrush^ dotBrush = gcnew SolidBrush(dotColor);

        // Рисуем кубик с рамкой
        g->FillRectangle(fillBrush, x, y, 60, 60);
        g->DrawRectangle(borderPen, x, y, 60, 60);

        int dotSize = 10;
        int centerX = x + 30;
        int centerY = y + 30;
        int offset = 15;

        // Рисуем точки в зависимости от значения
        switch (value)
        {
        case 1:
            g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
            break;
        case 2:
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
            break;
        case 3:
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
            break;
        case 4:
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
            break;
        case 5:
            g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
            break;
        case 6:
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - 20, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY + 20, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY - 20, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + 20, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - 20, dotSize, dotSize);
            g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY + 20, dotSize, dotSize);
            break;
        }
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;
        g->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;

        g->FillRectangle(Brushes::SkyBlue, 0, 0, this->ClientSize.Width, this->ClientSize.Height);
        DrawBoard(g);

        // Рисуем шашки в баре для игрока 1 (черные)
        for (int i = 0; i < barCheckersP1->Count; i++)
        {
            g->FillEllipse(Brushes::Black, barPositionP1.X - checkerRadius,
                barPositionP1.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
            g->DrawEllipse(Pens::Gray, barPositionP1.X - checkerRadius,
                barPositionP1.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
        }

        // Рисуем шашки в баре для игрока 2 (белые)
        for (int i = 0; i < barCheckersP2->Count; i++)
        {
            g->FillEllipse(Brushes::White, barPositionP2.X - checkerRadius,
                barPositionP2.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
            g->DrawEllipse(Pens::Gray, barPositionP2.X - checkerRadius,
                barPositionP2.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
        }

        // Подсветка возможных ходов
        for each (int point in highlightedPoints)
        {
            Point pos;
            bool isBearOffHighlight = false;
            if (point == 24)
            {
                pos = bearOffPositionP1;
                isBearOffHighlight = true;
            }
            else if (point == -1)
            {
                pos = bearOffPositionP2;
                isBearOffHighlight = true;
            }
            else
            {
                Color highlightColor = (currentPlayer == 1) ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240);
                pos = GetCheckerPositionOnPoint(point, GetPointCount(point, highlightColor));
            }

            g->FillEllipse(Brushes::Yellow, pos.X - checkerRadius - 5, pos.Y - checkerRadius - 5,
                checkerRadius * 2 + 10, checkerRadius * 2 + 10);
        }

        // Рисуем все шашки
        for (int i = 0; i < checkerPositions->Count; i++)
        {
            if (barCheckersP1->Contains(i) || barCheckersP2->Contains(i))
                continue;

            Color color = checkerColors[i];
            Brush^ brush = gcnew SolidBrush(color);
            g->FillEllipse(brush,
                checkerPositions[i].X - checkerRadius,
                checkerPositions[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);

            Pen^ borderPen = gcnew Pen(Color::FromArgb(100, 0, 0, 0), 1.5f);
            g->DrawEllipse(borderPen,
                checkerPositions[i].X - checkerRadius,
                checkerPositions[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);
        }

        // Рисуем кубики
        if (currentPlayer == 1)
        {
            DrawDice(g, 100, 100 + UI_VERTICAL_OFFSET, player1Dice1, Color::Black);
            DrawDice(g, 180, 100 + UI_VERTICAL_OFFSET, player1Dice2, Color::Black);
        }
        else
        {
            DrawDice(g, this->ClientSize.Width - 220, 100 + UI_VERTICAL_OFFSET, player2Dice1, Color::Red);
            DrawDice(g, this->ClientSize.Width - 140, 100 + UI_VERTICAL_OFFSET, player2Dice2, Color::Red);
        }

        // Информация о текущем ходе
        String^ turnText = String::Format("Текущий ход: Игрок {0}", currentPlayer);
        g->DrawString(turnText, gcnew Drawing::Font("Arial", 16, FontStyle::Bold),
            Brushes::DarkBlue, PointF(this->ClientSize.Width / 2 - 100, 10 + UI_VERTICAL_OFFSET));

        if (mustMoveFromBar)
        {
            String^ message = "Вы должны ходить шашкой из бара!";
            g->DrawString(message, gcnew Drawing::Font("Arial", 12, FontStyle::Bold),
                Brushes::Red, PointF(this->ClientSize.Width / 2 - 150, 50 + UI_VERTICAL_OFFSET));
        }
    }

    void CheckForAutomaticBearOff(int player)
    {
        List<int>^ currentPlayerCheckers = gcnew List<int>();
        Color playerColor = (player == 1) ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240);
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerColors[i] == playerColor && checkerPoints[i] != -1 && checkerPoints[i] != -2)
            {
                currentPlayerCheckers->Add(i);
            }
        }

        bool allInHome = true;
        for each (int checkerIndex in currentPlayerCheckers)
        {
            int point = checkerPoints[checkerIndex];
            if (player == 1)
            {
                if (point < 14 || point > 19)
                {
                    allInHome = false;
                    break;
                }
            }
            else
            {
                if (point < 0 || point > 5)
                {
                    allInHome = false;
                    break;
                }
            }
        }

        bool noCheckersInBar = (player == 1 && barCheckersP1->Count == 0) ||
            (player == 2 && barCheckersP2->Count == 0);

        if (allInHome && noCheckersInBar)
        {
            for each (int checkerIndex in currentPlayerCheckers)
            {
                if (player == 1)
                {
                    bearOffCheckersP1->Add(checkerIndex);
                    checkerPoints[checkerIndex] = -2; // условная точка для снятых
                    checkerPositions[checkerIndex] = Point(bearOffPositionP1.X, bearOffPositionP1.Y - checkerRadius + bearOffCheckersP1->Count * 30);
                }
                else
                {
                    bearOffCheckersP2->Add(checkerIndex);
                    checkerPoints[checkerIndex] = -2; // условная точка для снятых
                    checkerPositions[checkerIndex] = Point(bearOffPositionP2.X, bearOffPositionP2.Y - checkerRadius + bearOffCheckersP2->Count * 30);
                }
            }
            CheckForWin();
        }
    }

    void CheckForWin()
    {
        if (bearOffCheckersP1->Count == 15)
        {
            MessageBox::Show("Игрок 1 победил!", "Победа!", MessageBoxButtons::OK, MessageBoxIcon::Information);
            ResetGame();
        }
        else if (bearOffCheckersP2->Count == 15)
        {
            MessageBox::Show("Игрок 2 победил!", "Победа!", MessageBoxButtons::OK, MessageBoxIcon::Information);
            ResetGame();
        }
    }

    void ResetGame()
    {
        InitializeCheckers();
        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        currentPlayer = 1;
        player1Dice1 = player1Dice2 = 0;
        player2Dice1 = player2Dice2 = 0;
        moveCounter = 0;
        hasMovedFromBar = false;
        mustMoveFromBar = false;
        bearingOffPossible = false;
        endTurnButtonP1->Enabled = false;
        endTurnButtonP2->Enabled = false;
        rollDiceButton->Enabled = true;
        turnSeconds = 30;
        turnTimer->Stop();
        UpdateTurnTimerDisplay();
        player1TimerLabel->Text = "Время: 30 сек";
        player2TimerLabel->Text = "Время: 30 сек";
        this->Text = "Backgammon";
        this->Invalidate();
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (availableMoves->Count == 0) return;

        if (mustMoveFromBar)
        {
            for (int i = 0; i < checkerPositions->Count; i++)
            {
                bool isPlayer1 = (checkerColors[i].R < 100);
                if ((currentPlayer == 1 && isPlayer1 && barCheckersP1->Contains(i)) ||
                    (currentPlayer == 2 && !isPlayer1 && barCheckersP2->Contains(i)))
                {
                    int barIndex = isPlayer1 ? barCheckersP1->IndexOf(i) : barCheckersP2->IndexOf(i);
                    Point barCheckerPos = isPlayer1 ? Point(barPositionP1.X, barPositionP1.Y - checkerRadius + barIndex * 30) : Point(barPositionP2.X, barPositionP2.Y - checkerRadius + barIndex * 30);

                    int dx = e->X - barCheckerPos.X;
                    int dy = e->Y - barCheckerPos.Y;

                    if (dx * dx + dy * dy <= checkerRadius * checkerRadius)
                    {
                        selectedCheckerIndex = i;
                        initialCheckerPosition = checkerPositions[i];
                        CalculatePossibleMoves(i);
                        this->Invalidate();
                        return;
                    }
                }
            }
            return;
        }

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            bool isPlayer1 = (checkerColors[i].R < 100);
            if ((currentPlayer == 1 && isPlayer1) || (currentPlayer == 2 && !isPlayer1))
            {
                if ((isPlayer1 && barCheckersP1->Contains(i)) || (!isPlayer1 && barCheckersP2->Contains(i))
                    || (isPlayer1 && bearOffCheckersP1->Contains(i)) || (!isPlayer1 && bearOffCheckersP2->Contains(i)))
                {
                    continue;
                }

                int dx = e->X - checkerPositions[i].X;
                int dy = e->Y - checkerPositions[i].Y;
                if (dx * dx + dy * dy <= checkerRadius * checkerRadius)
                {
                    selectedCheckerIndex = i;
                    initialCheckerPosition = checkerPositions[i];
                    CalculatePossibleMoves(i);
                    this->Invalidate();
                    break;
                }
            }
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (selectedCheckerIndex != -1)
        {
            Point currentPos = checkerPositions[selectedCheckerIndex];
            currentPos.X = e->X;
            currentPos.Y = e->Y;
            checkerPositions[selectedCheckerIndex] = currentPos;
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (selectedCheckerIndex == -1) return;

        if (e->Button == System::Windows::Forms::MouseButtons::Right)
        {
            checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
            selectedCheckerIndex = -1;
            highlightedPoints->Clear();
            this->Invalidate();
            return;
        }

        bool isPlayer1 = (checkerColors[selectedCheckerIndex].R < 100);
        bool isInBar = (isPlayer1 && barCheckersP1->Contains(selectedCheckerIndex)) ||
            (!isPlayer1 && barCheckersP2->Contains(selectedCheckerIndex));

        bool moved = false;

        for each (int targetPoint in highlightedPoints)
        {
            Point targetPos;
            bool isBearOffTarget = false;

            if (targetPoint == 24 && isPlayer1)
            {
                targetPos = bearOffPositionP1;
                isBearOffTarget = true;
            }
            else if (targetPoint == -1 && !isPlayer1)
            {
                targetPos = bearOffPositionP2;
                isBearOffTarget = true;
            }
            else if (targetPoint >= 0 && targetPoint < 24)
            {
                Color targetColor = (currentPlayer == 1) ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240);
                targetPos = GetCheckerPositionOnPoint(targetPoint, GetPointCount(targetPoint, targetColor));
            }
            else
            {
                continue; // Некорректная цель
            }

            int dx = e->X - targetPos.X;
            int dy = e->Y - targetPos.Y;

            if (dx * dx + dy * dy <= checkerRadius * checkerRadius * 4)
            {
                if (isInBar)
                {
                    hasMovedFromBar = true;
                    mustMoveFromBar = false; // снимаем флаг обязательного хода из бара

                    int moveUsed = isPlayer1 ? (24 - targetPoint) : (targetPoint + 1);
                    bool validMoveFromBar = false;
                    for each (int move in availableMoves)
                    {
                        if (move == moveUsed)
                        {
                            validMoveFromBar = true;
                            availableMoves->Remove(move);
                            break;
                        }
                    }

                    if (validMoveFromBar)
                    {
                        checkerPositions[selectedCheckerIndex] = targetPos;
                        checkerPoints[selectedCheckerIndex] = targetPoint;

                        if (isPlayer1)
                            barCheckersP1->Remove(selectedCheckerIndex);
                        else
                            barCheckersP2->Remove(selectedCheckerIndex);

                        moveCounter++;
                        moved = true;
                    }
                    else
                    {
                        // неправильный ход из бара
                        checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
                    }
                }
                else if (isBearOffTarget)
                {
                    int currentPoint = checkerPoints[selectedCheckerIndex];
                    int distanceToBearOff = isPlayer1 ? (6 - currentPoint) : (currentPoint - 17);
                    // Проверка на правильность снятия
                    bool validBearOffMove = false;
                    int usedMove = -1;

                    for each (int move in availableMoves)
                    {
                        if (move == distanceToBearOff)
                        {
                            validBearOffMove = true;
                            usedMove = move;
                            break;
                        }
                    }

                    if (!validBearOffMove)
                    {
                        int maxAvailableMove = 0;
                        for each (int move in availableMoves)
                        {
                            if (move > maxAvailableMove)
                                maxAvailableMove = move;
                        }

                        if ((isPlayer1 && maxAvailableMove > distanceToBearOff) || (!isPlayer1 && maxAvailableMove > Math::Abs(distanceToBearOff)))
                        {
                            bool furtherCheckersExist = false;
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
                            if (!furtherCheckersExist)
                            {
                                validBearOffMove = true;
                                usedMove = maxAvailableMove;
                            }
                        }
                    }

                    if (validBearOffMove)
                    {
                        if (isPlayer1)
                        {
                            bearOffCheckersP1->Add(selectedCheckerIndex);
                            checkerPositions[selectedCheckerIndex] = Point(bearOffPositionP1.X, bearOffPositionP1.Y - checkerRadius + bearOffCheckersP1->Count * 30);
                        }
                        else
                        {
                            bearOffCheckersP2->Add(selectedCheckerIndex);
                            checkerPositions[selectedCheckerIndex] = Point(bearOffPositionP2.X, bearOffPositionP2.Y - checkerRadius + bearOffCheckersP2->Count * 30);
                        }
                        checkerPoints[selectedCheckerIndex] = -2;

                        availableMoves->Remove(usedMove);
                        moveCounter++;
                        moved = true;

                        CheckForWin();
                    }
                    else
                    {
                        // неправильный ход
                        checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
                    }
                }
                else
                {
                    int currentPoint = checkerPoints[selectedCheckerIndex];
                    int moveLength = Math::Abs(targetPoint - currentPoint);

                    bool validNormalMove = false;
                    for each (int move in availableMoves)
                    {
                        if (move == moveLength)
                        {
                            validNormalMove = true;
                            availableMoves->Remove(move);
                            break;
                        }
                    }

                    if (validNormalMove)
                    {
                        Color opponentColor = (currentPlayer == 1) ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);
                        int opponentCount = 0;

                        for (int i = 0; i < checkerPoints->Count; i++)
                        {
                            if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                            {
                                opponentCount++;
                            }
                        }

                        if (opponentCount < 2)
                        {
                            if (opponentCount > 0)
                            {
                                for (int i = 0; i < checkerPoints->Count; i++)
                                {
                                    if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                                    {
                                        if (currentPlayer == 1)
                                        {
                                            barCheckersP2->Add(i);
                                            checkerPoints[i] = -1;
                                            checkerPositions[i] = Point(barPositionP2.X, barPositionP2.Y - checkerRadius + barCheckersP2->Count * 30);
                                        }
                                        else
                                        {
                                            barCheckersP1->Add(i);
                                            checkerPoints[i] = -1;
                                            checkerPositions[i] = Point(barPositionP1.X, barPositionP1.Y - checkerRadius + barCheckersP1->Count * 30);
                                        }
                                        break;
                                    }
                                }
                            }
                            checkerPositions[selectedCheckerIndex] = targetPos;
                            checkerPoints[selectedCheckerIndex] = targetPoint;
                            checkerMoveCount[selectedCheckerIndex]++;
                            moveCounter++;
                            moved = true;
                        }
                        else
                        {
                            // поле занято
                            checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
                        }
                    }
                    else
                    {
                        // неправильный ход
                        checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
                    }
                }
                break;
            }
        }

        if (!moved)
        {
            checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
        }

        selectedCheckerIndex = -1;
        highlightedPoints->Clear();
        this->Invalidate();

        bearingOffPossible = CheckIfBearingOffPossible(currentPlayer);
    }

    bool CheckIfBearingOffPossible(int player)
    {
        List<int>^ currentPlayerCheckers = gcnew List<int>();
        Color playerColor = (player == 1) ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240);

        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerColors[i] == playerColor && checkerPoints[i] != -1 && checkerPoints[i] != -2)
            {
                currentPlayerCheckers->Add(i);
            }
        }

        if (currentPlayerCheckers->Count != 15 - ((player == 1) ? bearOffCheckersP1->Count : bearOffCheckersP2->Count))
        {
            return false; // Не все шашки на доске
        }

        // Проверяем, все ли шашки находятся в доме
        bool allInHome = true;
        for each (int checkerIndex in currentPlayerCheckers)
        {
            if (player == 1)
            {
                if (checkerPoints[checkerIndex] > 5)
                {
                    allInHome = false;
                    break;
                }
            }
            else
            {
                if (checkerPoints[checkerIndex] < 18)
                {
                    allInHome = false;
                    break;
                }
            }
        }

        // Проверяем, нет ли шашек в баре
        bool noCheckersInBar = (player == 1 && barCheckersP1->Count == 0) ||
            (player == 2 && barCheckersP2->Count == 0);

        return allInHome && noCheckersInBar;
    } 

};