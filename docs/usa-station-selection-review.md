# USA（New York）の選局の調査

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。`stations.h`、U3〜U8、USB中継、README、CHANGELOGは変えていない
- ひな型：[uk-station-selection-review.md](uk-station-selection-review.md)。全体：[location-selection-review.md](location-selection-review.md)
- 書き方：【Nielsen】＝報道で確かめた公表値、【接続】＝このMacからの接続（2026-10-21 UTC）、【推測】、**未確認**。**Macでの確認は実機の確認ではない**

## 1. 主要な局と根拠

| 項目 | 内容 |
| --- | --- |
| 調査機関 | Nielsen Audio（PPM） |
| 地域 | **ニューヨークの市場**（全国の順位はない。Locationの代表都市に合わせる） |
| 指標 | 6歳以上の聴取の占有率 |
| 期間・出典 | 2026年6月（Inside Radioの報道）。2026年8月の報道は [ranking-dial-feasibility.md](ranking-dial-feasibility.md) |
| 注意 | Cumulus社の局（WABC、WPLJなど）は契約の事情で順位に含まれない月がある |

| 順（6月） | 局 | 占有率 | 運営・内容 |
| --- | --- | --- | --- |
| 1 | WLTW 106.7 Lite FM | 7.5 | iHeart、大人向けのポップ |
| 2 | WINS 1010 | 7.1 | Audacy、24時間ニュース |
| 3 | WCBS-FM 101.1 | 5.8 | Audacy、懐かしいヒット曲 |
| 4 | Z100（WHTZ） | 5.6 | iHeart、ヒット曲 |
| 5 | Q104.3（WAXQ） | 4.9 | iHeart、クラシック・ロック |
| 8月 | WINSとWLTWが7.3で並び、Z100 5.6、WCBS-FM 5.4 | | |

- 公共・大学の局（WNYC、WQXR、WFUV、WBGO、WKCR）の2026年の占有率は今回見つからなかった（**未確認**）。Hot 97（WQHT、ヒップホップ）は2026年1月に朝の時間帯の25〜54歳で3位【報道】

## 2. 比較表

再生：◎＝HTTP・MP3・192k以下で応答【接続】、×＝合う配信が見つからない。実機は、Radio ParadiseとKEXPだけStage 2で鳴った記録がある（[stage-3.md](stage-3.md)）。ほかは**未確認**。利用条件はすべて**未確認**。

| 局 | 人気 | 内容 | 国らしさ | 再生（配信） | 判定 |
| --- | --- | --- | --- | --- | --- |
| WLTW、Z100、Q104.3（iHeart） | 1・4・5位 | ポップ、ヒット、ロック | 中 | × iHeartの配信はAAC（Z100の `stream.revma.ihrhls.com` はAAC）、HLS | 将来の技術 |
| WINS、WCBS-FM、WFAN（Audacy） | 2・3位 | ニュース、懐かしい曲、スポーツ | 高い | × 見つからない（試したURLは404・503） | 将来の技術 |
| WNYC 93.9 FM | 未確認（全米最大級の公共ラジオの局） | NPRのニュース、討論、文化 | **とても高い**（ニューヨークの公共ラジオ） | ◎ `fm939.wnyc.org/wnycfm` MP3 96k 32kHz | **推奨** |
| WQXR 105.9 | 未確認 | クラシック | 高い | ◎ 128k 48kHz（今の局） | **推奨** |
| WFUV 90.7 | 未確認 | 大人向けのロック・フォーク（フォーダム大学） | 高い | ◎ `wfuv-onair.streamguys.org/onair-hi` 128k | **推奨** |
| WBGO 88.3 | 未確認 | ジャズ（ニューアーク） | 高い | ◎ `ais-sa8.cdnstream1.com/3629_128.mp3` 128k 48kHz | **推奨** |
| WFMU 91.1 | 未確認 | 自由な選曲（フリーフォーム） | 高い（伝説的な独立局） | ◎ 128k（今の局） | **推奨** |
| KEXP 90.3（シアトル） | 未確認 | インディー・ロック | 高い（全米に知られる） | ◎ 128k（今の局） | **推奨** |
| WKCR 89.9 | 未確認 | ジャズ、クラシック（コロンビア大学） | 高い | ◎ 160k 48kHz | 保留（WBGOとジャズが重なる） |
| Hot 97（WQHT） | 朝の時間帯で上位 | ヒップホップ（ニューヨークはヒップホップの生まれた街） | **とても高い** | ◎ 64k。`playerservices.streamtheworld.com` → `<数字>.live.streamtheworld.com:80` へ毎回違う転送 | 保留（音質が低い、転送の規則が要る） |
| KCRW（ロサンゼルス） | 未確認 | 音楽と公共のニュース | 高い | ◎ 192k | 保留（ニューヨークではない） |
| WWOZ（ニューオーリンズ） | 未確認 | ジャズ、ブルース | 高い | ◎ 128k | 保留 |
| Radio Paradise（今の局） | ― | ネット局（カリフォルニア） | 中（世界で聴かれるネット局） | ◎ 128k | WORLDへ移す案 |
| 181.fm Classical（今の局） | ― | ネット局 | 低い | ◎ | 外す（WQXRとクラシックが重なる） |
| WBLS | 未確認 | R&B | 高い | × 403 | 将来の確認 |

## 3. 公式の配信と利用条件
- WNYC・WQXR（New York Public Radio）、WFUV、WFMU、KEXPは配信が局名を名乗る。公式サイトの案内か、利用条件は**未確認**
- iHeart・Audacyの局は、公式のアプリ・サイトでHTTPS・AAC・HLSの配信。今のRADIO DAYSでは再生できない

## 4. 推奨する局：6局

| 表示 | 局 | 種類 | 理由 |
| --- | --- | --- | --- |
| 1/6 | WNYC | 言葉 | ニューヨークの公共ラジオ。ニュースと討論 |
| 2/6 | WFUV | 音楽（ロック・フォーク） | ニューヨークの大学の局 |
| 3/6 | WQXR | 音楽（クラシック） | 今と同じ |
| 4/6 | WBGO | 音楽（ジャズ） | ニューヨーク圏のジャズの局 |
| 5/6 | WFMU | 音楽（自由な選曲・話も多い） | 今と同じ |
| 6/6 | KEXP | 音楽（インディー） | 今と同じ。6→1で言葉に戻る |

- 言葉の局が1つだけで、音楽の局が続く。ただし種類はすべて違う。言葉を増やすなら、iHeart・Audacyのニュース局は鳴らせないので、WNYC AM（今回は404で未確認）などを探す必要がある
- **問題**：ニューヨークの聴取率の上位5局はどれも入れられない。推奨は「アメリカの公共・大学ラジオ」という特色の構成
- 起動時の既定のLocationはUSAで、U8ではRadio Paradiseから始まる（Stage 2からの約束）。Radio Paradiseを外す場合、**最初に鳴る局が変わる**（作者の判断）

## 5. 将来の技術の拡張の候補
- WLTW、Z100、Q104.3（iHeart：AAC、HLS）、WINS、WCBS-FM、WFAN（Audacy）

## 6. 未確認のことと実機の確認
1. 公共・大学の局のニューヨークでの聴取率
2. 各局の利用条件、配信が公式の案内か
3. 実機：WNYC（96k、32kHz）、WBGO（48kHz）
4. USB中継：`fm939.wnyc.org:80`、`wfuv-onair.streamguys.org:80`、`ais-sa8.cdnstream1.com:80` を許可の一覧に加える必要がある

## 参考
- Nielsen 2026年6月（報道）：https://www.insideradio.com/free/ac-extends-lead-as-top-ranked-format-in-q2-ppm-markets/article_a8f4fe96-0221-4a20-8441-5a1c73029203.html
- Hot 97（報道）：https://briefglance.com/companies/mediaco-holding-inc/pulses/49053
