# FRANCE（Paris）の選局の調査

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。`stations.h`、U3〜U8、USB中継、README、CHANGELOGは変えていない
- ひな型：[uk-station-selection-review.md](uk-station-selection-review.md)。全体：[location-selection-review.md](location-selection-review.md)
- 書き方：【Médiamétrie】＝報道で確かめた公表値、【接続】＝このMacからの接続（2026-10-21 UTC）、【推測】、**未確認**。**Macでの確認は実機の確認ではない**

## 1. 主要な局と根拠

| 項目 | 内容 |
| --- | --- |
| 調査機関 | Médiamétrie「EAR National」 |
| 期間 | 2026年4〜6月（7月10日発表） |
| 指標 | 1日の聴取者数（audience cumulée、13歳以上）と、聴取時間の占有率（PdA）。月〜金 |
| 出典 | 報道（[Ozap](https://www.ozap.com/actu/audiences-radio-france-inter-leader-sous-les-7-millions-dauditeurs-nouvelle-gamelle-pour-nrj-ici-quitte-le-top-10-radio-nova-dejoue-tous-les-pronostics/655985)）。Médiamétrieの発表のPDFは今回取得できなかった（503）ので、**数字は報道の値** |

| 順 | 局 | 1日の聴取者 | 占有率 | 種類 |
| --- | --- | --- | --- | --- |
| 1 | France Inter | 691万 | 13.9% | 公共（Radio France）、総合・言葉 |
| 2 | RTL | 486万 | 11.2% | 民間、総合・言葉 |
| 3 | franceinfo | 473万 | 4.7% | 公共、ニュース |
| 4 | NRJ | 364万 | 5.1% | 民間、ヒット曲 |
| 5 | Nostalgie | 321万 | 5.5% | 民間、懐かしい曲 |
| 6 | Skyrock | 318万 | 4.0% | 民間、ラップ・ヒップホップ |
| 7 | RMC | 276万 | 5.2% | 民間、討論・スポーツ |
| 8 | Europe 1 | 255万 | 4.4% | 民間、総合・言葉 |
| 9 | France Culture | 218万 | 3.6% | 公共、文化・言葉 |
| 10 | Radio Nova | 210万 | 2.8% | 民間、ワールド・グルーヴ（前年比 +120万） |
| 11〜 | ICI（旧France Bleu）208万、RTL2 174万、Chérie FM 174万、Fun Radio 170万、RFM 157万、Rire et Chansons 146万、France Musique 100万、Europe 2 99万、Radio Classique 98万、FIP 77万 | | | |

## 2. 比較表

再生：◎＝HTTP・MP3・192k以下で応答【接続】、×＝合う配信が見つからない。実機は、FIPだけStage 2で鳴った記録がある（[stage-3.md](stage-3.md)）。ほかは**未確認**。利用条件はすべて**未確認**。

| 局 | 人気 | 内容 | 国らしさ | 再生（配信） | 判定 |
| --- | --- | --- | --- | --- | --- |
| France Inter | 1位 | ニュース、討論、文化、音楽 | とても高い | ◎ `icecast.radiofrance.fr/franceinter-midfi.mp3` 128k 48kHz（今の局） | **推奨** |
| RTL | 2位 | 総合、討論、ニュース | 高い | ◎ `icecast.rtl.fr/rtl-1-44-128` → `streaming-ice.audiomeans.fr` へ転送、128k 44.1kHz | **推奨** |
| franceinfo | 3位 | 24時間のニュース | 中 | ◎ 128k 48kHz | 保留（France Interと言葉の局が重なる） |
| NRJ | 4位 | ヒット曲 | 中 | × HTTPSへ転送 | 将来の技術 |
| Nostalgie | 5位 | 懐かしい曲 | 中 | × 旧URLは404、一覧のURLはHTTPS | 将来の技術 |
| Skyrock | 6位 | フランスのラップ | 高い（フランスはラップの大きな市場） | ◎ `icecast.skyrock.net/s/natio_mp3_128k` 128k 48kHz | **推奨** |
| RMC | 7位 | 討論、スポーツ | 中 | ◎ `audio.bfmtv.com/rmcradio_128.mp3` 128k | 保留 |
| Europe 1 | 8位 | 総合 | 中 | ◎ `europe1.lmn.fm/europe1.mp3` 128k | 保留 |
| France Culture | 9位 | 文化、文学、思想の番組 | とても高い | ◎ 128k 48kHz（今の局） | **推奨** |
| Radio Nova | 10位 | ワールド、ヒップホップ、エレクトロ（Parisの文化） | 高い | ◎ `novazz.ice.infomaniak.ch/novazz-128.mp3` 128k | **推奨** |
| FIP | 77万 | 曲間の話が少ない、ジャンルを越えた選曲 | とても高い（世界的に知られる） | ◎ 128k 48kHz（今の局） | **推奨** |
| France Musique | 100万 | クラシック | 高い | ◎（今の局） | 保留（クラシックはTSF Jazz・FIPと入れ替え） |
| FIP Jazz | 調査に出ない（ネットの派生局） | ジャズ | 中 | ◎（今の局） | 保留（TSF Jazzを優先） |
| TSF Jazz | 未確認（要約の表に出ない） | ジャズ（Parisの地上波） | 高い | ◎ `tsfjazz.ice.infomaniak.ch/tsfjazz-high.mp3` 128k | **推奨** |
| Radio Classique | 98万 | クラシック、経済ニュース | 中 | ◎ 128k | 保留 |
| RTL2、Fun Radio、Europe 2 | 174万、170万、99万 | ポップ・ロック、ダンス、ポップ | 中 | ◎ | 保留 |
| ICI Paris | ICI全体で208万 | 地域 | 中 | ◎ | 保留 |
| Chérie FM、Rire et Chansons、RFM | 174万、146万、157万 | | 中 | × HTTPSのみ、旧URLは404 | 将来の技術 |

## 3. 公式の配信と利用条件

| 運営 | 確かめたこと |
| --- | --- |
| Radio France | `icecast.radiofrance.fr` のMP3（midfi 128k）が応答。公式の開発者向けAPIで配信の一覧を出しているとの書き込みがある【資料：LinuxFrのコメント】が、**公式の配信かと利用条件は未確認** |
| RTL（M6グループ） | `icecast.rtl.fr` → `streaming-ice.audiomeans.fr`（固定のホスト、ポート80）へ転送。公式のプレーヤーの配信かは**未確認** |
| Skyrock、RMC（BFM）、Europe 1・2（Lagardère）、Nova、TSF Jazz、Radio Classique | 配信は応答。**公式の案内か・利用条件は未確認** |
| NRJグループ（NRJ、Nostalgie、Chérie、Rire et Chansons） | HTTPのMP3はHTTPSへ転送、または404 |

## 4. 推奨する局：7局

| 表示 | 局 | 種類 | 理由 |
| --- | --- | --- | --- |
| 1/7 | France Inter | 言葉 | 聴取率1位の公共放送。今と同じ |
| 2/7 | FIP | 音楽（ジャンルを越える） | 世界的に知られるフランスのラジオの象徴。今と同じ |
| 3/7 | RTL | 言葉 | 民間の1位。公共放送と違う「フランスの民放」の声 |
| 4/7 | Radio Nova | 音楽（グルーヴ） | 今年大きく伸びたParisの文化の局 |
| 5/7 | France Culture | 言葉 | 文化・思想の番組。今と同じ |
| 6/7 | Skyrock | 音楽（ラップ） | 若者の聴取の上位。フランスのラップ |
| 7/7 | TSF Jazz | 音楽（ジャズ） | Parisのジャズの地上波局。7→1で言葉に戻る |

- 言葉3局・音楽4局。音楽が続くのは6→7だけで、ラップ→ジャズと種類が違う
- 今の5局のうち France Musique、FIP Jazz を外す（クラシックの局がなくなる。残すなら France Musique を8局目にするか、TSF Jazzと入れ替え）
- U8のダイヤルでは7局で150段（局の間隔は約21段、約43 px）【式から計算】

## 5. 将来の技術の拡張の候補
- NRJ、Nostalgie、Chérie FM、Rire et Chansons（NRJグループ、HTTPS）、RFM（HTTPS）

## 6. 未確認のことと実機の確認
1. 各局の利用条件、配信が公式の案内か
2. RTLの転送先 `streaming-ice.audiomeans.fr` がこの先も固定か。USB中継では、`icecast.rtl.fr:80` と `streaming-ice.audiomeans.fr:80` を許可の一覧に加える必要がある（今の中継にはない）
3. 実機：48kHzの局（Radio France、Skyrock）の再生、転送のある局（RTL）

## 参考
- Ozap（Médiamétrie 2026年4〜6月）：上の表の出典
- Médiamétrie の発表：https://www.mediametrie.fr/system/files/2026-07/C2%20O%20COM%202026%2007%2010%20CP%20EAR%20National%20AJ26%20EN.pdf （今回は503で取得できず）
- Radio France の配信のAPIについての書き込み：https://linuxfr.org/users/devnewton/journaux/lettre-ouverte-a-radio-france
