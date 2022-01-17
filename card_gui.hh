/* Pairs_gui
 *
 * Program author:
 * Name: Aapo Kärki
 * Student number: H292001
 * UserID: gxaaka
 * E-Mail: aapo.karki@tuni.fi
 * Course: COMP.CS.110 K2021
 *
 * Card_gui class
 *
 * A class for holding a single card's information
 * inherits QPushbutton
 * */

#ifndef CARD_GUI_HH
#define CARD_GUI_HH

#include <QPushButton>
#include <QDebug>


// OPEN = blue | HIDDEN = Red | EMPTY = Grey
enum Visibility_type {OPEN, HIDDEN, EMPTY};

// Inherits QPushbutton, so card specific info
// is easier to add and keep track of
class Card_gui : public QPushButton
{
public:
    // Constructs an empty card with given sizes x & y. Mainwindow as parent
    Card_gui( QWidget *parent, unsigned int x, unsigned int y );

    // Destructor
    ~Card_gui();

    // Set given letter and visibility to this card
    void set_letter(const char c);

    void set_visibility(const Visibility_type visibility);

    // Obvious getters
    QChar get_letter() const;
    Visibility_type get_visibility() const;

    // Turns a card: changes the visibility from open to hidden and vice versa.
    // Changes colour between red and blue,
    // also shows the card's letter_ when turned to OPEN
    void turn();

    // Removes a card from the game board: changes the visibility to empty.
    // Turns the card grey and disables it
    void remove_from_game_board();

private:
    char letter_;
    Visibility_type visibility_ = EMPTY;

};

#endif // CARD_GUI_HH
