#include "mainwindow.h"
#include "./ui_mainwindow.h"


QString buffer = "";
bool mode = true;


std::unordered_map<char, QString> alphabet_map_char = {
    {' ', " "},
    {'!', "-.-.--"},
    {'\"', ".-..-."},

    {'$', "...-..-"},

    {'&', ".-..."},
    {'\'', ".----."},
    {'(' , "-.--."},
    {')' , "-.--.-"},

    {'+', ".-.-."},
    {',', "--..--"},
    {'-', "-....-"},
    {'.', ".-.-.-"},
    {'/', "-..-."},
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    {':', "---..."},
    {';', "-.-.-."},

    {'=', "-...-"},

    {'?', "..--.."},
    {'@', ".--.-."},

    {'_', "..--.-"},

    {'a', ".-"},
    {'b', "-..."},
    {'c', "-.-."},
    {'d', "-.."},
    {'e', "."},
    {'f', "..-."},
    {'g', "--."},
    {'h', "...."},
    {'i', ".."},
    {'j', ".---"},
    {'k', "-.-"},
    {'l', ".-.."},
    {'m', "--"},
    {'n', "-."},
    {'o', "---"},
    {'p', ".--."},
    {'q', "--.-"},
    {'r', ".-."},
    {'s', "..."},
    {'t', "-"},
    {'u', "..-"},
    {'v', "...-"},
    {'w', ".--"},
    {'x', "-..-"},
    {'y', "-.--"},
    {'z', "--.."},
};

std::unordered_map<QString, char> alphabet_map_code;




QString letter_to_morse(char letter)
{
    if ('A' <= letter && letter <= 'Z') {
        letter += 'a' - 'A';
    }

    switch (letter) {
    case ' ' : return "   ";
    case '!' : return "-.-.--";
    case '\"': return ".-..-.";

    case '$' : return "...-..-";

    case '&' : return ".-...";
    case '\'': return ".----.";
    case '(' : return "-.--.";
    case ')' : return "-.--.-";

    case '+': return ".-.-.";
    case ',': return "--..--";
    case '-': return "-....-";
    case '.': return ".-.-.-";
    case '/': return "-..-.";
    case '0': return "-----";
    case '1': return ".----";
    case '2': return "..---";
    case '3': return "...--";
    case '4': return "....-";
    case '5': return ".....";
    case '6': return "-....";
    case '7': return "--...";
    case '8': return "---..";
    case '9': return "----.";
    case ':' : return "---...";
    case ';' : return "-.-.-.";

    case '=' : return "-...-";

    case '?': return "..--..";
    case '@': return ".--.-.";

    case '_': return "..--.-";

    case 'a': return ".-";
    case 'b': return "-...";
    case 'c': return "-.-.";
    case 'd': return "-..";
    case 'e': return ".";
    case 'f': return "..-.";
    case 'g': return "--.";
    case 'h': return "....";
    case 'i': return "..";
    case 'j': return ".---";
    case 'k': return "-.-";
    case 'l': return ".-..";
    case 'm': return "--";
    case 'n': return "-.";
    case 'o': return "---";
    case 'p': return ".--.";
    case 'q': return "--.-";
    case 'r': return ".-.";
    case 's': return "...";
    case 't': return "-";
    case 'u': return "..-";
    case 'v': return "...-";
    case 'w': return ".--";
    case 'x': return "-..-";
    case 'y': return "-.--";
    case 'z': return "--..";

    default: return "";
    }
}


QString text_to_morse(QString text)
{
    QString r;
    for (auto c : text) {
        char letter = c.toLatin1();
        if ('A' <= letter && letter <= 'Z') {
            letter += 'a' - 'A';
        }
        if (alphabet_map_char.find(letter) != alphabet_map_char.end()) {
          QString mark = alphabet_map_char[letter];
          r.push_back(mark + " ");
        }
    }
    return r;
}


QString morse_to_text(QString text)
{
    QString r;

    QString code = "";

    for (auto c : text) {
        if (c == ' ') {
            if (alphabet_map_code.find(code) != alphabet_map_code.end()) {
                char letter = alphabet_map_code[code];
                r.push_back(letter);
            }
            code = "";
        } else {
            code.push_back(c);
            if (alphabet_map_code.find(code) != alphabet_map_code.end()) {
                char letter = alphabet_map_code[code];
                qInfo() << "code recognized: " << code << " => " << letter;
            } else {
                qWarning() << "code not recognized: " << c;
            }
        }
    }
    return r;
}


bool key_valid(int k)
{
    return (0x20 <= k && k <= 0x22)
        || (0x26 <= k && k <= 0x29)
        || (0x2b <= k && k <= 0x3b)
        || (0x41 <= k && k <= 0x5a)
        || (0x61 <= k && k <= 0x7a)
        || k == 0x24
        || k == 0x3d
        || k == 0x3f
        || k == 0x40
        || k == 0x5f
    ;
}


void MainWindow::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << "key pressed: " << event->text() << "\n";

    if (event->key() == Qt::Key_Tab) {
        mode = !mode;
        buffer = "";
    } else if (event->key() == Qt::Key_Backspace) {
        buffer.erase(buffer.end() - 1);
    } else if (key_valid(event->key())) {
        buffer.push_back(event->text());
    }

    if (buffer.size() > 32) {
        buffer.remove(0, buffer.size() - 32);
    }

    if (mode) {
        ui->label_text_english->setText(buffer);
        ui->label_text_morse->setText(text_to_morse(buffer));
    } else {
        ui->label_text_morse->setText(buffer);
        ui->label_text_english->setText(morse_to_text(buffer));
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (auto v : alphabet_map_char) {
        alphabet_map_code[v.second] = v.first;
    }

    buffer = "He is in the garb of a kwaker";

    ui->label_text_english->setText(buffer);
    ui->label_text_morse->setText(text_to_morse(buffer));
}

MainWindow::~MainWindow()
{
    delete ui;
}
