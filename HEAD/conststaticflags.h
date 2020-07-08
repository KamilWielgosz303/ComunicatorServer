#ifndef CONSTSTATICFLAGS_H
#define CONSTSTATICFLAGS_H
#include <QString>

namespace FLAG{

//TCP HEADERS

static const QString C_LOGIN    = "C_LOGIN";
static const QString S_LOGIN    = "S_LOGIN";
static const QString C_SEND     = "C_SEND";
static const QString S_SEND     = "S_SEND";
static const QString S_ULIST    = "S_ULIST";
static const QString C_ULIST    = "C_ULIST";
static const QString C_GET_MSG  = "C_GET_MSG";
static const QString S_GET_MSG  = "S_GET_MSG";
static const QString S_REGISTER  = "S_REG";
static const QString C_REGISTER  = "C_REG";

//TCP FLAGS

static const QString ACT = "ACT";
static const QString ERR = "ERR";
static const QString NEQ = "NEQ";

//DATABASE FLAGS

static const QString FORMAT_DATA = "yyyy-MM-dd hh:mm:ss";

//DEBUG FLAGS

static const QString ERROR = "Error: ";
static const QString NEW_LINE = "\r\n";
}
#endif // CONSTSTATICFLAGS_H
