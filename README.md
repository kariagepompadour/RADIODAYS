# RADIO DAYS

Cardputer ADV＋U135で動く、BCLラジオ風のインターネットラジオアプリ。

**RADIO DAYS — 21st Century Radio Magic**
*Turn the dial. Rediscover your world.*

ダイヤルを回して、世界の放送と偶然出会う。検索して再生するだけのアプリにはしない。作品の背景は [docs/concept.md](docs/concept.md)。

![RADIO DAYSの実機全体写真。Cardputer ADVを緑色の3Dプリント台座に載せ、大型のJogダイヤルを接続した状態。画面にはShonan Beach FMを再生中のラジオ画面が表示されている](docs/images/RADIODAYS_1.jpeg)

- ハードウェア：M5Stack Cardputer ADV＋Unit Encoder（U135）。追加の電子工作なし
- 導入：Launcherから起動する独立アプリ（app-onlyの `.ino.bin`）
- [Cardputer ADV Media Jog](https://github.com/kariagepompadour/Cardputer-ADV-Media-Jog)を出発点として作った作品。SOUND TERRARIUMも参照しているが、どちらのソースも変更しない

## 現在の状態

**最新はU10（2026-10-22 実装。USB中継での動作と画面デザインを実機で確認済み。長時間安定性は未確認。U9に、アイボリーの文字盤と縁取り、2つの電波塔の点滅を加えた版）。** 旧版（U1〜U9）は開発記録として作者のローカルに保存しており、このリポジトリには含まない。

| 版 | Cardputerに入れるファイル | Macの中継 | 状態 |
| --- | --- | --- | --- |
| U10 | `firmware/RadioDays_U10/RadioDays_U10.ino.bin` | `tools/usb-relay/RadioDaysRelay_U9.command` | U9＋アイボリーの文字盤・くぼんだ縁取り・電波塔（PLAYINGで左右交互に点灯）。実装・ビルド・Macでの試験済み、**USB中継での動作と画面デザインを実機で確認済み。長時間安定性は未確認**（[docs/usb-u10.md](docs/usb-u10.md)） |

### 対応環境（技術的な事項）

| 項目 | 内容 |
| --- | --- |
| 本体 | M5Stack Cardputer ADV＋Unit Encoder（U135、Groveポート）。U135がなくてもキーボードで操作できる |
| 導入 | Launcherから、app-onlyの `.ino.bin` を入れる。アプリの大きさは0x150000（1,376,256 B）以内（U10：1,371,360 B、残り4,896 B） |
| ビルド | Arduino IDE、ボード M5Cardputer（m5stack:esp32 3.3.9）、USB Mode：Hardware CDC and JTAG、USB CDC On Boot：Enabled（スケッチの冒頭に設定の一覧） |
| 聴く方法 | ① Wi-Fi（2.4 GHz、WPA2などのパスワード、または公開。**非公開（SSIDを隠した）ネットワークには対応しない**）。② USB中継：USBケーブルでつないだMacが放送局を受信し、Cardputerへ送る |
| **USB中継のスクリプト** | **macOS専用**（macOSに入っているPerl・curl・caffeinateを使う `.command`）。**Windows・Linuxは対応しておらず、試してもいない**。Macのネットワークの設定は変えない |
| 放送局 | `http://` のMP3だけ（HTTPS、HLS、AACは再生できない）。U10は7 Location・42局（`firmware/RadioDays_U10/stations.h`） |
| 時計 | Wi-FiのときはNTP、USB中継のときはMacの時刻（UTC）。表示はLocationの代表都市の時刻（U7から。WORLDは日本時間） |

### 画面（U10）

![U10の実機液晶写真。FM Setagayaを再生中（PLAYING 2/5）、JAPANのWiFi接続、アイボリーの4段ダイヤルと電波塔を表示](docs/images/RADIODAYS_3.jpeg)

- チューニングメーターはアイボリーの文字盤に黒い目盛り、くぼんだ灰色の縁取り。局の印は濃いオレンジ
- 局名の上の左右に小さな電波塔。PLAYINGの間は電波が左右交互に点灯（0.8秒ずつ）、バッファー中・接続中は弱く、それ以外は消える
- 操作・局・中継はU9と同じ（中継は `RadioDaysRelay_U9.command`）

### 使い方（U10）

- **放送局**（作者が選んだ標準の構成。人気の順位ではない）：JAPAN 5、USA 6、UK 7、FRANCE 7、WORLD 6、GERMANY 6、ITALY 5。一覧は [docs/u9-implementation-plan.md](docs/u9-implementation-plan.md) 第2節
- **起動**：Locationを保存していなければWORLDのRadio Paradiseから始まる。保存したLocationがあればそこから（U10にない国を保存していた場合はWORLD）
- **選局番号**：ラジオ画面の状態の行の右（音量の左）に `3/7` のように出る。そのLocationのダイヤルで何番目の局かを示す（人気の順位ではない）。AUTOで動いている間と、MANUALで局の間にいるときは出ない
- **USB中継**：`RadioDaysRelay_U9.command` を使う
- ほかの操作は以下のとおり

### Wi-Fi・USB中継・Location

- **Wi-Fiで聴く**：START画面で `W` を押し、ネットワークを選んでパスワードを入れる。接続できたら保存される（**最大3件**）。次からはキーかノブで始まり、最後につながったネットワークから順に試す（1回12秒、全体で30秒まで）。`W` の一覧で `S` を押すと、保存した一覧（今すぐ接続、削除）
- **USB中継で聴く**：Macで `RadioDaysRelay_U9.command` を起動する（ダブルクリック）。START画面で `U`（Wi-Fiを保存していなければ、どのキーでもよい）。中継が見つかるまで、画面に `USB RELAY REQUIRED`／`Start the relay on your computer.` と出る。`U` で始めた間は、Wi-Fiを一切使わない
- **Location**：`L` で地球儀の画面。ノブで国を選び、押すと切り替わる（選んだLocationは保存される）

![U10の実機液晶写真。USAのLOCATION選択画面。地球儀にNEW YORKの位置が示され、WNYC・WFUVなど6 stationsと表示されている](docs/images/RADIODAYS_4.jpeg)
- 操作の一覧はスケッチの冒頭と [docs/stage-3.md](docs/stage-3.md)

### 放送局の追加・変更

RADIO DAYSの標準の局は作者が選んだもの。自分の国や好きな局に替えてかまわない（ビルドし直してLauncherから入れる）。詳しくは [docs/station-customization-review.md](docs/station-customization-review.md)。

1. **局を探す**：RADIO DAYSが鳴らせるのは **`http://` のMP3だけ**（`https://`、HLS（`.m3u8`）、AAC、プレイリスト（`.m3u`、`.pls`）は不可）。ビットレートは192k以下（作者の方針）。局の公式サイトが案内するURLを優先し、**配信の利用条件を確かめる**（技術的に鳴ることと、利用が認められていることは別）
2. **`firmware/RadioDays_U10/stations.h` の表を直す**：1行が1局 `{"局名", "都市", "http://…", freq},`。局名はASCII（アクセント記号や日本語は出ない）で19文字まで。都市は記録用（画面に出ない）
3. **局数が変わったら、そのLocationの全局の `freq` を直す**：`python3 tools/freq-calc/freq_calc.py 6` のように局数を入れると、1番目からの値を出す（例：6局なら 810, 830, 850, 870, 890, 910）。並びの順がダイヤルの順と `n/N` の番号になる
4. **Locationを加える**：WORLDより後ろに1行（名前は大文字ASCIIで7文字まで、都市、緯度・経度、`true`、`800`、`RD_DIAL_MAX(表)`、表、`RD_COUNT(表)`、タイムゾーン）。最初の5つ（JAPAN、USA、UK、FRANCE、WORLD）は名前と位置を変えない。`stations.h` の終わりの `LOCATION_COUNT == 7` の数を直す
5. **ビルド**：設定はスケッチの冒頭のとおり。`freq` の誤り、長すぎる名前、Locationの数の不一致は**ビルドの時に止まる**（エラーの文で分かる）。アプリが0x150000（1,376,256 B）を超えないこと（局の文字列の分だけ大きくなる）
6. **USB中継で聴く場合**：中継は許可したホストだけを受信する。`tools/usb-relay/` に `allowed_hosts.txt` を置き、1行に `host:port`、転送先が毎回変わるものは `redirect *.example.com:80` と書く。**このファイルがあると組み込みの一覧は使われない**ので、今の局のホストも書く。確かめるには `perl tools/usb-relay/RadioDaysRelay_U9.command --check-url <URL>`、`--check-redirect <URL>`。IPアドレスのURLと300文字を超えるURLは中継が断る
7. 改造したバイナリを配布する場合は、ライセンスの節（GPL v3）に従う

### 既知の制約と注意

- 放送局からの供給が一時的に足りないと音切れが起き、自動で回復する（解消はしていない）
- 局のURLは、RNZの局を除き、公式に公開されたものか未確認。**各局の利用条件は未確認**（U9の42局も同じ。[docs/usb-u4.md](docs/usb-u4.md) 第7節、[docs/usb-u9.md](docs/usb-u9.md)）。局はURLを予告なく変えることがある
- U9の42局は、Macから接続して応答を確かめただけで、**Cardputerでの再生はほとんど未確認**（[docs/usb-u9.md](docs/usb-u9.md) 第5・6節）

- 職場などでUSB中継を使う場合は、USB機器の接続や音声のストリーミングについて、その場所の規則に従うこと。Macのインターネット共有などの設定は使わない
- 公開までの残作業：[docs/release-plan.md](docs/release-plan.md)

### 3Dプリント部品（オプション）

![RADIO DAYSの実機全体写真（正面）。Cardputer ADVを緑色の3Dプリント台座に載せ、Jogダイヤルを接続した状態](docs/images/RADIODAYS_2.jpeg)

スタンドとジョグダイヤルの3Dプリントデータは [Cardputer ADV Media Jog](https://github.com/kariagepompadour/Cardputer-ADV-Media-Jog)（`hardware/` フォルダ）にある（MIT License）。RADIO DAYSはCardputer ADV Media Jogから派生した作品で、Cardputer ADV・U135・3Dプリント製の台座とJogダイヤルは Media Jog で使用していたものをそのまま流用している。公開しているSTLと3MFは同一形状で、設計・製作者本人が実機で確認済み。

- `hardware/MediaJog_Base_A.stl`：平置き用ベース
- `hardware/MediaJog_Base_B.stl`：15°傾斜付きベース
- `hardware/MediaJog_Dial.stl`：ジョグダイヤル
- `hardware/Cardputer-ADV-Media-Jog.3mf`：Bambu Studio用プロジェクトファイル

RADIO DAYSは3Dプリント部品なしでも使える。

| 資料 | 内容 |
| --- | --- |
| [docs/concept.md](docs/concept.md) | **コンセプト**（作品の背景、キャッチコピー、設計の原則） |
| [docs/design.md](docs/design.md) | 設計書（調査結果、方式、画面・操作、Launcher、NVSの合格条件） |
| [docs/development-plan.md](docs/development-plan.md) | 開発計画（Stage 1〜4、各段階の確認と判定基準） |
| [docs/stage-3-3.md](docs/stage-3-3.md) | **Stage 3-3：太陽と月、昼夜の空、UFO** |
| [docs/usb-u10.md](docs/usb-u10.md) | **U10：アイボリーの文字盤、縁取り、電波塔。実機は未確認** |
| [docs/dial-face-tower-proposal.md](docs/dial-face-tower-proposal.md) | U10の設計（A案・B案、電波塔、プレビュー。B案を採用） |
| [docs/usb-u9.md](docs/usb-u9.md) | **U9：標準の7 Location・42局、選局番号 n/N、U9の中継。実機は未確認** |
| [docs/u9-implementation-plan.md](docs/u9-implementation-plan.md) | U9の実装計画（局の表、表示、容量、中継、試験の項目） |
| [docs/location-selection-review.md](docs/location-selection-review.md) | 標準の収録国の再検討（6か国＋WORLD） |
| [docs/uk-station-selection-review.md](docs/uk-station-selection-review.md)、[japan](docs/japan-station-selection-review.md)、[usa](docs/usa-station-selection-review.md)、[france](docs/france-station-selection-review.md)、[germany](docs/germany-station-selection-review.md)、[italy](docs/italy-station-selection-review.md) | 各国の選局の調査（聴取率、配信、再生、利用条件） |
| [docs/station-customization-review.md](docs/station-customization-review.md) | 利用者による放送局の追加・変更の方法 |
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
| `tools/usb-relay/` | USB中継スクリプト（`RadioDaysRelay_U9.command`、`allowed_hosts.txt` で許可ホストを指定） |
| `firmware/RadioDays_U10/`、`tools/u10-test/`、`tools/u10-preview/`、`tools/freq-calc/` | U10（`run_u10_test.sh`：文字盤・残像・電波塔の試験。`make_u10_preview.sh`：U10のコードで描いた画面の見本。`make_face_tower_preview_u10.py`：文字盤・電波塔の設計プレビュー。`freq_calc.py`：`freq` の計算） |

## ライセンス

**ソースファイル（`.ino`、`.h`）**：MIT License（[LICENSE](LICENSE)）。Copyright (c) 2026 kariagepompadour

**コンパイル済みバイナリ（`.ino.bin`）**：ESP8266Audio 2.4.1（GPL-3.0-or-later）を含む。バイナリの配布には GPL-3.0-or-later の条件が適用される（[LICENSES/GPL-3.0-or-later.txt](LICENSES/GPL-3.0-or-later.txt)）。Corresponding Source はこのリポジトリのソースファイルと、`third-party/ESP8266Audio-2.4.1/`（リポジトリ内に同梱）。ビルドには Arduino IDE と、システムにインストールした ESP8266Audio 2.4.1 を使う（`third-party/` の内容は参照用であり、ビルドには使わない）。改造したバイナリを配布する場合も同様（[放送局の追加・変更](#放送局の追加変更) 手順7参照）。

地球儀の地図：[Natural Earth](https://www.naturalearthdata.com/)（パブリックドメイン）
