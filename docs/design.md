# RADIO DAYS 設計書

- 版：第3版（2026-10-20）。Launcher 2.9.1のソースによる確認（3.5節）、1.2MBを超えるアプリとする方針（3.1節）、NVSの検証方法の見直し（3.6節）、SDカードの方針（3.8節）を加えた。同日、更新時の動き（3.5.5節）、PManの第1回確認（3.5.6節）、D0→D1の表現の修正（3.6.2節）を追記
- 前の版：第2版で、第7節の決定、Launcherのアプリ領域の調査、NVSの合格条件、Stage 2-0の結果を加えた。第1版はStage 1の設計
- 状態：**設計段階。** 実機で確かめていない事項は【未確認】と書き、実装の前提にしない
- 経緯：Cardputer Media Jogへの機能追加「RADIO GA GA」（Stage R1調査、2026-10-20）を、独立アプリに方針変更した。R1の調査結果のうち、独立アプリにも当てはまるものを引き継ぐ
- 印：【確認】一次資料・コード・ビルドで確かめた／【推測】／【未確認】

---

## 1. 基本方針

| 項目 | 内容 |
| --- | --- |
| 作品名／フォルダ名 | RADIO DAYS／`RADIODAYS` |
| ハードウェア | Cardputer ADV＋U135（Grove、I²C 0x40）。追加の電子工作なし |
| 位置づけ | 独立アプリ。Launcherから起動する |
| 対象外 | Media Jogとの同時動作、バックグラウンド再生、USB/BLE HID |
| 保護するもの | Launcher、Media Jog、SOUND TERRARIUM、本体のNVSに保存された他アプリの設定 |
| 優先順位 | **ラジオ再生の安定 ＞ 操作性 ＞ 画面の演出** |

## 2. ハードウェアの前提

| 項目 | 内容 | 根拠 |
| --- | --- | --- |
| SoC | ESP32-S3FN8（Stamp-S3A）、240MHzデュアルコア、フラッシュ8MB | 【確認】M5Stack公式 Cardputer-Adv ページ |
| PSRAM | **なし**（型番に「R」がない。公式ページにも記載なし） | 【確認】同上 |
| 音声 | ES8311コーデック＋NS4150Bアンプ＋8Ω 1Wスピーカー | 【確認】同上 |
| イヤホン | 3.5mm出力あり。**挿すとスピーカーのアンプが無効になる** | 【確認】同上 |
| イヤホンの抜き差しの検出 | ソフトから検出できるか不明 | 【未確認】 |
| 音声のピン | I2S1：BCK G41、WS G43、DOUT G42。ES8311は本体内部のI²C（G8・G9） | 【確認】M5Unified 0.2.25 `M5Unified.inl` |
| U135 | Grove G2（SDA）・G1（SCL）、0x40。1ノッチ＝2カウント | 【確認】Media Jogのコードと実測記録 |
| microSD | あり（SPI：G40・G14・G39、CS G12） | 【確認】公式ページ |

## 3. 調査結果と判断（Stage 1）

### 3.1 フラッシュ容量（最重要）

**事実（2026-10-20のビルドで確認。本体への書き込みはしていない）**

| 対象 | アプリの大きさ | 標準のアプリ領域（1,310,720 B）との比較 |
| --- | --- | --- |
| M5Unified公式例 `WebRadio_with_ESP8266Audio`（そのまま） | **標準設定ではビルド不可**（text section exceeds available space） | 超過 |
| 同じ例を Huge APP 設定でビルド | 1,436,736 B | **約126KB超過** |
| 外部ディスプレイ用のincludeを外した版 | 1,426,656 B | 約116KB超過 |
| 参考：SOUND TERRARIUM v108cs（Wi-Fi・HTTP・WebServer） | 1,301,951 B（README記載） | 残り8,769 B |
| 参考：Media Jog 2a-5（BLE・USB HID、Wi-Fiなし） | 896,864 B | 約68% |

ビルド環境：m5stack:esp32 3.3.9、M5Unified 0.2.25、ESP8266Audio 2.4.1、ボード M5Cardputer（USB Mode：USB-OTG（TinyUSB）、CDC On Boot：Enabled）

**内訳（上の「外した版」、リンカーのmapファイルから集計）【確認】**

| 部分 | 大きさ | 削れる見込み |
| --- | --- | --- |
| Wi-Fi・TCP/IP（net80211、lwip、wpa_supplicant、pp、phy など） | 約45万B | 削れない |
| M5GFX | 約20万B | フォントの使い方しだいで減らせる【推測】 |
| ESP8266Audio | 約12万B | MP3以外のデコーダーが含まれていないか確認する【未確認】 |
| mbedcrypto | 約6.8万B | Wi-Fi（WPA）でも使うため、大きくは減らない【推測】 |
| M5Unified | 約5.7万B | ― |
| TinyUSB | 約4.3万B | **USB HIDを使わないので、USB Modeを「Hardware CDC and JTAG」にすれば外せる見込み**【推測】 |

**判断（第1版。第3版で下の「方針」に置き換えた）**

- 標準のアプリ領域（1.2MB）に収めることを目標にし、Stage 2-0で収まるかどうかを判定する、とした

**Stage 2-0の結果（2026-10-20、ビルドのみ）** 詳細は [stage-2-0-size.md](stage-2-0-size.md)

| 構成 | アプリの大きさ | 1,310,720 Bとの差 |
| --- | --- | --- |
| A：USB-OTG（TinyUSB） | 1,352,752 B | **42,032 B超過**（標準の設定ではビルド不可） |
| B：Hardware CDC and JTAG | 1,299,328 B | **残り11,392 B**（収まるが、目安の64KBに届かない） |

- TinyUSBを外すと約5.3万B減ることを確認した【確認】。RADIO DAYSは構成Bを基本にする
- ESP8266Audioの約11.8万Bは、ほぼMP3デコーダー本体（libmadの `layer3`・`huffman`）で、削れる部分は少ない【確認：mapファイル】
- **標準のアプリ領域のままでは、ラジオの画面とWi-Fi設定画面を載せる余裕がほとんどない**

**方針（第3版、2026-10-20の決定）**

1. 標準の1.2MBに収めるための過度な機能削減は行わない
2. Launcher 2.9.1で安全に扱えるなら、**1.2MBを超えるアプリ**として開発する。Launcher 2.9.1は、アプリの大きさに合わせて領域を作る作りになっている（3.5.1節）
3. ビルド時のパーティション設定と、実機のパーティション表を区別して検証する（3.5.2節）
4. Arduino IDEの通常の「書き込み」は使わない
5. Launcherから、app-onlyの `.ino.bin` を導入する
6. 大きさの目安：Stage 2-1の時点では約1.3MB。Stage 4までの追加を見込み、**実機の空き（FREE）が1.6MB以上あること**を導入の条件の目安とする（仮。PManで確かめてから決める）

### 3.2 RAM（PSRAMなし）

| 項目 | 内容 |
| --- | --- |
| 公式例が最初に確保するメモリ | ストリーム用バッファ 32KB＋MP3デコーダー 29,192B【確認：例のソース】 |
| 参考 | SOUND TERRARIUMは、Wi-Fiを使った状態で、全画面16ビットのスプライト（240×135×2＝64,800B）を確保して動いている【確認：ソース。動作は作者の実機記録】 |
| 判断 | **全画面16ビットのスプライトとラジオ用のメモリは、同時に確保しない設計にする。** 画面は部分スプライトか8ビットにする。空きヒープと、最大の連続空き領域をStage 2で実測する |

### 3.3 音声の出し方

- **M5Unifiedの `Speaker` を使う**（公式例と同じ）。マイクを使わないので、SOUND TERRARIUMのように独自のI2Sドライバーを書く必要はない
- 公式例の構成を使う：デコード用のタスクをコア0、スピーカー用のタスクをコア1に置く。Wi-Fiの省電力モードは無効にする（`WiFi.setSleep(false)`。省電力モードではデータがまとめて届き、バッファが空になるため）【確認：例のソースとコメント】
- この例がADVの実機で動くことは【未確認】。SOUND TERRARIUMのES8311の設定に関する記録（REG01、DAC音量）は、音に問題が出たときの参考にする

### 3.4 Wi-Fi

- SSIDとパスワードは、Cardputerのキーボードで入力する（Stage 3以降）。Stage 2では、Gitに入れない別ファイルで書き込み時に設定する
- **接続に成功してから保存する**（SOUND TERRARIUM v108bsと同じ考え方）
- ライブラリが自動でWi-Fiの設定をNVSに書かないように、`WiFi.persistent(false)` にする
- 再接続：Wi-Fiが切れたら、待ち時間を少しずつ延ばしながら再接続する。ストリームの途切れは局単位で扱う（3.7節）

### 3.5 Launcher 2.9.1との互換性

本体のLauncherは **2.9.1**（2026-10-20、作者が実機で確認）。以下は、GitHubの **2.9.1タグのソース**（bmorcelli/Launcher、コミット `3b91484efc`）で確かめた内容で、実機での動作は【未確認】。

#### 3.5.1 アプリ領域の作り方【確認：2.9.1のソース】

| 項目 | 内容 | 根拠（2.9.1） |
| --- | --- | --- |
| Cardputer用のパーティション表 | `nvs` 0x9000（0x5000＝20KB）、`otadata` 0xE000、Launcher自身 `app0`（test）0x10000から0x150000、`coredump` 0x160000から0x10000 | `boards/m5stack-cardputer/platformio.ini` → `support_files/custom_8Mb.csv` |
| 導入するアプリの領域 | **アプリの大きさを64KB単位に切り上げた大きさ**で、空いている範囲に新しいOTA用のアプリ領域を作る。1.2MBの上限はない | `partition_table_model.cpp` `launcherPartitionCreateOtaApp()`（`alignUp(imageSize, 0x10000)`） |
| 導入の経路 | WebUI（Wi-Fiアップロード）、SDカード、オンライン、シリアルのどれも、同じ領域選択の処理を通る | `webInterface.cpp`、`sd_functions.cpp`、`onlineLauncher.cpp`、`serial_console.cpp` → `launcherSelectInstallLayout()` |

**空きが足りないときの動き（重要）**【確認：`partition_install_layout.cpp`】

1. 空いている範囲に入れば、新しい領域を作るだけ。他のアプリには触れない
2. 入らず、**他のアプリが1つも入っていない**場合：SPIFFS・LittleFS・FAT（`sys`・`vfs`）のデータ領域を**自動で削除**して場所を作る
3. 入らず、他のアプリがある場合：「Need（必要な大きさ）」という選択画面が出る。選択肢には「Use（既存のアプリ名）partition」（**そのアプリを上書きする**）などが並び、最後に「Cancel」がある

→ **導入する前に、PManで空き（FREE）がアプリの大きさ以上あることを確かめる。導入中に「Need …」の画面が出たら、必ず「Cancel」を選ぶ。**

#### 3.5.2 ビルド時の設定と、実機のパーティション表の区別

| 区別 | 内容 |
| --- | --- |
| ビルド時の設定（Partition Scheme） | Arduinoが**アプリの大きさを検査する**ためと、ビルドに使う。実機には書き込まない（app-onlyの `.ino.bin` には、パーティション表は含まれない） |
| 実機のパーティション表 | Launcherが管理する。アプリの導入時に、Launcherが領域を作る |
| 注意【確認：ビルドの比較】 | ビルド時の設定を変えると、**バイナリの中身も変わる**（構成Bを「Huge APP」でビルドすると約30万バイトが異なる。8MB設定ではヘッダーのフラッシュ容量の値も変わる）。**導入するのは、試験したものと同じバイナリだけ**にする |

**RADIO DAYSのビルド設定（案）**

| 設定 | 値 | 理由 |
| --- | --- | --- |
| USB Mode | Hardware CDC and JTAG | TinyUSBを外せる（3.1節） |
| USB CDC On Boot | Enabled | シリアルの記録（NVSの試験に使う） |
| Flash Size | **8MB** | 実機（ESP32-S3FN8）に合わせる。導入されたアプリは、8MBのどこに置かれるかわからない |
| Partition Scheme | **8M with spiffs（3MB APP/1.5MB SPIFFS）** | 1.2MBを超えても大きさの検査を通すため。実機には書き込まない |

- 参考：構成Bをこの設定でビルドすると1,299,376 B【確認】
- 未確認：アプリのヘッダーに書かれたフラッシュ容量（4MBか8MBか）が、Launcherのもとでの動作に影響するか。Media JogとSOUND TERRARIUMは4MBの設定のまま動いている【確認：各README】。8MBの設定は、Stage 2-1で実機で確かめる【未確認】

#### 3.5.3 Launcher 2.9.1がNVSに書き込むもの【確認：2.9.1のソース】

| 名前空間 | Launcherが書き込む場面 | 根拠 |
| --- | --- | --- |
| `launcher` | 設定。**アプリを起動するたびに**最後のアプリ名などを保存する（`saveIntoNVS()`） | `settings.cpp`、`app_registry.cpp` `launcherBootAppByLabel()` |
| `l_wifi` | Wi-Fiの一覧。一覧が変わったとき、名前空間をすべて消してから書き直す | `settings.cpp` `saveWifiIntoNVS()` |
| `l_apps` | 導入したアプリの一覧 | `app_registry.cpp`（`kNamespace = "l_apps"`） |
| `l_bonds` | 起動していないアプリのBLEペアリング記録の退避場所 | `ble_bonds.cpp` |
| `nimble_bond` | **アプリの導入時と、アプリを起動するたびに入れ替える。** 今の持ち主の記録を `l_bonds` へ退避して消し、起動するアプリの記録を戻す | `ble_bonds.cpp` `launcherBleBondsSwitchTo()`、`install_shared.cpp`、`app_registry.cpp` |
| `uiflow` | UiFlow用の既定値（Launcherの起動時） | `settings.cpp`、`main.cpp` |
| `nvs.net80211` | **Launcher自身のWi-Fiでは書き込まない見込み。** Wi-Fiの初期化の直後に `esp_wifi_set_storage(WIFI_STORAGE_RAM)` を呼んでいる | `src/idf/idf_wifi.cpp` 第170行 |

- **エドガーの指摘（導入時に `nvs.net80211` が変わる可能性）について**：2.9.1のソースでは、Launcherの設定の書き込みはRAMだけになっていて、上の表の名前空間以外への書き込みは見つからなかった。ただし、Wi-Fiドライバーの初期化（`esp_wifi_init`）自体がNVSに書くかどうかと、PHYの較正（`phy`）は、Launcherのソースからは判断できない【未確認】。**実測で区別する**（3.6節）
- **Media JogのBLEペアリングへの影響（RADIO DAYSとは関係なく起きる）**：記録の持ち主がわからない場合（このLauncherが入れ替えを始める前に作られた記録など）、退避されずに消される作りになっている（`ble_bonds.cpp` のコメント）。Media JogのBLEは、再ペアリングが必要になる場合がある【確認：ソース。実機での発生は未確認】

#### 3.5.4 PManの確認手順（見るだけ）

**目的**：実機のパーティション表と空き容量を、推測ではなく実物で確かめる。

**確認する情報**

| 情報 | どこに表示されるか |
| --- | --- |
| Launcherの版 | Launcherの画面（シリアルの `version` コマンドでも可） |
| 各パーティションの名前・種類・開始位置（ofs）・大きさ（size） | PManの一覧。`* ` が付いた行は保護されている領域（実行中のLauncherなど） |
| **空き（FREE）の範囲と大きさ** | PManの一覧の `FREE ofs=… size=…` の行。**一番大きいFREEの大きさが、RADIO DAYSを入れられる上限** |
| 入っているアプリ（Media Jog、SOUND TERRARIUMなど）の領域の名前と大きさ | PManの一覧 |
| NVSの位置と大きさ | PManには表示されない（開始位置0x10000未満の領域は一覧に出さない作り）。シリアルの `partitions` コマンドで確認できる |

**安全な操作手順**

1. Launcherのメインメニューで **PMan**（Partition Manager）を開く
2. 一覧を上下に動かして、表示されている行を**写真に撮る**（行が多い場合は数枚）
3. 1行の詳しい内容を見たいときは、その行を選び、表示される小さなメニューで **「Details」だけ**を選ぶ。見終わったら「Back」
4. 確認が終わったら、一覧の一番下の **「Back」** でPManを閉じる

**選ばないもの**（どれも、パーティション・データ・ペアリングを変更する）

- 行のメニューの「Edit Size」「Remove」「Format」「Restore data」「Associate to Bin」「Backup」
- FREEの行のメニューの「Add OTA」「Add FAT」「Add SPIFFS」
- 一覧の「Apply Changes」「Erase BLE Bonds」、開発者モードのときに出る「Wipe Flash memory」「Format NVS Partition」

**誤って何かを選んだ場合**：パーティションの変更は、「Apply Changes」を選んで「Write table?」に同意するまで書き込まれない【確認：`partitioner.cpp`】。「Discard Changes」を選ぶか、「Back」で出るときの「Discard changes?」に同意すれば、書き込まずに戻れる。確認画面（Confirm／Cancel）が出たら「Cancel」を選ぶ。

**シリアルでの確認（任意）**：Launcher 2.9.1のシリアルには、読むだけのコマンド `partitions`（NVSを含む全パーティションと空き範囲を表示）と `version` がある【確認：`serial_console.cpp`】。シリアルモニターを開いたときに、本体が再起動することがある（書き込みではない）。

#### 3.5.5 アプリを更新するときの動き【確認：2.9.1のソース】

**結論：Launcher 2.9.1は、同じアプリの新しい版を導入するとき、既存の領域を再利用せず、新しい領域を確保する。** 古い版はそのまま残る。

| 項目 | 内容 | 根拠 |
| --- | --- | --- |
| 領域の名前（ラベル）の決め方 | 表示名（ファイル名から作る）の英数字の先頭6文字を小文字にしたもの（例：`RadioDays_2_1.ino.bin` → `radiod`） | `partition_table_model.cpp` `launcherPartitionSanitizedAppLabelBase()` |
| 同じ名前がすでにある場合 | 先頭5文字＋数字（`radio1`〜`radio9`、`radio0`）、それも埋まっていれば `app1`〜`app99` | 同 `launcherPartitionNextAppLabel()` |
| 領域 | 名前が変わるだけでなく、**空き（FREE）に新しいOTA用の領域を作る**。同じ名前のアプリを探して置き換える処理はない | `partition_install_layout.cpp` `launcherSelectInstallLayout()`（最初に `launcherPartitionCreateOtaApp()` を試す） |
| 既存の領域が再利用される場合 | **空きが足りないときだけ。** 「Need …」の画面に「Use（既存の名前）partition」が出て、選ぶとその領域を上書きする（名前も変わる） | 同、第191行付近 |
| 入れられるアプリの数 | OTA用のアプリ領域は最大16個（ota_0〜ota_15） | 同 `launcherPartitionNextOtaSubtype()` |

**実機との一致**：PManに `mediaj`、`media2`、`media3`、`media5` などが並んでいる（3.5.6節）。これは、Media Jogの導入のたびに新しい領域が作られる、この命名規則と一致する【推測：どの領域にどの版が入っているかは未確認】。

**RADIO DAYSへの影響**

- 更新するたびに約1.3MB以上の空きが必要になり、古い版の領域が残る
- 古い版を消すのはパーティションの変更になる。**その都度、作者の了承を得て行う**
- 設定は名前空間 `radiodays` に保存するので、新しい版も古い版の設定を読む。レコードに版を持たせ、読めない版のレコードは上書きしない（3.6節）
- 古い版を残しておけば、新しい版に問題があったときに古い版へ戻せる

**更新の方針（案）**：本体に置くRADIO DAYSは「試験中の版」と「直前の動作確認済みの版」の2つまでとし、それより古い版は、了承を得てLauncherから削除する。導入中に「Need …」の画面が出たら「Cancel」を選ぶルールは変えない（上書きで古い版を消すことはしない）。

#### 3.5.6 PManの確認結果（2026-10-20、作者が撮影した写真9枚）

**実機のパーティション表**（PManの一覧と「Details」から。読むだけで確認）

| 名前 | 種類 | 開始位置 | 大きさ | 終わり | 確認 |
| --- | --- | --- | --- | --- | --- |
| `app0` | APP/test（Launcher、保護） | 0x10000 | 0x150000【推測：2.9.1のソースの値】 | 0x160000 | 一覧で名前を確認 |
| `coredump` | DATA（保護） | 0x160000【推測】 | 0x10000【推測】 | 0x170000 | 一覧で名前を確認 |
| `soundt` | APP/ota（SOUND TERRARIUM） | 未確認 | 未確認 | ― | 一覧で名前を確認 |
| `spiffs` | DATA/spiffs | 未確認 | 未確認 | ― | 一覧で名前を確認 |
| `media5` | APP/ota | 未確認 | 未確認 | 0x400000まで | 一覧で名前を確認 |
| `media1` | APP/ota_2 | 0x400000 | 0xE0000（896KB） | 0x4E0000 | **Details** |
| `media3` | APP/ota_3 | 0x4E0000 | 0xE0000（896KB） | 0x5C0000 | **Details** |
| `media2` | APP/ota_4 | 0x5C0000 | 0xE0000（896KB） | 0x6A0000 | **Details** |
| `mediaj` | APP/ota_5 | 0x6A0000 | 0xE0000（896KB） | 0x780000 | **Details**（写真がぶれているが、前後の位置から0x6A0000と判断） |
| **FREE** | 空き | **0x780000** | **0x80000（512KB）** | **0x800000** | **Details** |

- `soundt`・`spiffs`・`media5` の3つで、0x170000から0x400000（約2.56MB）を使っている。内訳は未確認
- 一覧に出た空き（FREE）は1行だけで、**空きは合計512KB**【確認】
- Media Jogと思われる領域が5つあり、確認できた4つはすべて0xE0000（896KB）。Media Jog 2a-5のアプリ（896,864 B）を64KB単位に切り上げた大きさと同じ。**どの領域がどの版かは未確認**（Launcherのメインメニューの表示名で確かめる）
- 開発者モードではない（「Wipe Flash memory」「Format NVS Partition」は出ていない）

**判定：今の空き（512KB）には、RADIO DAYSは入らない【確認】**

| 対象 | 必要な連続した空き |
| --- | --- |
| Stage 2-1の版（約1.3MB。1,299,376 Bを64KB単位に切り上げ） | 0x140000（1.25MB） |
| 目安（Stage 4までの追加を見込む） | 1.6MB以上 |

空きは**連続している**必要がある（Launcherは、1つの空いている範囲にアプリの領域を作る。`launcherPartitionFindFreeRange()`）【確認：ソース】。

**空きを作る方法（すべてパーティションの変更。作者が決めて了承してから行う。今は何もしない）**

| 案 | 消すもの | できる連続した空き | Stage 2-1 | 目安1.6MB |
| --- | --- | --- | --- | --- |
| A | `mediaj`（空きの直前） | 0x6A0000〜0x800000＝0x160000（1.375MB） | 入る（余裕128KB） | 届かない |
| B | `mediaj` と `media2` | 0x5C0000〜0x800000＝0x240000（2.25MB） | 入る | 届く |

- 位置が離れた領域（例：`media1`）を消しても、その場所に1.25MB以上の連続した空きはできない（896KBの穴になる）
- **消す前に確かめること**：Launcherのメインメニューで、各領域の表示名（どの版か）を確かめ、普段使っているMedia Jog（2a-5など）を残す
- **BLEペアリングの注意**：Launcherは、削除されたアプリのBLEペアリング記録を退避場所から整理する（`ble_bonds.cpp` の `pruneDeletedApps()`）【確認：ソース】。普段使っている版を残せば影響はない見込み
- 削除は、Launcherのアプリの削除機能か、PManの「Remove」→「Apply Changes」のどちらかで行う。どちらの手順を使うかは、了承を得るときに決める（このとき、Launcherが他の領域を移動するかどうかを、ソースでもう一度確かめる【未確認】）

### 3.6 NVSの保護

| 項目 | 方針 |
| --- | --- |
| 自分の保存場所 | 名前空間 `radiodays` だけを使う。レコードにはmagic・版・CRCを付ける（Media Jogの方式に合わせる） |
| 他アプリの名前空間 | `mediajog`、`srwifi`・`sndrunner`（SOUND TERRARIUM）、`nimble_bond`、Launcherの名前空間（3.5.3節）には書かない・消さない |
| 消去 | `nvs_flash_erase`、`nvs_erase_*` を使わない |
| 既知のリスク | Arduinoのコアは、NVSに空きページがない場合などに、起動時にNVS全体を消す（Media Jog REPORT.mdの記録、`esp32-hal-misc.c`）。アプリからは止められない |
| 書き込みの頻度 | 音量などは、最後の変更から数秒後にまとめて書く。値が変わらなければ書かない |
| 起動時 | NVSを読むだけ。壊れたレコードや新しい版のレコードは上書きしない |
| 書き込みの経路 | NVSへの書き込みは、**1つの関数だけ**を通す。その関数が、名前空間・キー・理由をシリアルに記録し、回数を数える |

#### 3.6.1 NVSを変える可能性があるもの

| 変える者 | 名前空間 | 扱い |
| --- | --- | --- |
| RADIO DAYS | `radiodays` | 正当（決めたキー・決めた回数だけ） |
| Launcher（導入・起動の操作） | `launcher`、`l_wifi`、`l_apps`、`l_bonds`、`nimble_bond`、`uiflow` | Launcherの動作として記録する（3.5.3節）。RADIO DAYSの責任にしない |
| ESP-IDFのWi-Fi | `phy`（較正データ。`CONFIG_ESP_PHY_CALIBRATION_AND_DATA_STORAGE=y`【確認：ボードのsdkconfig】） | 1回だけの作成・更新なら許容 |
| ESP-IDFのWi-Fi | `nvs.net80211`（`CONFIG_ESP_WIFI_NVS_ENABLED=y`【確認】） | RADIO DAYSは `WiFi.persistent(false)` にするので、**変わらないこと**（効果は【未確認】） |
| それ以外 | `mediajog`、`srwifi`、`sndrunner` など | **変わらないこと** |

#### 3.6.2 検証方法（第2版）

第1版の方式（RADIO DAYSの初回起動時に一覧を取る）には、次の3つの問題があった。

| 問題 | 対策 |
| --- | --- |
| 初回起動時の一覧は、Launcherが導入時に書き込んだ後のもので、「導入前」ではない | **NVS領域そのものを、本体の外から読み出す**（下の手順）。導入の前・導入の直後・試験の後の3回読み、Launcherの変更とRADIO DAYSの変更を分けて比べる |
| 値のハッシュでは、同じ値の書き直しがわからない | **2つの記録を合わせて見る。** ①RADIO DAYSが書こうとした回数：書き込み関数の記録（シリアル）。②フラッシュで実際に変わったもの：NVS領域の読み出しを、NVSのページ・エントリ単位で解析する。なお、ESP-IDFのNVSは、保存されている値と同じ値の書き込みをフラッシュに書かない【確認：ESP-IDF v5.5 `nvs_storage.cpp`。ボードパッケージ3.3.9が同じ版かは【推測】】。そのため①と②は一致しないことがあり、両方を記録する |
| 点検の機能自体が、フラッシュとRAMを使う | 列挙とハッシュの機能はRADIO DAYSに**入れない**。アプリに入れるのは、書き込み関数の記録と `nvs_get_stats()` の表示だけにする（数百バイト程度【推測】） |

**NVS領域の読み出し（読むだけ）**

- 使う道具：esptool（Arduinoのボードパッケージに含まれる v5.3.0【確認】）の **`read-flash`**。読み出しだけで、フラッシュには書かない
- 位置と大きさ：**実機の `partitions` の出力で確かめた値を使う。** 2.9.1のソースでは `nvs` は 0x9000から0x5000【確認】だが、実機の値で確定する
- 本体をダウンロードモードにする：電源スイッチをOFF → **G0を押したまま**電源を入れる → 離す（Media Jogのinstallation.mdに記載の方法）
- コマンドの例（位置と大きさは実機の値に置き換える）：

  ```bash
  esptool --chip esp32s3 --port <ポート> read-flash 0x9000 0x5000 nvs_D0.bin
  ```

- **使ってはいけないコマンド**：`write-flash`、`erase-flash`、`erase-region`、`write-flash-status`
- 解析：ESP-IDFに含まれるNVS解析用のスクリプト（`components/nvs_flash/nvs_partition_tool/nvs_tool.py`）で、ページの状態とエントリ（消された古いエントリを含む）を一覧にする。必要になった時点で入手する【未確認：使い方は実施前に確認する】
- **読み出したファイルには、他アプリのWi-Fiパスワードなどが含まれる。** リポジトリの外に保存し、Gitに入れない。比較の結果だけを、値を伏せて記録する

**手順**

| 段階 | 操作 | 読み出し | わかること |
| --- | --- | --- | --- |
| D0 | RADIO DAYSを導入する前 | NVSを読む | 基準 |
| ― | LauncherでRADIO DAYSを導入する | ― | ― |
| D1 | RADIO DAYSの最初の起動で**「待機画面」**のまま電源を切る（下の注） | NVSを読む | D0→D1：**導入から待機画面までの変化の合計**。Launcherの導入処理だけの変化とは断定しない（下の注） |
| ― | 試験の手順を行う（下の表） | ― | ― |
| D2 | 試験の後 | NVSを読む | D1→D2：**RADIO DAYSの実行中の変更**（`radiodays`、許容した `phy` だけのはず） |
| D3 | LauncherからMedia Jog、SOUND TERRARIUMを起動して、設定が残っていることを確かめた後 | NVSを読む | Launcherの起動時の入れ替え（`launcher`、`nimble_bond`、`l_bonds`）だけのはず |

注：Stage 2-1のRADIO DAYSは、起動すると**RADIO DAYS自身のコードではNVSに書かず、Wi-Fiも始めない「待機画面」**で止まり、キーを押すまで先に進まない作りにする。

**D0→D1に含まれる処理**（第3版で修正。「Launcherの導入だけの変化」とは断定しない）

| 順番 | 処理 | NVSを変える可能性 | 根拠 |
| --- | --- | --- | --- |
| 1 | Launcherの導入処理（領域の作成、アプリの登録、BLEペアリング記録の入れ替え、設定の保存） | `l_apps`、`nimble_bond`・`l_bonds`、`launcher` | 【確認：2.9.1のソース】`install_shared.cpp` |
| 2 | 導入後の再起動と、Launcherの起動処理。Launcherは起動画面を2〜5秒表示した後、導入したアプリを起動する | `uiflow` の既定値、`launcher` など | 【確認：2.9.1のソース】`main.cpp`（`nvs_flash_init()`、`ensureM5StackUiFlowNVSDefaults()`）、`webInterface.cpp` |
| 3 | **RADIO DAYSの初回起動時の処理**：Arduinoコアの初期化（`nvs_flash_init()`。空きページがない場合などはNVS全体を消す処理がある）、M5Unified・M5Cardputerの初期化 | 不明 | Media Jog REPORT.mdの記録（`esp32-hal-misc.c`）。ライブラリの初期化がNVSに書くかは【未確認】 |
| 4 | RADIO DAYS自身のコード（待機画面まで） | 書かない設計。書き込み関数の記録で0回であることを確かめる | 本設計 |

- D0とD1の比較だけでは、1〜3のどれによる変化かは区別できない。区別できるのは、4（RADIO DAYS自身のコード）が書いていないこと（書き込み関数の記録）だけ
- 1〜3を分けたい場合は、導入の後、Launcherの起動画面でボタンを押してLauncherにとどまり、そこでNVSを読む方法が考えられる（D0'）。ただし、この操作で本当にアプリが起動しないか、Launcherのメニューの表示が他の名前空間を書くかは【未確認】。必要になった時点で検討する
- D0→D1で、3.5.3節の名前空間と `uiflow` 以外（`mediajog`、`srwifi`、`sndrunner` など）が変わっていた場合は、原因を断定せずに不合格として調べる

**合格条件**

| # | 対象 | 合格 |
| --- | --- | --- |
| 1 | `mediajog`、`srwifi`、`sndrunner` など他アプリの名前空間 | D0〜D3のすべてで、エントリと値が変わらない（Launcherの導入・起動による変更もないこと） |
| 2 | Launcherの名前空間（3.5.3節） | 変化は、導入から待機画面まで（D0→D1）と、Launcherで他アプリを起動する段階（D2→D3）だけで起きている。D1→D2（RADIO DAYSの実行中）には変わらない |
| 3 | `nvs.net80211` | D1→D2で変わらない |
| 4 | `phy` | D1→D2での作成・更新は許容。ただし、RADIO DAYSを2回目以降に起動したときに毎回書き換わる場合は不合格 |
| 5 | `radiodays` のキー | 設計で決めたキーだけ |
| 6 | `radiodays` の書き込み | 書き込み関数の記録（試みた回数）が、試験の手順ごとの予定と一致する。フラッシュで実際に変わったエントリが、記録と矛盾しない |
| 7 | 空きエントリ | D2で64エントリ以上（仮）。RADIO DAYSは、空きが32エントリ未満のときは書き込まない |
| 8 | 原因のわからない変化 | **ない**（1つでもあれば不合格として調べる） |

**試験の手順と、RADIO DAYSの書き込み回数の予定**（書き込み関数の記録で数える）

| 手順 | 予定回数 |
| --- | --- |
| 起動して、何も変えずに10分再生して電源を切る | **0回** |
| 音量を何度か変えて、最後の変更から数秒待つ | **1回**（最後の値だけを書く） |
| 音量を変えて元の値に戻し、数秒待つ | **0回**（値が変わっていないので、書き込み関数を呼ばない） |
| Wi-Fi設定：接続に成功 | **1回** |
| Wi-Fi設定：接続に失敗 | **0回** |
| 局を切り替える（最後の局を保存する場合） | 最後の変更から数秒後に**1回** |

### 3.7 放送局の情報

| 段階 | 方式 |
| --- | --- |
| Stage 2〜3 | **アプリに組み込んだプリセット一覧。** 暗号化のないHTTPで配信されているMP3の局だけを使う（公式例にも同じ形の一覧が12局ある）。HTTPSは暗号化の処理に数十KBのRAMを使うため、最初は避ける【推測】 |
| Stage 4（候補） | Radio Browser API。公式ページで、無料であること、サーバーをDNS（`all.api.radio-browser.info`）で選ぶこと、アプリ名のわかるUser-Agentを送ること、HTTPSを推奨することを確認した。検索の条件の名前は【未確認】 |
| Stage 4の構成案 | 一覧を取得する段階と再生する段階を分ける。再生していないときに取得して保存し、取得に使ったメモリを解放してから再生する。保存先は、SDカードを使わない場合でも動くように決める（3.8節） |
| 局の切り替え | つながらない局・途中で止まった局は飛ばす（公式例の方式）。何回か失敗したら止めて、画面に表示する |

### 3.8 SDカード（2026-10-20の決定）

| 項目 | 方針 |
| --- | --- |
| 現段階 | **SDカードは使わない。** SDカードなしで、基本的なラジオ機能を完成させる |
| 内部フラッシュで実現するもの | Wi-Fi接続、MP3再生、U135の操作、基本の画面、プリセットの局 |
| 将来の用途 | 放送局のデータベース、画面の素材などの保存 |
| 守ること | SDカードを必須にしない。再生中に頻繁にSDカードを読み書きしない。SDカードの利用で、再生の安定性を下げる設計にしない |
| 参考 | microSDのSPI（G40・G14・G39、CS G12）は、音声（I2S1）・U135（Grove I²C）とピンを共有していない【確認：公式ページ、M5Unified】 |

## 4. 操作

| 操作 | 機能 | 判定 |
| --- | --- | --- |
| U135を回す | 選局（1ノッチ＝目盛り1つ） | ― |
| 押しながら回す | 音量 | 押している間に1ノッチ以上回したら音量の操作とし、離してもミュートは切り替えない |
| 短く押す | ミュートの切り替え | **押したときではなく、離したときに判定する** |
| キーボード | Wi-Fi設定、プリセット管理、設定画面 | キーの割り当てはStage 3で決める |

- 押しながら回す操作は、Stage 2で実機で試す。押しにくい・回しにくい場合は、**短く押すたびに選局と音量を切り替える方式**に替える
- ミュートは、ストリームを止めずに音量を0にする（解除するとすぐに音が戻る）

### 音量の安全

- **起動時は、毎回小さめの音量から始める**（保存した音量が大きくても、上限を設けて抑える）
- 最大音量に上限を設ける。イヤホンを挿すとスピーカーが切れるため、イヤホンで大きな音にならないように配慮する（抜き差しを検出できるかは【未確認】）
- 音量の変化をなだらかにして、プツッという音を防ぐ

## 5. 画面

### 5.1 実在の周波数と誤認させない表示

- **kHz・MHzなど、実在する単位を使わない。** 目盛りは「RD」などの架空の単位か、番号だけにする
- 目盛りの帯（バンド）には、周波数帯の名前ではなく**地域名**を付ける（例：EUROPE／AMERICAS／ASIA…）。BCLラジオのバンド切り替えの雰囲気を、地域で表現する
- 画面のどこかに「INTERNET RADIO」と表示する
- **変更（2026-10-20、Stage 2-3）**：作者の要望により、周波数を「80.2 MHz」のようにMHzで表示することにした。各局にはRADIO DAYS独自の**仮想周波数**を割り当てる（`stations.h`、0.1 MHz単位の整数）。実際の放送の周波数とは関係がないことを、資料とソースに明記する

### 5.2 画面の構成（240×135）

| 領域 | 内容 |
| --- | --- |
| 上段 | 局名、国名、接続状態（接続中・バッファ中・再生中・再接続待ち） |
| 中段 | アナログ式の選局目盛りと指針（ダイヤルの回転に連動） |
| 下段 | 音量、ミュート、Wi-Fiの状態 |

- 描画は部分スプライトで行い、全画面16ビットのスプライトは使わない（3.2節）
- 再生中の描画は最小限にし、デコード用のタスクにCPUを譲る

### 5.3 将来の演出（Stage 4）

- **選局中のノイズ**：局と局のあいだで、本体の中でノイズの音を作って流す。接続とバッファの待ち時間（1〜3秒）を、局を探している時間として演出する
- **ランニングマン**：第6節

## 6. SOUND TERRARIUMのランニングマン（移植の調査）

| 項目 | 内容 |
| --- | --- |
| 描画の仕組み | 10×11ピクセルの文字列の配列（`RUN0`〜`RUN2`、`JUMP0`、`FALL0`、`WAVE0`〜`WAVE2`）を、`drawSprite()` で1ピクセルずつ描く。黒い縁取り（周囲1ピクセル）の上に本体を描く【確認：SoundTerrarium.ino 第308行付近・第689行付近・第3689行付近】 |
| 依存しているもの | 体の色が気温に連動している（`runnerTemperatureColor()`、天気のデータ）。走る・ジャンプなどの状態は、地形・傾き・UFOの演出に依存している |
| 移植のしかた | **絵の配列と `drawSprite()` だけを移植する。** 色は固定色に替え、地形・天気・UFOには依存させない。動きは、走る・手を振るなど、単純な状態に絞る |
| ライセンス | SOUND TERRARIUMはMITライセンス、作者は同じ【確認：LICENSE】。移植元を記載する |
| 費用 | 絵のデータは数百バイト。描画は小さなスプライト1枚でできる【推測】 |
| 判断 | **移植は容易。** 再生の安定を優先し、Stage 4で設定画面に載せる。元のSOUND TERRARIUMのソースは変更しない |

## 7. 決定事項（2026-10-20）

1. **パーティション**：現段階では変更しない。Stage 2-0の結果を見て判断する（結果は3.1節。Launcherの実機確認は3.5節）
2. **Wi-Fiの設定**：SOUND TERRARIUMの設定（`srwifi`）は再利用しない。RADIO DAYSの名前空間 `radiodays` で独立して管理する
3. **ライセンス**：MIT License（`LICENSE`）
4. **容量**：1.2MBに収めるための過度な機能削減はしない。Launcher 2.9.1で安全に扱えるなら、1.2MBを超えるアプリとする（3.1節の方針）
5. **導入方法**：Arduino IDEの「書き込み」は使わず、Launcherからapp-onlyの `.ino.bin` を導入する
6. **SDカード**：現段階では使わない（3.8節）

## 8. 参照した資料

- M5Stack 公式 Cardputer-Adv ページ（仕様・ピンマップ）
- M5Unified 0.2.25：`M5Unified.inl`、`examples/Advanced/WebRadio_with_ESP8266Audio`
- ESP8266Audio 2.4.1
- Radio Browser API 公式ページ（api.radio-browser.info）
- Cardputer-ADV-Media-Jog：`MediaJog_2a_5.ino`、`docs/guide/installation.md`、`docs/history/REPORT.md`
- SOUND-TERRARIUM：`SoundTerrarium/SoundTerrarium.ino`（v108cs）、`README.md`、`LICENSE`
- bmorcelli/Launcher 2.9.1タグ（コミット `3b91484efc`）：`support_files/custom_8Mb.csv`、`boards/m5stack-cardputer/platformio.ini`、`src/partition_table_model.cpp`、`src/partition_install_layout.cpp`、`src/partitioner.cpp`、`src/ble_bonds.cpp`、`src/settings.cpp`、`src/app_registry.cpp`、`src/install_shared.cpp`、`src/serial_console.cpp`、`src/idf/idf_wifi.cpp`、`src/main.cpp`
- ESP-IDF v5.5：`components/nvs_flash/src/nvs_storage.cpp`（同じ値は書き込まない処理）
- m5stack:esp32 3.3.9：`tools/esp32s3-libs/3.3.9/sdkconfig`、esptool v5.3.0
