# U4（案）：昼の空、USB待機の案内、国別Locationと放送局、Wi-Fiの3件保存

- 作成日：2026-10-21（第2版：同日、作者の方針を受けて改訂）
- 状態：**作者が承認（2026-10-21）。実装は [usb-u4.md](usb-u4.md)**。実装での変更：表示名は `THAI`・`ARGENT`・`AUSSIE`・`NZ`、CANADAを追加、INDIAは見送り、Trójka→Radio Chopin、Mix Megapol→Radio Sotenäs（`*.sharp-stream.com` の決まりは入れない）、UFOを人工衛星に（⑤、作者の追加の指示）。④の大きさは見込みより大きく約5.7 KB
- もとにする版：U3（保存版として変更しない）。U3の時刻、USBの音声のやり取り、Wi-Fiの接続、UFO、夜の空は維持する
- 新しい版の名前：**U4**（作者の決定）

## 決定事項（作者、2026-10-21）

| # | 項目 | 決定 |
| --- | --- | --- |
| ① | 昼の空 | `#344F65`、雲を約55%に、雲の白を落ち着かせる。太陽・夜・周波数・オレンジの文字は変えない（**承認済み**） |
| ② | USB専用の待機 | 既存の待機表示で `USB RELAY REQUIRED`／`Start the relay on your computer.`。中継が見つかると消える。本体では「Mac」と書かない。READMEに「中継はmacOS専用、Windows・Linuxは未対応・未検証」（**承認済み**） |
| ③ | Location | **国別**（ASIA・LATAM・OCEANIAは作らない）。JAPAN・USA・UK・FRANCE・WORLDは維持。新しいLocationはWORLDの後ろ。オランダは**HOLLAND**。256〜320kbpsの局は除外。候補Bも採用の方向（ドイツとイタリアの公共放送を重視） |
| ④ | Wi-Fi | 最大3件を保存。起動の速さを最優先 |

---

## ① 昼の空：実装する箇所

| ファイル・関数 | 変更 |
| --- | --- |
| `drawSkyWindow()` の昼の部分 | 背景の `lcd.color565(80,150,230)` を `lcd.color565(0x34,0x4F,0x65)` に（1行） |
| `drawCloudSmall()` | 円3つ（半径1・2・1）と四角（4×2）にする。本体の色 `(200,206,212)`、縁の色 `(130,148,165)` |
| 雲の位置（`drawSkyWindow()` の2行） | 雲が小さくなる分、1ピクセルずつ右下へ（`SKY_X+5, SKY_Y+7`、`SKY_X+50, SKY_Y+16`） |
| 変えないもの | `drawSun()`、夜の部分、`drawFacedMoon()`、時刻の処理 |
| 容量 | ほぼ0（±50B【推測】） |

比較の図：[images/day-sky-proposal-u4.png](images/day-sky-proposal-u4.png)（左：今、右：案。Macで再現したもの）

## ② USB専用の待機：実装する箇所

| ファイル・関数 | 変更 |
| --- | --- |
| `drawRadio()` の状態の行 | 「中継がまだ答えていない（`!usbLink.ready()`）かつ（`usbOnly` または Wi-Fiが保存されていない）」で、状態が `RS_WAIT_WIFI` のとき、`WAITING FOR WI-FI/USB` の代わりに `USB RELAY REQUIRED`（黄）。Wi-Fiが保存されていて普通に起動した場合は、今のまま |
| `drawRadio()` の局名の行（y＝96） | 同じ条件のとき、局名の代わりに `Start the relay on your computer.`（小さい文字、中央）。条件が変わったら局名を描き直す（`view` に1項目を追加）。MANUALで局の間（`NO STATION`）でも案内は出す |
| 中継が見つかったとき | 条件が外れ、自動で局名と `CONNECTING`／`PLAYING` に戻る（新しい画面やダイアログは作らない） |
| `drawStartUsb()` | `USB   : waiting for the Mac relay` → `USB   : waiting for the relay` |
| `applyRelayTime()` の記録 | `[CLOCK] time from the Mac relay` → `[CLOCK] time from the USB relay` |
| README | 「中継のスクリプトは今のところmacOS用だけ（macOSに入っているPerl・curl・caffeinateを使う `.command`）。Windows・Linuxは対応しておらず、試してもいない」と書く |
| 変えないもの | Mac用のスクリプト（macOS用なので、名前と文言はそのまま） |
| 容量 | 約+0.3KB【推測】 |

本体の画面の表示で「Mac」を含むのは、START画面の1か所だけだった（確認：U3のスケッチの文字列）。

---

## ③ 国別Locationと放送局

### 3.1 設計の条件と、調べてわかったこと

| 条件 | 対応 |
| --- | --- |
| 代表都市の緯度・経度 | 各Locationに首都（または代表都市）の座標を入れる。地球儀の印と、太陽・月（昼と夜）に使う |
| 局の所在地とLocationの国 | 一致させる（局の都市名を、Location画面と記録に出す） |
| WORLDの維持 | WORLDの5局と順番は変えない |
| 保存したLocationとの互換性 | 本体は「Locationの番号と名前」を保存し、**名前で照合し直す**作りになっている（`savedLocationIndex()`：番号の名前が違えば、名前で探す）。**新しいLocationをWORLDの後ろに足せば、今の保存はそのまま使える**。既存の名前（JAPAN〜WORLD）は変えてはいけない |
| 表示名の長さ | **7文字まで**。理由：ラジオの画面の最下行のLocationの欄が42ピクセル（7文字）、保存の名前の欄が8バイト（7文字＋終わり）、Location画面の「(now)」が右端に収まる |
| ロータリーエンコーダーの操作 | Locationは5→20。ノブ1目盛りで1つ進み、端で一周する。地球儀は回転が追いつくまでなめらかに回る（今と同じ）。キーボードの `,`・`/` でも動かせる。20か所でも操作の仕組みは変えなくてよいと考える。実機で回しやすさを確かめる |
| 地球儀を作れないときの文字の一覧 | 今は5行しか入らない（y＝30から14ピクセルごと）。**選んでいる行が常に見えるように、5行ずつ送る**表示に変える（約0.2KB） |
| ニュージーランドの局 | WORLDのRNZ Nationalは**そのまま残し**、N.ZEALAND側にも同じ局を入れる（同じURLを2か所に登録しても問題はない）。WORLDから移すと、WORLDの局の並びが変わるため、移さない |

### 3.2 表示名（7文字を超える国）

| 国 | 案A（推奨：読める略語） | 案B（ISOの国コード） |
| --- | --- | --- |
| THAILAND（8） | `THAI` | `TH` |
| ARGENTINA（9） | `ARGENT` | `AR` |
| AUSTRALIA（9） | `AUSSIE` | `AU` |
| NEW ZEALAND（11） | `N.ZEAL` | `NZ` |

ほかは7文字以内：GERMANY、HOLLAND、POLAND、CZECHIA、SWEDEN、ITALY、HUNGARY、TAIWAN、INDIA、BRAZIL、CANADA。

### 3.3 Locationの順番（WORLDの後ろ）

地球儀がなめらかに進むよう、地域ごとに並べる：

JAPAN, USA, UK, FRANCE, WORLD（今のまま）→ **GERMANY, HOLLAND, POLAND, CZECHIA, SWEDEN, ITALY, HUNGARY**（ヨーロッパ）→ **INDIA, THAI, TAIWAN**（アジア）→ **AUSSIE, N.ZEAL**（オセアニア）→ **CANADA, BRAZIL, ARGENT**（アメリカ大陸）

### 3.4 国ごとの放送局（最終候補）

**区分**：A＝そのまま使える（`http://`、MP3、ホスト名、転送なし）／B＝転送先が毎回変わるため、Macの中継に「ドメインの後ろが一致する決まり」が必要（Wi-Fiでは、ESP8266Audioが転送をたどるので使える見込み）。ICY＝`ICY 200 OK` で答える局（中継は対応済み、Wi-FiはESP32の `HTTPClient` が数字だけ読むので使える見込み【確認：コード。実機未確認】）

**公式に公開されたURLか**：**RNZ（ニュージーランド）だけ確認できた**（RNZのヘルプのページで、第三者のプレーヤー用として公開されている）。それ以外の局は**未確認**（第三者の局一覧で見つけ、Macから接続して確かめただけ）。**各局の利用条件は、全局未確認**（既存の27局と同じ）。

| Location | 代表都市（緯度, 経度） | 局 | 局の都市 | 形式（確認） | 区分 | 種類 |
| --- | --- | --- | --- | --- | --- | --- |
| GERMANY | Berlin (52.52, 13.40) | Deutschlandfunk | Cologne | 128k 48kHz | B（`*.rndfnk.com`） | 公共 |
| | | Deutschlandfunk Kultur | Berlin | 128k 48kHz | B | 公共 |
| | | 1LIVE（WDR） | Cologne | 128k 48kHz | B | 公共 |
| | | WDR 5 | Cologne | 128k 48kHz | B | 公共 |
| | | Hit Radio FFH | Frankfurt | 128k 44.1kHz | A | 民間 |
| HOLLAND | Amsterdam (52.37, 4.90) | NPO Radio 1 | Hilversum | 192k 48kHz | A | 公共 |
| | | NPO Radio 2 | Hilversum | 192k 48kHz | A | 公共 |
| | | NPO 3FM | Hilversum | 192k 48kHz | A | 公共 |
| | | NPO Klassiek | Hilversum | 192k 48kHz | A | 公共 |
| | | Radio Continu | Groningen | 192k 44.1kHz | A | 民間 |
| POLAND | Warsaw (52.23, 21.01) | Polskie Radio Jedynka | Warsaw | 192k 48kHz | A・ICY | 公共 |
| | | Polskie Radio Dwójka | Warsaw | 192k 48kHz | A・ICY | 公共 |
| | | Polskie Radio Trójka | Warsaw | 192k 48kHz | A・ICY | 公共 |
| | | RMF FM | Kraków | 128k 44.1kHz | A | 民間 |
| | | Radio ZET | Warsaw | 128k 44.1kHz | B（`*.cdn.eurozet.pl`、2回転送） | 民間 |
| CZECHIA | Prague (50.08, 14.44) | ČRo Radiožurnál | Prague | 128k 44.1kHz | A | 公共 |
| | | ČRo Dvojka | Prague | 128k 48kHz | A | 公共 |
| | | ČRo Vltava | Prague | 128k 44.1kHz | A | 公共 |
| | | Radio Beat | Prague | 128k 44.1kHz | A（`play.cz`経由） | 民間 |
| | | Rádio Blaník | Prague | 128k 44.1kHz | A | 民間 |
| SWEDEN | Stockholm (59.33, 18.07) | Pirate Rock | Gothenburg | 192k 44.1kHz | A | 民間 |
| | | Country Rocks Radio | （スウェーデン） | 192k 48kHz | A | ネット局 |
| | | FUN RADIO 95.3 | （スウェーデン） | 192k 44.1kHz | A | 民間 |
| | | Mix Megapol | Stockholm | 128k 48kHz | B（`*.sharp-stream.com`） | 民間 |
| ITALY | Rome (41.90, 12.50) | Virgin Radio Italia | Milan | 128k 48kHz | A | 民間 |
| | | Radio 105 | Milan | 128k 48kHz | A | 民間 |
| | | Radio Monte Carlo | Milan | 128k 48kHz | A | 民間 |
| | | Radio Radicale | Rome | 64k 44.1kHz | A | 報道 |
| HUNGARY | Budapest (47.50, 19.04) | Kossuth Rádió | Budapest | 192k 48kHz | A | 公共 |
| | | Petőfi Rádió | Budapest | 192k 48kHz | A | 公共 |
| | | Rádió 1 | Budapest | 128k 44.1kHz | A | 民間 |
| INDIA | New Delhi (28.61, 77.21) | Radio BollyFM | （インド） | 128k 44.1kHz | A | ネット局 |
| | | Chennai Live | Chennai | 128k 44.1kHz | A | ネット局 |
| | | 2B! Radio Sangam | （インド） | 48k 44.1kHz | A | ネット局 |
| THAI | Bangkok (13.76, 100.50) | FM 94 Bangkok Smile | Bangkok | 128k 44.1kHz | A | 民間 |
| | | Smooth 105.5（MCOT） | Bangkok | 192k 48kHz | A | 公共系 |
| | | Voice of Navy 88.5 | Bangkok | 128k 44.1kHz | A | 海軍の局 |
| TAIWAN | Taipei (25.03, 121.57) | City FM 92.9 | Taipei | 128k 44.1kHz | A | 民間 |
| | | City FM 90.1 | Taichung | 128k 44.1kHz | A | 民間 |
| | | City FM 97.1 | Tainan | 128k 44.1kHz | A | 民間 |
| | | Family 97.7 Classical | Taichung | 192k 48kHz | A | 民間 |
| AUSSIE | Sydney (−33.87, 151.21) | Kiss FM | Melbourne | 128k 44.1kHz | A | コミュニティ |
| | | Three D Radio | Adelaide | 128k 48kHz | A | コミュニティ |
| | | Skylab Radio | Melbourne | 128k 44.1kHz | A | ネット局 |
| N.ZEAL | Wellington (−41.29, 174.78) | RNZ National（WORLDと同じ局） | Wellington | 64k 24kHz | A・**公式に公開** | 公共 |
| | | RNZ Concert | Wellington | 64k 24kHz | A・**公式に公開** | 公共 |
| | | RNZ Pacific | Auckland | 64k 24kHz | A・**公式に公開** | 公共 |
| | | The Most FM | Christchurch | 160k 22.05kHz | A | ネット局 |
| | | Compass FM | Kerikeri | 128k 44.1kHz | A | コミュニティ |
| CANADA | Ottawa (45.42, −75.70) | CHUP Soft Rock 97.7 | Calgary | 128k 44.1kHz | A | 民間 |
| | | CHOI Radio X | Québec City | 64k 44.1kHz mono | A | 民間 |
| | | DJFM | Toronto | 128k 44.1kHz | A | ネット局 |
| BRAZIL | Brasília (−15.79, −47.88) | Rádio Batuta | Rio de Janeiro | 128k 44.1kHz | A | 文化団体 |
| | | Batuta Clássicos | Rio de Janeiro | 128k 44.1kHz | A | 文化団体 |
| | | 91 Rock | Curitiba | 128k 44.1kHz | A | 民間 |
| | | Continental FM | Porto Alegre | 96k 44.1kHz | A | 民間 |
| ARGENT | Buenos Aires (−34.60, −58.38) | Radio Nacional Folklórica | Buenos Aires | 160k 44.1kHz | A | 公共 |
| | | Nacional Rock | Buenos Aires | 160k 44.1kHz | A | 公共 |
| | | Nacional Clásica | Buenos Aires | 192k 44.1kHz | A | 公共 |
| | | Radio Nacional AM 870 | Buenos Aires | 56k 44.1kHz | A | 公共 |
| | | Radio Nacional Córdoba | Córdoba | 112k 44.1kHz | A | 公共 |

合計：**61局**（A＝55、B＝6）、新しいLocation**15**。Location全体は20、局は全体で88。

### 3.5 外したもの・見つからなかったもの

| 局 | 理由 |
| --- | --- |
| **Rai Radio 1・2・3・Classica（イタリアの公共放送）** | 転送先が**256〜320kbps**（作者の方針で除外）。低いビットレートの公式の配信は見つからなかった。イタリアは民間局と報道局だけになる |
| Sveriges Radio（スウェーデンの公共放送） | HTTPSにだけ転送される |
| ABC（オーストラリアの公共放送） | 日本から `403`（地域制限） |
| CBC（カナダの公共放送）、AIR（インドの公共放送）、韓国の局 | `http://` のMP3が見つからない（HLSやHTTPSだけ） |
| Bartók Rádió（320k）、Tilos Rádió（256k）、RDS Relax（256k） | 高いビットレート |
| Antenne Bayern・Rock Antenne | 使えるが（B、`*.antenne.de`）、ドイツは5局で足りるため今回は入れない |
| Classical 97.7（台湾の別のURL）、2回目以降に見つかったIPアドレスの局 | 中継はIPアドレスのURLを受け付けない（同じ局はホスト名のURLで入れた） |
| 24kbpsの局（Radio Sai、SikhNet） | 音質が低い |
| Bandit Rock、Rix FM ほか | `401`／`500` |

**インド・タイ・オーストラリア・カナダは、公共放送が使えず、民間局・ネット局が中心**。インドの局は所在地が確認できていないもの（BollyFM、2B! Radio Sangam）がある【要確認】。国別の方針と合わない場合は、INDIAを見送ることもできる。

### 3.6 Macの中継：許可ホストの拡張（候補Bのため）

| 項目 | 設計 |
| --- | --- |
| 新しい書き方 | 許可の一覧に「`redirect *.rndfnk.com:80`」のような行を加える。**転送先だけ**に使える（最初のTUNEのURLは、今までどおりホスト名の完全一致） |
| ドメインの境界の検査 | ホスト名を小文字にし、今までの検査（使える文字、IPアドレスではない）を通した後、**「`.rndfnk.com` で終わり、かつ、その前にラベルがある」**ときだけ一致とする。`evilrndfnk.com`（境界がない）、`rndfnk.com.example.org`（後ろが違う）、`rndfnk.com` そのもの（ラベルがない）は一致しない |
| ポート | 完全一致（`:80`） |
| そのほかの安全 | `http://` だけ、転送は3回まで、IPアドレスと `ユーザー名@` は不可（今のまま）。転送先に使えるのは、許可した組織の配信網のドメインだけ |
| 追加する決まり（案） | `*.rndfnk.com:80`（ARD・ドイツ）、`*.cdn.eurozet.pl:80`（Radio ZET）、`*.sharp-stream.com:80`（Mix Megapol）。Raiを入れないので `*.msvdn.net` は入れない |
| 試験 | 中継の `--check-url` と、シミュレーションに「境界の違うホストへの転送が拒否されること」を加える |
| 新しいホストの登録 | 候補Aのホストとポートを、中継の許可一覧に足す（新しい中継のスクリプトとして。U3の中継は変えない） |
| Cardputerへの影響 | なし（中継の変更だけ） |

### 3.7 容量（実測）

U3のコピーに、上の61局と15のLocationを足して一時的にビルドした（リポジトリには入れていない）。

| 項目 | 大きさ |
| --- | --- |
| U3 | 1,363,120 B |
| 局とLocationを足したもの | **1,368,592 B（+5,472 B、1局あたり約90B）** |
| 0x150000までの残り | 7,664 B |

---

## ④ Wi-Fiを最大3件保存する

### 4.1 今の作り（U3）

| 項目 | 今 |
| --- | --- |
| 保存の件数 | **1件** |
| 保存の形式 | NVSの名前空間 `radiodays`、キー `wifi`。中身は `WifiRecord`（印 `RDWF`、版1、SSID 33バイト、パスワード65バイト、CRC32）。112バイト |
| 書くとき | 接続に成功したときだけ（`nvsWriteWifi()`。同じ内容なら書かない。NVSの空きが32未満なら書かない） |
| 起動時の接続 | 保存したネットワークに `WiFi.begin()`。**20秒**待つ。失敗すると5→10→20→40→60秒の間をあけてやり直す |
| 失敗の見分け | 背景の接続（`wifiService()`）は「つながった」か「20秒」だけを見る。**近くにないネットワークでも、毎回20秒待っている**（設定の画面の接続（`handleWifiJoin()`）は、3秒後から「見つからない」「パスワードが違う」ですぐに止める） |
| 画面への影響 | 接続は背景で行い、ラジオの画面はすぐに出る（中継があればUSBで再生できる）。ただし、Wi-Fiで聴くまでの時間は接続の時間で決まる |
| USB専用（`U`） | Wi-Fiを一切使わない（U2a4から） |

**ESP32のWi-Fiについて調べたこと**（Arduinoコア3.3.9）

| 項目 | 内容 |
| --- | --- |
| `WiFi.begin(ssid, pass)` | 中で全チャンネルを探してから接続する（`WIFI_FAST_SCAN`：見つかった時点で止める） |
| 近くにない | 探し終わると `WIFI_REASON_NO_AP_FOUND` → 状態が `WL_NO_SSID_AVAIL` になる（`STA.cpp` 146〜147行）。**20秒待たなくても、数秒でわかる** |
| パスワードが違う | `AUTH_FAIL` → `WL_CONNECT_FAILED`（同168行ほか） |
| 一覧の検索（`scanNetworks`） | 1チャンネル最大300ミリ秒（既定）。13チャンネルで**最大約4秒**【確認：`WiFiScan.cpp`。実測していない】 |
| チャンネルとBSSIDを指定した接続 | `WiFi.begin(ssid, pass, channel, bssid)` は、中の検索を省けるので速い |
| 接続にかかる時間の目安 | 認証と接続で1〜3秒、IPアドレスの取得で0.5〜2秒。電波が弱いと再送で数秒延びる【一般。実測していない】 |

### 4.2 方式の案（起動の速さを最優先）

```text
起動（Uでなければ）
  │
  ├─①［速い道］最後に成功したネットワークに WiFi.begin（中で探す）
  │      ├ つながった → 完了（ふだんはここで終わる。2〜5秒の見込み）
  │      ├ WL_NO_SSID_AVAIL（近くにない）／WL_CONNECT_FAILED → すぐ②へ（20秒待たない）
  │      └ 12秒たってもつながらない → ②へ
  │
  ├─②［保存が2件以上のときだけ］一覧を1回だけ検索（最大約4秒）
  │      ├ 保存したネットワークのうち見つかったもの（①で失敗したものは除く）を、電波の強い順に1つ選ぶ
  │      └ 見つからない → ④へ
  │
  ├─③ 選んだネットワークに、チャンネルとBSSIDを指定して WiFi.begin（中の検索を省く）
  │      ├ つながった → 完了。「最後に成功した」をこのネットワークに変える
  │      └ 失敗・12秒 → 次に強いもの（あれば）。ただし全体で30秒を超えない
  │
  └─④ 今回はあきらめ、今までどおり間をあけてやり直す（5→10→20→40→60秒）
         次のやり直しも①から（①は近くになければ数秒で終わる）
```

| 秒数 | 案 | 理由 |
| --- | --- | --- |
| 1回の接続の上限 | **12秒** | ふだんの接続（2〜5秒）の2倍以上あり、電波の弱い場所の再送にも余裕がある。今の20秒より短いが、「近くにない」「パスワードが違う」はESP32がすぐ知らせるので、12秒まで待つのは「見えているのにつながらない」場合だけ |
| 一覧の検索 | 1回、最大約4秒（既定の設定のまま） | 1件しか保存していない人には行わない |
| 1回の探索の全体 | **30秒まで** | ①12秒＋②4秒＋③12秒＋余裕。3件を順に20秒ずつ試す方式（最大60秒以上）を避ける |
| 最初の失敗からのやり直しの間 | 今と同じ（5→60秒） | ― |

**この方式で避けられること**：ふだん（家）は①だけで終わり、今と同じ速さ。最後に使ったネットワークが近くにないとき、今は毎回20秒待つが、新しい方式では数秒で次の候補に進む。3件を順番に長く待つことはない。

**さらに効率的な点**：①の後で検索するので、近くにあるネットワークだけを試す。③はチャンネルとBSSIDを指定して中の検索を省く。

### 4.3 保存の形式とNVSの互換性

| 項目 | 案 |
| --- | --- |
| 新しいキー | `wifis`（版1）：印、版、件数（0〜3）、最後に成功した番号、3件分（SSID 33バイト＋パスワード65バイト）、CRC32。約310バイト |
| 古いキー `wifi` | **残し、最後に成功したネットワークを書き続ける**。U3以前に戻しても、そのネットワークで動く |
| 引き継ぎ | 起動時に `wifis` がなく、`wifi` が正しく読めれば、それを1件目として使う。**起動時には書かない**（NVSに書くのは、追加・削除・最後に成功したネットワークが変わったときだけ） |
| 書く回数 | 「最後に成功した」が変わったときに、`wifis` と `wifi` の2つ。同じネットワークで毎回起動する人は、起動で書かない |
| 壊れたデータ・新しい版のデータ | 今と同じ扱い（勝手に上書きしない。壊れているときは利用者に確かめる） |
| NVSの空き | 今と同じく、空きが32未満なら書かない。約310バイトの記録は、NVSの約11個分 |

### 4.4 本体での追加・削除・選択（画面の案）

| 操作 | 案 |
| --- | --- |
| 追加 | 今の設定（`W`）のまま：一覧から選んでパスワードを入れ、接続に成功したら保存。すでに3件あるときは「どれと入れ替えるか」を出す（ノブで選んでEnter、Escで保存しない） |
| 一覧での印 | 検索の一覧で、保存しているネットワークに `*` を付ける |
| 保存した一覧の画面 | 設定の一覧の画面で `S`：保存した最大3件を表示（最後に成功したものに印）。Enterで今すぐ接続、`Del` で削除（`Y`／`N` で確かめる） |
| 非公開のSSID | **今と同じく対応しない**（SSIDを手で入れる画面がない。追加するなら別の作業） |
| 登録したネットワークが1つも見つからないとき | ④のとおり、間をあけてやり直す。ラジオの画面は使え、`W` で設定、USBの中継があればUSBで聴ける |

### 4.5 容量とRAM

| 項目 | 見込み |
| --- | --- |
| フラッシュ | 約2〜3KB【推測：今のWi-Fiの設定と保存の処理の大きさから】 |
| RAM | 保存した3件を常に持つ：約310バイト。検索の結果は今の設定の画面と同じ |

---

## ⑤ 容量の総合見積もり

| 項目 | 増加 | 根拠 |
| --- | --- | --- |
| ① 昼の空 | 約0 | 推測 |
| ② USB待機の案内、表記 | 約+0.3KB | 推測 |
| ③ 61局と15のLocation | **+5,472 B** | **実測** |
| ③ 文字の一覧の送り | 約+0.2KB | 推測 |
| ④ Wi-Fi 3件 | 約+2〜3KB | 推測 |
| **合計** | **約+8〜9KB** | |
| U3の残り | 13,136 B | 実測 |
| **U4の残り（見込み）** | **約4〜5KB** | 0x150000以内に収まる見込み |

**足りなくなった場合の順番と代わりの案**

1. まず①・②・③を入れ、④の前に実測する
2. 足りなければ：地球儀の地図の圧縮（約2.3KB、`usb-u3`以前の調査で実測済み）、記録の文字列を短くする（約1KB）
3. それでも足りなければ：③の局を減らす（1局約90B）、または④を「保存は3件、画面は追加と削除だけ」に絞る
4. 「0x150000以内」を広げる判断（作者。Launcherは64KB単位の領域を作るだけ）

## 6. 確認していただきたいこと

1. **表示名**：THAILAND・ARGENTINA・AUSTRALIA・NEW ZEALANDを、案A（`THAI`・`ARGENT`・`AUSSIE`・`N.ZEAL`）にするか、案B（`TH`・`AR`・`AU`・`NZ`）にするか
2. **局の一覧（第3.4節）**：このままでよいか。特に、イタリアの公共放送（Rai）が高いビットレートのため入らないこと、インドの局の所在地が未確認であること、CANADAを加えるか
3. **候補B**：Macの中継に「転送先だけに使う、ドメインの後ろが一致する決まり」を加えるか（第3.6節）
4. **Wi-Fiの方式（第4.2節）**：1回12秒、全体30秒、「最後に成功→検索→強い順」でよいか。画面の案（第4.4節）
5. **公開前の確認**：「公式に公開されたURL」と「利用条件」は、ほとんどの局で未確認。公開の前にまとめて確認する（release-plan.mdのA2）

## 参考

- RNZのストリームのURL（公式）：[What audio is available? — RNZ](https://www.Rnz.Co.Nz/listen/audiohelp)
- そのほかの局の候補は、Radio Browser（公開の局データベース）から取り出し、2026-10-21にこのMacから接続して確かめた
