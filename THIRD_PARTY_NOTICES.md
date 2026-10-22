# Third-party notices and binary license

**English | [日本語の要約](#日本語の要約)**

The source code written for RADIO DAYS (`.ino`, `.h`, scripts and tools) is licensed under the MIT License; see [LICENSE](LICENSE).

This file covers the **compiled firmware binaries** distributed in [`firmware/RadioDays_U10/`](firmware/RadioDays_U10/) and the third-party components they contain.

## 1. Compiled binaries are distributed under GPL-3.0-or-later

| File | Purpose | Size | SHA-256 |
| --- | --- | ---: | --- |
| `RadioDays_U10.ino.bin` | App-only image for the Cardputer ADV Launcher | 1,371,360 B | `f6c4d40db3e29f1336fcacb50d79254f38e00fdaca7591c1e9002bb5b9b70e61` |
| `RadioDays_U10.ino.merged.bin` | Full flash image (bootloader, partition table, boot_app0 and app) for M5Burner or a full flash write | 4,194,304 B | `940735c5f2cf2b6963f14983f1923d0bde63ccc8a297e275bf67fc6309a92303` |

Both binaries link **ESP8266Audio 2.4.1** (Copyright (c) 2016-2024 Earle F. Philhower, III), which is licensed under the **GNU General Public License, version 3 or later**. ESP8266Audio embeds **libmad** (Copyright (c) 2000-2004 Underbit Technologies, Inc.), licensed under GPL-2.0-or-later. The combined binaries are therefore distributed under **GPL-3.0-or-later**. The full license text is in [`LICENSES/GPL-3.0-or-later.txt`](LICENSES/GPL-3.0-or-later.txt).

The binaries come with **no warranty**, to the extent permitted by law (GPL-3.0, sections 15 and 16).

## 2. Corresponding Source

The Corresponding Source for both binaries consists of:

1. **RADIO DAYS source:** [`firmware/RadioDays_U10/`](firmware/RadioDays_U10/) (`RadioDays_U10.ino` and its `.h` files).
2. **ESP8266Audio 2.4.1** (including libmad), as a verbatim copy in [`third-party/ESP8266Audio-2.4.1/`](third-party/ESP8266Audio-2.4.1/), with its original license and copyright notices. Upstream: <https://github.com/earlephilhower/ESP8266Audio> (tag `2.4.1`). The build uses the copy installed by the Arduino Library Manager; the bundled copy is identical to it.
3. **The other libraries and the board package**, listed in section 3, at the versions given there. They are published by their authors at the URLs shown.
4. **Build settings**, below.

### Build settings

Both binaries were built with the **Arduino IDE** and the **M5Stack board package `m5stack:esp32` 3.3.9**, board **M5Cardputer**, with:

| Setting | `RadioDays_U10.ino.bin` (app-only) | `RadioDays_U10.ino.merged.bin` (full image) |
| --- | --- | --- |
| USB Mode | Hardware CDC and JTAG | Hardware CDC and JTAG |
| USB CDC On Boot | Enabled | Enabled |
| Flash Size | 8MB (64Mb) | 4MB (32Mb) |
| Partition Scheme | 8M with spiffs (3MB APP/1.5MB SPIFFS) — used only for the size check; the Launcher creates the real partition | Huge APP (3MB No OTA/1MB SPIFFS) |
| Other options | Board defaults | Board defaults |

The equivalent `arduino-cli` board identifiers are:

```text
app-only:  m5stack:esp32:m5stack_cardputer:USBMode=hwcdc,CDCOnBoot=cdc,FlashSize=8M,PartitionScheme=default_8MB
full image: m5stack:esp32:m5stack_cardputer:USBMode=hwcdc,CDCOnBoot=cdc,FlashSize=4M,PartitionScheme=huge_app
```

The full image contains the bootloader, partition table and `boot_app0` produced by the same board package. Rebuilding produces a working firmware from the same source; a byte-identical binary is not guaranteed, because file paths of the build machine and the exact library versions are embedded in the image.

## 3. Third-party components in the binaries

| Component | Version | License | Source |
| --- | --- | --- | --- |
| ESP8266Audio (includes libmad) | 2.4.1 | GPL-3.0-or-later (libmad: GPL-2.0-or-later) | <https://github.com/earlephilhower/ESP8266Audio> |
| M5Cardputer | 1.1.1 | MIT, Copyright (c) 2025 M5Stack Technology CO LTD | <https://github.com/m5stack/M5Cardputer> |
| M5Unified | 0.2.25 | MIT, Copyright (c) 2021 M5Stack | <https://github.com/m5stack/M5Unified> |
| M5GFX (LovyanGFX-based) | 0.2.32 | MIT, Copyright (c) 2021 M5Stack; bundled components keep their own notices | <https://github.com/m5stack/M5GFX> |
| Arduino core for ESP32 (M5Stack board package `m5stack:esp32`) | 3.3.9 | LGPL-2.1 | <https://github.com/espressif/arduino-esp32>, M5Stack board package via the Arduino Boards Manager |
| ESP-IDF (precompiled libraries, bootloader) | v5.5.4, as shipped in the board package (`esp32s3-libs` 3.3.9) | Apache-2.0; bundled components (lwIP, mbedTLS, FreeRTOS and others) keep their own licenses | <https://github.com/espressif/esp-idf> |

All of these licenses are compatible with distributing the combined binary under GPL-3.0-or-later.

### MIT License text (M5Cardputer, M5Unified, M5GFX)

```text
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 4. Other material

| Material | License | Notes |
| --- | --- | --- |
| Globe land data in `globe_land.h`, generated by `tools/make_globe_land.py` from [Natural Earth](https://www.naturalearthdata.com/) | Public domain | |
| Sun, Moon and UFO faces, drawn after [SOUND TERRARIUM](https://github.com/kariagepompadour/SOUND-TERRARIUM) | MIT, same author | |
| 3D-printed stand and knob, from [Cardputer ADV Media Jog](https://github.com/kariagepompadour/Cardputer-ADV-Media-Jog) | MIT, same author | Not included in this repository |

Station names and stream URLs in `stations.h` identify the broadcasters' own streams. No audio is included in this repository. The rights to the broadcasts belong to the broadcasters, and their terms of use apply.

## 日本語の要約

- RADIO DAYSのために書いたソース（`.ino`、`.h`、スクリプト、ツール）はMIT License（[LICENSE](LICENSE)）です。
- 配布しているバイナリ2つ（Launcher用のapp-only `RadioDays_U10.ino.bin`、M5Burner用の全体イメージ `RadioDays_U10.ino.merged.bin`）は、GPL-3.0-or-laterのESP8266Audio 2.4.1（libmadを含む）を組み込んでいるため、**GPL-3.0-or-later**で配布します。
- 対応するソースは、`firmware/RadioDays_U10/`、`third-party/ESP8266Audio-2.4.1/`、第3節のライブラリ（記載の版）、第2節のビルド設定です。
- 第3節のライブラリ（M5Cardputer、M5Unified、M5GFX：MIT。Arduino core for ESP32：LGPL-2.1。ESP-IDF：Apache-2.0）は、いずれもGPL-3.0-or-laterでの配布と両立します。
- 放送の権利は各放送局にあり、各局の利用条件が適用されます。このリポジトリに音声は含まれていません。
