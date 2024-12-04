#pragma once
#include "GameObject.h"
class UI :
    public GameObject
{


public:
    bool GetHasFocus()const { return m_HasFocus; }
protected:
    bool m_HasFocus;
    
};

