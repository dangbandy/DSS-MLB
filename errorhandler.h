/*
 *  Author: Andy Dang
 *  Date: 8/3/2019
 *  Description:
 */

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <iostream>
#include <fstream>
#include <QDebug>
#include <QMessageBox>

class ErrorHandler{

public:
    ErrorHandler();
    ~ErrorHandler();

    void errorBox(QWidget *wid, const std::string & msg) const;
    void errorLog( const std::string & msg);
private:
    std::ofstream errOut;
};

#endif // ERRORHANDLER_H
