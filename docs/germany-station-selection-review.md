# GERMANY（Berlin）の選局の調査

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。`stations.h`、U3〜U8、USB中継、README、CHANGELOGは変えていない
- ひな型：[uk-station-selection-review.md](uk-station-selection-review.md)。全体：[location-selection-review.md](location-selection-review.md)
- 書き方：【ma】＝ma Audioの公表値、【報道】、【接続】＝このMacからの接続（2026-10-21 UTC）、【推測】、**未確認**。**Macでの確認は実機の確認ではない**

## 1. 主要な局と根拠

| 項目 | 内容 |
| --- | --- |
| 調査機関 | agma「ma Audio」（Media-Micro-Census） |
| 資料 | ma 2026 Audio I の報道向けの表（radioszene.deが公開したPDF、2026年3月）。最新の **ma 2026 Audio II（2026年7月14日発表）は全体の表を取得できず**、報道の一部だけ |
| 指標 | 1日の聴取者数（Hörer pro Tag、月〜金、ドイツ語を話す14歳以上、百万人） |
| 注意 | ドイツの局の多くは**州の局**。全国の順位は州の人口の大きさに左右される。Locationの代表都市（Berlin）の局が上位に来るとは限らない |

ma 2026 Audio I、1日の聴取者（単独の局、百万人）【ma】：

| 順 | 局 | 聴取者 | 地域・種類 |
| --- | --- | --- | --- |
| 1 | SWR3 | 5.36 | 南西部（バーデン＝ヴュルテンベルク、ラインラント＝プファルツ）。ARDのポップ |
| 2 | WDR 2 | 4.57 | ノルトライン＝ヴェストファーレン。ARDの総合 |
| 2 | BAYERN 1 | 4.57 | バイエルン。ARDの大人向け |
| 4 | 1LIVE | 4.34 | ノルトライン＝ヴェストファーレン。ARDの若者向け |
| 5 | ANTENNE BAYERN | 4.03 | バイエルン。民間最大 |
| 6 | BAYERN 3 | 3.90 | バイエルン。ARDのポップ |
| 7 | NDR 2 | 3.85 | 北部。ARDのポップ |
| 8 | RADIO BOB!（全体） | 3.75 | 民間のロック（全国の配信を含む） |
| 9 | HIT RADIO FFH（全体） | 2.46 | ヘッセン。民間 |
| 10 | hr3 | 1.95 | ヘッセン。ARD |

- Deutschlandfunk（全国の公共、ニュース・文化）：「最も広い聴取者層」（Weitester Hörerkreis）は 9.94百万人【ma】。1日の聴取者は表の形から読み取れず**未確認**
- radioeins（Berlin・Brandenburg）：1日 0.56百万人【ma】
- ma 2026 Audio II の報道：radio NRW（局の連合）が1時間あたり約130万人で首位、Bayern 1は100万人を下回った、Bayern 3（+13.9%）とAntenne Bayern（+5%）が伸び、SWR3は −5.3%【報道：[turi2](https://www.turi2.de/?p=4113410)、[Horizont](https://www.horizont.net/medien/nachrichten/ma-2026-audio-ii-bayern-1-faellt-unter-die-millionengrenze-bayern-3-gewinnt-235084)】

## 2. 比較表

再生：◎＝HTTP・MP3・192k以下で応答【接続】。実機はすべて**未確認**。利用条件はすべて**未確認**。

| 局 | 人気 | 内容 | 国らしさ | 再生（配信、転送先） | 判定 |
| --- | --- | --- | --- | --- | --- |
| Deutschlandfunk | 全国の公共。最も広い聴取者層 9.94百万 | ニュース、討論、文化 | とても高い | ◎ 128k 48kHz、`*.rndfnk.com` へ（今の局） | **推奨** |
| SWR3 | 1位 | ポップ、番組の笑い（コメディ） | 高い | ◎ 128k 48kHz、`dispatcher.rndfnk.com` → `*.rndfnk.com` | **推奨** |
| WDR 2 | 2位 | 総合、サッカー中継 | 高い | ◎ 128k 48kHz（地域版が複数）、`*.rndfnk.com` | 保留 |
| BAYERN 1 | 2位 | 大人向けの懐かしい曲、バイエルンの地域色 | 高い | ◎ 128k、`*.rndfnk.com` | 保留（第一候補） |
| 1LIVE | 4位 | 若者向けのヒット曲 | 高い | ◎ 128k 48kHz、`*.rndfnk.com`（今の局） | **推奨** |
| ANTENNE BAYERN | 5位（民間1位） | ヒット曲 | 中 | ◎ 128k 44.1kHz、`mp3channels.webradio.antenne.de` → `s1／s3／s5-webradio.antenne.de`（毎回変わる） | **推奨** |
| BAYERN 3、NDR 2、hr3 | 6・7・10位 | ポップ | 中 | ◎ 128k、`*.rndfnk.com` | 保留 |
| RADIO BOB! | 8位 | ロック | 中 | ◎ 192k、`regiocast.streamabc.net` へ | 保留 |
| HIT RADIO FFH | 9位 | ヒット曲 | 中 | ◎ 128k（今の局） | 保留（外す） |
| DLF Kultur | 全国の公共 | 文化、ラジオドラマ、音楽 | 高い（Berlinの局） | ◎ 128k、`*.rndfnk.com`（今の局） | **推奨** |
| radioeins | 0.56百万（Berlin・Brandenburg） | 大人向けのロック・ポップ、Berlinの文化 | 高い（代表都市の局） | ◎ 128k、`dispatcher.rndfnk.com` → `*.rndfnk.com` | **推奨** |
| WDR 5 | 未確認 | 言葉の局 | 中 | ◎（今の局） | 保留（外す。言葉はDLFとDLF Kulturで足りる） |
| Fritz、DLF Nova | 未確認 | 若者向け | 中 | ◎ | 保留 |
| Klassik Radio | 1.42百万（コンビ） | クラシック、映画音楽 | 中 | ◎ 192k、`klassikr.streamabc.net` へ | 保留 |

## 3. 公式の配信と利用条件
- ARDの局とDeutschlandradioは、配信が局名を名乗り（`icy-name` に「Westdeutscher Rundfunk Koeln」など）、ARDの配信網（`*.rndfnk.com`）から届く。**局のサイトがこのURLを案内しているか、利用条件は未確認**
- Antenne Bayern、Regiocast（Radio BOB!、Klassik Radio）：**未確認**
- 今のUSB中継は `*.rndfnk.com:80` への転送の規則を持つ（U4）。ただし最初のURLのホスト（`dispatcher.rndfnk.com:80`、`liveradio.swr.de:80` など）は許可の一覧に**加える必要がある**。Antenne Bayernは転送先が毎回変わるので `*.antenne.de:80` の規則が要る

## 4. 推奨する局：6局

| 表示 | 局 | 種類 | 理由 |
| --- | --- | --- | --- |
| 1/6 | Deutschlandfunk | 言葉 | 全国の公共のニュース・文化の局。今と同じ |
| 2/6 | SWR3 | 音楽（ポップ） | 1日の聴取者が最も多い単独の局 |
| 3/6 | DLF Kultur | 言葉・文化 | Berlinに本拠を置く公共の文化の局。今と同じ |
| 4/6 | radioeins | 音楽（大人のロック・ポップ） | 代表都市Berlinの局 |
| 5/6 | 1LIVE | 音楽（若者） | 若者向けの最大の局。今と同じ |
| 6/6 | Antenne Bayern | 音楽（ヒット曲） | 民間の最大の局。6→1で言葉に戻る |

- 音楽が続くのは4→5→6（大人のロック → 若者のヒット → 民放のヒット）。5・6は似ているので、6をBAYERN 1（懐かしい曲、バイエルン）に替える案もある
- 今の5局のうち WDR 5、Hit Radio FFH を外す
- 推奨の6局はすべて「今のRADIO DAYSで鳴る見込み」（Macで確認）

## 5. 将来の技術の拡張の候補
- なし（主要局はどれもHTTPのMP3が応答した）。ただしHTTPSだけに移る局が出ないかは継続して確かめる

## 6. 未確認のことと実機の確認
1. ma 2026 Audio II の全体の表、Deutschlandfunkの1日の聴取者数
2. 各局の利用条件、配信のURLが公式の案内か
3. 実機：48kHzの局、2段の転送（SWR3）、毎回変わる転送先（Antenne Bayern）

## 参考
- ma 2026 Audio I 報道向けの表：https://www.radioszene.de/wp-content/uploads/2026/03/ma-2026-Audio-I-Pressetabellen.pdf
- ma 2026 Audio II の報道：https://www.horizont.net/medien/nachrichten/ma-2026-audio-ii-bayern-1-faellt-unter-die-millionengrenze-bayern-3-gewinnt-235084 、https://www.turi2.de/?p=4113410
