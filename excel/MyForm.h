#pragma once
#include <Windows.h>
#include <gcroot.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

public ref class BackgammonForm : public Form
{
private:
    List<Point>^ checkerPositions;
    List<Color>^ checkerColors;
    List<int>^ checkerPoints; // Текущий пункт для каждой шашки
    int checkerRadius;

    Random^ random;

    // Game state
    int currentPlayer;
    int player1Dice1, player1Dice2;
    int player2Dice1, player2Dice2;
    List<int>^ availableMoves;
    List<int>^ highlightedPoints;
    int selectedCheckerIndex;
    Point initialCheckerPosition; // Переменная для хранения начальной позиции шашки

    // UI elements
    Button^ rollDiceButton;
    Button^ endTurnButtonP1;
    Button^ endTurnButtonP2;

    // Board dimensions
    int boardWidth;
    int boardHeight;
    int triangleWidth;
    int triangleHeight;
    int boardOffsetX;

    // Move-related variables
    int moveCounter; // Счетчик перемещенных шашек
    List<int>^ checkerMoveCount; // Счетчик перемещений для каждой шашки в данном ходе

public:
    BackgammonForm()
    {
        this->Text = "Backgammon";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1200, 700);
        this->BackColor = Color::FromArgb(240, 240, 240);

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
        moveCounter = 0; // Инициализируем счётчик перемещенных шашек

        // Инициализируем счетчик перемещений для каждой шашки
        checkerMoveCount = gcnew List<int>();

        InitializeUI();
        InitializeCheckers();

        // Каждая шашка изначально не перемещалась
        for (int i = 0; i < checkerPositions->Count; i++)
        {
            checkerMoveCount->Add(0);
        }

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
            this->ClientSize.Width / 2 - rollDiceButton->Width / 2,
            this->ClientSize.Height - 60);
        rollDiceButton->BackColor = Color::LightSkyBlue;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        rollDiceButton->Click += gcnew EventHandler(this, &BackgammonForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

        endTurnButtonP1 = gcnew Button();
        endTurnButtonP1->Text = "Завершить ход";
        endTurnButtonP1->Size = System::Drawing::Size(100, 40);
        endTurnButtonP1->Location = System::Drawing::Point(50, this->ClientSize.Height - 60);
        endTurnButtonP1->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP1->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP1->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP1_Click);
        endTurnButtonP1->Enabled = false;
        this->Controls->Add(endTurnButtonP1);

        endTurnButtonP2 = gcnew Button();
        endTurnButtonP2->Text = "Завершить ход";
        endTurnButtonP2->Size = System::Drawing::Size(100, 40);
        endTurnButtonP2->Location = System::Drawing::Point(
            this->ClientSize.Width - 150,
            this->ClientSize.Height - 60);
        endTurnButtonP2->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP2->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP2->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP2_Click);
        endTurnButtonP2->Enabled = false;
        this->Controls->Add(endTurnButtonP2);
    }

    void AddCheckersForPoint(int pointIndex, int count, Color color)
    {
        Point position = GetPointPosition(pointIndex);
        int direction = (pointIndex < 12) ? -1 : 1; // Вверх для нижних, вниз для верхних
        int spacing = 20; // Расстояние между шашками

        // Подсчет уже существующих шашек на данной позиции
        int existingCount = 0;
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerPoints[i] == pointIndex)
            {
                existingCount++;
            }
        }

        // Добавление новых шашек с учетом уже существующих
        for (int i = 0; i < count; i++)
        {
            int y = position.Y + ((existingCount + i) * spacing * direction); // Увеличиваем Y в зависимости от количества уже существующих шашек
            checkerPositions->Add(Point(position.X, y));
            checkerColors->Add(color);
            checkerPoints->Add(pointIndex);
            checkerMoveCount->Add(0); // Изначально шашка не перемещалась
        }
    }

    Point GetPointPosition(int pointIndex)
    {
        int x, y;

        if (pointIndex < 12)
        {
            // Нижние треугольники (1-12)
            x = boardOffsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
            y = boardHeight - 30;
        }
        else
        {
            // Верхние треугольники (13-24)
            x = boardOffsetX + (pointIndex - 12) * triangleWidth * 2 + triangleWidth;
            y = 70;
        }

        return Point(x, y);
    }

    void InitializeCheckers()
    {
        checkerPositions->Clear();
        checkerColors->Clear();
        checkerPoints->Clear();
        checkerMoveCount->Clear(); // Очищаем счетчики перемещений

        // Player 1 (dark) checkers
        AddCheckersForPoint(0, 2, Color::FromArgb(50, 50, 50));   // Пункт 1
        AddCheckersForPoint(11, 5, Color::FromArgb(50, 50, 50));  // Пункт 12
        AddCheckersForPoint(16, 3, Color::FromArgb(50, 50, 50));  // Пункт 17
        AddCheckersForPoint(18, 5, Color::FromArgb(50, 50, 50));  // Пункт 19

        // Player 2 (light) checkers
        AddCheckersForPoint(5, 5, Color::FromArgb(240, 240, 240));  // Пункт 6
        AddCheckersForPoint(7, 3, Color::FromArgb(240, 240, 240));  // Пункт 8
        AddCheckersForPoint(12, 5, Color::FromArgb(240, 240, 240)); // Пункт 13
        AddCheckersForPoint(23, 2, Color::FromArgb(240, 240, 240)); // Пункт 24
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        if (moveCounter >= 2) // Проверка, можно ли бросить кубики
        {
            MessageBox::Show("Вы уже переместили две шашки. Завершите свой ход.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        availableMoves->Clear(); // Очистка доступных ходов перед броском кубиков

        if (currentPlayer == 1)
        {
            player1Dice1 = random->Next(1, 7);
            player1Dice2 = random->Next(1, 7);
            if (player1Dice1 == player1Dice2) // Проверяем, выпал ли дубль
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
            if (player2Dice1 == player2Dice2) // Проверяем, выпал ли дубль
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

        this->Text = String::Format("Backgammon - Player {0}'s Turn | Moves: {1} and {2}",
            currentPlayer, availableMoves[0], availableMoves[1]);

        // Отображаем, что это дубль
        if (availableMoves->Count > 2)
        {
            this->Text += " (Double!)";
        }

        rollDiceButton->Enabled = false; // Блокируем бросок кубиков
        this->Invalidate();
    }

    void EndTurnButtonP1_Click(Object^ sender, EventArgs^ e)
    {
        currentPlayer = 2;
        ResetTurn();
        this->Text = "Backgammon - Player 2's Turn";
        this->Invalidate();
    }

    void EndTurnButtonP2_Click(Object^ sender, EventArgs^ e)
    {
        currentPlayer = 1;
        ResetTurn();
        this->Text = "Backgammon - Player 1's Turn";
        this->Invalidate();
    }

    void ResetTurn()
    {
        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        endTurnButtonP1->Enabled = false;
        endTurnButtonP2->Enabled = false;
        rollDiceButton->Enabled = true;

        // Сбрасываем счетчик перемещенных шашек и счетчики перемещений для шашек
        moveCounter = 0;
        for (int i = 0; i < checkerMoveCount->Count; i++)
        {
            checkerMoveCount[i] = 0; // Сбрасываем флаг перемещения каждой шашки
        }
    }

    void CalculatePossibleMoves(int checkerIndex)
    {
        highlightedPoints->Clear();

        int currentPoint = checkerPoints[checkerIndex];
        bool isPlayer1 = (checkerColors[checkerIndex].R < 100);

        for each(int move in availableMoves)
        {
            int targetPoint;
            if (isPlayer1)
            {
                // Игрок 1 движется по возрастанию (1->24)
                targetPoint = currentPoint + move;
            }
            else
            {
                // Игрок 2 движется по убыванию (24->1)
                targetPoint = currentPoint - move;
            }

            // Проверяем, что пункт существует (0-23)
            if (targetPoint >= 0 && targetPoint < 24)
            {
                // Проверяем, можно ли установить шашку на целевую позицию
                int opponentCount = 0;
                Color opponentColor = isPlayer1 ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                    {
                        opponentCount++;
                    }
                }

                // Добавляем подсветку, если на целевой позиции не более одной противника
                if (opponentCount < 2)
                {
                    highlightedPoints->Add(targetPoint);
                }
            }
        }
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;
        g->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;

        DrawBoard(g);
        DrawHomeArea(g); // Рисуем дома

        // Рисуем подсвеченные пункты
        for each(int point in highlightedPoints)
        {
            Point pos = GetPointPosition(point);
            g->FillEllipse(Brushes::Yellow, pos.X - checkerRadius - 5, pos.Y - checkerRadius - 5,
                checkerRadius * 2 + 10, checkerRadius * 2 + 10);
        }

        // Рисуем шашки
        for (int i = 0; i < checkerPositions->Count; i++)
        {
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
            DrawDice(g, 100, 100, player1Dice1, Color::Black);
            DrawDice(g, 180, 100, player1Dice2, Color::Black);
        }
        else
        {
            DrawDice(g, this->ClientSize.Width - 220, 100, player2Dice1, Color::Red);
            DrawDice(g, this->ClientSize.Width - 140, 100, player2Dice2, Color::Red);
        }

        // Отображаем текущего игрока
        String^ turnText = String::Format("Текущий ход: Игрок {0}", currentPlayer);
        g->DrawString(turnText, gcnew Drawing::Font("Arial", 16, FontStyle::Bold),
            Brushes::DarkBlue, PointF(this->ClientSize.Width / 2 - 100, 10));
    }

    void DrawBoard(Graphics^ g)
    {
        // Светло-коричневый цвет для всей доски
        Color boardColor = Color::FromArgb(210, 180, 140); // Светло-коричневый

        // Заливаем левую половину доски светло-коричневым цветом
        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX, 50, boardWidth, boardHeight);

        // Заливаем правую половину доски светло-коричневым цветом
        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX + boardWidth, 50, boardWidth, boardHeight);

        // Рисуем границу доски
        g->DrawRectangle(Pens::Black, boardOffsetX, 50, boardWidth, boardHeight);

        // Рисуем треугольные пункты (поинты)
        for (int i = 0; i < 12; i++)
        {
            DrawPoint(g, i, true);  // Верхние треугольники (пункты 1-12)
            DrawPoint(g, i, false); // Нижние треугольники (пункты 13-24)
        }

        // Рисуем номера пунктов
        DrawPointNumbers(g);
    }

    void DrawHomeArea(Graphics^ g)
    {
        // Рисуем дом игрока 1 (нижняя часть)
        Color homeColor1 = Color::FromArgb(200, 220, 255);
        g->FillRectangle(gcnew SolidBrush(homeColor1), boardOffsetX, boardHeight - 30, boardWidth / 2, 30);
        g->DrawString("Дом Игрока 1", gcnew Font("Arial", 12), Brushes::Black, boardOffsetX + 10, boardHeight - 25);

        // Рисуем дом игрока 2 (верхняя часть)
        Color homeColor2 = Color::FromArgb(255, 220, 220);
        g->FillRectangle(gcnew SolidBrush(homeColor2), boardOffsetX + boardWidth / 2, 50, boardWidth / 2, 30);
        g->DrawString("Дом Игрока 2", gcnew Font("Arial", 12), Brushes::Black,
            boardOffsetX + boardWidth / 2 + 10, 55);
    }

    void DrawPoint(Graphics^ g, int pointIndex, bool isTop)
    {
        int x = boardOffsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;
        int y = isTop ? 50 : 50 + triangleHeight;

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

        Color darkColor = Color::FromArgb(139, 69, 19);
        Color lightColor = Color::FromArgb(245, 222, 179);

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
            // Верхние треугольники (13-24)
            int upperNumber = 13 + i;
            int upperX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            g->DrawString(upperNumber.ToString(), font, brush, PointF(upperX, 50 - 25));

            // Нижние треугольники (1-12)
            int lowerNumber = 12 - i;
            int lowerX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            g->DrawString(lowerNumber.ToString(), font, brush, PointF(lowerX, 50 + boardHeight + 10));
        }
    }

    void DrawDice(Graphics^ g, int x, int y, int value, Color dotColor)
    {
        g->FillRectangle(Brushes::White, x, y, 60, 60);
        g->DrawRectangle(Pens::Black, x, y, 60, 60);
        Brush^ dotBrush = gcnew SolidBrush(dotColor);
        int dotSize = 10;
        int dotOffset = 15;

        switch (value)
        {
        case 1:
            g->FillEllipse(dotBrush, x + 30 - dotSize / 2, y + 30 - dotSize / 2, dotSize, dotSize);
            break;
        case 2:
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 3:
            g->FillEllipse(dotBrush, x + 30 - dotSize / 2, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 4:
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 5:
            g->FillEllipse(dotBrush, x + 30 - dotSize / 2, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 6:
            g->FillEllipse(dotBrush, x + dotOffset, y + 10, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 50, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 10, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 50, dotSize, dotSize);
            break;
        }
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (availableMoves->Count == 0) return;

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            bool isPlayer1 = (checkerColors[i].R < 100);
            if ((currentPlayer == 1 && isPlayer1) || (currentPlayer == 2 && !isPlayer1))
            {
                int dx = e->X - checkerPositions[i].X;
                int dy = e->Y - checkerPositions[i].Y;
                if (dx * dx + dy * dy <= checkerRadius * checkerRadius)
                {
                    selectedCheckerIndex = i;
                    initialCheckerPosition = checkerPositions[i]; // Сохраняем начальную позицию
                    CalculatePossibleMoves(i);
                    this->Invalidate();
                    break;
                }
            }
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        // Проверяем, выбрана ли шашка
        if (selectedCheckerIndex != -1)
        {
            // Получаем текущую позицию шашки
            Point currentPos = checkerPositions[selectedCheckerIndex];

            // Перемещаем шашку в положение курсора мыши с некоторыми ограничениями
            currentPos.X = e->X - (checkerRadius); // Смещаем для центрирования
            currentPos.Y = e->Y - (checkerRadius); // Смещаем для центрирования

            // Обновляем позицию шашки
            checkerPositions[selectedCheckerIndex] = currentPos;

            // Перерисовываем форму, чтобы отобразить изменения
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (e->Button == System::Windows::Forms::MouseButtons::Right) // Проверяем, правая кнопка мыши
        {
            // Возвращаем шашку на начальную позицию
            if (selectedCheckerIndex != -1)
            {
                checkerPositions[selectedCheckerIndex] = initialCheckerPosition; // Возвращаем на исходное место
                selectedCheckerIndex = -1; // Сбрасываем выбранную шашку
                this->Invalidate(); // Перерисовываем форму
            }
            return; // Выходим, чтобы не выполнять дальнейших действий
        }

        if (selectedCheckerIndex != -1 && highlightedPoints->Count > 0)
        {
            // Проверяем, куда переместили шашку
            for each(int targetPoint in highlightedPoints)
            {
                Point targetPos = GetPointPosition(targetPoint);
                int dx = e->X - targetPos.X;
                int dy = e->Y - targetPos.Y;

                if (dx * dx + dy * dy <= checkerRadius * checkerRadius * 4)
                {
                    // Вычисляем длину хода
                    int currentPoint = checkerPoints[selectedCheckerIndex];
                    int moveLength = Math::Abs(targetPoint - currentPoint);

                    // Проверяем, что такой ход допустим
                    if (availableMoves->Contains(moveLength))
                    {
                        // Подсчитываем количество шашек на целевой позиции
                        int opponentCount = 0;
                        Color opponentColor = (checkerColors[selectedCheckerIndex].R < 100) ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                        // Считаем количество шашек противника на целевой позиции
                        for (int i = 0; i < checkerPoints->Count; i++)
                        {
                            if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                            {
                                opponentCount++;
                            }
                        }

                        // Проверяем, что на целевой позиции можно установить шашку
                        if (opponentCount < 2) // Если 0 или 1 противник, то можно ставить
                        {
                            // Обновляем состояние шашки
                            checkerPositions[selectedCheckerIndex] = targetPos;
                            checkerPoints[selectedCheckerIndex] = targetPoint;

                            // Увеличиваем счетчик перемещения для данной шашки
                            checkerMoveCount[selectedCheckerIndex]++;
                            moveCounter++; // Увеличиваем счетчик перемещенных шашек

                            // Удаляем использованный ход
                            availableMoves->Remove(moveLength);

                            // Проверяем, готовы ли шашки к выбрасыванию
                            if (CanExitAllCheckers())
                            {
                                // Игрок готов начать выбрасывать шашки
                                this->Text = String::Format("Игрок {0} может выбрасывать шашки. Нажмите 'Бросить кубики' для броска.", currentPlayer);
                            }

                            // Проверяем, если достигнуто максимальное количество перемещений
                            if (moveCounter >= 2)
                            {
                                // Блокируем возможность последующих ходов
                                rollDiceButton->Enabled = false;
                                this->Text = String::Format("Backgammon - Player {0}'s Turn | Все ходы выполнены.", currentPlayer);
                            }

                            break;
                        }
                    }
                }
            }

            selectedCheckerIndex = -1;
            highlightedPoints->Clear();
            this->Invalidate();
        }
    }

    bool CanExitAllCheckers()
    {
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerColors[i] == (currentPlayer == 1 ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240)))
            {
                if (checkerPoints[i] >= 0 && checkerPoints[i] <= 5) // Проверяем, находятся ли шашки в своем доме
                    return false;
            }
        }
        return true; // Все шашки в доме
    }

    void RemoveCheckersFromBoard()
    {
        // Удаляем шашки с доски, которые находятся в доме и соответствуют выпавшим значениям
        List<int>^ outgoingCheckers = gcnew List<int>();

        if (currentPlayer == 1)
        {
            // Удаление шашек для игрока 1
            if (checkerPoints->Contains(player1Dice1 - 1)) // Если выпало значение первого кубика
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == player1Dice1 - 1 && checkerColors[i] == Color::FromArgb(50, 50, 50))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
            if (checkerPoints->Contains(player1Dice2 - 1)) // Если выпало значение второго кубика
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == player1Dice2 - 1 && checkerColors[i] == Color::FromArgb(50, 50, 50))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
        }
        else
        {
            // Удаление шашек для игрока 2
            if (checkerPoints->Contains(24 - player2Dice1)) // Если выпало значение первого кубика
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == 24 - player2Dice1 && checkerColors[i] == Color::FromArgb(240, 240, 240))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
            if (checkerPoints->Contains(24 - player2Dice2)) // Если выпало значение второго кубика
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == 24 - player2Dice2 && checkerColors[i] == Color::FromArgb(240, 240, 240))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
        }

        // Удаляем найденные шашки из списка
        for each(int index in outgoingCheckers)
        {
            checkerPositions->RemoveAt(index);
            checkerColors->RemoveAt(index);
            checkerPoints->RemoveAt(index);
        }
    }

    // После завершения хода игрока, добавим вызов выполнимой функции для выбрасывания шашек
    void CompleteTurn()
    {
        if (CanExitAllCheckers())
        {
            MessageBox::Show("Игрок может начать выбрасывать шашки!", "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            // Игрок выбрасывает шашки
            RemoveCheckersFromBoard();
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1; // Передача хода
        ResetTurn(); // Сброс состояния хода
    }
};