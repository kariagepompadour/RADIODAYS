# UK（London）の選局の調査

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。`stations.h`、U3〜U8、USB中継、README、CHANGELOGは変えていない。今のUKの5局もそのまま
- 方針：[station-lineup-survey-plan.md](station-lineup-survey-plan.md) の「選局の方針」（RADIO DAYSが選ぶ局、局数は自由、表示は n/N で順位ではない）
- 書き方：【RAJAR】＝RAJARの公表値、【接続】＝このMacからの接続の確認（2026-10-21 UTCに2回）、【資料】＝報道・公式ページ、【推測】＝推測、**未確認**＝今回確かめられなかったこと
- **Macでの接続の確認は、Cardputer ADVでの実機の確認ではない**

## 1. 主要な局と根拠

### 1.1 根拠の資料

| 項目 | 内容 |
| --- | --- |
| 調査機関 | RAJAR（Radio Joint Audience Research）。表記は「RAJAR/Ipsos/RSMB」 |
| 資料 | 2026年第2四半期の要約の表（Quarterly Summary Figures、2026年8月6日発表） |
| 指標 | 週の到達（平均的な1週間に5分以上聴いた人の数、15歳以上）と、聴取時間の占有率（Share、その局の調査地域の中） |
| 地域 | UK全体（人口59,047千人）。London版の局はLondonの地域（13,106千人） |
| 期間 | Q＝2026年4月6日〜6月28日の3か月、H＝2026年1月5日〜6月28日の6か月（局ごとにどちらかで発表） |

### 1.2 週の到達の多い順（UK全体、単位は千人）【RAJAR】

| 順 | 局（RAJARの表の名前） | 期間 | 週の到達 | 到達率 | 占有率 |
| --- | --- | --- | --- | --- | --- |
| 1 | Heart Brand (UK) | H | 12,826 | 22% | 8.1% |
| 2 | BBC Radio 2 | Q | 12,367 | 21% | 13.3% |
| 3 | Hits Radio Portfolio（Bauerの局の集計） | H | 12,240 | 21% | 10.3% |
| 4 | BBC Radio 4 | Q | 8,934 | 15% | 10.4% |
| 5 | Capital Brand (UK) | H | 8,864 | 15% | 4.7% |
| 6 | Smooth Brand (UK) | H | 7,374 | 12% | 5.0% |
| 7 | BBC Radio 1 | Q | 6,904 | 12% | 4.2% |
| 8 | Greatest Hits Radio | H | 5,794 | 10% | 5.4% |
| 9 | BBC Radio 5 Live | Q | 5,503 | 9% | 3.1% |
| 10 | Absolute Radio Network | H | 5,398 | 9% | 3.5% |
| 11 | Classic FM | H | 4,286 | 7% | 3.4% |
| 12 | KISS Network | H | 3,738 | 6% | 1.8% |
| 13 | LBC Brand (UK) | H | 3,688 | 6% | 3.0% |
| 14 | talkSPORT | H | 3,321 | 6% | 2.2% |
| 15 | Magic Network | H | 3,299 | 6% | 1.7% |

参考（16位以下の一部）：BBC 6 Music 2,586（2.4%）、Radio X Brand 2,411（1.9%）、BBC Radio 3 1,881（1.5%）、Gold 1,535（1.1%）、Virgin Radio 1,008、BBC World Service 943（0.5%）、GB News Radio 731、Boom Radio 635、Times Radio 660、Kerrang! 615、Jazz FM 406。Londonの地域では LBC London 1,400（占有率7.8%）【RAJAR】。

**注意**
- 「Brand」「Network」「Portfolio」は、複数の放送（全国版・地域版・派生の局）をまとめた数字。RADIO DAYSで鳴らすのはその中の1本の配信
- Radio Carolineは、この要約の表に**載っていない**（RAJARで計られていない。人気が低いという意味ではない）
- 上の順は候補を探すための参考。**RADIO DAYSの表示（n/N）とは関係しない**

## 2. 比較表

再生：◎＝今のRADIO DAYSの条件（HTTP、MP3、192k以下、IPアドレスでないURL）に合う配信が応答した【接続】、×＝合う配信が見つからない。実機は**すべて未確認**（BBC World Serviceだけは過去の版で実機で鳴っている。stage-3.md）。

| 局 | 人気（UK全体の週の到達） | 内容 | 国らしさ | 再生 | 利用条件 | 安定性 | 判定 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| BBC World Service | 943千 | 国際ニュース、ドキュメンタリー（英語） | 高い（BBCの国際放送） | ◎ MP3 56k 24kHz | 未確認 | 長年同じURL（Stage 2-1から） | **推奨**（今と同じ） |
| Heart（London／UK） | 12,826千（Brand） | 大人向けのポップ | 中（全国の民放の代表） | ◎ MP3 128k | 未確認 | 未確認 | **推奨** |
| BBC Radio 2 | 12,367千 | 大人向けのポップ、名物の司会 | 高い | × HLS・HTTPSだけ | 未確認 | ― | 将来の技術 |
| Hits Radio | 12,240千（集計） | ヒット曲 | 中 | × | 未確認 | ― | 将来の技術 |
| BBC Radio 4 | 8,934千 | 言葉の番組（ニュース、ドラマ、討論） | とても高い | × 旧URLは bbc.co.uk へ転送 | 未確認 | ― | 将来の技術 |
| Capital（London／UK） | 8,864千（Brand） | ヒット曲、ダンス | 中 | ◎ MP3 128k | 未確認 | 未確認 | **推奨** |
| Smooth（London／UK） | 7,374千（Brand） | やわらかいポップ、懐かしい曲 | 中 | ◎ MP3 128k | 未確認 | 未確認 | 保留（Heartと近い） |
| BBC Radio 1 | 6,904千 | 若者向けのヒット曲 | 高い | × | 未確認 | ― | 将来の技術 |
| Greatest Hits Radio | 5,794千 | 70〜90年代の曲 | 中 | × | 未確認 | ― | 将来の技術 |
| BBC Radio 5 Live | 5,503千 | ニュース、スポーツの実況 | 高い | × | 未確認（スポーツの権利で海外は制限の見込み【推測】） | ― | 将来の技術 |
| Absolute Radio | 5,398千（Network） | ロック、ポップ | 中 | × HTTPのAAC・HTTPS | 未確認 | ― | 将来の技術 |
| Classic FM | 4,286千 | クラシック（親しみやすい選曲） | 高い | ◎ MP3 128k | 未確認 | Stage 3から登録 | **推奨**（今と同じ） |
| KISS | 3,738千（Network） | ダンス、R&B | 中 | ◎ MP3 112k 48kHz | 未確認 | 未確認 | 保留（Capitalと近い） |
| LBC London | 3,688千（Brand）。London 1,400千 | 電話参加の討論、ニュース | 高い（Londonの話し言葉の局） | ◎ MP3 48k 44.1kHz | 未確認 | 未確認 | **推奨** |
| talkSPORT | 3,321千 | スポーツの話題と実況 | 高い | × HTTPSのAAC | 未確認 | ― | 将来の技術 |
| Magic | 3,299千（Network） | やわらかいポップ | 中 | × HTTPSのMP3 | 未確認 | ― | 将来の技術 |
| Radio X（London／UK） | 2,411千（Brand） | ブリティッシュ・ロック、インディー | 高い | ◎ MP3 128k | 未確認 | 未確認 | 保留（追加の第一候補） |
| Gold | 1,535千 | 60〜80年代の曲 | 中 | ◎ MP3 128k | 未確認 | 未確認 | 保留 |
| Boom Radio | 635千 | 年配向けの懐かしい曲 | 中 | ◎ MP3 128k | 未確認 | 未確認 | 保留 |
| GB News Radio | 731千 | 政治の討論、ニュース | 中（政治的な立場が強い） | ◎ MP3 128k | 未確認 | 未確認 | 保留（言葉の局ではLBCを優先） |
| Radio Caroline | RAJARの表になし | アルバム・ロック。1964年の海賊放送が始まり | とても高い（歴史） | ◎ MP3 128k、ポート8040 | 未確認 | Stage 3から登録 | **推奨**（今と同じ） |
| Times Radio、Virgin Radio | 660千、1,008千 | ニュースと討論／ポップ・ロック | 中 | × HTTPSへ転送 | 未確認 | ― | 将来の技術 |
| Jazz FM、Kerrang! | 406千、615千 | ジャズ／ロック | 中 | × | 未確認 | ― | 将来の技術 |

## 3. 公式の配信と利用条件

### 3.1 配信の方式（2026-10-21 UTC、このMacから。日本の回線）【接続】

| 運営 | 局 | 確かめた配信 | 結果 |
| --- | --- | --- | --- |
| Global | Heart、Capital、Smooth、Classic FM、Radio X、Gold | `http://media-ice.musicradio.com/<名前>MP3` | 200、MP3 128k 44.1kHz、`icy-name` に局名 |
| Global | LBC London | `http://media-ice.musicradio.com/LBCLondonMP3`（`media-sov…/LBC973MP3Low` も同じ） | 200、MP3 48k 44.1kHz |
| Global | LBC（UK） | `http://media-ice.musicradio.com/LBCUKMP3` | 200、MP3 48k。ただし `icy-name` は「LBC News UK」で、**LBC本体か、別の局のLBC Newsかは未確認** |
| BBC | World Service | `http://stream.live.vc.bbcmedia.co.uk/bbc_world_service` | 200、MP3 56k 24kHz |
| BBC | Radio 1、2、3、4、5 Live、6 Music | 同じ形の旧URL | 302で `https://www.bbc.co.uk/` へ（配信ではない）。海外向けの配信は HLS（`as-hls-ww-live.akamaized.net`）のプレイリスト |
| Bauer（Rayo） | Greatest Hits Radio、Hits Radio、Absolute、KISSTORY、Magic、Kerrang!、Jazz FM | `stream-*.planetradio.co.uk`、`stream-*.hellorayo.co.uk`（HTTP・HTTPS）、`sharp-stream.com` の旧URL | HTTPは500、404、名前が引けない。HTTPSも500（日本からの接続を断っている可能性【推測】） |
| Bauer（Rayo） | KISS | `http://live-kiss.sharp-stream.com/kiss100.mp3` | 200、MP3 112k 48kHz。**公式の案内の配信かは未確認**（Radio Browserで見つけたURL） |
| News UK（Wireless） | talkSPORT、Times Radio、Virgin Radio | 各局の `/stream` | 301でHTTPSへ |
| Boom Radio | Boom Radio | `http://listen-boomradio.sharp-stream.com/65_boom_radio_live_128` | 200、MP3 128k |
| GB News | GB News Radio | `http://listen-gbnews.sharp-stream.com/gbnews.mp3` | 200、MP3 128k |
| Radio Caroline | Radio Caroline | `http://sc6.radiocaroline.net:8040/stream` | 200、MP3 128k |

Globalの配信のURLは、局のサイトの案内ではなく、今の登録とRadio Browserの一覧から得たもの。**各社の公式サイトが案内する配信かどうかは未確認**。

### 3.2 利用条件

| 運営 | 確かめたこと | 結果 |
| --- | --- | --- |
| Global（Global Player） | 利用規約のページ（`globalplayer.com/terms-and-conditions/`）を開こうとした | 404。**規約の本文は未確認**。第三者の機器・アプリでの再生、UKの外からの聴取を認めるかは**未確認** |
| BBC | 利用規約（`bbc.co.uk/usingthebbc/terms`） | このMacの調べる手段では開けなかった。**未確認** |
| BBC（海外の聴取） | 報道 | BBC Soundsのアプリは2025年7月からUKの外で使えなくなり、World ServiceとRadio 4はBBC.comとBBCのアプリで聴けるとの報道【資料：[Connexion](https://www.connexionfrance.com/news/bbc-says-no-plans-for-content-subscription-service-in-france/773224)】。旧URLのWorld ServiceのMP3が今後も続くかは**未確認** |
| Bauer、News UK、Boom、GB News、Radio Caroline | ― | **未確認** |

**技術的に再生できること（◎）と、利用が認められていることは別**。◎の局も、利用条件は全部「未確認」のまま。

## 4. 今のRADIO DAYSでの再生

| 判定 | 局 |
| --- | --- |
| 条件に合う（Macで確認。実機は未確認） | BBC World Service、Heart（UK、London）、Capital（UK、London）、Smooth（UK、London）、Classic FM、LBC London、LBC（UK、名前に疑問）、Radio X（UK、London）、Gold、KISS、Boom Radio、GB News Radio、Radio Caroline |
| 合わない | BBC Radio 1・2・3・4・5 Live・6 Music（HLS、HTTPS）、Greatest Hits Radio・Hits Radio・Absolute・KISSTORY・Magic・Kerrang!・Jazz FM（HTTPのMP3が応答しない、HTTPS・AAC）、talkSPORT・Times Radio・Virgin Radio（HTTPSへ転送） |

- USB中継はhttpだけを扱うので、HTTPSの局は中継でも鳴らない（今の作り）
- ビットレートは全部192k以下。LBC（48k）とWorld Service（56k、24kHz）は低い
- KISSは48kHz。今の局の多くは44.1kHzで、48kHzの局が実機で問題なく鳴るかは**未確認**

## 5. 推奨する局

| 局 | 理由 |
| --- | --- |
| **BBC World Service** | BBCの局で今鳴らせる唯一の局。英語のニュースと特集。今と同じで、過去に実機で鳴っている |
| **Heart（London）** | 週の到達が全国で1位のブランド。UKの毎日のポップを代表する |
| **Classic FM** | 全国のクラシックの局で、到達も大きい（4,286千）。今と同じ |
| **LBC London** | London市場の占有率7.8%。電話で参加する討論の局で、Londonの話し言葉がそのまま聴ける。言葉の局がWorld Serviceだけにならない |
| **Capital（London）** | 到達の大きいヒット曲の局（Brand 8,864千）。今と同じ（London版） |
| **Radio Caroline** | RAJARの表にはないが、1964年の海賊放送から続く、UKのラジオの歴史そのものの局。アルバム・ロックで、ほかの局と選曲が重ならない。今と同じ |

**局数：6局**（今の5局から、Smooth Londonを外し、HeartとLBC Londonを加える）。**確定ではない**。Radio Xを加えた7局案も残す（下の表）。最終の判断は作者

| 案 | 局 |
| --- | --- |
| 6局案 | World Service、Heart London、LBC London、Classic FM、Capital London、Radio Caroline |
| 7局案 | 6局案＋Radio X London（例：…Capital London → Radio X → Radio Caroline。ロックが2つ続くので、Radio XをClassic FMとCapitalの間に置く並びも可） |

- London版とUK版の両方がある局（Heart、Capital）は、Locationの代表都市に合わせてLondon版を推奨。UK版も同じく応答する。どちらにするかは作者の判断
- Smooth Londonを外す理由：Heartと同じ大人向けのポップで、6局の中で重なる。人気（7,374千）はあるので、局を増やす場合の候補に残す

**保留（増やす場合の候補）**
- Radio X：ブリティッシュ・ロックの代表。Radio Carolineとロックで重なるため、今回は保留。7局にするなら第一候補
- Smooth、Gold、Boom Radio：懐かしい曲・やわらかいポップの局で、Heartと役割が近い
- KISS：Capitalと近い。配信が公式の案内かも未確認
- GB News Radio：話し言葉の局はLBCを優先。政治的な立場が強いので、入れる場合は作者が判断

## 6. 局数とダイヤルの順

人気の順ではなく、言葉の局と音楽の局が交互になり、音楽の局が続く場合も種類が違うように並べる（ダイヤルは循環するので、6/6の次は1/6）。

| 表示 | 局 | 種類 |
| --- | --- | --- |
| 1/6 | BBC World Service | 言葉（国際ニュース） |
| 2/6 | Heart（London） | 音楽（ポップ） |
| 3/6 | LBC London | 言葉（討論） |
| 4/6 | Classic FM | 音楽（クラシック） |
| 5/6 | Capital（London） | 音楽（ヒット曲） |
| 6/6 | Radio Caroline | 音楽（ロック） |

- 音楽の局が続くのは5→6だけ（ヒット曲→ロックで種類は違う）。6→1で言葉の局に戻る
- 言葉の局は2局しかないので、どの並びでも音楽の局が2つ続く場所が1か所できる
- U8のダイヤルでは6局で長さが120ステップ（4周期分、240 px）、局の間は20ステップ（40 px）【U6の式から計算。実機の見え方は未確認】
- 1/6などの表示は**まだ作らない**（作者の指示を待つ）

## 7. 将来の技術の拡張で候補になる局

| 局 | 必要な技術 | 人気（週の到達） | ほかの課題 |
| --- | --- | --- | --- |
| BBC Radio 2 | HTTPS、HLS（AAC） | 12,367千 | BBCの海外向けの提供と利用条件が未確認 |
| BBC Radio 4 | 同上 | 8,934千 | 同上。言葉の局としてUKらしさがとても高い |
| BBC Radio 1、6 Music、Radio 3 | 同上 | 6,904千、2,586千、1,881千 | 同上 |
| BBC Radio 5 Live | 同上 | 5,503千 | スポーツの権利による海外の制限の見込み【推測】 |
| Hits Radio、Greatest Hits Radio、Absolute、Magic、KISSTORY、Kerrang!、Jazz FM（Bauer） | HTTPS、AAC（一部はMP3） | 12,240千（集計）〜406千 | 日本から500で応答しない。UKの外で聴けるか未確認 |
| talkSPORT、Times Radio、Virgin Radio（News UK） | HTTPS（AACまたはMP3） | 3,321千、660千、1,008千 | 利用条件が未確認 |

- HTTPS（TLS）とAACの復号を加えると、0x150000の決まりに収まらない見込み【推測：[cardputer-adv-radiko-review.md](cardputer-adv-radiko-review.md)】。**大きさは実際にビルドして測るまで断定しない**
- USB中継をHTTPSに対応させる案も、中継の変更になるので今回は扱わない

## 8. 未確認のことと、実機での確認

### 8.1 未確認
1. 全運営者（Global、BBC、Bauer、News UK、Boom、GB News、Radio Caroline）の利用条件。第三者の機器での再生、UKの外からの聴取が認められるか
2. Globalの `media-ice.musicradio.com` の配信が、局の公式の案内にあるものか
3. `LBCUKMP3` がLBC本体かLBC Newsか（`icy-name` は「LBC News UK」）
4. KISSの `live-kiss.sharp-stream.com` が公式の配信か
5. Bauerの局が日本から500を返す理由（地域の制限か、URLの変更か）
6. BBC World Serviceの旧URLのMP3がこの先も続くか
7. 長期の安定性（今回は同じ日の2回の接続だけ）

### 8.2 実機（Cardputer ADV）で確かめること
1. Heart London、LBC London、Capital London、Classic FM、Radio Caroline の再生（今の登録局も、World Service以外は実機で鳴った記録がない）
2. LBC London（48k、44.1kHz）の音質と途切れ
3. Radio Caroline（ポート8040）がWi-FiとUSB中継のどちらでも鳴るか
4. 6局でのダイヤル（局の間隔40 px）の見え方とAUTOの移動
5. 1/6 などの表示を作った場合の画面（実装後）

## 参考
- RAJAR、Quarterly Summary Figures 2026 Q2（2026年8月6日）：https://www.rajar.co.uk/ （表記：RAJAR/Ipsos/RSMB）
- BBC Soundsの海外での提供終了の報道：https://www.connexionfrance.com/news/bbc-says-no-plans-for-content-subscription-service-in-france/773224
- Radio Browser（配信のURLを探すためだけに使用。人気の根拠にはしない）：https://www.radio-browser.info/
