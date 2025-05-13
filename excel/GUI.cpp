#include "GUI.h"
#include "Board.h"
#include "Bar.h"
#include "Dice.h"
#include "Rules.h"

// Внутренние константы
const int VERTICAL_OFFSET = 50;
const int UI_VERTICAL_OFFSET = 100;
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
void Form_Paint(Object^ sender, PaintEventArgs^ e) {
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
    void ShowRulesButton_Click(Object^ sender, EventArgs^ e)
    {
        GameRules^ rules = GameRules::GetInstance();
        MessageBox::Show(rules->GetRules(), "Правила игры в Нарды", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
    void UpdateTurnTimerDisplay()
    {
        if (currentPlayer == 1)
            player1TimerLabel->Text = String::Format("Время: {0} сек", turnSeconds);
        else
            player2TimerLabel->Text = String::Format("Время: {0} сек", turnSeconds);
    }