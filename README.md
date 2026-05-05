# FPS Changer – Geode Mod

Ermöglicht das Ändern der Framerate direkt in den Einstellungen von Geometry Dash.  
Der Button erscheint unter **Einstellungen → Grafik → Erweitert**.

-----

## Funktionen

- Slider zum stufenlosen Einstellen der FPS (30–240)
- Eingabefeld für exakte Zahlen
- Schnellwahl-Buttons: 30 / 60 / 90 / 120 Hz
- Gespeicherter Wert wird beim Start automatisch geladen
- Benachrichtigung nach Übernehmen des Wertes

-----

## Voraussetzungen

- Geode SDK (https://geode-sdk.org)
- CMake 3.21+
- Xcode (für iOS/iPad) **oder** Visual Studio / GCC (für Windows/Android)
- Geometry Dash 2.2081

-----

## Kompilieren (Mac / iOS)

```bash
# 1. Geode SDK einrichten (einmalig)
geode sdk install

# 2. In den Projektordner wechseln
cd FPSChanger

# 3. Build-Ordner erstellen und kompilieren
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Die fertige `.geode`-Datei liegt danach in `build/yourname.fpschanger.geode`.

-----

## Installation auf dem iPad

1. Öffne die **Geode**-App auf deinem iPad.
1. Tippe auf das **Ordner-Symbol** (Mods-Ordner öffnen).
1. Kopiere die `yourname.fpschanger.geode`-Datei in diesen Ordner  
   (z.B. via AirDrop oder iTunes Dateifreigabe).
1. Starte Geometry Dash neu.
1. Gehe zu **Einstellungen → Grafik → Erweitert** → FPS Changer.

-----

## Anpassen

- In `mod.json`: `"developer"` auf deinen Namen ändern.
- In `mod.json`: `"id"` auf `"deinname.fpschanger"` setzen.
- FPS-Grenzen können in `src/main.cpp` über `FPS_MIN` / `FPS_MAX` angepasst werden.

-----

## Lizenz

MIT – frei verwendbar und veränderbar.
