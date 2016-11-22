#ifndef ELEMENT_H
#define ELEMENT_H

class QTGUI;

class AElement
{
public:
    AElement();
    ~AElement();

public:
    void setGui(QTGUI *);

protected:
    QTGUI * _gui;
};

#endif // ELEMENT_H
