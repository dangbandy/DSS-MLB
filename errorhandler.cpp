#include "errorhandler.h"

ErrorHandler::ErrorHandler()
{
    errOut.open("ErrorLog.txt");
}

ErrorHandler::~ErrorHandler()
{
    errOut.close();
}

void ErrorHandler::errorBox(QWidget * wid, const std::string & msg) const
{
    QMessageBox::critical(wid, "Info", msg.c_str());
}

void ErrorHandler::errorLog( const std::string & msg)
{
    errOut << msg << std::endl;
}
