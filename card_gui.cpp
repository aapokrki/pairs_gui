#include "card_gui.hh"

Card_gui::Card_gui(QWidget *parent, unsigned int x, unsigned int y):
    QPushButton(parent),
    visibility_(EMPTY)

{
    // Set card's font and size
    setFixedSize(QSize(x,y));
    setFont(QFont("Helvetica [Cronyx]", 30));
}

Card_gui::~Card_gui()
{
//    qDebug() << "Card deleted";
}

void Card_gui::set_letter(const char c)
{
    letter_ = c;
}

void Card_gui::set_visibility(const Visibility_type visibility)
{
    visibility_ = visibility;
    if(visibility == HIDDEN){
        setStyleSheet("background: red");

    }else if (visibility == OPEN){
        setStyleSheet("background: blue");
    }

}

QChar Card_gui::get_letter() const
{
    return letter_;
}

Visibility_type Card_gui::get_visibility() const
{
    return visibility_;
}

void Card_gui::turn()
{
    if(visibility_ == HIDDEN)
    {
        set_visibility(OPEN);

        setText(QString(letter_));
        setEnabled(false);
    }
    else if(visibility_ == OPEN)
    {
        set_visibility(HIDDEN);

        setText("");
        setEnabled(true);
    }
    else // if(visibility_ == EMPTY)
    {
        // Should never get here, since empty cards should be disabled
        qDebug() << "Cannot turn an empty place";
    }
}

void Card_gui::remove_from_game_board()
{
    set_visibility(EMPTY);
    setText("");
    setStyleSheet("background: grey");
    setEnabled(false);
}


