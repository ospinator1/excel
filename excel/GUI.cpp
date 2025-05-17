#include "GUI.h"

BackgammonForm::BackgammonForm()
{
    this->Text = "Backgammon";
    this->DoubleBuffered = true;
    this->ClientSize = System::Drawing::Size(1200, 750);
    this->BackColor = System::Drawing::Color::SkyBlue;

    InitializeGameComponents();
    InitializeUI();

    this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &BackgammonForm::Form_MouseDown);
    this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &BackgammonForm::Form_MouseMove);
    this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &BackgammonForm::Form_MouseUp);
    this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &BackgammonForm::Form_Paint);
}

void BackgammonForm::InitializeGameComponents()
{
    int boardWidth = 700;
    int boardHeight = 500;
    int boardOffsetX = (this->ClientSize.Width - boardWidth) / 2;

    board = gcnew Board(boardWidth, boardHeight, boardOffsetX);
    checkers = gcnew Checkers(20);
    dice = gcnew Dice();
    game = gcnew Game();

    Point barPositionP1 = Point(board->BoardOffsetX + board->BoardWidth - 20, board->BoardHeight / 2 + 150);
    Point barPositionP2 = Point(board->BoardOffsetX + board->BoardWidth + 20, board->BoardHeight / 2 + 150);
    bar = gcnew Bar(barPositionP1, barPositionP2);

    timer = gcnew GameTimer();
    timer->turnTimer->Tick += gcnew System::EventHandler(this, &BackgammonForm::TurnTimer_Tick);

    rules = GameRules::GetInstance();

    CheckerFactory^ player1Factory = gcnew Player1CheckerFactory();
    CheckerFactory^ player2Factory = gcnew Player2CheckerFactory();
    checkers->InitializeCheckers(player1Factory, player2Factory);
}

void BackgammonForm::InitializeUI()
{
    rollDiceButton = gcnew System::Windows::Forms::Button();
    rollDiceButton->Text = "Бросить кубики";
    rollDiceButton->Size = System::Drawing::Size(120, 40);
    rollDiceButton->Location = System::Drawing::Point(
        this->ClientSize.Width / 2 - rollDiceButton->Width / 2 + 350,
        this->ClientSize.Height - 100 + 100);
    rollDiceButton->BackColor = System::Drawing::Color::LightSkyBlue;
    rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    rollDiceButton->Click += gcnew System::EventHandler(this, &BackgammonForm::RollDiceButton_Click);
    this->Controls->Add(rollDiceButton);

    endTurnButtonP1 = gcnew System::Windows::Forms::Button();
    endTurnButtonP1->Text = "Завершить ход";
    endTurnButtonP1->Size = System::Drawing::Size(100, 40);
    endTurnButtonP1->Location = System::Drawing::Point(
        this->ClientSize.Width - 1100,
        this->ClientSize.Height - 100);
    endTurnButtonP1->BackColor = System::Drawing::Color::FromArgb(220, 220, 220);
    endTurnButtonP1->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    endTurnButtonP1->Click += gcnew System::EventHandler(this, &BackgammonForm::EndTurnButtonP1_Click);
    endTurnButtonP1->Enabled = false;
    this->Controls->Add(endTurnButtonP1);

    endTurnButtonP2 = gcnew System::Windows::Forms::Button();
    endTurnButtonP2->Text = "Завершить ход";
    endTurnButtonP2->Size = System::Drawing::Size(100, 40);
    endTurnButtonP2->Location = System::Drawing::Point(
        this->ClientSize.Width - 100,
        this->ClientSize.Height - 100);
    endTurnButtonP2->BackColor = System::Drawing::Color::FromArgb(220, 220, 220);
    endTurnButtonP2->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    endTurnButtonP2->Click += gcnew System::EventHandler(this, &BackgammonForm::EndTurnButtonP2_Click);
    endTurnButtonP2->Enabled = false;
    this->Controls->Add(endTurnButtonP2);

    showRulesButton = gcnew System::Windows::Forms::Button();
    showRulesButton->Text = "Правила игры";
    showRulesButton->Size = System::Drawing::Size(120, 40);
    showRulesButton->Location = System::Drawing::Point(10, 10);
    showRulesButton->BackColor = System::Drawing::Color::LightGreen;
    showRulesButton->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    showRulesButton->Click += gcnew System::EventHandler(this, &BackgammonForm::ShowRulesButton_Click);
    this->Controls->Add(showRulesButton);

    player1TimerLabel = gcnew System::Windows::Forms::Label();
    player1TimerLabel->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    player1TimerLabel->ForeColor = System::Drawing::Color::Black;
    player1TimerLabel->Location = System::Drawing::Point(100, 180 + 100);
    player1TimerLabel->Size = System::Drawing::Size(100, 20);
    player1TimerLabel->Text = "Время: 30 сек";
    this->Controls->Add(player1TimerLabel);

    player2TimerLabel = gcnew System::Windows::Forms::Label();
    player2TimerLabel->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    player2TimerLabel->ForeColor = System::Drawing::Color::Black;
    player2TimerLabel->Location = System::Drawing::Point(this->ClientSize.Width - 200, 180 + 100);
    player2TimerLabel->Size = System::Drawing::Size(100, 20);
    player2TimerLabel->Text = "Время: 30 сек";
    this->Controls->Add(player2TimerLabel);
}

void BackgammonForm::RollDiceButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (timer->IsActive) return;

    if (game->MoveCounter >= 2 && game->AvailableMoves->Count > 0)
    {
        System::Windows::Forms::MessageBox::Show("Вы уже переместили две шашки. Завершите свой ход.", "Ошибка",
            System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Warning);
        return;
    }

    dice->RollDice();
    game->RollDice(dice->GetDiceValues());

    if (game->CurrentPlayer == 1)
    {
        endTurnButtonP1->Enabled = true;
    }
    else
    {
        endTurnButtonP2->Enabled = true;
    }

    game->MustMoveFromBar = (game->CurrentPlayer == 1 && bar->Player1Checkers->Count > 0) ||
        (game->CurrentPlayer == 2 && bar->Player2Checkers->Count > 0);

    if (game->MustMoveFromBar)
    {
        System::Windows::Forms::MessageBox::Show("Вы должны сначала переместить шашку из бара!", "Внимание",
            System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
    }

    this->Text = System::String::Format("Backgammon - Ход игрока {0} | Ходы: {1} и {2}",
        game->CurrentPlayer, game->AvailableMoves[0], game->AvailableMoves[1]);

    if (game->AvailableMoves->Count > 2)
    {
        this->Text += " (Дубль!)";
    }

    timer->StartTurnTimer();
    this->Invalidate();
}

void BackgammonForm::EndTurnButtonP1_Click(System::Object^ sender, System::EventArgs^ e)
{
    timer->StopTimer();
    bool nextPlayerHasBar = (bar->Player2Checkers->Count > 0);
    game->SwitchPlayer();
    game->MustMoveFromBar = nextPlayerHasBar;
    endTurnButtonP1->Enabled = false;
    rollDiceButton->Enabled = true;
    CheckForAutomaticBearOff(1);
    this->Invalidate();
}

void BackgammonForm::EndTurnButtonP2_Click(System::Object^ sender, System::EventArgs^ e)
{
    timer->StopTimer();
    bool nextPlayerHasBar = (bar->Player1Checkers->Count > 0);
    game->SwitchPlayer();
    game->MustMoveFromBar = nextPlayerHasBar;
    endTurnButtonP2->Enabled = false;
    rollDiceButton->Enabled = true;
    CheckForAutomaticBearOff(2);
    this->Invalidate();
}

void BackgammonForm::ShowRulesButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    System::Windows::Forms::MessageBox::Show(rules->GetRules(), "Правила игры в Нарды",
        System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
}

void BackgammonForm::Form_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
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

void BackgammonForm::Form_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    if (selectedCheckerIndex != -1)
    {
        System::Drawing::Point currentPos = checkers->GetCheckerPosition(selectedCheckerIndex);
        currentPos.X = e->X;
        currentPos.Y = e->Y;
        checkers->MoveChecker(selectedCheckerIndex, currentPos, checkers->GetCheckerPoint(selectedCheckerIndex));
        this->Invalidate();
    }
}

void BackgammonForm::Form_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
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

void BackgammonForm::Form_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
    System::Drawing::Graphics^ g = e->Graphics;
    g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
    g->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;

    g->FillRectangle(System::Drawing::Brushes::SkyBlue, 0, 0, this->ClientSize.Width, this->ClientSize.Height);
    board->DrawBoard(g);

    // Отрисовка шашек в баре
    for (int i = 0; i < bar->Player1Checkers->Count; i++)
    {
        int checkerIndex = bar->Player1Checkers[i];
        System::Drawing::Color color = checkers->GetCheckerColor(checkerIndex);
        g->FillEllipse(gcnew System::Drawing::SolidBrush(color),
            bar->Player1Position.X - checkers->CheckerRadius,
            bar->Player1Position.Y - checkers->CheckerRadius + i * 30,
            checkers->CheckerRadius * 2, checkers->CheckerRadius * 2);
    }

    for (int i = 0; i < bar->Player2Checkers->Count; i++)
    {
        int checkerIndex = bar->Player2Checkers[i];
        System::Drawing::Color color = checkers->GetCheckerColor(checkerIndex);
        g->FillEllipse(gcnew System::Drawing::SolidBrush(color),
            bar->Player2Position.X - checkers->CheckerRadius,
            bar->Player2Position.Y - checkers->CheckerRadius + i * 30,
            checkers->CheckerRadius * 2, checkers->CheckerRadius * 2);
    }

    // Отрисовка всех шашек
    for (int i = 0; i < checkers->Positions->Count; i++)
    {
        if (bar->Contains(i, true) || bar->Contains(i, false)) continue;

        System::Drawing::Color color = checkers->GetCheckerColor(i);
        g->FillEllipse(gcnew System::Drawing::SolidBrush(color),
            checkers->GetCheckerPosition(i).X - checkers->CheckerRadius,
            checkers->GetCheckerPosition(i).Y - checkers->CheckerRadius,
            checkers->CheckerRadius * 2,
            checkers->CheckerRadius * 2);
    }

    // Отрисовка кубиков
    if (game->CurrentPlayer == 1)
    {
        dice->DrawDice(g, 100, 100 + 100, dice->GetDiceValues()[0], System::Drawing::Color::Black);
        dice->DrawDice(g, 180, 100 + 100, dice->GetDiceValues()[1], System::Drawing::Color::Black);
    }
    else
    {
        dice->DrawDice(g, this->ClientSize.Width - 220, 100 + 100, dice->GetDiceValues()[0], System::Drawing::Color::Red);
        dice->DrawDice(g, this->ClientSize.Width - 140, 100 + 100, dice->GetDiceValues()[1], System::Drawing::Color::Red);
    }

    // Информация о текущем ходе
    System::String^ turnText = System::String::Format("Текущий ход: Игрок {0}", game->CurrentPlayer);
    g->DrawString(turnText, gcnew System::Drawing::Font("Arial", 16, System::Drawing::FontStyle::Bold),
        System::Drawing::Brushes::DarkBlue, System::Drawing::PointF(this->ClientSize.Width / 2 - 100, 10 + 100));
}

void BackgammonForm::TurnTimer_Tick(System::Object^ sender, System::EventArgs^ e)
{
    timer->TurnSeconds--;
    if (game->CurrentPlayer == 1)
        player1TimerLabel->Text = System::String::Format("Время: {0} сек", timer->TurnSeconds);
    else
        player2TimerLabel->Text = System::String::Format("Время: {0} сек", timer->TurnSeconds);

    if (timer->TurnSeconds <= 0)
    {
        timer->StopTimer();
        System::Windows::Forms::MessageBox::Show("Время на ход истекло!", "Внимание",
            System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Warning);
        if (game->CurrentPlayer == 1)
            EndTurnButtonP1_Click(nullptr, nullptr);
        else
            EndTurnButtonP2_Click(nullptr, nullptr);
    }
}

void BackgammonForm::CheckForAutomaticBearOff(int player)
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

bool BackgammonForm::CheckIfBearingOffPossible(int player)
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

void BackgammonForm::CheckForWin()
{
    if (game->CheckWinCondition(bar->Player1Checkers, bar->Player2Checkers))
    {
        System::String^ winner = (bar->Player1Checkers->Count == 15) ? "Игрок 1" : "Игрок 2";
        System::Windows::Forms::MessageBox::Show(winner + " победил!", "Победа!",
            System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
        ResetGame();
    }
}

void BackgammonForm::ResetGame()
{
    checkers->InitializeCheckers(gcnew Player1CheckerFactory(), gcnew Player2CheckerFactory());
    game->StartNewGame();
    bar->Clear();
    timer->StopTimer();
    player1TimerLabel->Text = "Время: 30 сек";
    player2TimerLabel->Text = "Время: 30 сек";
    this->Text = "Backgammon";
    this->Invalidate();
}