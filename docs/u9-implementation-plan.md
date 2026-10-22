# U9 実装計画

- 作成日：2026-10-22
- 状態：**計画だけ**。U9のコードは書いていない。ビルドもしていない。U3〜U8のファームウェア、`stations.h`、USB中継、README、CHANGELOGは変えていない
- **2026-10-22 追記：この計画に沿ってU9を実装した** → [usb-u9.md](usb-u9.md)。計画との違い：`n/N` は x 181で終わるようにした（音量の前に空きを作るため。5文字は x 154から）、2桁の番号（99/99まで）に対応、RTLの転送先は「転送先だけのホスト」ではなく許可の一覧の1つのホストとして加えた（中継の照合の仕組みを変えないため）、Radio Swiss Jazzの転送先は `livestreaming-node-1`〜`4` を確認
- もとにした資料：[location-selection-review.md](location-selection-review.md)、各国の選局の調査（[japan](japan-station-selection-review.md)、[usa](usa-station-selection-review.md)、[uk](uk-station-selection-review.md)、[france](france-station-selection-review.md)、[germany](germany-station-selection-review.md)、[italy](italy-station-selection-review.md)）、[station-customization-review.md](station-customization-review.md)、U8のコード（`firmware/RadioDays_U8/`）
- 書き方：【コード】＝U8のコードで確認、【接続】＝このMacからの接続（2026-10-21 20:01 UTC、42局すべて）、【推測】＝推測、**未確認**。**Macでの接続の確認は、Cardputer ADVでの再生の確認ではない**

## 1. U9の目的と基本方針

1. 標準の収録を **7 Location・42局** にする（作者が聴きたい国と、魅力的な局に絞る。国数・局数の多さは目的にしない）
2. U8の19 Location・85局は**そのまま保存**する。U9は新しいフォルダ `firmware/RadioDays_U9/` に作り、U8を上書きしない
3. U8のダイヤルと操作感（4段の目盛り、オレンジの指針、循環、AUTOの滑らかな移動、MANUAL、Locationの切り替え、時計・タイムゾーン・昼夜、USBとWi-Fiの再生）は変えない
4. 選局番号 `n/N` を表示する（人気の順位ではない）
5. 今のRADIO DAYSで安定して聴けることを優先する。HTTPS・HLSへの対応は進めない
6. 利用者が局を加え・替えられることを大事にする（READMEでの説明を中心に）

## 2. 7 Location・42局の候補一覧

並びは人気の順ではない（言葉の局と音楽の局、音楽の種類が続かないように）。`freq` はU8の式（`800 + (2i+1)×1周÷(2×局数)`）で計算した値【式で計算】。ビットレートと周波数は【接続】。

### JAPAN（5局、1周90段）

| n/N | 局名（画面） | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/5 | Shonan Beach FM | Zushi | `http://shonanbeachfm.out.airtime.pro:8000/shonanbeachfm_a` | 809 | 128k 44.1kHz |
| 2/5 | FM Setagaya | Setagaya | `http://fmsetagaya834.out.airtime.pro:8000/fmsetagaya834_a` | 827 | 128k 44.1kHz（新） |
| 3/5 | OTTAVA | Tokyo | `http://ottava2.out.airtime.pro:8000/ottava2_a` | 845 | 128k 44.1kHz |
| 4/5 | FM Kahoku | Kahoku | `http://radio.kahoku.net:8000/;` | 863 | 96k 44.1kHz |
| 5/5 | Otsuchi Seashore | Iwate | `http://mp3s.nc.u-tokyo.ac.jp/OTSUCHI_CyberForest.mp3` | 881 | 192k 48kHz |

### USA（6局、1周120段）

| n/N | 局名 | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/6 | WNYC | New York | `http://fm939.wnyc.org/wnycfm` | 810 | 96k 32kHz（新） |
| 2/6 | WFUV | New York | `http://wfuv-onair.streamguys.org/onair-hi` | 830 | 128k 44.1kHz（新） |
| 3/6 | WQXR | New York | `http://stream.wqxr.org/wqxr` | 850 | 128k 48kHz |
| 4/6 | WBGO | Newark | `http://ais-sa8.cdnstream1.com/3629_128.mp3` | 870 | 128k 48kHz（新） |
| 5/6 | WFMU | Jersey City | `http://stream0.wfmu.org/freeform-128k` | 890 | 128k 44.1kHz |
| 6/6 | KEXP | Seattle | `http://kexp-mp3-128.streamguys1.com/kexp128.mp3` | 910 | 128k 44.1kHz |

### UK（7局、1周150段）

| n/N | 局名 | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/7 | BBC World Service | London | `http://stream.live.vc.bbcmedia.co.uk/bbc_world_service` | 810 | 56k 24kHz |
| 2/7 | Heart London | London | `http://media-ice.musicradio.com/HeartLondonMP3` | 832 | 128k 44.1kHz（新） |
| 3/7 | Radio X London | London | `http://media-ice.musicradio.com/RadioXLondonMP3` | 853 | 128k 44.1kHz（新） |
| 4/7 | LBC London | London | `http://media-ice.musicradio.com/LBCLondonMP3` | 875 | 48k 44.1kHz（新） |
| 5/7 | Classic FM | London | `http://media-ice.musicradio.com:80/ClassicFMMP3` | 896 | 128k 44.1kHz |
| 6/7 | Capital London | London | `http://media-ice.musicradio.com/CapitalMP3` | 917 | 128k 44.1kHz |
| 7/7 | Radio Caroline | Essex | `http://sc6.radiocaroline.net:8040/stream` | 939 | 128k 44.1kHz |

Radio XとRadio Carolineはどちらもロックなので、間を空けた（Radio Xを3番目に）。

### FRANCE（7局、1周150段）

| n/N | 局名 | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/7 | France Inter | Paris | `http://icecast.radiofrance.fr/franceinter-midfi.mp3` | 810 | 128k 48kHz |
| 2/7 | FIP | Paris | `http://icecast.radiofrance.fr/fip-midfi.mp3` | 832 | 128k 48kHz |
| 3/7 | RTL | Paris | `http://icecast.rtl.fr/rtl-1-44-128` | 853 | 128k 44.1kHz、転送1回（新） |
| 4/7 | Radio Nova | Paris | `http://novazz.ice.infomaniak.ch/novazz-128.mp3` | 875 | 128k 44.1kHz（新） |
| 5/7 | France Culture | Paris | `http://icecast.radiofrance.fr/franceculture-midfi.mp3` | 896 | 128k 48kHz |
| 6/7 | Skyrock | Paris | `http://icecast.skyrock.net/s/natio_mp3_128k` | 917 | 128k 48kHz（新） |
| 7/7 | TSF Jazz | Paris | `http://tsfjazz.ice.infomaniak.ch/tsfjazz-high.mp3` | 939 | 128k 44.1kHz（新） |

### WORLD（6局、1周120段）

| n/N | 局名 | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/6 | Radio Paradise | California | `http://stream.radioparadise.com/mp3-128` | 810 | 128k 44.1kHz（USAから移動） |
| 2/6 | Radio Swiss Jazz | Bern | `http://stream.srg-ssr.ch/m/rsj/mp3_128` | 830 | 128k 48kHz、転送2回 |
| 3/6 | RNZ National | Wellington | `http://radionz-ice.streamguys.com/national.mp3` | 850 | 64k 24kHz |
| 4/6 | Groove Salad | San Francisco | `http://ice1.somafm.com/groovesalad-128-mp3` | 870 | 128k 44.1kHz |
| 5/6 | Secret Agent | San Francisco | `http://ice1.somafm.com/secretagent-128-mp3` | 890 | 128k 44.1kHz |
| 6/6 | Nightwave Plaza | Internet | `http://radio.plaza.one/mp3` | 910 | 128k 44.1kHz |

### GERMANY（6局、1周120段）

| n/N | 局名 | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/6 | Deutschlandfunk | Cologne | `http://st01.sslstream.dlf.de/dlf/01/128/mp3/stream.mp3` | 810 | 128k 48kHz、`*.rndfnk.com` へ |
| 2/6 | SWR3 | Baden-Baden | `http://dispatcher.rndfnk.com/swr/swr3/live/mp3/128/stream.mp3` | 830 | 128k 48kHz、`*.rndfnk.com` へ（新） |
| 3/6 | DLF Kultur | Berlin | `http://st02.sslstream.dlf.de/dlf/02/128/mp3/stream.mp3` | 850 | 128k 48kHz、`*.rndfnk.com` へ |
| 4/6 | radioeins | Berlin | `http://dispatcher.rndfnk.com/rbb/radioeins/live/mp3/mid` | 870 | 128k 48kHz、`*.rndfnk.com` へ（新） |
| 5/6 | 1LIVE | Cologne | `http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3` | 890 | 128k 48kHz、`*.rndfnk.com` へ |
| 6/6 | Antenne Bayern | Munich | `http://mp3channels.webradio.antenne.de/antenne` | 910 | 128k 44.1kHz、`s<数字>-webradio.antenne.de` へ（新） |

SWR3は、調査の時の `http://liveradio.swr.de/…`（転送2回）ではなく、その転送先の `dispatcher.rndfnk.com` を直接使う案（転送が1回減り、中継の許可も1つ少なくて済む）。

### ITALY（5局、1周90段）

| n/N | 局名 | city | URL | freq | 配信 |
| --- | --- | --- | --- | --- | --- |
| 1/5 | Radio 24 | Milan | `http://shoutcast2.radio24.it:8000/;` | 809 | 64k 22.05kHz（新） |
| 2/5 | Radio 105 | Milan | `http://icecast.unitedradio.it/Radio105.mp3` | 827 | 128k 48kHz |
| 3/5 | Radio Radicale | Rome | `http://live.radioradicale.it/live.mp3` | 845 | 64k 44.1kHz |
| 4/5 | Radio Monte Carlo | Milan | `http://icecast.unitedradio.it/RMC.mp3` | 863 | 128k 48kHz |
| 5/5 | Virgin Radio Italia | Milan | `http://icecast.unitedradio.it/Virgin.mp3` | 881 | 128k 48kHz |

- 局名はすべてASCIIで19文字以内（最長は「Virgin Radio Italia」の19文字）
- **42局すべてが、2026-10-21 20:01 UTCにこのMacから HTTP 200・MP3・192k以下で応答した**【接続】。実機での再生は第10節
- POLANDは収録しない（将来の候補。U8に残る）
- `city` は画面に出ない（記録用）。新しい局の `city` は案

## 3. U8から変える項目

| 項目 | 変更 |
| --- | --- |
| 局の表 | 85局 → 42局（第2節）。U8から残す27局（USAからWORLDへ移すRadio Paradiseを含む）、加える15局 |
| Location | 19 → 7（JAPAN、USA、UK、FRANCE、WORLD、GERMANY、ITALY の順。最初の5つは名前と位置を変えない） |
| `static_assert(LOCATION_COUNT == 19 …)` | 7に |
| 起動時の既定（`LOCATION_DEFAULT`） | 第3.1節 |
| 選局番号 `n/N` | ラジオ画面の状態の行に加える（第5節） |
| 版の表示・記録 | スケッチの冒頭の説明、シリアルの記録の版の名前、`SHA256SUMS.txt` |
| ホストの試験 | `tools/u4-test` の「19 Locations」「85 stations」の検査を、U9用の試験（例 `tools/u9-test`）で7と42に（U8の試験は変えない） |
| USB中継 | 新しい中継（例 `RadioDaysRelay_U9.command`）。U4の中継は変えない（第7節） |

### 3.1 起動時の既定の扱い

U8の起動【コード】：保存したLocationがなければ `LOCATION_DEFAULT`（1＝USA）を使い、ダイヤルをそのLocationの**最初の局**に置く（`dialFreq = locStations()[0].freq`）。受信の処理も局0から始める。

| 案 | 内容 | 評価 |
| --- | --- | --- |
| **A（推奨）** | `LOCATION_DEFAULT = 4`（WORLD）。Radio ParadiseをWORLDの1/6に置く | Stage 2-1からの「最初にRadio Paradiseが鳴る」が続く。Radio Paradiseは実機で鳴った記録がある（[stage-3.md](stage-3.md)）。WORLDの時計は日本時間、空は東京（U7のまま） |
| B | USAのまま。最初の局はWNYC | 最初の音が変わる。WNYC（96k、32kHz）は実機で未確認 |
| C | JAPAN（作者の国）。最初は湘南ビーチFM | 時計・空も東京と一致。湘南ビーチFMは実機で未確認 |

- 保存したLocationがあれば、それが優先される（U8と同じ）。U8で外す国（例：POLAND）を保存していた本体は、名前が見つからず既定で始まる【コード：`savedLocationIndex()`】
- 案Aで、Radio Paradiseを1/6にしない場合は、既定の局が別の局になる（ダイヤルは常に1番目の局から始まる）

## 4. U8から維持する項目

| 項目 | 維持のしかた |
| --- | --- |
| ダイヤル（4段の目盛り、枠、オレンジの指針、局の印、循環） | `drawDial()`・`rdRing()`・`rdPos()` を変えない。局数からの1周の長さは自動（5局→90段、6局→120段、7局→150段。U8にもある長さ） |
| AUTOの滑らかな移動 | `glideAdd()`・`glideService()`・定数（`GLIDE_STEP_MS = 20`、`GLIDE_MAX_STATIONS = 3`）を変えない |
| MANUAL | `tuneBy()`・`tuneService()`・`TUNE_SETTLE_MS = 350` を変えない |
| Locationの切り替え、地球儀、保存 | `applyLocation()`・`savedLocationIndex()`・保存の記録の形を変えない |
| 時計・タイムゾーン・昼夜 | 7 LocationのTZはU7と同じ文字列（JST、US東部、UK、CET） |
| USB・Wi-Fiの再生 | 受信・復号・中継の通信の約束（`radiodays_link.h`）を変えない |

### 4.1 受信開始の条件（U8のコードで確認）と、変えないための注意

| 操作 | U8の動き【コード】 |
| --- | --- |
| AUTOで回す | `glideAdd()` が移動を始め、その時点で `sendTarget(REQ_NOISE)` で今の局を止める。20 msに1段（2 px）で次の局へ。先の局は3つまで。着いたら `glideService()` が**待たずに**その局を要求 |
| MANUALで回す | 局の外へ出たらその場で `REQ_NOISE`（雑音）。局の位置では `tunePending` を立て、**350 ms止まってから** `tuneService()` が要求 |
| 移動中にA（MODE） | 移動を止めてMANUALに。局の上なら350 ms後に要求、局の間なら雑音のまま |
| MANUAL→AUTO（局の間） | いちばん近い局へ跳び、350 ms後に要求 |
| 移動中 | `tuneService()` は何もしない（移動の側が要求する） |
| 局が鳴らない | 受信の処理が次の局へ進み、ダイヤルもその局へ動く（`autoMoveSeq`） |

**注意**：
1. `n/N` の表示は**読むだけ**にする。`dialFreq`・`tunePending`・`glideActive`・`sendTarget()` に書き込まない
2. 表示の判定は、局名の表示と同じ式（`dialGliding() ? -1 : stationAtFreq(dialFreq)`）を使い、新しい状態の変数を作らない
3. 局の表の並びを変えても、局の位置は `freq` だけで決まる。`freq` を式どおりにしないとビルドが止まる（`dialsValid()`）。この検査は残す
4. AUTOの試験（`tools/u8-test` の移動の試験）は、U9の局数（5・6・7局）でも通ることを確かめる

## 5. 選局番号表示の設計

### 5.1 画面の場所【コード】

| 行 | U8の内容 | 空き |
| --- | --- | --- |
| 上（y 2） | 「RADIO DAYS」（x 4〜124）、「AUTO／MANUAL」（x 156〜228） | なし |
| ダイヤル（y 19〜92） | 左に信号のメーター（x 6〜28、y 20〜65）、中央にダイヤル（x 31〜172）、右に時計と空 | メーターの下（x 4〜30、y 70〜90）に少し |
| 局名（y 96、大きい字） | 局名を中央に、19文字まで（最大228 px） | なし（長い名前と重なる） |
| **状態（y 114、小さい字）** | 状態の文字 x 4〜154（最長「WAITING FOR WI-FI/USB」21文字＝126 px、消す幅150 px）、音量 x 186〜240 | **x 156〜185（30 px＝5文字）** |
| 下（y 126） | Location名 x 4〜46、Wi-Fi・メモリ x 50〜164、「A:MODE L:LOC」x 166〜 | なし |

**推奨：状態の行の右側、音量の左**（例 x 160、小さい字、`7/7` は3文字＝18 px）。局名・状態・音量と重ならない。色は目立ちすぎない `COL_DIM`。
別の案：メーターの下（x 6、y 76）。ダイヤルの近くだが、メーターと紛らわしい。

- 2桁（`10/10`）は5文字＝30 pxで音量の文字に接する。U9は最大7局なので1桁だけ。利用者が10局以上にした場合の扱いは、2桁なら `n/N` を詰める（例：「10/12」をx 156から）か、表示しない。計画では「9局まで表示、10局以上は表示しない」を推奨（簡単で安全）

### 5.2 何を表示するか

| 状態 | 表示 |
| --- | --- |
| 局の位置で止まっている（AUTO・MANUAL） | `n/N`（n＝局の番号＋1、N＝そのLocationの局数） |
| AUTOで移動中 | **消す**（局名も消え、状態は「TUNING...」。U8と同じ考え方） |
| MANUALで局の間 | **消す**（局名も消え、状態は「NO STATION」） |
| USB中継を待つ案内の表示中 | 消す（局名の行に案内が出ている間） |
| 局が鳴らず次の局へ自動で進んだ | ダイヤルがその局へ動くので、その局の番号（表示はダイヤルの位置から決まる） |

- 番号は**ダイヤルの位置から決める**（今鳴っている局からではない）。局名と同じ決め方なので、名前と番号が必ずそろう
- 別の案：局の間で `-/N` を出す（局数が分かる）。推奨は「消す」（局名の行と同じ動き、画面が静か）

### 5.3 Locationを替えたとき
- Locationを替えると、ラジオ画面は全体を描き直し（`drawRadio(true)` で `view = RadioView()`）、ダイヤルは新しいLocationの1番目の局に置かれる【コード】。番号の表示も `view` に「表示中の番号」を持たせれば、全体の描き直しで必ず描き直される（`view` の初期値を「未表示」にしておく）
- 注意：局名の表示は「局の番号が変わったとき」だけ描き直す。Locationが変わって番号が同じ（どちらも1番目）でも、全体の描き直しで `view` が初期化されるので問題ない。`N` が変わることもあるので、番号の比較は `n` と `N` の両方で行う

### 5.4 作り（案）

```cpp
// drawRadio() の中、局名の処理の後（st は局名と同じ値）
int pos = (st >= 0 && !relayHint) ? st : -1;
if (pos != view.posShown || locCount() != view.posCount) {
    view.posShown = pos;
    view.posCount = locCount();
    if (pos >= 0 && locCount() <= 9) snprintf(line, sizeof(line), "%d/%d", pos + 1, locCount());
    else line[0] = '\0';
    drawText(160, Y_STATUS, 1, COL_DIM, line, 24);  // 消す幅24 px（4文字）
}
```

- `RadioView` に2つの値（`posShown = -2`、`posCount = -1`）を加える
- 状態の文字の消す幅（150 px）と重ならないことを確かめる（x 4＋150＝154 < 160）

### 5.5 コードの大きさ
- 加えるのは、上の数行（比較、`snprintf`、`drawText`）と、`RadioView` の8バイトほど。`snprintf` と `drawText` はすでに使っているので、新しいライブラリは入らない
- 見込み：**数十〜200バイト程度【推測】**。実際の増え方は**ビルドして測るまで確定しない**

## 6. ファームウェア容量の見通し

| 項目 | 値 | 種類 |
| --- | --- | --- |
| Launcherのアプリの領域 | 0x150000＝1,376,256 B | 決まり |
| U8のアプリ | 1,374,400 B、残り1,856 B | **実測**（U8のビルド） |
| U8の局の文字列（局名・都市・URL） | 5,510 B（重なる文字列をまとめると5,128 B） | `stations.h` から数えた値（ビルドの結果ではない） |
| U8の局の構造体 | 85×16 B＝1,360 B | 構造体の大きさからの計算【推測：32ビットのポインタ3つ＋int】 |
| U8のLocationの構造体 | 19×40 B＝760 B | 同上【推測】 |
| U9の局の文字列 | 約2,400〜2,800 B | 第2節の案から数えた値（`city` は平均9文字と仮定） |
| U9の局・Locationの構造体 | 42×16＋7×40＝952 B | 【推測】 |
| **局とLocationを減らした分** | **約3.5〜4.5 KBの減少の見込み** | **【推測】**。文字列の並び（4バイト境界）、同じ文字列の統合、コンパイラの配置で変わる |
| `n/N` の表示 | 数十〜200 B程度の増加 | 【推測】 |
| U9の残り | **約5〜6 KBの見込み** | **【推測】。ビルドして測るまで確定しない** |

- ビルドの時の検査（`static_assert` の再帰）はコンパイルの時だけで、アプリの大きさには入らない【推測：constexprの評価】
- 新しい機能を加えなくても、局とLocationを減らすだけで余裕が増える見込み。`n/N` の表示はその範囲で収まる見込み
- **容量が足りない場合**（実測で0x150000を超えた場合）の対処：①使っていない記録の文字列（シリアルの説明）を短くする、②`n/N` の表示を簡単にする（`snprintf` の代わりに数字2つを直接描く）、③局の `city` を空にする（画面に出ないため）。いずれも動作は変わらない。Launcherの領域（パーティション）は変えない（作者の約束）

## 7. USB中継の変更計画

### 7.1 42局の接続先【接続、2026-10-21 20:01 UTC】

| 区分 | ホスト:ポート | 局 |
| --- | --- | --- |
| **今の中継で許可済み**（最初のURL） | `shonanbeachfm.out.airtime.pro:8000`、`ottava2.out.airtime.pro:8000`、`radio.kahoku.net:8000`、`mp3s.nc.u-tokyo.ac.jp:80`、`stream.wqxr.org:80`、`stream0.wfmu.org:80`、`kexp-mp3-128.streamguys1.com:80`、`stream.live.vc.bbcmedia.co.uk:80`、`media-ice.musicradio.com:80`（UKの5局）、`sc6.radiocaroline.net:8040`、`icecast.radiofrance.fr:80`（3局）、`stream.radioparadise.com:80`、`stream.srg-ssr.ch:80`、`radionz-ice.streamguys.com:80`、`ice1.somafm.com:80`（2局）、`radio.plaza.one:80`、`st01.sslstream.dlf.de:80`、`st02.sslstream.dlf.de:80`、`wdr-1live-live.icecast.wdr.de:80`、`icecast.unitedradio.it:80`（3局）、`live.radioradicale.it:80` | 30局 |
| **加える必要**（最初のURL） | `fmsetagaya834.out.airtime.pro:8000`、`fm939.wnyc.org:80`、`wfuv-onair.streamguys.org:80`、`ais-sa8.cdnstream1.com:80`、`icecast.rtl.fr:80`、`novazz.ice.infomaniak.ch:80`、`icecast.skyrock.net:80`、`tsfjazz.ice.infomaniak.ch:80`、`dispatcher.rndfnk.com:80`（SWR3、radioeins）、`mp3channels.webradio.antenne.de:80`、`shoutcast2.radio24.it:8000` | 11ホスト（12局） |
| **転送先：今の規則で許可** | `*.rndfnk.com:80`（Deutschlandfunk、DLF Kultur、1LIVE、SWR3、radioeinsの転送先 `f111.rndfnk.com` など） | |
| **転送先：加える必要** | `streaming-ice.audiomeans.fr:80`（RTL、固定のホスト）→ **ホスト:ポートとして**加える。`s<数字>-webradio.antenne.de:80`（Antenne Bayern、数字が毎回変わる：s1・s2・s3・s5を確認）→ ホスト名が `s1-webradio.antenne.de` の形で `*.webradio.antenne.de` には当てはまらないので **`redirect *.antenne.de:80`**。`livestreaming-node-<数字>.srg-ssr.ch:80`（Radio Swiss Jazz、node-2・node-3を確認）→ **`redirect *.srg-ssr.ch:80`** | |

- **重要（U8にも関わる）**：Radio Swiss Jazzの転送先は、U2aの時は同じホストだった（[usb-u2a-design.md](usb-u2a-design.md)）が、今は別のホスト `livestreaming-node-2.srg-ssr.ch` へ転送する【接続】。今のU4の中継の許可にないので、**U8でもUSB中継ではRadio Swiss Jazzが断られる可能性が高い**【推測：中継のコードの照合の決まりから。実機・中継では未確認】。U8の中継は変えない（今回の範囲外）
- 注意：中継の転送の規則は「転送先だけ」に使われ、**最初のURLには使われない**【コード】。`dispatcher.rndfnk.com` は `*.rndfnk.com` に当てはまるが、最初のURLなので `dispatcher.rndfnk.com:80` を許可の一覧に加える必要がある

### 7.2 安全性を保つための方針
- 許可は**ホスト:ポートを1つずつ**。規則（`*.`）は、転送先が毎回変わるものだけ（`*.rndfnk.com`、`*.antenne.de`、`*.srg-ssr.ch`）。どれも放送局・配信網自身のドメインで、ポート80だけ
- `*.cdn.eurozet.pl:80`（Radio ZET）はU9の局に要らない。新しい中継では**外す**（必要以上に広げない）。U8の局のホストもU9の中継の組み込みの一覧から外す（U8を使う人はU4の中継を使う）
- `http://` だけ、転送3回まで、IPアドレスと利用者名の拒否は変えない

### 7.3 中継の版
- 推奨：**U9用の新しい中継**（例 `RadioDaysRelay_U9.command`）。U4の中継のコードの中身は同じで、組み込みの一覧だけをU9の42局に合わせる。U4の中継は変えない（U4〜U8用に残す）
- 別の案：中継は変えず、`allowed_hosts.txt` で足す。ただし次の注意がある

### 7.4 `allowed_hosts.txt` の注意【コード】
- 中継と同じフォルダにこのファイルがあると、**組み込みの一覧と規則を全部やめて、このファイルの行だけ**を使う。今ある局のホストも全部書く必要がある
- 書き方：1行に `host:port`、転送先の規則は `redirect *.domain:port`、`#` 以降はコメント。形が違う行は警告を出して無視
- ファイルを置いたまま別の版の本体を使うと、その版の局が断られることがある

### 7.5 確認の方法
- `perl RadioDaysRelay_U9.command --check-url <URL>`：最初のURLが許可されるか
- `perl RadioDaysRelay_U9.command --check-redirect <URL>`：転送先が許可されるか
- U9の42局の最初のURLと、確認した転送先（第7.1節）を全部この2つにかけ、全部許可・それ以外（例：`https://`、IPアドレス、ほかのドメイン）が断られることを確かめる（U4の `tools/relay-rule-test` と同じ形の試験をU9用に）

## 8. カスタマイズ性の改善案

| 案 | 内容 | ダイヤルへの影響 | 容量 | 評価 |
| --- | --- | --- | --- | --- |
| **A：U9では作りを変えず、READMEで手順を説明** | `freq` の値の表（局数1〜10）と、ビルドの時の検査の意味を書く（[station-customization-review.md](station-customization-review.md) 第4・6節） | なし | なし | **推奨**（安定性を優先） |
| B：Macで `freq` を計算する小さなスクリプト | 局数を入れると `freq` の並びを出す。`tools/` に置く。ファームウェアは変えない | なし | なし（本体の外） | Aと一緒に用意すると良い |
| C：`freq` を自動にする（`freq` の欄をなくし、番号から `rdPos()` で計算） | 局の行を1行足すだけになる | 位置の値は今と同じ式なので変わらない見込み。ただし `freq` を読む箇所（`stationAtFreq()`、`nearestStationFreq()`、`nextStationFreq()`、Locationの切り替え、起動、局が鳴らないときの移動など）を**すべて関数に置き換える**必要があり、ダイヤルとAUTO・MANUALの処理に手が入る | 構造体が4 B×42小さくなる一方、計算の処理が増える。差は**未確認** | U9では**見送り**を推奨（安定性）。やるならU9の後の版で、試験を整えてから |
| D：`freq` の欄は残し、値をマクロで計算 | 例：`{"WNYC", "New York", "http://…", RD_POS(0, 6)}`。局数を書き間違えるとビルドで止まる | なし（値は同じ） | 増えない見込み【推測：コンパイルの時の計算】 | 中間の案。局を足すときに全行の局数を直す必要は残る |

**推奨**：U9では **A＋B**。ビルドの時の検査（`dialsValid()` など）が誤りを書き込み前に止めるので、READMEの手順で十分。Cは後の版の候補。

## 9. 実装手順と作業の分割

| 段階 | 作業 | 確認 |
| --- | --- | --- |
| 0 | 作者の判断（第12節） | ― |
| 1 | `firmware/RadioDays_U8/` を `firmware/RadioDays_U9/` に写し、スケッチの名前を変える。U8のSHA-256が変わらないことを確かめる | U3〜U8のチェックサム |
| 2 | `stations.h`：7 Location・42局、`LOCATION_DEFAULT`、`LOCATION_COUNT == 7` | ビルドの時の検査が通る |
| 3 | `RadioDays_U9.ino`：`n/N` の表示（第5節）と版の名前だけ | 差分がこの2つだけであること（U8との比較） |
| 4 | ホストの試験：U4〜U8の試験をU9用に写し、7 Location・42局・5〜7局のダイヤル・AUTOの移動・`n/N` の表示の判定を確かめる | Macで全部通る |
| 5 | ビルドして大きさを**実測**（0x150000以内） | 実測の値を記録 |
| 6 | U9用のUSB中継（組み込みの一覧の差し替えだけ）と規則の試験 | `--check-url`・`--check-redirect`、規則の試験 |
| 7 | Macから42局の接続の確認（第7.1節の表を更新） | ― |
| 8 | 実機の確認（第10節）。作者が行う | 記録 |
| 9 | 資料：`docs/usb-u9.md`、README・CHANGELOG（実装の段階なので更新する） | ― |

段階2・3（ファームウェア）と段階6（中継）は別のファイルなので分けて進められる。段階4・5は2・3の後。

## 10. 実機検証項目

**Macでの接続の成功は、実機での再生の成功ではない**。以下は作者がCardputer ADVで行う。

| # | 項目 | 確かめること |
| --- | --- | --- |
| 1 | U8の基本動作 | 起動、Wi-Fi・USBの選択、画面、音量、ミュート、時計 |
| 2 | AUTO | 1つ・速く何度も回す（最大3局先）、逆に回す、1周、着いたら待たずに鳴る |
| 3 | MANUAL | 局の間の雑音、局で350 ms後に鳴る、移動中のAでMANUALに、MANUAL→AUTOで近い局へ |
| 4 | Locationの切り替え | 7つすべて、1番目の局に置かれる、時計・空の更新、保存と再起動 |
| 5 | 42局の再生 | 1局ずつ鳴るか（Wi-Fi、USB）。結果を局の表に記録 |
| 6 | 48kHzの局 | France Inter、FIP、France Culture、Skyrock、WQXR、WBGO、Radio Swiss Jazz、ドイツの6局、Radio 105・Monte Carlo・Virgin、大槌 |
| 7 | 低いビットレート・低い周波数 | BBC World Service（56k 24kHz）、RNZ National（64k 24kHz）、Radio 24（64k 22.05kHz）、LBC London（48k）、WNYC（96k 32kHz）、Radio Radicale（64k）、FM Kahoku（96k） |
| 8 | 転送のある局 | RTL、ドイツの6局、Antenne Bayern、Radio Swiss Jazz（Wi-FiとUSB） |
| 9 | USB中継 | U9の中継で42局。U4の中継との取り違えがないこと |
| 10 | 長時間 | 例：1局を2時間以上、Locationを替えながら1時間。途切れ、メモリ（下の行の空きの数字） |
| 11 | 起動時の既定 | 保存なしで既定のLocationと最初の局（第3.1節）。U8でPOLANDなどを保存していた本体で、既定に戻ること |
| 12 | `n/N` の表示 | ダイヤルの局の位置と番号の一致（7 Location）、AUTOの移動中に消える、局の間で消える、Locationを替えると更新、自動で次の局へ進んだときの番号 |

**利用条件の確認**は、再生の確認とは別に行う（第11節）。

## 11. 未確認事項・リスク

| 区分 | 内容 |
| --- | --- |
| 実機 | 42局のうち実機で鳴った記録があるのはRadio Paradise、FIP、KEXP、BBC World Serviceだけ（[stage-3.md](stage-3.md)）。U8自体も実機は未確認（U5は作者が確認、U3は保存版） |
| 容量 | 第6節の数字は推測。ビルドして測るまで確定しない |
| 利用条件（技術とは別） | 全42局で未確認。分かっていること：RNZは放送局自身がURLを公開、大槌はCC BY-NC-SA 4.0（出典の表示、非営利）、SomaFMは個人の利用だけの条件がある（[usb-u4.md](usb-u4.md) 第7節）。radikoのような明確な禁止は、今回の42局では見つかっていない |
| 配信が公式か | 多くの局で、URLが局の公式の案内にあるか未確認（Radio Swiss Jazzは公式のURLが別。[usb-u4.md](usb-u4.md)） |
| 配信の変化 | URLや転送先は予告なく変わる（例：Radio Swiss Jazzの転送先が別のホストに変わった） |
| 中継 | U8＋U4の中継でRadio Swiss Jazzが断られる可能性（第7.1節）。`*.antenne.de`・`*.srg-ssr.ch` の規則がこの先も足りるか |
| 低い音質 | Radio 24、Radio Radicale、LBC London、BBC World Serviceは低いビットレート |
| 時計 | 7 LocationのTZはU7と同じ（実機は未確認） |

## 12. 作者の判断が必要な事項

1. **起動時の既定**：A WORLD（Radio Paradiseから。推奨）／B USA（WNYCから）／C JAPAN
2. **各Locationの並び**（第2節）。特にUK（Radio Xの位置）、WORLD（Radio Paradiseを1番目に）
3. **`n/N` の表示**：場所（状態の行の右、推奨）、局の間・移動中は消す（推奨）か `-/N`、10局以上は出さない（推奨）
4. **USB中継**：U9用の新しい中継を作る（推奨）か、`allowed_hosts.txt` で足すか。`*.antenne.de`・`*.srg-ssr.ch` の規則を加えてよいか
5. **カスタマイズ**：U9ではREADMEの説明＋Macの計算スクリプト（推奨）か、`freq` の自動化まで行うか
6. **U8のRadio Swiss Jazz**：U8＋U4の中継の問題を記録だけにするか（U8は変えない方針）
7. 新しい局の `city` の書き方（画面には出ない）
8. 実機の確認の範囲と、利用条件の問い合わせ（日本の局など）をいつ行うか
