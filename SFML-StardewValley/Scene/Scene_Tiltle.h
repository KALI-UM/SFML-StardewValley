#pragma once

class ButtonObject;
class TitleSceneUI;
class Scene_Tiltle :
    public SceneBase
{
public:
    Scene_Tiltle();
    ~Scene_Tiltle();

    bool Initialize() override;
    void Enter() override;
    void Update(float dt) override;
    void OnWindowResize()override;
    
protected:
    SpriteObject* m_Panorama;
    TitleSceneUI* m_UI;
};

