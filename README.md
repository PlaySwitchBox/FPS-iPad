# Custom FPS Mod für Geode

## Beschreibung
Dieses Mod erlaubt es dir, eine eigene Ziel-FPS (Zielbildrate) in den Grafikeinstellungen (Advanced Tab) zu definieren.
Dies kann nützlich sein, um das Spiel auf höherer Frequenz zu rendern oder Performance-Probleme zu umgehen.

## Installation
1. Lade die `custom_fps.geode` Datei herunter.
2. Öffne die Geometry Dash App.
3. Gehe zu Einstellungen -> Mods -> Import.
4. Wähle die Datei aus.

## Konfiguration
1. Öffne das Spiel.
2. Gehe zu `Einstellungen` -> `Grafik` -> `Erweitert`.
3. Du siehst nun das Feld "Target FPS".
4. Schreibe deinen Wunschwert rein (z.B. 120) und tippe außerhalb des Feldes oder drücke "Zurück", um zu speichern.

## Technische Details (für macOS Build)
Dieses Projekt verwendet das Geode SDK v1.7.0.
Es ist für die Zielplattform `iOS` konfiguriert (ARM64).

### Build-Anleitung für macOS
1. Installiere Xcode Command Line Tools: `xcode-select --install`
2. Installiere CMake (optional, aber empfohlen)
3. Öffne das Terminal im Projektverzeichnis.
4. Erstelle den Build-Ordner: `mkdir build && cd build`
5. **WICHTIG:** Konfiguriere für iOS Build:
   `cmake -G "Xcode" -DTARGET_IPHONEOS=ON ..`
6. Kompiliere:
   `cmake --build . --config Release`
7. Die `.dylib` Datei findest du im `build/Release-iphoneos` Ordner. Umbenennen in `.geode` für den Import.
