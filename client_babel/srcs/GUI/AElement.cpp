#include "QTGUI.hh"
#include "AElement.h"

AElement::AElement()
{}

AElement::~AElement()
{}

void AElement::setGui(QTGUI * gui)
{
    _gui = gui;
}
