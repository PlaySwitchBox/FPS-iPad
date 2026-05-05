#include <Geode/Geode.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

// ──────────────────────────────────────────────
//  Hilfsfunktion: FPS anwenden
// ──────────────────────────────────────────────
static void applyFPS(int fps) {
fps = std::clamp(fps, 30, 240);
CCDirector::sharedDirector()->setAnimationInterval(1.0 / fps);
Mod::get()->setSavedValue<int>(“fps-value”, fps);
}

// ──────────────────────────────────────────────
//  FPS-Changer Popup
// ──────────────────────────────────────────────
class FPSChangerPopup : public geode::Popup<> {
protected:
Slider*    m_slider    = nullptr;
TextInput* m_input     = nullptr;
int        m_currentFPS;

```
static constexpr int   FPS_MIN  = 30;
static constexpr int   FPS_MAX  = 240;
static constexpr float POPUP_W  = 320.f;
static constexpr float POPUP_H  = 220.f;

bool setup() override {
    m_currentFPS = Mod::get()->getSavedValue<int>("fps-value", 60);
    this->setTitle("FPS Changer");

    auto* layer = this->m_mainLayer;
    float cx = POPUP_W / 2.f;

    // Beschriftung
    auto* label = CCLabelBMFont::create("Ziel-FPS (30 - 240)", "bigFont.fnt");
    label->setScale(0.45f);
    label->setPosition({cx, 168.f});
    layer->addChild(label);

    // Eingabefeld
    m_input = TextInput::create(80.f, "FPS");
    m_input->setPosition({cx, 138.f});
    m_input->setFilter("0123456789");
    m_input->setMaxCharCount(3);
    m_input->setString(std::to_string(m_currentFPS));
    m_input->setCallback([this](const std::string& val) {
        onInputChanged(val);
    });
    layer->addChild(m_input);

    // Slider
    m_slider = Slider::create(this, menu_selector(FPSChangerPopup::onSliderMoved), 0.85f);
    m_slider->setPosition({cx, 100.f});
    m_slider->setValue(fpsToSlider(m_currentFPS));
    layer->addChild(m_slider);

    // Schnellwahl-Buttons
    auto* presetMenu = CCMenu::create();
    presetMenu->setID("fps-preset-menu");
    presetMenu->setPosition({0.f, 0.f});
    layer->addChild(presetMenu);

    const int presets[] = {30, 60, 90, 120};
    const float startX  = cx - 105.f;
    for (int i = 0; i < 4; ++i) {
        int fps = presets[i];
        auto* btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(
                fmt::format("{}hz", fps).c_str(),
                50, true, "bigFont.fnt", "GJ_button_04.png", 0, 0.45f
            ),
            this,
            menu_selector(FPSChangerPopup::onPresetButton)
        );
        btn->setTag(fps);
        btn->setPosition({startX + i * 70.f, 62.f});
        presetMenu->addChild(btn);
    }

    // Uebernehmen-Button
    auto* bottomMenu = CCMenu::create();
    bottomMenu->setPosition({0.f, 0.f});
    auto* applyBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Ubernehmen", "GJ_button_01.png"),
        this,
        menu_selector(FPSChangerPopup::onApply)
    );
    applyBtn->setPosition({cx, 26.f});
    bottomMenu->addChild(applyBtn);
    layer->addChild(bottomMenu);

    return true;
}

void onSliderMoved(CCObject* sender) {
    auto* thumb = static_cast<SliderThumb*>(sender);
    m_currentFPS = sliderToFPS(thumb->getValue());
    m_input->setString(std::to_string(m_currentFPS));
}

void onInputChanged(const std::string& val) {
    if (val.empty()) return;
    int fps = std::clamp(std::stoi(val), FPS_MIN, FPS_MAX);
    m_currentFPS = fps;
    m_slider->setValue(fpsToSlider(fps));
}

void onPresetButton(CCObject* sender) {
    m_currentFPS = sender->getTag();
    m_input->setString(std::to_string(m_currentFPS));
    m_slider->setValue(fpsToSlider(m_currentFPS));
}

void onApply(CCObject*) {
    applyFPS(m_currentFPS);
    this->onClose(nullptr);
    Notification::create(
        fmt::format("FPS auf {} gesetzt", m_currentFPS),
        NotificationIcon::Success
    )->show();
}

static float fpsToSlider(int fps) {
    return static_cast<float>(fps - FPS_MIN) / (FPS_MAX - FPS_MIN);
}
static int sliderToFPS(float v) {
    return FPS_MIN + static_cast<int>(std::round(v * (FPS_MAX - FPS_MIN)));
}
```

public:
static FPSChangerPopup* create() {
auto* popup = new FPSChangerPopup();
if (popup->initAnchored(POPUP_W, POPUP_H)) {
popup->autorelease();
return popup;
}
CC_SAFE_DELETE(popup);
return nullptr;
}
};

// ──────────────────────────────────────────────
//  Hook: OptionsLayer (stabil seit GD 2.1,
//  garantiert vorhanden in 2.2081)
//  Der FPS-Changer-Button erscheint direkt
//  im Einstellungs-Overlay unten rechts.
//  Kein Abhang von AdvancedSettingsLayer,
//  dessen interne Struktur sich aendern kann.
// ──────────────────────────────────────────────
struct $modify(FPSOptionsLayer, OptionsLayer) {
bool init() {
if (!OptionsLayer::init()) return false;
this->addFPSButton();
return true;
}

```
void addFPSButton() {
    auto* winSize = CCDirector::sharedDirector()->getWinSize();

    auto* sprite = ButtonSprite::create("FPS Changer", "GJ_button_05.png");
    sprite->setScale(0.65f);

    auto* btn = CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(FPSOptionsLayer::onFPSButton)
    );

    auto* menu = CCMenu::create();
    menu->setID("fps-changer-menu");
    menu->addChild(btn);
    // Unten rechts im Einstellungs-Overlay
    menu->setPosition({winSize.width / 2.f + 130.f, 52.f});
    this->addChild(menu, 10);
}

void onFPSButton(CCObject*) {
    FPSChangerPopup::create()->show();
}
```

};

// ──────────────────────────────────────────────
//  Mod-Einstiegspunkt: gespeicherte FPS laden
// ──────────────────────────────────────────────
$on_mod(Loaded) {
int savedFPS = Mod::get()->getSavedValue<int>(“fps-value”, 60);
applyFPS(savedFPS);
log::info(“FPS Changer geladen - FPS: {}”, savedFPS);
}
