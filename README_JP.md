# RADIO DAYS

**21st Century Radio Magic**  
*Turn the dial. Rediscover your world.*

**[English](README.md) | 日本語**

インターネットが当たり前になる、はるか昔。**ラジオで世界は繋がっていた。**

ダイヤルを回すと、ザーッという雑音の向こうから、知らない街の放送が聞こえてくる。周波数を合わせ、少しずつ音がはっきりしてくる瞬間。何が聞こえるかわからないからこそ、ダイヤルを回すのが楽しかった。

1980年代、BCLラジオやFM放送に夢中になった少年たちがいた。SONY ICF-2001のようなラジオに憧れ、FM STATIONをめくりながら、まだ見ぬ世界に思いを馳せていた時代。

あれから40年以上。インターネットのおかげで、世界中のラジオ放送を簡単に聴けるようになった。けれど、放送局を検索してクリックするだけでは、あの頃のダイヤルを回す楽しさは味わえない。

そんなある日、自作の **[Cardputer ADV Media Jog](https://github.com/kariagepompadour/Cardputer-ADV-Media-Jog)** のダイヤルを回していた私は、ふと思いついた。

**「このダイヤルで、ラジオのチューニングができたら面白いんじゃないか？」**

かつて夢中になったラジオと、今、自分の手元にある小さなコンピューター。その二つが、ひとつにつながった瞬間だった。

それが **RADIO DAYS** の始まりです。

昔のラジオをそのまま復元するのではなく、20世紀のラジオ少年が夢見た未来のラジオを、21世紀の技術で作る。ダイヤルを回して、世界の放送と偶然出会うための、小さなインターネットラジオです。

作品の背景と設計思想は [docs/concept.md](docs/concept.md) に記録しています。

![RADIO DAYSの実機。緑色の3Dプリント台座にCardputer ADVと大型ダイヤルを載せ、Shonan Beach FMを再生中](docs/images/RADIODAYS_1.jpeg)

## RADIO DAYSでできること

- **ダイヤルで探す**：ロータリーエンコーダーで局を選ぶ。AUTO／MANUALの選局と、局間の雑音も体験の一部。
- **世界を巡る**：地球儀から7つのLocationを選択。標準で42局を収録（作者による選定で、人気ランキングではありません）。
- **レトロな画面**：アイボリーの4段ダイヤル、固定指針、局の目印、LEDレベルメーター、交互に点灯する2つの電波塔。
- **空と時刻**：Locationの代表都市の時刻、昼夜の空、太陽と月などを表示。WORLDの時計は日本時間。
- **2つの聴き方**：Cardputer自身の2.4 GHz Wi-Fi（パソコン不要）、またはMacかWindows PCがストリームを受信し、MP3のデータをUSBでCardputerへ送る中継方式。
- **自分好みに改造**：放送局とLocationはソースから変更可能（再ビルドが必要）。

![U10の実機画面。FM Setagayaを再生中。アイボリーの4段ダイヤルと電波塔](docs/images/RADIODAYS_3.jpeg)

![USAのLocation選択画面。地球儀とNEW YORK、6局の表示](docs/images/RADIODAYS_4.jpeg)

**まずはウェブ版で試せます。[ウェブデモを開く →](https://kariagepompadour.github.io/RADIODAYS/radiodays-web.html)** ブラウザで動きます。実機不要。◀ ▶で選局、TUNING / VOLUME ボタンで◀ ▶の操作を音量調整に切り替え、PLAY／STOP、LOCATIONで操作します（PCではキーボードも使えます）。ウェブ版では、ブラウザのセキュリティ上の制約により、一部の放送局を再生できない場合があります（実機には影響しません）。

## 用意するもの

- **M5Stack Cardputer ADV**。U135がなくてもキーボードで操作できます。
- **M5Stack Unit Encoder（U135、任意）**：ダイヤル操作に使用。Cardputer ADVのGroveポートへ接続。追加のはんだ付けなどは不要です。なくても、本体のキーボードだけで選局・音量・ミュート・地域選択のすべてができます。
- **音声を受信する環境**：対応する2.4 GHz Wi-Fi（Cardputerだけで聴けます）、またはUSBケーブルで接続したMacかWindows PC（中継スクリプトを使用）。
- **3Dプリントの台座・ノブ（任意）**：後述。なくても動作します。

## U10の導入

**現在公開するファームウェアはU10のみです。** 旧版U1〜U9のファームウェアは開発記録として作者のローカルに保存しており、このリポジトリには含めていません。[`firmware/RadioDays_U10/`](firmware/RadioDays_U10/) には導入方法ごとに2つのファイルがあります。**どちらか一方の方法を選んでください。2つのファイルは互換ではありません。**

| | A. M5Burner（全体を書き込む） | B. Launcher（他のアプリを残す） |
| --- | --- | --- |
| ファイル | [`RadioDays_U10.ino.merged.bin`](firmware/RadioDays_U10/RadioDays_U10.ino.merged.bin) | [`RadioDays_U10.ino.bin`](firmware/RadioDays_U10/RadioDays_U10.ino.bin)（app-only） |
| 内容 | Cardputerのフラッシュ全体をRADIO DAYSに置き換える | LauncherのアプリのひとつとしてRADIO DAYSを追加する |
| Launcherと他のアプリ | **上書きされる** | 残る |
| サイズ／SHA-256 | 4,194,304 B／`940735c5…92303` | 1,371,360 B／`f6c4d40d…70e61` |

SHA-256の全体は [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) にあります。app-onlyのファイルは [`SHA256SUMS.txt`](firmware/RadioDays_U10/SHA256SUMS.txt) にも記載しています。

### A. M5Burnerで導入する

[M5Burner](https://docs.m5stack.com/en/download) はM5Stackの書き込みツールです。完全なイメージ（ブートローダー、パーティション表、アプリ）である `RadioDays_U10.ino.merged.bin` を、アドレス0x0から書き込みます。

> **注意：** 全体を書き込むと、**M5Stack Launcherと、Launcherで導入したすべてのアプリが消えます**。残したい場合はBの方法を使ってください。

1. Cardputer ADVをUSBデータケーブルでパソコンに接続します。
2. M5BurnerでCardputerの分類を選び、**RADIO DAYS** を検索します。
3. ダウンロードし、Cardputerのシリアルポートへ書き込みます（Burn）。終わったらCardputerを再起動すると、RADIO DAYSが直接起動し、START画面の右上に `U10` と表示されます。

保存済みのWi-FiとLocationは、全体の書き込みで消える場合があります。必要なら設定し直してください。

作者はRADIO DAYSをM5Burnerに登録し、merged.binをM5BurnerからUSBで書き込んで、RADIO DAYSが起動・動作することを確認しています。RADIO DAYS U10は**2026-10-22にM5Burnerで公開済み**です。見つからない場合、このリポジトリの `RadioDays_U10.ino.merged.bin` は、アドレス **0x0** に書き込むESP32-S3の標準的な全体イメージです。M5Burner以外のツールでの書き込みは、作者は試していません。

ビルド設定は *Flash Size: 4MB*、*Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)* です。すべての設定は [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) を参照してください。

### B. Launcherでアプリとして導入する

有志が開発している [M5Stack Launcher](https://github.com/bmorcelli/Launcher)（**Launcher 2.9.1**で確認）を使い、他のアプリを残したまま導入します。

1. ファイルを確認：`firmware/RadioDays_U10/` で `shasum -a 256 -c SHA256SUMS.txt` を実行し、`OK` になること。
2. Launcherのパーティション管理（**PMan**）で、**FREEが1,376,256 B（0x150000）以上**あることを確認します。
3. **app-only** の `RadioDays_U10.ino.bin` を、SDカードから、またはLauncherのWi-Fiアップロード（WebUI）で導入します。
4. **「Need …」で始まる画面が出たら、必ず「Cancel」を選んでください。** もう一方の選択肢では既存のアプリが上書きされることがあります。
5. Launcherが再起動してRADIO DAYSが起動します。Launcherに戻るには、Cardputerを再起動し、Launcherの起動画面でボタンを押します。

**Launcherから `RadioDays_U10.ino.merged.bin` を導入しないでください。** Launcherを残したい場合は、Arduino IDEの「書き込み」やesptoolも使わないでください（フラッシュに直接書き込むため）。

## 聴く

### Wi-Fiで聴く（Cardputerだけ）

Wi-Fiでの再生に必要なのは、**Cardputer ADVと2.4 GHzのネットワークだけ**です。パソコンや中継は要りません。

1. START画面で `W` を押し、Wi-Fiネットワークを選んでパスワードを入力します。
2. 接続に成功したネットワークは最大3件まで保存されます。次回以降は、最後に接続したネットワークから順に試します（各12秒、合計最大30秒）。
3. `W` の一覧画面で `S` を押すと保存済みネットワークを表示し、接続や削除ができます。

Wi-Fiは**2.4 GHz**に対応します。パスワード付き（WPA2など）と公開ネットワークに対応しますが、**SSIDを隠したネットワークには対応しません**。

### Mac経由のUSB中継で聴く

CardputerをWi-Fiにつなげない場所で使います。Macが自分のネットワーク接続でストリームを受信し、MP3のデータをUSBでCardputerへ送ります。中継スクリプト **[`tools/usb-relay/RadioDaysRelay_U9.command`](tools/usb-relay/RadioDaysRelay_U9.command)** が必要です。

**スクリプトの入手**（どちらか）：

- リポジトリ全体をダウンロードし（GitHubの **Code → Download ZIP**、または `git clone`）、`tools/usb-relay/` を開く。
- スクリプトだけをダウンロードする：GitHubでファイルを開いて **Download raw file** を選ぶか、次を実行する。

  ```bash
  curl -LO https://raw.githubusercontent.com/kariagepompadour/RADIODAYS/main/tools/usb-relay/RadioDaysRelay_U9.command
  ```

**準備**：CardputerをUSB**データ**ケーブルでMacにつなぎ、RADIO DAYSを起動しておきます。Cardputerのシリアルポートを使う他のプログラム（Arduino IDEのシリアルモニタなど）は閉じてください。

**中継の起動**：

1. スクリプトのあるフォルダでターミナルを開き、次を実行します。

   ```bash
   perl RadioDaysRelay_U9.command
   ```

   ダウンロードしたファイルもこのまま使えます。Finderでダブルクリックして起動することもできます。ファイル単体でダウンロードした場合は、先に `chmod +x RadioDaysRelay_U9.command` が必要なことがあります。macOSがダウンロードしたファイルを開けない場合は、上の `perl` のコマンドを使ってください。
2. 中継はCardputer（`/dev/cu.usbmodem…`）を自動で見つけます。USBシリアルポートが複数あるときは、Cardputerのポートの番号を入力するよう求められます。
3. CardputerのSTART画面で `U` を押します（Wi-Fi設定を保存していなければ、どのキーやノブでも開始できます）。`U` で開始した間はWi-Fiを使用しません。
4. 中継が応答するまでは `USB RELAY REQUIRED` / `Start the relay on your computer.` と表示されます。
5. 中継を止めるには、Control-Cを押すか、ターミナルのウィンドウを閉じます。

**ファイル名にU9とありますが、U10でも使用する正式な中継スクリプトです**（スクリプト内の説明もU9のままです。U10は通信の方式も局もU9と同じです）。macOSに標準搭載のPerl・curl・caffeinateを使用し、動作中はMacをスリープさせず、標準の局のホストだけを許可します。**Linuxには対応しておらず、動作確認もしていません。** Macのインターネット共有など、ネットワーク設定の変更は不要です。

### Windows経由のUSB中継で聴く

Windows版の中継は、Mac版と同じ働きをします（許可するホストと安全の決まりも同じ）。**Windowsに標準で入っているWindows PowerShell**で動くので、インストールは不要です。作者が **Windows 11** で動作を確認しています。

[`tools/usb-relay/RadioDaysRelay_U9_Windows.cmd`](tools/usb-relay/RadioDaysRelay_U9_Windows.cmd) と [`tools/usb-relay/RadioDaysRelay_U9_Windows.ps1`](tools/usb-relay/RadioDaysRelay_U9_Windows.ps1) の**2つのファイルを同じフォルダ**に置いて使います。GitHubの **Code → Download ZIP**（または `git clone`）で入手するか、それぞれを **Download raw file** でダウンロードしてください。

1. CardputerをUSB**データ**ケーブルでPCにつなぎ、RADIO DAYSを起動します。同じCOMポートを使う他のプログラム（Arduino IDEのシリアルモニタなど）は閉じてください。
2. `RadioDaysRelay_U9_Windows.cmd` をダブルクリックします。ウィンドウが開き、中継がCardputerのCOMポートを自動で見つけます。見つからない場合は、コマンドプロンプトから `RadioDaysRelay_U9_Windows.cmd --port COM5` のように起動します（番号はデバイスマネージャーの「ポート (COM と LPT)」で確認）。
3. CardputerのSTART画面で、Mac版と同じく `U` を押します。
4. 中継を止めるには、Control-Cを押すか、ウィンドウを閉じます。

`.cmd` は、その1回の起動に限って `-ExecutionPolicy Bypass` でスクリプトを実行します。PCの設定は変えません。会社のPCなど、規則でスクリプトの実行が禁止されている場合は動きません。その場所の規則に従ってください。詳細は [docs/usb-relay-windows.md](docs/usb-relay-windows.md)。

### 操作

**ラジオ画面**（すべてキーボードだけで操作できます。◀ ▶ ↑ ↓は本体の `,` `/` `;` `.` キーに印刷された矢印です）

| 操作 | エンコーダー（U135） | キーボード |
| --- | --- | --- |
| 選局 | ノブを回す。**AUTO**：次／前の局までダイヤルが流れる。**MANUAL**：ダイヤルが少しずつ動き、局の間では雑音 | `,`／`/`（`P`／`N` も可）：1目盛り下げる／上げる |
| AUTO／MANUALの切替 | ― | `A`（起動時は常にAUTO） |
| 音量 | ノブを押しながら回す | `;` 上げる／`.` 下げる |
| ミュートの入／切 | 短く押す | `M` |
| Locationの選択 | ― | `L` で地球儀の画面へ。ノブ（または `,` `/`）で選び、ノブを押すか `ok` で決定、`esc` で戻る |
| Wi-Fiの設定 | ― | `W` |
| 診断の行の表示／非表示 | ― | `I` |

**START画面**：`W` Wi-Fiの設定、`U` USB中継だけで聴く、それ以外のキーかノブを押すとラジオを開始。

### 局・Location・時計

- 選択したLocationは保存されます。Locationを保存していなければ、初回はWORLDのRadio Paradiseから始まります。U10に存在しない古いLocation設定が残っている場合もWORLDに戻ります。
- 選局中の `3/7` のような表示は、そのLocation内の局の順番です（人気順位ではありません）。AUTO動作中やMANUALで局間にいるときは表示しません。
- 時計は、Wi-Fiのときはインターネット（NTP）から、USB中継のときはMacの時計（UTC）から合わせます。時刻を受け取るまでは `--:--` と表示します。

### 標準の放送局

| Location | 局数 |
| --- | ---: |
| JAPAN | 5 |
| USA | 6 |
| UK | 7 |
| FRANCE | 7 |
| WORLD | 6 |
| GERMANY | 6 |
| ITALY | 5 |
| **合計** | **42** |

収録局の定義は [`stations.h`](firmware/RadioDays_U10/stations.h)、選定経緯は [docs/u9-implementation-plan.md](docs/u9-implementation-plan.md) 第2節にあります。

## 動作状況と技術的な制約

**U10（2026-10-22）**：U9にアイボリーの文字盤、くぼんだ縁取り、2つの電波塔（PLAYING中は左右交互に0.8秒ずつ点灯）を追加。

| 項目 | 状況 |
| --- | --- |
| ビルド、Macでのホスト試験 | 完了（[docs/usb-u10.md](docs/usb-u10.md)） |
| Cardputer ADVでのUSB中継による再生と画面デザイン | **作者が実機で確認済み** |
| M5BurnerによるUSBでのmerged.binの書き込み、起動、動作 | **作者が実機で確認済み** |
| Launcherでのapp-only `.bin` の導入 | 旧版と同じ手順（Stage 2-1まで実機で確認）。U10としての記録はなし |
| Windows版のUSB中継（Windows 11）：COMポートの自動検出、起動時にCardputerがリセットされない、複数局の切り替え、時計、ケーブルの抜き差し後の再接続 | **作者が確認済み** |
| U10でのWi-Fiによる再生 | **確認の記録なし** |
| 長時間の安定性（Windows版USB中継） | **作者が30分以上途切れなく再生できることを確認済み** |

`RadioDays_U10.ino` 冒頭のコメントは「Not yet run on hardware」のままです。U10のソースは保存版として変更しない（`SHA256SUMS.txt` の対象）ため、現在の状況はこのREADMEと [docs/usb-u10.md](docs/usb-u10.md) に記録しています。同じコメントの操作一覧も古い段階のものなので、上の操作表を使ってください。

- 再生できるのは **`http://` のMP3ストリームのみ**。HTTPS、HLS、AAC、`.m3u` / `.pls` プレイリストには対応しません。
- 局側のデータ供給が一時的に不足すると音切れが起こり、自動回復する場合があります。問題は解消していません。
- 標準42局のURLはMacからの応答確認を中心に調査したもので、**Cardputer実機での再生は大半が未確認**です。配信URLは予告なく変更される場合があります。
- RNZを除き、URLが局の公式公開情報に基づくものか未確認です。**各局の利用条件も未確認**です。技術的に再生できることと、配信利用が許可されていることは別です（[docs/usb-u4.md](docs/usb-u4.md)、[docs/usb-u9.md](docs/usb-u9.md)）。
- 職場などでUSB中継を利用する際は、USB機器の接続やストリーミングについて、その場所の規則に従ってください。
- アプリ領域の上限は **0x150000（1,376,256 B）**。U10は **1,371,360 B**、残り **4,896 B** です。

## ソースからのビルド

| 項目 | 版・設定 |
| --- | --- |
| Arduino IDEのボードパッケージ | **M5Stack `m5stack:esp32` 3.3.9**（ESP-IDF v5.5.4）、ボード **M5Cardputer** |
| ライブラリ | **ESP8266Audio 2.4.1**、**M5Cardputer 1.1.1**、**M5Unified 0.2.25**、**M5GFX 0.2.32** |
| USB Mode／USB CDC On Boot | **Hardware CDC and JTAG**／**Enabled**（違う設定ではスケッチが `#error` で止まる） |
| app-only `.ino.bin`（Launcher用） | Flash Size **8MB**、Partition Scheme **8M with spiffs (3MB APP/1.5MB SPIFFS)**（容量の確認用。実際の領域はLauncherが作る）。「スケッチ → コンパイルしたバイナリを出力」を使う |
| 全体イメージ `.ino.merged.bin`（M5Burner用） | Flash Size **4MB**、Partition Scheme **Huge APP (3MB No OTA/1MB SPIFFS)** |

配布しているバイナリは上の版で作りました。新しい版のライブラリでも動く可能性はありますが試しておらず、サイズが変わることがあります。ビルドしたマシンのファイルの場所がバイナリに入るため、作り直したバイナリが1バイト単位で一致するとは限りません。Arduino IDEはスケッチのフォルダの中に `build/` フォルダを作りますが、`.gitignore` で除外しています。

## 放送局を追加・変更する

RADIO DAYSの標準構成は作者の選定です。好きな局や地域へ変更できますが、**ソースの編集と再ビルドが必要**です（[ソースからのビルド](#ソースからのビルド)）。背景の調査は [docs/station-customization-review.md](docs/station-customization-review.md) にあります（U8・19 Location時点の資料。U10の手順は以下のとおり）。

1. **ストリームを探す**：`http://` のMP3だけを使用し、192 kbps以下を目安にします（作者の方針）。`https://`、HLS（`.m3u8`）、AAC、プレイリスト（`.m3u` / `.pls`）は非対応。局の公式サイトを優先し、利用条件を確認してください。
2. **[`stations.h`](firmware/RadioDays_U10/stations.h) を編集**：1行1局で `{"局名", "都市", "http://…", freq},`。局名はASCIIで19文字まで（日本語・アクセント文字不可）。都市名は記録用で画面には表示されません。
3. **周波数を再計算**：局数が変わったら、そのLocationのすべての `freq` を変更します。例：`python3 tools/freq-calc/freq_calc.py 6` → `810, 830, 850, 870, 890, 910`。配列順がダイヤルの並びと `n/N` 表示の順番です。
4. **Locationを追加**：WORLDの後ろに定義を追加します（大文字ASCIIの名前7文字以内、都市、緯度経度、`true`、`800`、`RD_DIAL_MAX(表)`、表、`RD_COUNT(表)`、タイムゾーン）。最初の5つ（JAPAN、USA、UK、FRANCE、WORLD）の名前と位置は変更しないでください。末尾の `LOCATION_COUNT == 7` も更新します。
5. **ビルド**：周波数、局名長、Location数の不整合はビルドエラーになります。バイナリが1,376,256 Bを超えないようにしてください。
6. **USB中継の許可ホストを更新**：`tools/usb-relay/allowed_hosts.txt` を置く場合、1行ごとに `host:port`、必要なら `redirect *.example.com:80` と記述します。このファイルが存在すると内蔵の許可一覧は使われないため、既存局のホストも含めてください。Mac版・Windows版とも、自分と同じフォルダのこのファイルを読みます。確認コマンドは `perl tools/usb-relay/RadioDaysRelay_U9.command --check-url <URL>` と `--check-redirect <URL>`（Windowsでは `RadioDaysRelay_U9_Windows.cmd --check-url <URL>`）。IPアドレス形式のURLと300文字超のURLは拒否されます。
7. **改造バイナリを配布する場合**：下記のGPLライセンス条件を確認してください。

## 3Dプリント部品（任意）

![斜めから見たRADIO DAYSの実機。緑色の台座と大型ダイヤル](docs/images/RADIODAYS_2.jpeg)

台座とダイヤルは、先に作った **[Cardputer ADV Media Jog](https://github.com/kariagepompadour/Cardputer-ADV-Media-Jog)** のものを**そのまま流用**しています。STLと3MFは同じ形状で、設計者が印刷して実機で確かめています。ファイルはMedia Jogの `hardware/` フォルダにあり、RADIO DAYS側には複製していません（MIT License）。

- `hardware/MediaJog_Base_A.stl`：平置き用
- `hardware/MediaJog_Base_B.stl`：15°傾斜付き
- `hardware/MediaJog_Dial.stl`：ダイヤル
- `hardware/Cardputer-ADV-Media-Jog.3mf`：Bambu Studio用

## 開発資料

- [ウェブデモ](https://kariagepompadour.github.io/RADIODAYS/radiodays-web.html)（[ソース](docs/radiodays-web.html)） — Cardputerの画面をブラウザで試せるインタラクティブなデモ。スマートフォン用の操作ボタン付き（実機不要）
- [コンセプト](docs/concept.md) — 作品の背景と設計思想
- [設計書](docs/design.md)・[開発計画](docs/development-plan.md)
- [U10の実装・確認記録](docs/usb-u10.md)・[U9の選局とUSB中継](docs/usb-u9.md)
- [放送局カスタマイズの背景調査](docs/station-customization-review.md)（U8時点。U10の手順は上記）
- [第三者ライセンスとバイナリのライセンス](THIRD_PARTY_NOTICES.md)
- [変更履歴](docs/CHANGELOG.md)・[公開準備計画](docs/release-plan.md)

<details>
<summary>開発・調査資料の全一覧（クリックで展開）</summary>

以下は開発過程の記録です。**過去版の「実機未確認」などの表記は当時の記録であり、U10の現在の確認状況を示すものではありません。**

| 資料 | 内容 |
| --- | --- |
| [docs/concept.md](docs/concept.md) | **コンセプト**（作品の背景、キャッチコピー、設計の原則） |
| [docs/design.md](docs/design.md) | 設計書（調査結果、方式、画面・操作、Launcher、NVSの合格条件） |
| [docs/development-plan.md](docs/development-plan.md) | 開発計画（Stage 1〜4、各段階の確認と判定基準） |
| [docs/stage-3-3.md](docs/stage-3-3.md) | **Stage 3-3：太陽と月、昼夜の空、UFO** |
| [docs/usb-u10.md](docs/usb-u10.md) | **U10：アイボリーの文字盤、縁取り、電波塔。USB中継での動作と画面デザイン、M5Burnerでのmerged.binの書き込みを作者が実機で確認（Wi-Fiでの再生と長時間の安定性は未確認）** |
| [docs/dial-face-tower-proposal.md](docs/dial-face-tower-proposal.md) | U10の設計（A案・B案、電波塔、プレビュー。B案を採用） |
| [docs/usb-u9.md](docs/usb-u9.md) | **U9：標準の7 Location・42局、選局番号 n/N、U9の中継。実機は未確認** |
| [docs/u9-implementation-plan.md](docs/u9-implementation-plan.md) | U9の実装計画（局の表、表示、容量、中継、試験の項目） |
| [docs/location-selection-review.md](docs/location-selection-review.md) | 標準の収録国の再検討（6か国＋WORLD） |
| [docs/uk-station-selection-review.md](docs/uk-station-selection-review.md)、[japan](docs/japan-station-selection-review.md)、[usa](docs/usa-station-selection-review.md)、[france](docs/france-station-selection-review.md)、[germany](docs/germany-station-selection-review.md)、[italy](docs/italy-station-selection-review.md) | 各国の選局の調査（聴取率、配信、再生、利用条件） |
| [docs/station-customization-review.md](docs/station-customization-review.md) | 利用者による放送局の追加・変更の背景調査（U8時点。U10の手順はREADME） |
| [docs/station-lineup-survey-plan.md](docs/station-lineup-survey-plan.md)、[docs/ranking-dial-feasibility.md](docs/ranking-dial-feasibility.md)、[docs/cardputer-adv-radiko-review.md](docs/cardputer-adv-radiko-review.md) | 選局の方針と調査の計画、順位の調査（参考）、Cardputer-Adv-Radikoの技術調査 |
| [docs/usb-u8.md](docs/usb-u8.md) | **U8：AUTOでスケールが次の局まで流れる。実機は未確認** |
| [docs/japan-stations-review.md](docs/japan-stations-review.md) | JAPANの7局の調査と評価（局は未変更） |
| [docs/japan-candidates-review.md](docs/japan-candidates-review.md) | JAPANの代わりの候補の調査と比較（局は未変更） |
| [docs/japan-popular-stations-review.md](docs/japan-popular-stations-review.md) | 知名度を優先した日本の主要局と、radikoの利用方法の調査（局は未変更） |
| [docs/usb-u7.md](docs/usb-u7.md) | **U7：時計をLocationの現地時刻に。実機は未確認** |
| [docs/auto-glide-proposal.md](docs/auto-glide-proposal.md) | AUTOの選局アニメーションの調査・提案（U8で実装） |
| [docs/clock-local-time-proposal.md](docs/clock-local-time-proposal.md) | 時計の現地時刻化と昼夜判定の調査・提案（U7で実装） |
| [docs/usb-u6.md](docs/usb-u6.md) | **U6：等間隔4段・循環式・枠付きのダイヤル。実機は未確認** |
| [docs/usb-u5.md](docs/usb-u5.md) | **U5：固定指針・可動スケールのアナログダイヤル（数字なし）、最下行の修正。実機は未確認** |
| [docs/analog-dial-proposal.md](docs/analog-dial-proposal.md) | U5の設計（第1〜3部、承認済み） |
| [docs/usb-u4.md](docs/usb-u4.md) | **U4：昼の空、USB待機の案内、国別Location（19か所・85局）、人工衛星、Wi-Fiの3件保存。実機は未確認** |
| [docs/u4-proposal.md](docs/u4-proposal.md) | U4の設計（承認済み） |
| [docs/usb-u3.md](docs/usb-u3.md) | **U3：USBだけのときも時計・太陽・月（Macから時刻）。作者が正常動作を確認（保存版）** |
| [docs/usb-u3-time-design.md](docs/usb-u3-time-design.md) | U3の設計（承認済み） |
| [docs/usb-u2a4.md](docs/usb-u2a4.md) | **U2a4：Wi-FiなしでもUSB中継で聴ける、Uキー（実機は未確認）** |
| [docs/release-plan.md](docs/release-plan.md) | **完成・公開までの残作業（A：必須、B：推奨、C：公開後）** |
| [docs/usb-u2a3.md](docs/usb-u2a3.md) | **U2a3：UFOの浮遊と交信、プログラム容量の調査（実機試験合格）** |
| [docs/usb-u2a2.md](docs/usb-u2a2.md) | USB接続 U2a2：U2aの調査と修正（**実機試験済み**） |
| [docs/usb-u2a.md](docs/usb-u2a.md) | **USB接続 U2a（USB中継での選局）：実装、試験の結果、実機の手順（実機は未確認）** |
| [docs/usb-u2a-design.md](docs/usb-u2a-design.md) | USB接続 U2aの設計（第2版、承認済み） |
| [docs/usb-u2-design.md](docs/usb-u2-design.md) | USB接続 U2の設計案（決定前） |
| [docs/usb-u1.md](docs/usb-u1.md) | USB接続 U1（USBシリアル中継の試作。**実機で動作確認済み**）：手順と結果 |
| [docs/usb-relay-design.md](docs/usb-relay-design.md) | **USB接続方式の再検討（KariPomとの比較、USBシリアル中継の設計）** |
| [docs/usb-connection-research.md](docs/usb-connection-research.md) | Wi-FiのないときのUSB接続（第1回の調査） |
| [docs/stage-3.md](docs/stage-3.md) | Stage 3-1・3-2：WORLD RADIO（Location、地球儀）。操作とテスト項目 |
| [docs/stations.md](docs/stations.md) | **放送局の一覧と確認記録、周波数の割り当て（判断待ち）** |
| [docs/stage-2-5.md](docs/stage-2-5.md) | Stage 2-5：時計とムーンフェイス |
| [docs/stage-2-4.md](docs/stage-2-4.md) | Stage 2-4（実機で動作確認済み）：レベルメーター、放送局についての調査 |
| [docs/stage-2-3.md](docs/stage-2-3.md) | Stage 2-3：AUTO／MANUAL、ノイズ |
| [docs/stage-2-2.md](docs/stage-2-2.md) | Stage 2-2（実機で動作確認済み）：選局の仕組み |
| [docs/stage-2-1.md](docs/stage-2-1.md) | Stage 2-1 試作版（実機で動作確認済み）：導入手順、操作、確認項目 |
| [docs/stage-2-0-size.md](docs/stage-2-0-size.md) | Stage 2-0 容量の試作の結果 |
| [docs/CHANGELOG.md](docs/CHANGELOG.md) | 変更履歴 |
| `tools/make_globe_land.py` | 地球儀の地図を作るスクリプト |
| `tools/usb-relay/` | USB中継スクリプト（Mac：`RadioDaysRelay_U9.command`、Windows：`RadioDaysRelay_U9_Windows.cmd`／`.ps1`、`allowed_hosts.txt` で許可ホストを指定） |
| [docs/usb-relay-windows.md](docs/usb-relay-windows.md)、`tools/usb-relay-windows-test/` | Windows用のUSB中継（Mac版との違い、試験、Windows 11での確認） |
| `firmware/RadioDays_U10/`、`tools/u10-test/`、`tools/u10-preview/`、`tools/freq-calc/` | U10（`run_u10_test.sh`：文字盤・残像・電波塔の試験。`make_u10_preview.sh`：U10のコードで描いた画面の見本。`make_face_tower_preview_u10.py`：文字盤・電波塔の設計プレビュー。`freq_calc.py`：`freq` の計算）。**`tools/u10-test/` と `tools/u10-preview/` は公開していない旧版（`firmware/RadioDays_U9/`、`tools/u9-test/`、`tools/dial-preview/`）を使うため、作者の開発環境でのみ動きます** |

</details>

## ライセンス

- **RADIO DAYSのために書いたソース**（`.ino`、`.h`、スクリプト、ツール）：MIT License（[LICENSE](LICENSE)）。Copyright (c) 2026 kariagepompadour。
- **配布バイナリ**（`RadioDays_U10.ino.bin` と `RadioDays_U10.ino.merged.bin`）：GPL-3.0-or-laterのESP8266Audio 2.4.1（libmadを含む）を組み込んでいるため、**GPL-3.0-or-later**で配布します（[ライセンス本文](LICENSES/GPL-3.0-or-later.txt)）。対応するソースは、このリポジトリのファームウェアのソース、ESP8266Audioをそのまま収録した [`third-party/ESP8266Audio-2.4.1/`](third-party/ESP8266Audio-2.4.1/)、[ソースからのビルド](#ソースからのビルド) に記載したライブラリとビルド設定です。ビルドではArduinoのライブラリマネージャーで入れたESP8266Audioを使い、収録したコピーはそれと同一です。改造したバイナリを再配布する場合もGPLの条件に従ってください。
- **第三者のコンポーネントと告知**（M5Cardputer、M5Unified、M5GFX：MIT。Arduino core for ESP32：LGPL-2.1。ESP-IDF：Apache-2.0）：[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) を参照してください。
- 地球儀の地図データ：[Natural Earth](https://www.naturalearthdata.com/)（パブリックドメイン）。
- 太陽・月・UFOの顔は、同じ作者の [SOUND TERRARIUM](https://github.com/kariagepompadour/SOUND-TERRARIUM)（MIT）をもとに描いています。
- 放送の権利は各放送局にあります。このリポジトリに音声は含まれていません。各局の利用条件に従ってください。
