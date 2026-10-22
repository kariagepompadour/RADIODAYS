# ITALY（Rome）の選局の調査

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。`stations.h`、U3〜U8、USB中継、README、CHANGELOGは変えていない
- ひな型：[uk-station-selection-review.md](uk-station-selection-review.md)。全体：[location-selection-review.md](location-selection-review.md)
- 書き方：【Audiradio】＝報道で確かめた公表値、【接続】＝このMacからの接続（2026-10-21 UTC）、【推測】、**未確認**。**Macでの確認は実機の確認ではない**

## 1. 主要な局と根拠

| 項目 | 内容 |
| --- | --- |
| 調査機関 | Audiradio（2025年からの新しい方式。前のTERとは比べられない） |
| 期間 | 2026年第2四半期：全国の局 4月14日〜6月22日（7月22日ごろ発表） |
| 指標 | 1日の聴取者数（giorno medio）、15分あたりの平均の聴取者数 |
| 出典 | 報道（[ItaliaOggi](https://www.italiaoggi.it/marketing-e-media/media/audiradio-calano-leggermente-gli-ascoltatori-nel-secondo-trimestre-ribg8lye)）。**局ごとの全体の表は取得できず** |

| 局 | 1日の聴取者【Audiradio】 | 種類 |
| --- | --- | --- |
| RTL 102.5 | 630万超（1位） | 民間、ヒット曲と番組（ラジオとテレビの同時放送） |
| Radio Italia | 578万（2位） | 民間、イタリアの曲だけ |
| RDS | 560万（3位） | 民間、ヒット曲 |
| Radio 105 | 15分平均 52.6万（15分平均の1位） | 民間（Mediaset）、ヒット曲、「Lo Zoo di 105」 |
| Deejay | 15分平均 48.2万 | 民間（GEDI）、DJの番組 |
| Radiofreccia、Radio Zeta | 160万、120万 | 民間（RTL 102.5のグループ） |
| Rai Radio 1・2・3、Virgin、R101、Radio Monte Carlo、Radio 24、m2o、Radio Capital、Kiss Kiss | 2026年の数字は今回見つからなかった（**未確認**）。2026年第1四半期の報道では、RTL 102.5、Radio Italia、RDS、Deejayの順 | |

## 2. 比較表

再生：◎＝HTTP・MP3・192k以下で応答【接続】、△＝応答するが作者の方針（192k以下）を超える、×＝合う配信が見つからない。実機はすべて**未確認**。利用条件はすべて**未確認**。

| 局 | 人気 | 内容 | 国らしさ | 再生（配信） | 判定 |
| --- | --- | --- | --- | --- | --- |
| RTL 102.5 | 1位 | ヒット曲と番組 | 高い | × HTTPSのHLS（AAC） | 将来の技術 |
| Radio Italia | 2位 | イタリアの曲だけ | **とても高い** | × HTTPSのHLS | 将来の技術（第一候補） |
| RDS | 3位 | ヒット曲 | 中 | △ `icstream.rds.radio/rds` は MP3 256k | 将来の検討（ビットレート） |
| Radio 105 | 15分平均1位 | ヒット曲、人気の番組 | 高い | ◎ `icecast.unitedradio.it/Radio105.mp3` 128k 48kHz（今の局） | **推奨** |
| Deejay | 15分平均2位 | DJの番組 | 高い | × HLS | 将来の技術 |
| Virgin Radio Italia | 未確認 | ロック | 中 | ◎ 128k 48kHz（今の局） | **推奨** |
| Radio Monte Carlo | 未確認 | 落ち着いたポップ、ラウンジ | 中 | ◎ 128k 48kHz（今の局） | **推奨** |
| R101 | 未確認 | 大人向けのポップ | 中 | ◎ 128k 48kHz | 保留（Radio Monte Carloと近い） |
| Rai Radio 1・2・3 | 未確認 | 公共放送（ニュース・スポーツ／ポップ／文化・クラシック） | **とても高い** | △ MP3 256k・256k・320k（`icestreaming.rai.it` → `icecdn-…msvdn.net`）。U4から**方針で除外** | 将来の検討（ビットレート） |
| Radio 24 | 未確認 | ニュース、経済、討論（Il Sole 24 Ore） | 高い | ◎ `shoutcast2.radio24.it:8000/;` MP3 64k 22.05kHz（音質は低い） | **推奨** |
| Radio Radicale | 未確認 | 国会・裁判・政党の会議の中継 | **とても高い**（世界でも珍しい局） | ◎ 64k 44.1kHz（今の局） | **推奨** |
| Radio Kiss Kiss | 未確認 | ヒット曲（ナポリ） | 中 | × HTTPSへ転送 | 将来の技術 |
| Radio Capital、m2o | 未確認 | | 中 | × HLS、または見つからない | 将来の技術 |

## 3. 公式の配信と利用条件
- Mediasetの4局（105、Virgin、Monte Carlo、R101）は `icecast.unitedradio.it` で、配信の名前が「FM_105_mp3_radiomediaset」など、運営の名前を含む。**公式の案内か、利用条件は未確認**
- Rai：MP3は256〜320kだけ見つかった。**低いビットレートのMP3は見つからない**
- Radio 24、Radio Radicale：**未確認**

## 4. 推奨する局：5局

| 表示 | 局 | 種類 | 理由 |
| --- | --- | --- | --- |
| 1/5 | Radio 24 | 言葉（ニュース・経済） | イタリアの主要紙の局。言葉の局を1つ確保 |
| 2/5 | Radio 105 | 音楽（ヒット曲） | 15分平均の1位。今と同じ |
| 3/5 | Radio Radicale | 言葉（政治の中継） | イタリアにしかない種類の局。今と同じ |
| 4/5 | Radio Monte Carlo | 音楽（落ち着いたポップ） | 今と同じ |
| 5/5 | Virgin Radio Italia | 音楽（ロック） | 今と同じ。5→1で言葉に戻る |

**問題**：聴取率の上位3局（RTL 102.5、Radio Italia、RDS）、Deejay、公共放送Raiを入れられない。推奨の5局のうち3局はMediasetで、運営が偏る。言葉の2局はどちらも64kで音質が低い。

## 5. 将来の技術の拡張・検討の候補

| 局 | 必要なこと |
| --- | --- |
| Radio Italia、RTL 102.5、Deejay、Radio Capital | HTTPS、HLS、AAC |
| Rai Radio 1・2・3、RDS | 256〜320kのMP3を、作者の方針（192k以下）の例外として実機で試すか。**実機で安定して鳴るかは未確認** |
| Radio Kiss Kiss | HTTPS |

## 6. 未確認のことと実機の確認
1. Audiradio 2026年第2四半期の局ごとの全体の表（Rai、Virgin、Radio Monte Carlo、Radio 24などの数字）
2. 各局の利用条件、配信が公式の案内か
3. 実機：64k・22.05kHz（Radio 24）の音質、48kHzの局

## 参考
- Audiradio 2026年第2四半期の報道：https://www.italiaoggi.it/marketing-e-media/media/audiradio-calano-leggermente-gli-ascoltatori-nel-secondo-trimestre-ribg8lye
- 2026年第1四半期の報道：https://www.italiaoggi.it/marketing-e-media/media/ascolti-radio-rtl-1025-si-conferma-prima-ecco-la-classifica-los12puz
- U4でRaiを除いた記録：[u4-proposal.md](u4-proposal.md)
