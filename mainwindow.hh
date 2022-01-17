/* Pairs_gui
 *
 * Program author:
 * Name: Aapo Kärki
 * Student number: H292001
 * UserID: gxaaka
 * E-Mail: aapo.karki@tuni.fi
 * Course: COMP.CS.110 K2021
 *
 * Read instructions.txt
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsScene>

#include "card_gui.hh"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using Game_row_type = std::vector<Card_gui*>;
using Game_board_type = std::vector<std::vector<Card_gui*>>;

struct Player{
    QString name;
    unsigned int points;
    QGraphicsView * pairs;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    // Turns the clicked card
    void on_card_clicked();

    // Adds a points to a player and
    // removes the two turned cards from the board | pair found
    void add_point();

    // Turns the two turned cards back to HIDDEN | pair not found
    void reset();

    // Timer update
    void on_timer_timeout();


    void on_close_button_clicked();

    // Adds a given player to the game
    void on_add_button_clicked();

    // Updates pairLcdScreen on even numbers of the updated slider value
    void on_pair_slider_valueChanged(int value);

    // Checkbox where the user can choose whether a new random seed is made
    // for every new game
    void on_random_check_box_stateChanged(int arg1);

    // Resets everything to their base values
    void on_reset_button_clicked();

    // Signals when a new seed is given, impacts the random card layout
    void on_seed_line_edit_textChanged(const QString &arg1);

    // Starts the game or prepares the board for a new game, depenging on state
    void on_start_button_clicked();

private:
    Ui::MainWindow *ui;
    Game_board_type g_board;
    QTimer* timer;

    // vector of only 0-2 cards, that is used to check the g_board status
    std::vector<Card_gui*> turned_cards_;

    // Vector of players in this game
    std::vector<Player*> players_;

    // Gameplay values, that change during gameplay
    unsigned int cards_removed = 0;
    unsigned int PIT = 0; // PIT, because "player_in_turn" is too long

    // Game defaults
    unsigned int DELAY = 500;
    unsigned int RANDOM_SEED = 2;
    unsigned int MAX_PLAYERS = 6;

    // Game settings based on user inputs
    unsigned int COLUMN_AMOUNT = 0;
    unsigned int ROW_AMOUNT = 0;
    unsigned int CARD_X = 0;
    unsigned int CARD_Y = 0;
    bool AUTORANDOM = true;

    // Rotates the current player in turn (PIT)
    void next_player();

    // Called when all cards are turned, determines the winner(s)
    // based on the amount of points and activates a prompt to start a new game
    void win_condition();

    // Called every time a card is clicked,
    // if two cards are open, checks if they are a pair (add_point)
    // or not (reset)
    void check_board_status();

    // Deletes all players and clears players_ vector
    void delete_players();

    // Resets the g_board and cardLayout grid
    void clear_board();

    // Calculates the two closests factors of a number
    // Used to calculate row & column amounts
    // Sometimes output is messy for the board eg.(38 = 2 * 19),
    // but if would be weird to remove these kinds of card options
    void calculate_rows_columns();

    // Creates a rows*columns amount of empty Card_gui cards
    // and adds, them to the g_board vector
    void init_with_empties(Game_board_type& g_board,
                           unsigned int rows, unsigned int columns);

    // Lays the cards from g_board to the premade cardLayout grid
    void lay_cards_out(Game_board_type &g_board);

    // Gives empty cards their base values, distributes card-pairs randomly
    // based on a given seed value (RAMDOM_SEED)
    void distribute_values(Game_board_type &g_board, unsigned int seed);

    // Called by 'distribute_values' to find next empty cell ready for values
    // unsigned int 'start' given randomly by 'distribute_values'
    unsigned int next_free(Game_board_type& g_board, unsigned int start);

};

#endif // MAINWINDOW_HH
