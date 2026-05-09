# RP2040 Zero Testing

Laufendes Projekt zum Testen der Hardware und des Raspberry Pi Pico SDK auf dem **Waveshare RP2040 Zero**.

<!-- ![Board](docs/images/board.jpg) -->

---

## Board

| Merkmal | Detail |
|---|---|
| MCU | RP2040 (Dual-Core ARM Cortex-M0+, 133 MHz) |
| Flash | 2 MB QSPI |
| RGB LED | WS2812B (NeoPixel) — GPIO16 |
| USB | USB-C (Bootloader + seriell) |
| ADC-Pins | GP26–GP29 |
| Formfaktor | 23.5 × 18 mm, castellated edges |

### Pinout

```
        USB-C
    ┌───┤├───┐
GP0 │         │ VBUS (5V)
GP1 │         │ GND
GP2 │         │ 3V3
GP3 │         │ GP29 (ADC3)
GP4 │         │ GP28 (ADC2)
GP5 │         │ GP27 (ADC1)
GP6 │         │ GP26 (ADC0)
GP7 │         │ GP15
GP8 │         │ GP14
GP9 │         │ GP13
    └─────────┘
         │
      GP16 → WS2812B (intern)
```

---

## Demos

### 1. RGB Farbwechsel + Kapazitiver Touch

**Datei:** `main.c`

- WS2812B wird via **PIO** mit 800 kHz angesteuert (GRB-Format)
- **GP26** als kapazitiver Touch-Sensor — kein Bauteil nötig
- Antippen → nächste Farbe: Rot → Grün → Blau → Cyan → Magenta → Gelb → Weiss

**Funktionsprinzip Touch:**
Der Pin wird kurz auf HIGH getrieben und dann auf Eingang mit Pull-Down umgeschaltet. Die Zeit bis zur Entladung wird als Loop-Iterationen gezählt. Ein Finger erhöht die Kapazität ~50×, was zuverlässig erkannt wird.

<!-- ![Demo](docs/images/demo.jpg) -->

---

## Toolchain

| Tool | Version |
|---|---|
| Pico SDK | master |
| ARM Toolchain | 15.2.rel1 (offizieller ARM-Tarball) |
| CMake | 4.x |
| Host | macOS (Apple Silicon) |

> Hinweis: `brew install arm-none-eabi-gcc` reicht nicht — es fehlt `nosys.specs`. Den offiziellen ARM GNU Toolchain Tarball von [developer.arm.com](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) verwenden.

---

## Build

```bash
mkdir build && cd build
export PICO_SDK_PATH=$HOME/pico/pico-sdk
cmake .. -DPICO_TOOLCHAIN_PATH=$HOME/pico/arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi/bin
make -j4
```

## Flashen

```bash
# BOOTSEL halten + USB einstecken → RPI-RP2 erscheint
cp build/rgb_demo.uf2 /Volumes/RPI-RP2/

# oder direkt via picotool (ohne BOOTSEL):
picotool load build/rgb_demo.uf2 -f
```
