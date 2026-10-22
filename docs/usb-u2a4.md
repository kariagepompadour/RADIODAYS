# U2a4：Wi-Fiを設定していなくても、USB中継でラジオを聴けるようにする

- 作成日：2026-10-21
- 状態：**実装、ビルド、ホストでの試験、回帰試験まで。実機ではまだ試していない**（導入は作者が結果を確かめてから）
- もとにした版：U2a3（2026-10-21 実機試験合格。`firmware/RadioDays_U2a3/` は変更せず保存）
- 作者の決定（2026-10-21）：オプションa（START画面にUSBの状態）・b（Wi-Fi未設定のときの表示）・c（Uキーで「今回はUSBだけ」、正式機能）を採用。d（Wi-Fi設定からの戻り方）は変えない。名前はU2a4
- **Macの中継は、U2a2と同じ `RadioDaysRelay_U2a2.command`**（やり取りは変えていない）
- Stage 3-3以前、U1、U2a、U2a2、U2a3のファイルは変更していない（SHA-256の一致を確認）

---

## 1. 変更の内容

| 場面 | U2a3 | U2a4 |
| --- | --- | --- |
| Wi-Fiが保存されていない、キーかノブ | Wi-Fiの設定へ（ラジオの画面に行けない） | **ラジオの画面へ**。Macの中継が答えると、すぐにUSBで再生 |
| Wi-Fiが保存されていない、W | Wi-Fiの設定へ | 同じ |
| Wi-Fiが保存されている、キーかノブ | Wi-Fiでラジオ | **同じ** |
| Wi-Fiが保存されている、W | Wi-Fiの設定へ | **同じ** |
| **U**（どちらの場合も） | （なし） | **「今回はUSBだけ」**：ラジオの画面へ。この起動の間はWi-Fiにつなごうとしない |
| Wi-Fiの設定からEscで戻る | 保存あり：ラジオへ／なし：START画面へ | 同じ（オプションdは不採用） |

### 1.1 Uキー（USBだけで起動）の決まり

- その起動の間は、Wi-Fiの接続を試みない（Wi-Fiの処理を止める。電波も使わない）
- 保存したWi-Fiは**消さない**。次に普通に起動すると、今までどおり使われる
- その起動の途中で `W`（Wi-Fiの設定）を開くと、「USBだけ」は終わる（利用者がWi-Fiを使うと決めたため）。設定から戻ると、U2a3と同じくWi-Fiにつなぐ
- 中継が答えるまでは「WAITING FOR WI-FI/USB」で待つ。選局、音量、`L`、`W` などの操作はすべて使える

### 1.2 画面の表示

**START画面（Wi-Fiが保存されていない）**

```text
RADIO DAYS                           U2a4
INTERNET RADIO  test build
Wi-Fi : not set
Saved : none
U135  : found
USB   : waiting for the Mac relay        ← 中継が答えると「USB   : relay found」（緑）
Press a key or the knob for USB radio
W: Wi-Fi setup
```

**START画面（Wi-Fiが保存されている）**

```text
Wi-Fi : （保存したネットワーク名）
Saved : ok
U135  : found
USB   : waiting for the Mac relay
Press a key or the knob to start          ← U2a3と同じ
W: Wi-Fi setup   U: USB only               ← 「U: USB only」を追加
```

**ラジオの画面の最下行（Wi-Fiの欄）**

| 場合 | U2a3 | U2a4 |
| --- | --- | --- |
| Wi-Fiが保存されていない | `WiFi retry`（誤解をまねく） | `WiFi unset` |
| Uで起動した | ― | `WiFi off` |
| 中継がつながっている | `USB relay …` | 同じ |
| そのほか（保存あり） | `WiFi -60dBm`／`joining`／`retry` | 同じ |

### 1.3 利用者への説明（案）

- **Wi-Fiなしで聴く**：RADIO DAYSを起動し、Macで `RadioDaysRelay_U2a2.command` を起動する。START画面でキーかノブを押す（Wi-Fiを保存している場合は `U`）。中継が見つかると再生が始まる
- **Wi-Fiで聴く**：START画面で `W` を押してWi-Fiを設定する。次からは、キーかノブで始まる

## 2. 変更したところ（U2a3との差分）

| ファイル | 内容 |
| --- | --- |
| `RadioDays_U2a4.ino` | 冒頭の説明、`BUILD_TAG`（"U2a4"）、導入の注意のファイル名、`usbOnly` の変数、`wifiService()` の最初の1行（USBだけのときは何もしない）、`drawStartUsb()`（新規）と `drawStart()` の表示、`wifiLabel()`（新規。最下行の言葉）、`startScan()` の1行（USBだけを終える）、`startUsbOnly()`（新規）、`handleStart()`、`loop()` の2行（START画面のUSBの行の描き直し）。**これ以外はU2a3と同じ**（`diff` で確認。UFOの部分も同じ） |
| `radiodays_link.h`、`radiodays_types.h`、`stations.h`、`globe_land.h`、`AudioOutputM5Speaker.h` | **U2a3と同じ**（`cmp` で確認） |

USBのやり取り、再生、選局、音量、Location、UFO、Wi-Fiの設定の画面は変えていない。Wi-FiとUSBの自動切り替えは作っていない。

## 3. ビルド

| 項目 | U2a3 | U2a4 |
| --- | --- | --- |
| アプリの大きさ | 1,361,904 B | **1,362,304 B**（+400 B） |
| 0x150000までの残り | 14,352 B | **13,952 B** |
| Launcherが作る領域 | 0x150000 | 0x150000（変わらない） |
| 静的RAM | 59,004 B | 59,004 B |
| SHA-256 | `8523189a…` | `ecf5be98cb2fff684280b682f17438726816da36754973258e7357625a7cf608` |
| 警告 | ― | 自分のコードの警告0件 |

## 4. 試験

### 4.1 START画面のホスト試験（新規：`tools/start-test/`）

```bash
sh tools/start-test/run_start_test.sh
```

スケッチの関数（`handleStart`・`startRadio`・`startUsbOnly`・`startScan`・`leaveSetup`・`wifiConnectActive`・`wifiService`・`drawStart`・`drawStartUsb`・`wifiLabel` など）を名前で切り出して、Macでそのまま動かす。Wi-Fiと画面は、呼ばれたことを記録する代わりの部品。

**結果：25項目すべて合格。**

| 分類 | 確かめたこと |
| --- | --- |
| **Wi-Fiが保存されていない**（作者の本体では試せない経路） | START画面の表示（`Wi-Fi : not set`、`Press a key or the knob for USB radio`、`W: Wi-Fi setup` だけ、USBの行）／キー・ノブ・Enterでラジオの画面へ、Wi-Fiの接続も検索もしない／`W` で設定へ（U2a3と同じ）／`U` でラジオへ／ラジオの画面で2分たってもWi-Fiの接続を試みない／最下行は `WiFi unset`／設定からEscでSTART画面へ（変えていない） |
| **Wi-Fiが保存されている** | START画面の案内は変わらない、`U: USB only` が加わる／キーでラジオとWi-Fi（U2a3と同じ）／`W` で設定へ（同じ）／`U` でラジオ、3分たってもWi-Fiを試みない、最下行は `WiFi off`、保存したネットワークは残る／`W` を開くと「USBだけ」が終わり、設定から戻るとWi-Fiにつなぐ（U2a3と同じ）／最下行の言葉は保存ありの場合U2a3と同じ |
| **START画面のUSBの行** | 中継が答えると `relay found`、いなくなると `waiting` に戻る |

**試験が正しく働くことの確認**：`handleStart()` だけをU2a3の作り（保存がなければWi-Fiの設定へ）に戻すと、3項目（キー・ノブ・Enter）が不合格になった。

**このホスト試験でわかること・わからないこと**

- わかる：Wi-Fiが保存されていないときに、どの画面へ進み、Wi-Fiを試みるかどうか（スケッチのコードそのもの）
- わからない：実機の画面の見た目、実機でのキーとノブ、実機の再生
- ラジオの画面に入った後の再生は、Wi-Fiの保存の有無に関係しないコード（中継が答えたかどうかで決まる）。この部分はU2a3と同じで、U2a2・U2a3の実機試験とシミュレーションで確かめている
- **Uキーの実機試験だけで、Wi-Fiが保存されていない場合を実証したとは扱わない**（作者の指示）。保存されていない場合の画面の流れは、このホスト試験だけで確認している

### 4.2 回帰試験

U2a4のリンクの処理（`radiodays_link.h`）はU2a2・U2a3とまったく同じなので、U2a2の `sim2` をそのまま実行した。UFOの部分もU2a3と同じ（比較で確認）なので、`tools/ufo-test` も実行した。

| 試験 | 結果 |
| --- | --- |
| `sh tools/usb-relay/sim2/run_sim.sh` | **41項目すべて合格** |
| `sh tools/usb-relay/sim2/run_sim.sh --negative`（確認用） | あふれを検出（テストが正しく働く） |
| `sh tools/ufo-test/run_ufo_test.sh` | U2a3と同じ結果（地球儀の上の点0、UFOは常に147/147、浮遊±5／±3、交信26回、描き直し毎秒8.2回） |
| ビルド | 0x150000以内、警告0件 |

## 5. 実機で確かめる手順（導入は作者の判断で）

### 準備

1. PManで、空き（FREE）が0x150000以上あることを確かめる
2. Launcherで `firmware/RadioDays_U2a4/RadioDays_U2a4.ino.bin` を入れる（「Need …」が出たら「Cancel」）。START画面の右上が「U2a4」
3. Macでは `RadioDaysRelay_U2a2.command` を使う

### 手順

| # | 場面 | 期待すること |
| --- | --- | --- |
| 1 | START画面 | `USB   : waiting for the Mac relay`。Macの中継を起動すると `USB   : relay found`（緑）に変わる。案内の最後の行が `W: Wi-Fi setup   U: USB only` |
| 2 | 職場：`U` を押す | ラジオの画面。最下行は中継が見つかると `USB relay …`。USBで再生する。シリアルの記録に `[WIFI] connecting` が出ない |
| 3 | 職場：`U` の後、中継を止める | 最下行が `WiFi off …` になり、`[WIFI] connecting` は出ない。中継を起動し直すと再生が戻る |
| 4 | `U` の後、`W` を押す | Wi-Fiの設定の画面（U2a3と同じ）。Escで戻ると、保存したWi-Fiにつなごうとする |
| 5 | 本体を再起動し、普通にキーを押す（家） | U2a3と同じく、保存したWi-Fiにつないで再生する。`U` の影響は残らない |
| 6 | 普通にキーを押す（職場、中継あり） | U2a3と同じ（Wi-Fiの試みは中継がつながると止まる。USBで再生） |
| 7 | 選局、音量、MANUALのノイズ、Location、UFO | U2a3と同じ |

**Wi-Fiが保存されていない場合**は、作者の本体では試さない（保存したWi-Fiは消さない）。この場合はホスト試験（第4.1節）だけで確認している。

## 6. 未確認事項

| 項目 | 内容 |
| --- | --- |
| U2a4の実機での動作 | 未確認（第5節） |
| Wi-Fiが保存されていない本体での動作 | 実機では未確認。ホスト試験で画面の流れを確認した |
| START画面の新しい行の見た目 | 実機で未確認 |
