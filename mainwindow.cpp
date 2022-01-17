#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGridLayout>
#include <QFormLayout>
#include <QTimer>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this))

{
    // Startup settings
    ui->setupUi(this);

    // Makes the borders around the game round,
    // by setting a mask over the central widget holding the card layout
    QPainterPath path;
    path.addRoundedRect(ui->central->rect(),15,15);
    QRegion mask = QRegion(path.toFillPolygon().toPolygon());
    ui->central->setMask(mask);

    // Set base colours
    ui->central->setStyleSheet("background: white");
    ui->pair_lcd_number->setStyleSheet("background: green");
    ui->lcd_number_min->setPalette(Qt::black);
    ui->lcd_number_sec->setPalette(Qt::black);

    // Set base values
    ui->start_button->setDisabled(true);
    ui->reset_button->setEnabled(false);
    ui->pair_slider->setValue(20);
    ui->pair_lcd_number->display(20);

    // Sets a random starter seed
    srand(time(nullptr));
    ui->seed_line_edit->setText(QString::number(rand()));

    // Timer stuff
    connect(timer, &QTimer::timeout, this, &MainWindow::on_timer_timeout);
}

MainWindow::~MainWindow()
{
    // Deconstructs everything when the program is closed
    delete ui;
    delete_players();
}

void MainWindow::on_timer_timeout(){


    int current_min = ui->lcd_number_min->intValue();
    int current_sec = ui->lcd_number_sec->intValue();

    if(current_sec == 59){
        ui->lcd_number_min->display(current_min + 1);
        ui->lcd_number_sec->display(0);

    }else{
        ui->lcd_number_sec->display(current_sec+1);
    }
}

void MainWindow::init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns){

    g_board.clear();
    Game_row_type row;

    // Creates the desired amount of Card_gui cards
    // and connects all of them to a clicked slot
    // Also adds all created cards to the g_board
    for(unsigned int i = 0; i < columns; ++i){
        for(unsigned int i = 0; i < rows; ++i){

            Card_gui *card = new Card_gui(this, CARD_X, CARD_Y );

            connect(card, &QPushButton::clicked,
                    this, &MainWindow::on_card_clicked);

            // Creates a row of cards
            row.push_back(card);
        }

        // Adds row to g_board and clears row, ready for new cards
        g_board.push_back(row);
        row.clear();
    }
}

void MainWindow::lay_cards_out(Game_board_type& g_board)
{
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    for(unsigned int c=0;c <= rows-1 ; ++c)
    {
        for(unsigned int j = 0; j<= columns-1 ; ++j){
            Card_gui *card = g_board.at(c).at(j);

            // Lays cards out to a premade gridLayout made in the ui tool
            ui->card_layout->addWidget(card, c, j);
        }
    }
}

unsigned int MainWindow::next_free(Game_board_type& g_board, unsigned int start){

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i){

        if(g_board.at(i / columns).at(i % columns)->get_visibility() == EMPTY){
            return i;
        }
    }

    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i){

        if(g_board.at(i / columns).at(i % columns)->get_visibility() == EMPTY){
            return i;
        }
    }

    // You should never reach this
    qDebug()<< "No more empty space";
    return rows * columns - 1;
}
void MainWindow::distribute_values(Game_board_type &g_board, unsigned int seed){

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);

    // Wiping out the first random number
    // (that is always the lower bound of the distribution)
    distr(randomEng);

    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c){

        // Adding two identical cards (a pair) to the game board
        for(unsigned int j = 0; j < 2; ++j){

            unsigned int cell = distr(randomEng);

            cell = next_free(g_board, cell);

            // Gives a card it's letter and set visibility
            g_board.at(cell / columns).at(cell % columns)->set_letter(c);
            g_board.at(cell / columns).at(cell % columns)->set_visibility(HIDDEN);
        }
    }
}

void MainWindow::calculate_rows_columns()
{
    // The amount of cards, the player wants in the game
    unsigned int cards = ui->pair_lcd_number->value();

    // Finds the nearest number that's sqrt() is an integer
    int i = (int)sqrt(cards);
        while (cards % i != 0)
            i--;

    //Based on found integer, we can calculate the COLUMN_AMOUNT and ROW_AMOUNT
    COLUMN_AMOUNT = cards/i;
    ROW_AMOUNT = i;

    // Calculate cards' sizes based on the number of cards.
    // This is so that the cards scale accordingly, filling out the gridLayout,
    // whathever the amount of cards is. (550 is based on the gridLayout's size)
    CARD_X = 550/COLUMN_AMOUNT;
    CARD_Y = 550/ROW_AMOUNT;

//    qDebug()<<COLUMN_AMOUNT << "|" << ROW_AMOUNT;

}

void MainWindow::check_board_status(){

//    qDebug() << "board check";

    // Player has turned two card, so now this will check if they match
    if ( turned_cards_.size() == 2 ){

        // Disable all other cards for the time being
        for(auto row: g_board){
            for (auto card : row){
                card->setEnabled(false);
            }
        }

        Card_gui *card1 = turned_cards_.at(0);
        Card_gui *card2 = turned_cards_.at(1);

        // Pair found
        if(card1->get_letter() == card2->get_letter()){

            QTimer::singleShot(DELAY, this, SLOT(add_point()));

        // Pair not found
        }else{

            QTimer::singleShot(DELAY, this, SLOT(reset()));
        }
    }
}

void MainWindow::delete_players()
{
    // Deletes all created players
    for (Player* player : players_)
        delete player;
}

void MainWindow::clear_board()
{
    // Deletes all cards and clears the board
    for(auto row : g_board)
        for (auto card : row)
            delete card;

    g_board.clear();
}

void MainWindow::add_point()
{
//    qDebug()<<"Pair found";

    Card_gui *card1 = turned_cards_.at(0);
    Card_gui *card2 = turned_cards_.at(1);

    // Remove cards from the game, set them grey and disabled
    card1->remove_from_game_board();
    card2->remove_from_game_board();

    // To keep easy count of removed cards
    cards_removed += 2;

    // Add points to the player (number of cards, NOT pairs)
    players_.at(PIT)->points += 2;

    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);

    // Draw a pair of cards to player's hand

    // Stylized so that the pair is stacked,
    // making it look more 'realistic' i guess
    // Card size here is fixed unlike in the game board, to make it clearer

    players_.at(PIT)->pairs->scene()->addRect
            (players_.at(PIT)->points*11, 1, 15, 22,blackPen, redBrush);

    players_.at(PIT)->pairs->scene()->addRect
            (3+players_.at(PIT)->points*11, 3, 15, 22,blackPen, redBrush);

    // Adds the pair's letter on top of the card rectangle
    players_.at(PIT)->pairs->scene()->addText(card1->get_letter())
            ->setPos(3+players_.at(PIT)->points*11, 2);

    // Card's x-location is based on the player's points, so that the pairs
    // are added next to one another and not stacked to the same location

    // Clear the temporary vector of turned cards
    turned_cards_.clear();

    // If all cards have been removed, the game ends
    if(cards_removed == ui->pair_lcd_number->value()){
        win_condition();
    }

    //Enables all cards again, for the next turn
    for(auto row: g_board){
        for (auto card : row){
            if(card->get_visibility() != EMPTY){
                card->setEnabled(true);
            }
        }
    }
}

void MainWindow::reset()
{
//    qDebug()<<"Pair not found";

    Card_gui *card1 = turned_cards_.at(0);
    Card_gui *card2 = turned_cards_.at(1);

    // Turn cards back around, emptis the temporary turned cards vector
    // and gives the next player their turn
    card1->turn();
    card2->turn();
    turned_cards_.clear();
    next_player();

    // Enables cards for the next turn
    for(auto row: g_board){
        for (auto card : row){
            if(card->get_visibility() != EMPTY){
                card->setEnabled(true);
            }
        }
    }
}

void MainWindow::next_player()
{
    // Goes through the players and changes the PIT (player-in-turn)
    if(PIT == players_.size()-1){
        PIT = 0;

    }else{
        PIT++;
    }

    // Indicator on whose turn it is
    QString text = "It's " + players_.at(PIT)->name + "'s turn!";
    ui->player_in_turn->setText(text);
}

void MainWindow::win_condition()
{
    // Stops the timer and shows game time
    timer->stop();

    // Set a winner defaults for comparisons
    Player *winner = players_.at(0);
    unsigned int largest = winner->points;
    std::vector<Player*> possible_tie = {winner};

    // Compares all player's points and determines the winner(s)
    for(unsigned long i = 1 ; i < players_.size() ; ++i){

        // If a new winner was found
        if(players_.at(i)->points > largest){

            largest = players_.at(i)->points;
            winner = players_.at(i);

            // Clears the possible_tie vector,
            // because there is no more a winner tie
            possible_tie.clear();
            possible_tie.push_back(winner);

        // If winner and current player's points are the same,
        // add current player to the tie vector
        }else if (players_.at(i)->points == largest){
            possible_tie.push_back(players_.at(i));
        }
    }

    // Default text expecting a tie situation
    QString text = "TIE: ";

    if(possible_tie.size()>= 2){ // More than 1 winner

        // Displays all players with a winner tie and their amount of pairs
        for(Player * player : possible_tie){

             text = text + player->name + ", ";
        }

        // NOTE: pairs = points/2
        text = text + "with "
                + QString::number(winner->points/2) + " pair(s)!";


    }else{ // If there was a singular winner

        // NOTE: pairs = points/2
        text =  winner->name + " has won with "
                + QString::number(winner->points/2) + " pair(s)!";
    }

    // Display the game results and activate a prompt for a new game
    ui->player_in_turn->setText(text);
    ui->start_button->setEnabled(true);
    ui->start_button->setText("New game");

}

void MainWindow::on_seed_line_edit_textChanged(const QString &seed)
{
    RANDOM_SEED = seed.toInt();
}

void MainWindow::on_pair_slider_valueChanged(int value)
{
    // Since only even numbers are allowed,
    // this slider updates the pairLcdNumber screen only on even numbers
    if(value % 2 == 0){
        ui->pair_lcd_number->display(value);
    }
}

void MainWindow::on_random_check_box_stateChanged(int arg)
{
    // arg is 1 = true or 0 = false
    AUTORANDOM = arg;
//    qDebug() << AUTORANDOM;
    if(AUTORANDOM){
        ui->seed_line_edit->setDisabled(true);
    }else{
        ui->seed_line_edit->setEnabled(true);
    }
}

void MainWindow::on_close_button_clicked()
{
    close();
}

void MainWindow::on_add_button_clicked()
{
    // If the given name is over 10 char, inform user and dont add the name
    if(ui->name_edit->text().size() != 0){
        if(ui->name_edit->text().size() >= 10){
            ui->name_edit->setText("MAX 10 CHAR");
            return;
        }

        // Game can only be started with atleast one player
        ui->start_button->setDisabled(false);


        QString player_name = ui->name_edit->text();
        ui->name_edit->clear();

        // Creates the player's hand which holds their pairs
        QGraphicsScene *scene = new QGraphicsScene(this);
        QGraphicsView *view = new QGraphicsView(scene);

        // Adds players to the game's player layout
        ui->form_layout->addRow(player_name, view);


        // Creates a new 'Player' struct with their own hand(view) and 0 points
        // Adds player to vector of players
        Player *player = new Player {player_name, 0, view};
        players_.push_back( player);

        // Maximum amount of players is MAX_PLAYERS (6). No more can be added
        if(players_.size() >= MAX_PLAYERS){
            ui->name_edit->setDisabled(true);
            ui->add_button->setDisabled(true);
        }
    }
}

void MainWindow::on_reset_button_clicked()
{
    // Restarts timer
    ui->lcd_number_min->display(0);
    ui->lcd_number_min->display(0);
    timer->stop();

    // Deletes all players from the game
    for(unsigned long i = 0 ; i < players_.size() ; ++i)
        ui->form_layout->removeRow(0);

    delete_players();
    players_.clear();

    // Clears board
    if(!g_board.empty())
        clear_board();

    // Resets everything to what it was when the program was started

    ui->random_check_box->setEnabled(true);
    if(!AUTORANDOM)
        ui->seed_line_edit->setEnabled(true);

    ui->reset_button->setEnabled(false);
    ui->start_button->setEnabled(false);
    ui->start_button->setText("Start");
    ui->name_edit->setEnabled(true);
    ui->add_button->setEnabled(true);
    ui->pair_slider->setEnabled(true);
    ui->player_in_turn->clear();
    cards_removed = 0;
    PIT = 0;
}

void MainWindow::on_start_button_clicked()
{
    // To start the game
    if(ui->start_button->text() == "Start"){

        // Starts game timer
        timer->start(1000);

        // If automatic random seeds is checked, set a random seed
        if(AUTORANDOM)
            ui->seed_line_edit->setText(QString::number(rand()));

        // Calculate board size
        calculate_rows_columns();

        // Create , distribute and lay out cards
        init_with_empties(g_board, COLUMN_AMOUNT, ROW_AMOUNT );
        distribute_values(g_board, RANDOM_SEED);
        lay_cards_out(g_board);

        // Prompt for player in turn
        QString text = "It's " + players_.at(PIT)->name + "'s turn!";
        ui->player_in_turn->setText(text);

        // Robustness. Setting buttons disabled for the time being
        ui->start_button->setDisabled(true);
        ui->add_button->setDisabled(true);
        ui->pair_slider->setDisabled(true);
        ui->name_edit->setDisabled(true);
        ui->reset_button->setEnabled(true);

        ui->seed_line_edit->setDisabled(true);
        ui->random_check_box->setDisabled(true);

    }else{ // If ui->startButton->text() == "New game"

        // Restarts timer for a new game
        ui->lcd_number_min->display(0);
        ui->lcd_number_sec->display(0);
        timer->stop();

        //Clears the current board and resets points
        clear_board();
        for(Player* player : players_){
            player->points = 0;
            player->pairs->scene()->clear();
        }

        // Sets the game, to the moment just before starting a game

        ui->random_check_box->setEnabled(true);
        if(!AUTORANDOM)
            ui->seed_line_edit->setEnabled(true);

        ui->start_button->setText("Start");
        ui->start_button->setEnabled(true);
        ui->player_in_turn->clear();

        // New players or a different amount of cards can be added
        ui->name_edit->setEnabled(true);
        ui->add_button->setEnabled(true);
        ui->pair_slider->setEnabled(true);

        // Reset vectors from previous game's data
        cards_removed = 0;
        PIT = 0;
    }
}

void MainWindow::on_card_clicked()
{
    // Storing global cursor position
    QPoint global_click_position = QCursor::pos();

    // Counting local cursor position, i.e. decreasing
    // Main Window's and centralWidget's locations from the global one
    int local_x = global_click_position.x() - geometry().x()
            - ui->central->geometry().x();

    int local_y = global_click_position.y() - geometry().y()
            - ui->central->geometry().y();

    QPoint local_click_position = QPoint(local_x, local_y);

    // Searching for the button clicked
    for(unsigned int i = 0; i < g_board.size(); ++i)
    {
        for(unsigned int j = 0; j < g_board.at(0).size(); ++j){

            if(g_board.at(i).at(j)->geometry().contains(local_click_position))
            {

                Card_gui *card = g_board.at(i).at(j);

                // Clicking a card turns it, adds it to a temporary vector
                // of turned cards and checks if two cards have been turned
                card->turn();
                turned_cards_.push_back(card);
                check_board_status();

                return; // For efficiency reasons
                        // (only one button can be clicked at a time)
            }
        }
    }
}
