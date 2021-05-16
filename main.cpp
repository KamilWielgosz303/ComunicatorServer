#include "src/SERVER/server.h"


using namespace std;

int main(int argc, char *argv[])
    {
    QCoreApplication a(argc, argv);
    Server server(PORT);
    return a.exec();

}


