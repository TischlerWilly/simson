#include"umwandeln.h"


std::string int_to_string(int zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return returnstring;
}

QString int_to_qstring(int zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return QString::fromStdString(returnstring);
}

std::string float_to_string(float zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return returnstring;
}

QString float_to_qstring(float zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return QString::fromStdString(returnstring);
}

std::string double_to_string(double zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return returnstring;
}

QString double_to_qstring(double zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return QString::fromStdString(returnstring);
}
QString double_to_qstring(double zahl, int anz_nachkommastellen)
{
    // 'f' steht für das Fixed-Point Format (kein wissenschaftliches E+0x)
    // Die 3 steht für die Anzahl der Nachkommastellen
    return QString::number(zahl, 'f', anz_nachkommastellen);
}
