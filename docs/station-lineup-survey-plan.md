# 19のLocationの放送局：今の登録局と、優先して調べる局（調査の計画）

- 作成日：2026-10-22
- 状態：**調査の計画と整理だけ**。`stations.h`、U3〜U8、USB中継は変えていない。局の追加・削除・入れ替えもしていない
- 選局の方針（作者、2026-10-22）：①その国での知名度・聴取率・支持、②毎日聴きたくなる内容、③多様さ、④公式の配信と利用条件、⑤RADIO DAYSで再生できるか。HTTP MP3は絶対の条件ではない
- 2026-10-22追記：順位で並べるダイヤルの構想に照らした再評価は [ranking-dial-feasibility.md](ranking-dial-feasibility.md)
- **2026-10-22 方針の確定**：各Locationの局は**RADIO DAYSが選ぶ局**（人気の順位表ではない）。詳しくは第0節。順位で並べる案（`1st`、`2nd`…）は採用せず、[ranking-dial-feasibility.md](ranking-dial-feasibility.md) は参考の資料として残す
- 各Locationの調査の結果：UK → [uk-station-selection-review.md](uk-station-selection-review.md)
- **2026-10-22 標準収録国の再検討**：標準の版は6か国＋WORLD（JAPAN、USA、UK、FRANCE、GERMANY、ITALY、WORLD）を有力な案とした → [location-selection-review.md](location-selection-review.md)。各国：[japan](japan-station-selection-review.md)、[usa](usa-station-selection-review.md)、[france](france-station-selection-review.md)、[germany](germany-station-selection-review.md)、[italy](italy-station-selection-review.md)。それ以外のLocationの詳しい調査は、作者が残すと決めた場合だけ行う（U8の19 Location・85局は保存）
- 第三者による局の追加・変更：[station-customization-review.md](station-customization-review.md)
- この文書は全体の整理。各Locationの詳しい調査（聴取率の資料、公式の配信方式、接続の確認）は、この順番で後から行う

## 0. 選局の方針（2026-10-22 確定）

1. **RADIO DAYSが選ぶ局**：人気の順位をそのまま写さない。知名度・聴取率、もう一度聴きたくなる内容、その国らしさ、種類の多様さ、RADIO DAYSで再生できるか、公式の配信と利用条件を合わせて選ぶ
2. **局数は自由**：Locationごとに5局、4局、6局などでよい。何度も聴きたい局だけを入れる
3. **表示は「今の位置／局数」**：`1/5`、`2/5`…`5/5`（循環）。**順位ではない**。並びは人気の順ではなく、聴いて楽しい順（同じ種類の局が長く続かないように）。表示の実装は、調査の結果を見てから作者が指示する（U8はそのまま）
4. **順位の調査は続ける**：RAJAR、Médiamétrieなどの聴取率は、候補を探す手段として使う。公開の順位がない国（JAPAN、THAI、TAIWANなど）も、知名度と内容で**同じ重さで**調べる
5. **人気と再生できるかは別の項目**：今のRADIO DAYSで鳴らせない人気の局も調査から外さず、「将来の技術の拡張の候補」として記録する。技術的に再生できることと、利用条件で認められていることも別に評価する
6. **特定の放送局のグループを除かない**：以前のNHK、フジサンケイグループを除く方針は撤回。すべて同じ基準で評価する
7. **今までの調査は残す**：この文書の第2節以下と、各国の資料はそのまま
8. Macでの接続の確認は、Cardputer ADVでの実機の確認ではない。確かめられなかったことは「未確認」と書く

### 0.1 表示を作るときの前提（U8の実装を確認、2026-10-22）

| 操作 | U8の受信開始の条件（`RadioDays_U8.ino`） |
| --- | --- |
| AUTO：ダイヤルを回す | `glideAdd()` が流れる移動を始め、その時点で `REQ_NOISE` を送って今の局を止める。一定の速さ（20 msに1ステップ）で次の局へ動き、先の局は最大3つまで。**着いたら待たずに**その局を要求（`glideService()`） |
| MANUAL：ダイヤルを回す | 局の外に出たら、その場で `REQ_NOISE`（雑音）。局の位置に止まったら、**350 ms（`TUNE_SETTLE_MS`）止まってから**局を要求（`tuneService()`） |
| 流れる途中でAを押す | 移動を止めてMANUALに。止まった所が局なら350 ms後に要求、局の間なら雑音 |
| MANUAL→AUTO（局の間にいる） | いちばん近い局へ跳び、350 ms後に要求 |
| 流れる途中 | `tuneService()` は何もしない（移動の側が要求する） |

`n/N` の表示を加える場合、局名の表示と同じく「流れる途中は出さない」かどうかを決める必要がある。**コードの大きさの増え方は、実際にビルドして測るまで断定しない**（U8の残りは1,856 B）。

## 1. 書き方

| 印 | 意味 |
| --- | --- |
| 【一般・要確認】 | 広く知られていることだが、この段階では資料で確かめていない。詳しい調査で根拠を確かめる |
| 【推測・要確認】 | 配信方式や再生できるかの見込み。詳しい調査で接続して確かめる |
| 聴取率の資料 | 各国の公式な聴取率調査の名前（【一般・要確認】）。詳しい調査ではこれを第一の根拠にする |

**Radio Browser（局の一覧サービス）の投票数・クリック数は、その国での知名度・聴取率とは別のもの**として扱う（世界中の一覧の利用者の数で、国内の聴取の実績ではない）。使う場合は「Radio Browserの投票数」と明記し、知名度の根拠にはしない。

## 2. 今の登録局（U8、19のLocation・85局）

`firmware/RadioDays_U8/stations.h` から作った一覧。どれも今のRADIO DAYSで再生できる `http://` のMP3（Macからの接続で確認済み。Cardputerでの再生は多くが未確認）。

| Location（代表都市） | 局（局の都市） | 局数 |
| --- | --- | --- |
| JAPAN（Tokyo） | Shonan Beach FM（Zushi）、FM Kahoku（Kahoku）、OTTAVA（Tokyo）、Japan Hits（Toronto）、J1 Gold（Tokyo）、NHK WORLD (WRN)（Tokyo）、Otsuchi Seashore（Iwate） | 7 |
| USA（New York） | Radio Paradise（California）、WQXR（New York）、KEXP（Seattle）、WFMU（Jersey City）、181.fm Classical（USA） | 5 |
| UK（London） | Classic FM（London）、Capital London（London）、Smooth London（London）、Radio Caroline（Essex）、BBC World Service（London） | 5 |
| FRANCE（Paris） | France Inter（Paris）、FIP（Paris）、France Musique（Paris）、France Culture（Paris）、FIP Jazz（Paris） | 5 |
| WORLD（Everywhere） | Radio Swiss Jazz（Bern）、RNZ National（Wellington）、Groove Salad（San Francisco）、Secret Agent（San Francisco）、Nightwave Plaza（Internet） | 5 |
| GERMANY（Berlin） | Deutschlandfunk（Cologne）、DLF Kultur（Berlin）、1LIVE（Cologne）、WDR 5（Cologne）、Hit Radio FFH（Frankfurt） | 5 |
| HOLLAND（Amsterdam） | NPO Radio 1（Hilversum）、NPO Radio 2（Hilversum）、NPO 3FM（Hilversum）、NPO Klassiek（Hilversum）、Radio Continu（Groningen） | 5 |
| POLAND（Warsaw） | PR Jedynka（Warsaw）、PR Dwojka（Warsaw）、PR Radio Chopin（Warsaw）、RMF FM（Krakow）、Radio ZET（Warsaw） | 5 |
| CZECHIA（Prague） | CRo Radiozurnal（Prague）、CRo Dvojka（Prague）、CRo Vltava（Prague）、Radio Beat（Prague）、Radio Blanik（Prague） | 5 |
| SWEDEN（Stockholm） | Pirate Rock（Gothenburg）、Country Rocks（Sweden）、FUN RADIO 95.3（Sweden）、Radio Sotenas（Sotenas） | 4 |
| ITALY（Rome） | Virgin Radio Italia（Milan）、Radio 105（Milan）、Radio Monte Carlo（Milan）、Radio Radicale（Rome） | 4 |
| HUNGARY（Budapest） | Kossuth Radio（Budapest）、Petofi Radio（Budapest）、Radio 1（Budapest） | 3 |
| THAI（Bangkok） | FM 94 Bangkok（Bangkok）、Smooth 105.5（Bangkok）、Voice of Navy 88.5（Bangkok） | 3 |
| TAIWAN（Taipei） | City FM 92.9（Taipei）、City FM 90.1（Taichung）、City FM 97.1（Tainan）、Family 97.7（Taichung） | 4 |
| AUSSIE（Sydney） | Kiss FM（Melbourne）、Three D Radio（Adelaide）、Skylab Radio（Melbourne） | 3 |
| NZ（Wellington） | RNZ National（Wellington）、RNZ Concert（Wellington）、RNZ Pacific（Auckland）、The Most FM（Christchurch）、Compass FM（Kerikeri） | 5 |
| CANADA（Ottawa） | Soft Rock 97.7（Calgary）、CHOI Radio X（Quebec City）、DJFM（Toronto） | 3 |
| BRAZIL（Brasilia） | Radio Batuta（Rio de Janeiro）、Batuta Classicos（Rio de Janeiro）、91 Rock（Curitiba）、Continental FM（Porto Alegre） | 4 |
| ARGENT（Buenos Aires） | Nacional Folklorica（Buenos Aires）、Nacional Rock（Buenos Aires）、Nacional Clasica（Buenos Aires）、Radio Nacional 870（Buenos Aires）、Nacional Cordoba（Cordoba） | 5 |

## 3. 各Locationで優先して調べる局

「今と重なる」は、今の登録局に含まれている局。「配信の見込み」は【推測・要確認】。

### JAPAN
- 聴取率の資料：ビデオリサーチ（首都圏ラジオ調査など）【一般・要確認】。全局の順位表は公開されていない（[japan-popular-stations-review.md](japan-popular-stations-review.md)）
- 優先して調べる局：NHKラジオ第1、NHK-FM（全国）、TBSラジオ、ニッポン放送、文化放送、TOKYO FM、J-WAVE（東京）、MBSラジオ、ABCラジオ、FM802（大阪）、CBCラジオ（名古屋）、各地の県域FM（bayfm、NACK5、FMヨコハマ、ZIP-FM）
- 今と重なる：なし（今の7局は、地域局・ネット局が中心）
- 配信の見込み：主要局はradiko（HTTPS・HLS・AAC）で、**今のRADIO DAYSでは再生できない**（確認済み、上の資料）。radiko対応は保留。Cardputer単独でradikoを受信する先行事例（Cardputer-Adv-Radiko）があり、技術的な実現性と利用規約上の問題を分けて評価する（[cardputer-adv-radiko-review.md](cardputer-adv-radiko-review.md)）

### USA
- 聴取率の資料：Nielsen Audio【一般・要確認】
- 優先して調べる局：WNYC（NPR、ニューヨーク）、WINS 1010（ニュース）、Z100（ヒット曲）、WFAN（スポーツ）、KIIS FM（ロサンゼルス）、KQED（NPR、サンフランシスコ）、WQXR、KEXP
- 今と重なる：WQXR、KEXP
- 配信の見込み：iHeartやAudacyの局はHTTPS・HLSが多い、NPRの局はMP3の配信を持つ局がある【推測・要確認】
- 注意：国が大きく地域ごとに市場が違う。代表都市（ニューヨーク）を中心にするか、全国の知名度で選ぶかを決める

### UK
- 聴取率の資料：RAJAR【一般・要確認】
- 優先して調べる局：BBC Radio 2、BBC Radio 4、BBC Radio 1、Heart、Capital、Classic FM、LBC、Greatest Hits Radio、Smooth、Kiss、Absolute Radio、Times Radio
- 今と重なる：Classic FM、Capital（London）、Smooth（London）
- 配信の見込み：BBCの国内向けはHLS・AAC中心、Globalの局（Heart、Capital、Classic FM、Smooth、LBC）は今と同じMP3の配信がある見込み【推測・要確認】
- **調査済み（2026-10-22）**：[uk-station-selection-review.md](uk-station-selection-review.md)。推奨6局（World Service、Heart、LBC London、Classic FM、Capital、Radio Caroline）。BBCの国内向けは今のRADIO DAYSでは再生できないことを確認

### FRANCE
- 聴取率の資料：Médiamétrie（EAR National）【一般・要確認】
- 優先して調べる局：France Inter、RTL、France Info、RMC、Europe 1、NRJ、Nostalgie、Fun Radio、Skyrock、RTL2、Chérie FM
- 今と重なる：France Inter（ほかの4局もRadio Franceの局）
- 配信の見込み：Radio Franceは今と同じMP3、民間の局もMP3の配信を持つ局が多い【推測・要確認】

### WORLD
- **国の聴取率がない枠**。今は作品として選んだ5局（スイスのジャズ、ニュージーランドの公共放送、サンフランシスコのネット局2、ネット局1）
- 方針：「各国の人が毎日聴く局」とは別の、選んで集めた枠として扱う。見直す場合は、国際放送（BBC World Serviceなど）や世界的に知られたネット局をどう入れるかを作者が決める

### GERMANY
- 聴取率の資料：ma Audio（agma）【一般・要確認】
- 優先して調べる局：Bayern 1、SWR3、WDR 2、1LIVE、NDR 2、Antenne Bayern、radio NRW系の局、Bayern 3、hr3、Hit Radio FFH、Deutschlandfunk
- 今と重なる：1LIVE、Hit Radio FFH、Deutschlandfunk（DLF Kultur、WDR 5も公共放送）
- 配信の見込み：ARDの局はMP3（転送あり。U4の中継で対応済み）、民間の局もMP3が多い【推測・要確認】

### HOLLAND
- 聴取率の資料：NLO（Nationaal Luisteronderzoek）【一般・要確認】
- 優先して調べる局：NPO Radio 2、Qmusic、Radio 538、Sky Radio、Radio 10、NPO Radio 1、100% NL、NPO 3FM
- 今と重なる：NPO Radio 2、NPO Radio 1、NPO 3FM
- 配信の見込み：NPOはMP3（今と同じ）、民間の大きな局（Qmusic、538、Sky Radio）はHTTPS・AACが多い【推測・要確認】

### POLAND
- 聴取率の資料：Radio Track【一般・要確認】
- 優先して調べる局：RMF FM、Radio ZET、Polskie Radio Jedynka、Trójka、Radio Eska、TOK FM、Radio Złote Przeboje
- 今と重なる：RMF FM、Radio ZET、Jedynka
- 配信の見込み：今の局と同じMP3が多い。TrójkaはU4のときに接続できなかった【推測・要確認】

### CZECHIA
- 聴取率の資料：Radioprojekt【一般・要確認】
- 優先して調べる局：Rádio Impuls、Evropa 2、Frekvence 1、ČRo Radiožurnál、ČRo Dvojka、Kiss、Rádio Blaník
- 今と重なる：Radiožurnál、Dvojka、Blaník
- 配信の見込み：民間の局もMP3の配信が多い【推測・要確認】

### SWEDEN
- 聴取率の資料：Kantar Sifo【一般・要確認】
- 優先して調べる局：Sveriges Radio P4、P1、P3、Mix Megapol、Rix FM、NRJ、Star FM、Bandit Rock
- 今と重なる：なし
- 配信の見込み：Sveriges RadioはHTTPSだけ（U4の調査で確認）、Bauerの局はU4のときに接続できなかった。**今は再生しにくい局が多い**【推測・要確認】

### ITALY
- 聴取率の資料：TER（Tavolo Editori Radio）【一般・要確認】
- 優先して調べる局：RTL 102.5、Radio Deejay、RDS、Rai Radio 1・Radio 2、Radio 105、Radio Italia、Radio Kiss Kiss、Virgin Radio、Radio 24
- 今と重なる：Radio 105、Virgin Radio
- 配信の見込み：RaiのMP3は256〜320kbps（U4で除外）。民間の局は調べる【推測・要確認】

### HUNGARY
- 聴取率の資料：（要確認）
- 優先して調べる局：Retro Rádió、Rádió 1、Kossuth Rádió、Petőfi Rádió、Sláger FM
- 今と重なる：Rádió 1、Kossuth、Petőfi
- 配信の見込み：今の局はMP3。Retro Rádió・Sláger FMは調べる【推測・要確認】

### THAI
- 聴取率の資料：Nielsen（タイ）【一般・要確認】
- 優先して調べる局：Cool Fahrenheit 93、Green Wave 106.5、EFM 94、Chill 89、Virgin Hitz、MCOTの局（FM 100.5 ニュース、Smooth 105.5）
- 今と重なる：Smooth 105.5（FM 94 Bangkokが EFM 94 と同じかは要確認）
- 配信の見込み：大手はアプリ・HLSの配信が多い【推測・要確認】

### TAIWAN
- 聴取率の資料：（要確認。公開された調査があるか不明）
- 優先して調べる局：Hit FM、Kiss Radio、ICRT、News98、中廣（BCC）、警察廣播電台、好事聯播網、UFO Radio
- 今と重なる：なし（今は台北・台中・台南のCity FMと古典音楽の局）
- 配信の見込み：大手は自社のアプリ・HLSが多い【推測・要確認】

### AUSSIE
- 聴取率の資料：GfK（商業ラジオの調査）【一般・要確認】
- 優先して調べる局：ABC Radio Sydney、ABC Classic、triple j、KIIS 106.5、smoothfm、2GB、WSFM、Nova、Triple M
- 今と重なる：なし（今はコミュニティ・ネット局）
- 配信の見込み：ABCは日本から403（U4の調査で確認）、民間の大手はアプリ・HLSが多い【推測・要確認】

### NZ
- 聴取率の資料：GfK（商業ラジオの調査）【一般・要確認】
- 優先して調べる局：RNZ National、Newstalk ZB、The Edge、ZM、The Breeze、More FM、The Rock、Mai FM
- 今と重なる：RNZ National（RNZ Concert、RNZ Pacific）
- 配信の見込み：RNZは公式にMP3を公開（確認済み）。民間の大手はアプリ・HTTPSが多い【推測・要確認】

### CANADA
- 聴取率の資料：Numeris【一般・要確認】
- 優先して調べる局：CBC Radio One、CBC Music、ICI Radio-Canada Première、CHUM 104.5、CHFI、98.5 Montréal、NewsTalk 1010、Virgin Radio
- 今と重なる：なし（CHOI Radio Xはケベック市で知られた局【一般・要確認】）
- 配信の見込み：CBCは `http://` のMP3が見つからなかった（U4の調査）。民間は調べる【推測・要確認】

### BRAZIL
- 聴取率の資料：Kantar IBOPE Media【一般・要確認】
- 優先して調べる局：Jovem Pan、CBN、Band FM、Antena 1、Nativa FM、Rádio Gaúcha、Mix FM、Alpha FM
- 今と重なる：なし（今は文化団体の局とロック・地域の局）
- 配信の見込み：大手はHTTPS・HLSが多い【推測・要確認】

### ARGENT
- 聴取率の資料：Kantar IBOPE Media【一般・要確認】
- 優先して調べる局：Radio Mitre、Cadena 3、La 100、Radio Rivadavia、Radio con Vos、Aspen、Metro、Radio Nacional
- 今と重なる：Radio Nacional（今の5局はすべてRadio Nacional）
- 配信の見込み：民間の大手は調べる【推測・要確認】

## 4. 詳しい調査の順番（今の局と、人気の局の差が大きい順）

| 順 | Location | 理由（今の時点の見立て） |
| --- | --- | --- |
| 1 | SWEDEN | 今の4局は小さな局・ネット局だけで、人気の局と重ならない。主要局の再生にも難しさがある |
| 2 | TAIWAN | 今の4局は大手のネットワークを含まない |
| 3 | AUSSIE | 今の3局はコミュニティ・ネット局。ABCは日本から聴けない |
| 4 | BRAZIL | 今の4局は大手を含まない |
| 5 | THAI | 大手の音楽局を含まない |
| 6 | JAPAN | 主要局がradikoで、今のRADIO DAYSでは再生できない（調査済み。方針は作者が決める） |
| 7 | HOLLAND | 民間の大手（Qmusic、538、Sky Radio）を含まない |
| 8 | ITALY | 最大手（RTL 102.5、Deejay、RDS）を含まない |
| 9 | CZECHIA | 民間の最大手（Impuls、Evropa 2、Frekvence 1）を含まない |
| 10 | ARGENT | 公共放送だけで、民間の大手を含まない |
| 11 | CANADA | 公共放送（CBC）と大都市の大手を含まない |
| 12 | NZ | RNZはあるが民間の大手を含まない |
| 13 | USA | 知られた局はあるが、ニュース・ヒット曲の大手を含まない |
| 14 | HUNGARY | 公共放送とRádió 1はある。Retro Rádióなどを確かめる |
| 15 | GERMANY | 公共放送と大手の一部がある |
| 16 | POLAND | 上位2局と公共放送がある |
| 17 | UK | Globalの大手がある。BBCの国内向けの扱いを確かめる |
| 18 | FRANCE | 公共放送の主要局がある。民間の大手を確かめる |
| ― | WORLD | 国の聴取率がない枠。別に方針を決める |

## 5. 各Locationの詳しい調査で確かめること

1. その国の聴取率の資料で、上位の局を確かめる（資料の名前、時期、指標。公開されていなければ限界を書く）
2. 今の登録局と比べる
3. 公式の配信（局のサイトが案内する方法、配信方式：MP3・AAC・HLS、HTTP・HTTPS、地域の制限、アプリだけか）
4. 今のRADIO DAYSで再生できるか（このMacから接続して確かめる。Cardputerは別）
5. 再生できない場合の理由
6. 利用条件の記載
