#pragma once

class D9SliceSprite;
class InventoryUI :
    public UIObject
{
public:
    InventoryUI();
    ~InventoryUI();

    bool Initialize() override;
    void Reset()override;
    void LateUpdate(float dt)override;
    void Release()override;

    void SetItemIcon(int index, const std::string& icontexId, int count);
    void SetCurrentIndex(int index);
protected:


    int m_CurrentIndex = 0;
    D9SliceSprite*      m_Frame;
    DRectangle*         m_Background;
    DRectangle*         m_Block[10];
    DRectangle*         m_Item[10];
    DText*              m_Count[10];
};

