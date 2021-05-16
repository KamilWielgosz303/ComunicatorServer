#ifndef CONSTSTATICFLAGS_H
#define CONSTSTATICFLAGS_H

namespace FLAG{

//TCP HEADERS

constexpr char C_LOGIN[]    = "C_LOGIN";
constexpr char S_LOGIN[]    = "S_LOGIN";
constexpr char C_SEND[]     = "C_SEND";
constexpr char S_SEND[]     = "S_SEND";
constexpr char S_ULIST[]    = "S_ULIST";
constexpr char C_ULIST[]    = "C_ULIST";
constexpr char C_GET_MSG[]  = "C_GET_MSG";
constexpr char S_GET_MSG[]  = "S_GET_MSG";
constexpr char S_REGISTER[]  = "S_REG";
constexpr char C_REGISTER[]  = "C_REG";

//TCP FLAGS

constexpr char ACT[] = "ACT";
constexpr char ERR[] = "ERR";
constexpr char NEQ[] = "NEQ";

//DATABASE FLAGS

constexpr char FORMAT_DATA[] = "yyyy-MM-dd hh:mm:ss";

//DEBUG FLAGS

constexpr char ERROR[] = "Error: ";
constexpr char NEW_LINE[] = "\r\n";
}
#endif // CONSTSTATICFLAGS_H
