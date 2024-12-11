#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>


QString buffer = "";

const int MODE_CHAR = 1;
const int MODE_CODE = 2;
int mode = MODE_CHAR;


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

    QStringList codes = text.split(' ', Qt::SkipEmptyParts);

    for (auto code : codes) {
        if (alphabet_map_code.find(code) != alphabet_map_code.end()) {
            char letter = alphabet_map_code[code];
            r.push_back(letter);
        }
    }

    return r;
}


bool key_valid_character(int k)
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


bool key_valid_code(int k)
{
    return k == 0x20
        || k == 0x2d
        || k == 0x2e
    ;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{

    if (event->key() == Qt::Key_Tab) {

        if (mode == MODE_CHAR) {
            buffer = text_to_morse(buffer);
            mode = MODE_CODE;
            ui->label_character_input->setStyleSheet("background: gray");
            ui->label_code_input->setStyleSheet("background: white");
        } else if (mode == MODE_CODE) {
            buffer = morse_to_text(buffer);
            mode = MODE_CHAR;
            ui->label_character_input->setStyleSheet("background: white");
            ui->label_code_input->setStyleSheet("background: gray");
        }

        return;

    } else if (event->key() == Qt::Key_Backspace) {

        buffer.erase(buffer.end() - 1);

    } else if (mode == MODE_CHAR && key_valid_character(event->key())) {

        if (buffer.size() > 255) {
            QMessageBox mbox;
            mbox.critical(0, "ERROR", "Input buffer size is limited to 256 characters");
            return;
        }

        if (event->text() == ' ') {
            ui->list_log_words->addItem(buffer);
            ui->list_log_codes->addItem(text_to_morse(buffer));
            buffer = "";

        } else {
            buffer.push_back(event->text());
        }


    } else if (mode == MODE_CODE && key_valid_code(event->key())) {

        if (buffer.size() > 255) {
            QMessageBox mbox;
            mbox.critical(0, "ERROR", "Input buffer size is limited to 256 characters");
            return;
        }

        if (buffer.endsWith(' ') && event->text() == ' ') {
            ui->list_log_words->addItem(morse_to_text(buffer));
            ui->list_log_codes->addItem(buffer);
            buffer = "";

        } else {
            buffer.push_back(event->text());
        }

    } else {
        return;
    }

    if (mode == MODE_CHAR) {
        ui->label_character_input->setText(buffer);
        ui->label_code_input->setText(text_to_morse(buffer));

    } else if (mode == MODE_CODE) {
        ui->label_code_input->setText(buffer);
        ui->label_character_input->setText(morse_to_text(buffer));
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table_character_code->setRowCount(alphabet_map_char.size());

    int i = 0;
    for (auto v : alphabet_map_char) {

        alphabet_map_code[v.second] = v.first;

        QTableWidgetItem * item_character = new QTableWidgetItem((QString)v.first);
        QTableWidgetItem * item_code = new QTableWidgetItem(v.second);
        QTableWidgetItem * item_ascii = new QTableWidgetItem(QString::number(v.first));
        item_character->setFlags(item_character->flags() ^ Qt::ItemIsEditable);
        item_code->setFlags(item_code->flags() ^ Qt::ItemIsEditable);
        item_ascii->setFlags(item_ascii->flags() ^ Qt::ItemIsEditable);

        ui->table_character_code->setItem(i, 0, item_character);
        ui->table_character_code->setItem(i, 1, item_code);
        ui->table_character_code->setItem(i, 2, item_ascii);

        i++;
    }

    ui->label_character_input->setStyleSheet("background: white");
    ui->label_code_input->setStyleSheet("background: gray");

    ui->label_code_input->setText(text_to_morse(buffer));
    ui->label_character_input->setText(buffer);
}

MainWindow::~MainWindow()
{
    delete ui;
}
