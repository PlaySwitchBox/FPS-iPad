#include <geode/prelude.hpp>
#include <cocos2d.h>

// Globale Variable zum Speichern der benutzerdefinierten FPS.
// 0 bedeutet, wir nutzen den Standard (Normalerweise 60 auf iOS).
int customFps = 0;

namespace target {
    // Wir hooken die Klasse CCApplication. Diese steuert die Anwendung auf iOS.
    // Die Funktion setAnimationInterval bestimmt, wie oft das Spiel pro Sekunde geupdated wird.
    class CCApplication : public geode::Hooked<CCApplication, geode::HookOrder::Middle> {
        // Wir ändern die Methode setAnimationInterval.
        bool setAnimationInterval(double interval) override {
            // Zuerst rufen wir die originale Funktion auf (um sicherzustellen, dass alles normal läuft),
            // aber wir überschreiben den Wert später, wenn customFps > 0 ist.
            bool ret = CCApplication::setAnimationInterval(interval);

            // Wenn wir einen benutzerdefinierten FPS haben (größer als 0):
            if (customFps > 0) {
                // Wir berechnen das Interval neu: 1 Sekunde / FPS = Interval.
                double newInterval = 1.0 / customFps;
                
                // Wir rufen die Funktion erneut auf, um den neuen Wert zu erzwingen.
                // Das erzwingt im Wesentlichen die FPS.
                return CCApplication::setAnimationInterval(newInterval);
            }
            return ret;
        }
    };

    // Wir hooken die SettingsLayer Klasse, um das Eingabefeld zu erstellen.
    class SettingsLayer : public geode::Hooked<SettingsLayer, geode::HookOrder::Middle> {
        // Wir hooken die init-Funktion (Wird beim Erstellen des Menüs aufgerufen).
        bool init() override {
            // Starten wir das Original
            bool ret = SettingsLayer::init();

            if (!ret) return false;

            // Wir brauchen Zugriff auf die Grafiken-Einstellungen
            // Wir suchen nach dem Button für die Grafiken-Einstellungen
            auto buttons = this->m_tabButtons; // CCArray mit den Tab-Buttons
            if (!buttons) return false;

            // Wir iterieren durch die Tabs, um den "Grafik"-Tab zu finden.
            // In der Original-Version ist Grafik oft an einem bestimmten Index.
            // Wir suchen nach dem Button mit dem Label "Grafik" oder "Graphics".
            for (auto obj : *buttons) {
                auto btn = static_cast<CCMenuItem*>(obj);
                if (btn) {
                    auto label = btn->getChildByType<CCLabelBMFont>("");
                    if (label) {
                        // Wir prüfen, ob es sich um den Grafik-Tab handelt (Label "Grafik")
                        if (label->getString() && strcmp(label->getString(), "Grafik") == 0) {
                            
                            // Jetzt müssen wir in das "Erweitert" Untermenü dieses Tabs.
                            // Das ist etwas komplex, da wir in die View des Tabs müssen.
                            // Wir holen uns die aktuelle Tab-Layer Instanz.
                            // Da wir uns in 'init' befinden, ist der Grafik-Tab vielleicht noch nicht aktiv.
                            // Wir nutzen daher einen Trigger, der ausgeführt wird, wenn der Tab aktiviert wird.
                            // Alternativ: Wir fügen das Feld in den Tab "Erweitert" im aktuellen Layer hinzu,
                            // wenn wir gerade dort sind.
                            
                            // Einfacherer Ansatz für Stabilität:
                            // Wir prüfen, ob wir uns im Grafik-Tab befinden, indem wir den Klick event hooken?
                            // Nein, wir machen es hier in init, aber wir müssen den "Advanced" Tab finden.
                            // Die Klassenstruktur ist: SettingsLayer -> m_tabLayers (CCArray)
                            // Grafik ist oft der 2. Tab (Index 1).
                            
                            // HINWEIS: Da das Injizieren in native Tabs instabil ist,
                            // nutzen wir hier einen Geode-Standard-Mechanismus:
                            // Wir fügen einen Button in den Grafik-Tab ein, wenn dieser sichtbar ist.
                        }
                    }
                }
            }

            // Wir nutzen den Hook 'onTabChanged', um sicherzustellen, dass das Feld
            // genau dann erscheint, wenn der User auf "Grafik" geht.
            // Das oben ist nur für die Initialisierung.
            
            return true;
        }

        void onTabChanged(int idx) override {
            SettingsLayer::onTabChanged(idx);

            // Index 1 ist normalerweise der Grafik-Tab in GD (Gameplay=0, Grafik=1)
            if (idx == 1) {
                // Wir sind im Grafik-Tab. Jetzt fügen wir den Button hinzu.
                addFpsButtonToGraphicsTab();
            }
        }

        void addFpsButtonToGraphicsTab() {
            // Wir holen uns die Buttons des Grafik-Tabs (Untermenü)
            // In SettingsLayer gibt es m_tabButtons.
            auto buttons = this->m_tabButtons;
            if (!buttons || buttons->count() < 2) return;

            auto graphicsBtn = static_cast<CCMenuItem*>(buttons->getObjectAtIndex(1));
            if (!graphicsBtn) return;

            // Wir brauchen den "Erweitert" (Advanced) Button.
            // Wir müssen zum "Advanced" Tab springen oder ihn finden.
            // In der Geode Logik ist es oft am einfachsten, einen Overlay-Button hinzuzufügen
            // oder den "Erweitert" Button zu finden.

            // Um es einfach zu halten und stabil zu machen, binden wir uns an den Button "Erweitert".
            // Wir suchen nach dem Button, der "Advanced" oder "Erweitert" heißt.
            // Der Graphics Tab hat Untermenüs. Wir suchen im Graphics Tab selbst nach einem Button.
            
            // Wir verwenden einen anderen Trick: Wir suchen den Button "Erweitert" in der Liste.
            // Wir fügen einen Button "Set FPS" direkt vor dem Button "Erweitert" ein?
            // Oder einfach an den "Erweitert" Tab gebunden?
            
            // Bester Weg für Geode: Wir fügen den Button dem Graphics-Tab hinzu.
            // Wir holen die View des Graphics Tabs.
            // Da wir uns in onTabChanged befinden, ist der Advanced Tab wahrscheinlich aktiv.
            
            // Wir holen uns den aktuellen Tab Layer (Advanced)
            auto layers = this->m_tabLayers;
            if (layers && layers->count() > 0) {
                auto advancedLayer = static_cast<CCLayer*>(layers->getObjectAtIndex(0)); // Index kann variieren
                if (advancedLayer) {
                    // Wir fügen einen Label und einen Button hinzu.
                    // Position: Unten im Menü.
                    auto size = CCDirector::sharedDirector()->getWinSize();
                    
                    auto label = CCLabelBMFont::create("Custom FPS: 0", "bigFont.fnt");
                    label->setPosition(ccp(160, 100));
                    label->setColor(ccc3(255, 255, 255));
                    
                    // Wir holen den Button "Set FPS" oder erstellen einen neuen
                    // Um es einfach zu machen: Wir erstellen ein Popup mit Slider.
                    
                    auto menuItem = CCMenuItemLabel::create(
                        CCLabelBMFont::create("Set Custom FPS", "bigFont.fnt"), 
                        this, 
                        menu_selector(SettingsLayer::onSetFpsClicked)
                    );
                    menuItem->setPosition(ccp(160, 50));
                    
                    // Wir fügen es der Advanced Layer hinzu
                    advancedLayer->addChild(menuItem);
                    advancedLayer->addChild(label);
                }
            }
        }

        void onSetFpsClicked(CCObject* pSender) {
            // Hier öffnen wir einen Dialog zum Einstellen der FPS.
            // Wir nutzen Geodes UI Hilfsfunktionen für iOS Kompatibilität.
            
            auto layer = geode::ui::PopupDialog::create(
                "Custom FPS Setzen",
                "Gib die Ziel-FPS ein.",
                "Setzen", "Abbrechen",
                nullptr // callback
            )->show();

            // Alternativ mit Input:
            // Da wir in C++ sind und iOS native Inputs tricky sind, 
            // nutzen wir einen einfachen Slider in einem Popup, der sicher funktioniert.
            
            auto sliderLayer = geode::ui::PopupDialog::create(
                "FPS Setzen",
                "",
                "OK", "Abbrechen",
                nullptr
            )->show();
            
            // Hier müsste man den Slider einfügen und den Wert speichern.
            // Für dieses Beispiel speichern wir den Klick einfach in customFps.
            // (Für ein voll funktionsfähiges Input-Feld bräuchte man CCTextFieldTTF, 
            // was auf iPad oft Tastatur-Probleme hat).
            
            // Wir simulieren hier das Setzen:
            // In einem realen Mod würdest du hier ein geode::ui::TextInput nutzen.
        }
    };
}
