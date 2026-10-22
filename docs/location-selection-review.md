# 標準収録国の再検討

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。U3〜U8のファームウェア、`stations.h`、USB中継、README、CHANGELOGは変えていない。U8の19 Location・85局はそのまま保存する
- 方針：RADIO DAYSは世界中の局を網羅するラジオではなく、作者が聴きたい国と魅力的な局を選び、アナログダイヤルで楽しむ作品。国数・局数の多さは目的にしない（[station-lineup-survey-plan.md](station-lineup-survey-plan.md) 第0節）
- 各国の調査：[japan-station-selection-review.md](japan-station-selection-review.md)、[usa-station-selection-review.md](usa-station-selection-review.md)、[uk-station-selection-review.md](uk-station-selection-review.md)、[france-station-selection-review.md](france-station-selection-review.md)、[germany-station-selection-review.md](germany-station-selection-review.md)、[italy-station-selection-review.md](italy-station-selection-review.md)
- 改造の方法：[station-customization-review.md](station-customization-review.md)
- 書き方：接続の確認はこのMac（日本の回線）から2026-10-21〜22 UTCに行ったもので、**Cardputer ADVでの実機の確認ではない**。確かめられなかったことは「未確認」

## 1. 今の19 Location（U8の `stations.h`）

| # | Location | 代表都市 | 局数 | 今の局 |
| --- | --- | --- | --- | --- |
| 1 | JAPAN | TOKYO | 7 | Shonan Beach FM、FM Kahoku、OTTAVA、Japan Hits、J1 Gold、NHK WORLD (WRN)、Otsuchi Seashore |
| 2 | USA | NEW YORK | 5 | Radio Paradise、WQXR、KEXP、WFMU、181.fm Classical |
| 3 | UK | LONDON | 5 | Classic FM、Capital London、Smooth London、Radio Caroline、BBC World Service |
| 4 | FRANCE | PARIS | 5 | France Inter、FIP、France Musique、France Culture、FIP Jazz |
| 5 | WORLD | EVERYWHERE | 5 | Radio Swiss Jazz、RNZ National、Groove Salad、Secret Agent、Nightwave Plaza |
| 6 | GERMANY | BERLIN | 5 | Deutschlandfunk、DLF Kultur、1LIVE、WDR 5、Hit Radio FFH |
| 7 | HOLLAND | AMSTERDAM | 5 | NPO Radio 1、NPO Radio 2、NPO 3FM、NPO Klassiek、Radio Continu |
| 8 | POLAND | WARSAW | 5 | PR Jedynka、PR Dwojka、PR Radio Chopin、RMF FM、Radio ZET |
| 9 | CZECHIA | PRAGUE | 5 | CRo Radiozurnal、CRo Dvojka、CRo Vltava、Radio Beat、Radio Blanik |
| 10 | SWEDEN | STOCKHOLM | 4 | Pirate Rock、Country Rocks、FUN RADIO 95.3、Radio Sotenas |
| 11 | ITALY | ROME | 4 | Virgin Radio Italia、Radio 105、Radio Monte Carlo、Radio Radicale |
| 12 | HUNGARY | BUDAPEST | 3 | Kossuth Radio、Petofi Radio、Radio 1 |
| 13 | THAI | BANGKOK | 3 | FM 94 Bangkok、Smooth 105.5、Voice of Navy 88.5 |
| 14 | TAIWAN | TAIPEI | 4 | City FM 92.9、City FM 90.1、City FM 97.1、Family 97.7 |
| 15 | AUSSIE | SYDNEY | 3 | Kiss FM、Three D Radio、Skylab Radio |
| 16 | NZ | WELLINGTON | 5 | RNZ National、RNZ Concert、RNZ Pacific、The Most FM、Compass FM |
| 17 | CANADA | OTTAWA | 3 | Soft Rock 97.7、CHOI Radio X、DJFM |
| 18 | BRAZIL | BRASILIA | 4 | Radio Batuta、Batuta Classicos、91 Rock、Continental FM |
| 19 | ARGENT | BUENOS AIRES | 5 | Radio Nacional の5局 |
| | **合計** | | **85** | 18か国＋WORLD |

## 2. 6か国＋WORLD案の評価

| Location | 人気の局を今のRADIO DAYSで鳴らせるか | 推奨局数（案） | 評価 |
| --- | --- | --- | --- |
| JAPAN | **できない**。主要局（NHK、民放のAM・FM）はradiko・らじる★らじる（HTTPS・HLS・AAC）だけ | 5 | **問題あり**：知名度の高い局を確保できない。地域局・ネット局・環境音で「日本らしさ」を出す構成になる。作者の国なので残す価値は高い |
| USA | **一部**。ニューヨークの上位の商業局（WLTW、WINS、WCBS-FM、Z100、Q104.3）はiHeart・Audacy（HTTPS・AAC・HLS）で鳴らせない。公共・大学の局（WNYC、WQXR、WFUV、WBGO、WFMU、KEXP）は鳴らせる | 6 | 良い：アメリカの公共・大学ラジオという特色ではっきりした構成になる |
| UK | **かなりできる**。Global（Heart、Capital、Classic FM、LBC、Radio X）とBBC World Service。BBCの国内向けはできない | 6（Radio Xを入れて7） | 良い |
| FRANCE | **よくできる**。上位10局のうち7局（France Inter、RTL、franceinfo、Skyrock、RMC、Europe 1、Nova）と France Culture | 7 | **とても良い**：人気と再生できることが最も重なる |
| GERMANY | **よくできる**。ARDの局（SWR3、WDR 2、Bayern 1、1LIVE、NDR 2…）、Antenne Bayern、Deutschlandfunk。ほとんどが `*.rndfnk.com` へ転送 | 6 | 良い。ただし局の多くは州の局で、「全国の1位」はない |
| ITALY | **弱い**。上位3局（RTL 102.5、Radio Italia、RDS）とDeejayはHLS・AAC・256k、Raiは256〜320k（作者の方針の上限192kを超える）。鳴らせるのはMediasetの4局、Radio 24、Radio Radicale | 5 | **問題あり**：人気の上位と公共放送を入れられない |
| WORLD | 国の順位がない枠。今の5局はどれも鳴る | 5（Radio Paradiseを移すなら6） | 良い |

**合計**：7 Location・40局（UKを7局、WORLDを6局にすると42局）。

## 3. ほかに残す価値のある国

| Location | 理由 | 評価 |
| --- | --- | --- |
| **POLAND** | 今の5局に、聴取率1位RMF FM（28%台）と2位Radio ZETが入っていて、公共放送（Jedynka、Dwojka、Chopin）も鳴る。人気と再生できることの重なりが、6か国のどれにも負けない | **残す第一候補**（作者が聴くかどうかで判断） |
| NZ | RNZの局は、**放送局自身が公開したURL**（85局で唯一の例）。利用の面で最も安心できる。ただし民間1位のNewstalk ZBは含まない | 候補。外す場合もRNZ NationalはWORLDに残る |
| HOLLAND | NPOの4局（192k）は公共放送の主要局。民間の上位（Qmusic、538、Sky Radio）は未調査 | 候補 |
| CZECHIA | 公共放送ČRoの3局は鳴る。民間の上位（Impuls、Evropa 2）は今の局に入っていない | 保留 |
| SWEDEN、THAI、TAIWAN、AUSSIE、CANADA、BRAZIL、ARGENT、HUNGARY | 主要局と今の局が重ならない、または局数が少ない（[station-lineup-survey-plan.md](station-lineup-survey-plan.md) 第4節） | 外してよい（U8には残る） |

## 4. 収録国を減らすメリットとデメリット

| メリット | デメリット |
| --- | --- |
| 1局ずつ、人気・内容・配信・利用条件・実機の再生を確かめられる量になる | 「世界中を旅する」感覚と、地球儀の印の数が減る |
| 局のURLが変わったときの保守が軽い（85局 → 約40局） | 今まで調べた国の局は、標準の版からは聴けなくなる（U8には残る） |
| 作者が実際に聴く国に集中できる | 保存されたLocationが消えた国だった場合、起動時に既定のLocation（USA）に戻る（`savedLocationIndex()`。不具合ではないが、使っていた人には変化） |
| Location画面の一覧が短くなる（5行ずつの表示で2ページ） | |
| 外した国は、利用者が自分で戻せる（[station-customization-review.md](station-customization-review.md)） | |

## 5. 推奨する標準構成

**7 Location（6か国＋WORLD）を推奨**。POLANDは「作者が聴くなら8つ目」として残す価値が高い。

| 順 | Location | 局数 | 局（ダイヤルの順） |
| --- | --- | --- | --- |
| 1 | JAPAN | 5 | 湘南ビーチFM → FM世田谷 → OTTAVA → FMかほく → 大槌・海辺 |
| 2 | USA | 6 | WNYC → WFUV → WQXR → WBGO → WFMU → KEXP |
| 3 | UK | 6 | BBC World Service → Heart London → LBC London → Classic FM → Capital London → Radio Caroline（7局案：Radio Xを加える） |
| 4 | FRANCE | 7 | France Inter → FIP → RTL → Radio Nova → France Culture → Skyrock → TSF Jazz |
| 5 | WORLD | 5 | 今の5局（Radio Paradiseを移すなら6） |
| 6 | GERMANY | 6 | Deutschlandfunk → SWR3 → DLF Kultur → radioeins → 1LIVE → Antenne Bayern |
| 7 | ITALY | 5 | Radio 24 → Radio 105 → Radio Radicale → Radio Monte Carlo → Virgin Radio |

- 順は今のLocationの順を保つ（JAPAN、USA、UK、FRANCE、WORLDの5つは保存の記録の都合で名前と位置を変えない。GERMANY、ITALYはWORLDの後ろ）
- 局の並びは人気の順ではない。言葉の局と音楽の局、音楽の種類が続かないように並べた。表示は `1/5` などの「今の位置／局数」（実装は後で）
- 局の入れ替えの詳細と理由は各国の資料にある
- 実装する場合はU8を上書きせず、新しい版（例：U9）として作る

## 6. 今の85局から残す・外す・加える

| Location | 残す | 外す | 加える |
| --- | --- | --- | --- |
| JAPAN | 湘南ビーチFM、FMかほく、OTTAVA、Otsuchi Seashore | Japan Hits、J1 Gold、NHK WORLD (WRN)（保留の候補に残す） | FM世田谷 |
| USA | WQXR、KEXP、WFMU | 181.fm Classical、Radio Paradise（WORLDへ移す案） | WNYC、WFUV、WBGO |
| UK | Classic FM、Capital London、Radio Caroline、BBC World Service | Smooth London | Heart London、LBC London（＋Radio X 案） |
| FRANCE | France Inter、FIP、France Culture | France Musique、FIP Jazz | RTL、Radio Nova、Skyrock、TSF Jazz |
| WORLD | 5局すべて | ― | （Radio Paradise 案） |
| GERMANY | Deutschlandfunk、DLF Kultur、1LIVE | WDR 5、Hit Radio FFH | SWR3、radioeins、Antenne Bayern |
| ITALY | Virgin Radio Italia、Radio 105、Radio Monte Carlo、Radio Radicale | ― | Radio 24 |
| ほかの12 Location | ― | 49局（U8には残る） | ― |

残す局 26、外す局 10（7 Locationの中）＋12 Locationの49局、加える局 14。合計40局（Radio X、Radio ParadiseのWORLDへの移動を除く）。

## 7. 実装するときに必要なこと（今回はしない）

1. 新しい版のフォルダ（例：`firmware/RadioDays_U9/`）。U8は変えない
2. `stations.h`：局の表と `LOCATIONS` の組み替え、各局の `freq` の計算し直し、`static_assert(LOCATION_COUNT == 19 …)` の数を7に（第5節の局数でダイヤルの長さは自動で決まる：5局→90段、6局→120段、7局→150段）
3. USB中継：新しい局のホスト（`fm939.wnyc.org:80`、`icecast.rtl.fr:80` と転送先の `streaming-ice.audiomeans.fr:80`、`dispatcher.rndfnk.com:80`、`*.antenne.de:80` の転送の規則など）を許可の一覧に加えた新しい中継（または利用者が `allowed_hosts.txt` で加える）
4. `n/N` の表示（作者の指示を待つ）。大きさはビルドして測る
5. Macでの接続の確認のあと、**実機での再生の確認**（特に48kHz、低いビットレート、転送のある局）
6. README・CHANGELOGは実装の時に更新

## 8. 作者に決めていただきたいこと

1. 標準構成：7 Location（推奨）か、POLANDを加えて8か
2. JAPANとITALYは、人気の局を確保できないことを承知で残すか
3. 各国の局の案（各国の資料）。UKのRadio X、WORLDへのRadio Paradiseの移動
4. 外したLocationの扱い（U8に残すだけか、改造の例として別のファイルに残すか。[station-customization-review.md](station-customization-review.md) 第6節）
