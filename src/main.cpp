#include <geode/prelude.hpp>
#include <geode/binding.hpp> // WICHTIG: Bindings für GD-Klassen

// Wir nutzen den Standard-Namespace, um viel Code zu sparen (z.B. statt geode::SaveFile nutzen wir SaveFile)
using namespace geode::prelude;

// 1. Konfiguration definieren
// Diese Struktur wird automatisch in JSON übersetzt.
struct Save {
    float targetFps = 0.0f; // Standardwert ist 0 (deaktiviert)
};

// 2. Der Hauptteil des Mods
class CustomFPSMod : public geode::Mod<Save> {
public:
    bool onInitialize(geode::Loader* loader) override {
        log::info("Custom FPS Mod geladen.");
        return true;
    }
};

// Wir registrieren die Instanz.
static CustomFPSMod mod;

// 3. Hook für den Advanced Tab in den Grafikeinstellungen
// Dieser Hook fügt das UI-Element hinzu.
struct $modify(AdvancedGraphicsController) : geode::HookHandler<AdvancedGraphicsController, void, AdvancedGraphicsController*> {
    // Wir haken die Funktion ein, die aufgerufen wird, wenn der Tab sichtbar wird
    void onEnter(AdvancedGraphicsController* self) {
        // Zuerst den Originalcode ausführen, damit die Tab-Inhalte geladen werden
        self->AdvancedGraphicsController::onEnter(self);

        // Prüfen, ob wir das Element schon hinzugefügt haben, um Duplikate zu vermeiden
        if (self->m_advancedFpsLabel) return;

        log::debug("Füge FPS-Feld hinzu...");

        // Wir suchen den Layout-Container des Tabs
        // In GD ist das oft die ScrollView oder ein Container-Node.
        // Wir nehmen den "Advanced" Tab Container an.
        auto* tab = self->getChildByID("Advanced"); // Dies ist eine Geode-Helper-Methode, falls verfügbar
        // Falls ID nicht funktioniert, nehmen wir das Root Node
        auto* root = self->getChildByIDUnsafe("advanced"); 
        
        // Wir brauchen einen Container, um das Label und das Feld nebeneinander zu legen
        // Da die GD-UI etwas tricky ist, nutzen wir ein einfach Layout
        auto* node = CCNode::create();
        node->setContentSize(CCSizeMake(100, 30));
        
        // 1. Label erstellen ("Target FPS:")
        auto* label = CCLabelBMFont::create("Target FPS:", "bigFont.fnt");
        label->setColor(ccc3(255, 255, 255));
        label->setPosition(CCPoint(10, 15));
        node->addChild(label);

        // 2. TextField erstellen (Eingabefeld)
        auto* textfield = TextFieldTTF::create("", "Arial", 20);
        textfield->setPlaceholderColor(ccc3(100, 100, 100));
        textfield->setAnchorPoint(CCPoint(0, 0.5f));
        textfield->setPosition(CCPoint(120, 15));
        textfield->setTag(1337); // Tag zum späteren Finden
        textfield->setKeyboardInstrument(TextFieldKeyboardInstrument::KeyboardInstrument::Number); // Nur Zahlen
        textfield->setPlaceholderTextColor(ccc3(150, 150, 150));
        textfield->setPlaceHolder("60");
        
        // Event Handler: Speichern, wenn der Benutzer fertig ist (onEditEnded)
        textfield->addEventListener([](CCObject* sender, CCControlEvent event) {
            if (event == Control::Event::TouchUpInside || event == Control::Event::EditingDidEndOnExit) {
                auto* field = static_cast<TextFieldTTF*>(sender);
                // Speichere den Wert im Konfigurations-File
                mod.save()->targetFps = std::stof(field->getString());
                mod.save(true); // true = sofort speichern
            }
        });

        node->addChild(textfield);
        self->m_advancedFpsLabel = node; // Wir speichern den Pointer in der Struktur, damit wir ihn später löschen können
    }

    // Wir überschreiben auch onExit, um das UI-Element sauber zu entfernen, wenn man den Tab verlässt
    void onExit(AdvancedGraphicsController* self) {
        if (self->m_advancedFpsLabel) {
            self->m_advancedFpsLabel->removeFromParent();
            self->m_advancedFpsLabel->release(); // Release, da CCNode das Ownership nicht übernimmt
            self->m_advancedFpsLabel = nullptr;
        }
        self->AdvancedGraphicsController::onExit(self);
    }

    // Hilfsstruktur für den Modifikator, damit wir den Node speichern können
    struct Member {
        CCNode* m_advancedFpsLabel = nullptr;
    };
} * advancedGraphicsControllerHook = new AdvancedGraphicsControllerHook();

// 4. Hook um die FPS tatsächlich im Spiel zu ändern
// Wir haken GameSpeed an.
struct $modify(GameSpeed) : geode::HookHandler<GameSpeed> {
    // Hook die setSpeed Funktion oder start Funktion
    void start(GameSpeed* self, float speed) {
        // Den normalen Startvorgang ausführen
        self->GameSpeed::start(self, speed);

        // Prüfen, ob der Benutzer einen FPS-Wert > 0 eingestellt hat
        if (mod.save()->targetFps > 0.0f) {
            // Wir berechnen die FrameDuration (Zeit pro Bild)
            // GD nutzt FrameDelay. 60 FPS = 1.0f Delay, 30 FPS = 0.5f Delay.
            // Formel: 1 / FPS
            float targetDelay = 1.0f / mod.save()->targetFps;
            
            // Wir setzen die aktuelle FrameDelay Variable im GameSpeed Objekt
            // Dies überschreibt das normale Timing
            self->m_frameDelay = targetDelay;
        }
    }
    
    // Wir brauchen eine Member-Definition für die Hook-Struktur, damit sie sauber kompiliert
    struct Member {
        float m_frameDelay; 
    };
};
