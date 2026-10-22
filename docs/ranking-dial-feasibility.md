# 人気順位で並べるダイヤル（1st、2nd…）：実現性の調査と設計の検討

- 作成日：2026-10-22
- 状態：**調査と設計の検討だけ**。`stations.h`、U3〜U8、USB中継は変えていない
- **2026-10-22 追記**：順位の表示（`1st`、`2nd`…）は**採用しない**ことになった。表示は「今の位置／局数」（`1/5` など、順位ではない）に決まった（[station-lineup-survey-plan.md](station-lineup-survey-plan.md) 第0節）。この文書は、各国の聴取率の資料を探すための**参考**として残す
- 構想（作者とエドガー）：各Locationの人気の局を順位の順に並べ、ダイヤルの位置を順位に対応させ、局名とともに `1st`、`2nd`、`3rd`… を表示する。将来は順位の更新で登録局が入れ替わる
- 原則：**信頼できる順位がないところに、推測で `1st` などを付けない**
- 前の資料：[station-lineup-survey-plan.md](station-lineup-survey-plan.md)（19のLocationの今の局と、優先して調べる局）、[cardputer-adv-radiko-review.md](cardputer-adv-radiko-review.md)

## 1. 結論（要約）

1. **1位から7位を客観的に決められるLocationは限られる**。公開の聴取率調査があり、最新の結果が手に入るのは、UK、FRANCE、GERMANY、POLAND、CZECHIA、ITALY、および都市の市場単位でUSA（ニューヨーク）とAUSSIE（シドニー）。それ以外は、調査はあっても全体の順位表が公開されていない、今年の結果が見つからない、または調査が見つからない
2. 順位は**指標で変わる**（例：チェコは「1日の聴取者数」ではRadiožurnál、「週の到達」ではImpulsが1位）。Locationごとに、調査機関・指標・地域・期間を1つに決めて記録する必要がある
3. 更新は、調査の発表に合わせて**人が確かめてデータを直す**形が現実的（多くの国で年2〜12回）。本体が自動で順位を取りに行くのは、公開のAPIがなく、資料の多くが報道やPDFなので難しい
4. **人気の順位と、RADIO DAYSで再生できるかは、別の項目として持つ**。上位の局ほど、HTTPS・HLS・AACの配信が多く、今のRADIO DAYSで再生できない場合がある
5. U8のダイヤル（4段の目盛り、枠、循環、AUTOの流れ）は**そのまま使える**。局の並びを順位の順にするだけで、局の数でダイヤルの長さが決まる仕組みも変わらない。変えるのは「順位の表示」と「再生できない局の扱い」

## 2. 各Locationで順位を決められるか（2026-10-22の調査）

| Location | 調査機関（指標） | 対象地域・期間・頻度 | 最新の公開結果 | 1〜7位を決められるか |
| --- | --- | --- | --- | --- |
| UK | RAJAR（週の到達など） | 全国・地域。四半期ごと | 2026年第2四半期（8月6日発表）。全国はHeart 1,280万人、BBC Radio 2 1,240万人（Globalの発表。RAJARの全国の局の表で確認が必要）【資料】 | **できる**（RAJARの表から） |
| FRANCE | Médiamétrie「EAR National」（聴取の占有率 PdA、1日の到達） | 全国。年数回 | 2026年4〜6月（7月10日発表）。France Interが1位（週末14.1%）、RTLが続く【資料】 | **できる** |
| GERMANY | agma「ma Audio」（1時間あたり・1日の到達） | 全国（局の多くは州単位）。年2回 | ma 2026 Audio II（7月）。568の局・まとめの表あり【資料】 | **できる**（全国の表から。ただし州の局が多く、ベルリンなど代表都市の地域で並べるかを決める必要） |
| POLAND | Radio Track（聴取時間の占有率） | 全国。四半期ごと | 2026年第2四半期：RMF FM 28%台、Radio ZET 14.2%、Jedynka 4.9%（報道）【資料】 | **できる**（公式の報告で確認） |
| CZECHIA | Radioprojekt（1日の聴取者数、週の到達） | 全国。四半期ごと（半年の累積） | 2026年1〜6月：1日の聴取者数はRadiožurnál、Evropa 2、Impulsの順。週の到達はImpulsが1位【資料】 | **できる**（指標を決めれば） |
| ITALY | Audiradio（平均的な1日の聴取者数）。TERから移行、2025年から四半期ごと | 全国。四半期ごと | 2026年第1四半期（5月14日発表）：RTL 102.5、Radio Italia、RDS、Deejay、105、Kiss Kissの順【資料】 | **できる** |
| USA | Nielsen Audio（PPM、6歳以上の占有率） | **都市の市場ごと**。毎月 | ニューヨーク2026年8月：WINS（ニュース）とWLTW（Lite FM）が7.3で並び、Z100 5.6、WCBS-FM 5.4【資料】。Cumulus社の局は契約の事情で含まれない月がある | **ニューヨークの市場としてならできる**（全国の順位はない） |
| AUSSIE | GfK（占有率） | **都市ごと**。年8回 | シドニー2026年第6回（10月7日）：smoothfm 13.2%、2GB 10.8%…【資料】 | **シドニーの市場としてならできる** |
| CANADA | Numeris（PPM、占有率） | 都市の市場ごと。年数回 | 2026年の結果は見つからなかった（過去はトロントでCHFIとCBC Radio Oneが首位を争う）【資料・古い】 | 未確認（資料を探せば都市単位でできる見込み） |
| NZ | GfK（民間局の調査） | 全国。年数回 | 2026年第1回：Newstalk ZBが1位（14.4%）、The Breezeが音楽局の1位【資料】。**公共放送RNZは民間の調査に含まれず、別の発表** | 民間局の中ではできる。RNZを含む全体の順位は**作れない** |
| HUNGARY | NMHH（国の機関）の聴取の調査 | 全国・地域。四半期ごと | 2026年は地方の報告だけ見つかった。全国は2019年まで：Retro Rádió、Petőfi、Kossuth、Rádió 1の順【資料・古い】 | 未確認（全国の最新の報告を探す） |
| HOLLAND | NMO（2023年にNLOから移行） | 全国 | 2026年の結果は見つからなかった。過去はQmusicとNPO Radio 2が首位を争う【資料・古い】 | 未確認 |
| SWEDEN | Kantar Sifo（PPM。2013年から公式の数値） | 全国 | 2026年の局別の結果は見つからなかった【資料・古い】 | 未確認 |
| JAPAN | ビデオリサーチ（首都圏ラジオ調査など） | 首都圏は年6回 | **全局の順位表は公開されていない**。各局が自局の首位の区分だけを発表（例：ニッポン放送が2026年の個人全体・週平均で首位、TOKYO FMが12〜69歳・全日平均で首位を続けた）【資料】 | **できない**（公開の資料では） |
| BRAZIL | Kantar IBOPE Media（都市ごと） | 都市ごと | 2026年のラジオの順位は見つからなかった | 未確認 |
| ARGENT | Kantar IBOPE Media、Proyección「Monitor de Audiencia」 | ブエノスアイレス | 日付の分からない報道：AMはMitre、FMはLa 100が1位【資料・日付不明】 | 未確認 |
| THAI | Nielsen（バンコク） | ― | 公開の結果は見つからなかった（Soundchartsの順位はエアプレイの測定で、聴取率ではない） | **できない**（公開の資料では） |
| TAIWAN | （公開の全国調査は見つからなかった） | ― | 地方局の調査だけ | **できない**（公開の資料では） |
| WORLD | ― | ― | 国の順位という考え方が当てはまらない | **付けない**（選んだ局として扱う） |

**Radio Browser の投票数・クリック数は使わない**（世界中の一覧の利用者の数で、その国での聴取の実績ではない）。Soundchartsなどの「エアプレイの到達」も、聴取率とは別のものとして扱う。

## 3. 順位の根拠として記録すること

Locationごとに次を1組だけ決め、資料（URL、発表日）と一緒に記録する。

| 項目 | 例（POLAND） |
| --- | --- |
| 調査機関 | Radio Track |
| 指標 | 聴取時間の占有率（全体） |
| 対象 | 全国、15〜75歳（調査の定義に従う） |
| 期間 | 2026年4〜6月 |
| 発表日・資料 | 公式の報告（または報道）のURL |
| 確認した日 | 2026-10-xx |

国の大きいUSA・AUSSIE・CANADAは、**代表都市の市場**（ニューヨーク、シドニー、オタワまたはトロント）で順位を取るのが自然（時計と空も代表都市で決めている）。その場合「全国の1位」ではないことを資料に書く。

## 4. ランキングの更新

| 方法 | 評価 |
| --- | --- |
| 人が調査の発表を確かめ、データ（局の表）を直して新しい版を作る | **現実的**。調査の頻度は国により年2回（ドイツ）〜毎月（アメリカ） |
| 本体がネットから順位を取りに行く | **難しい**。公開のAPIがなく、資料は報道やPDF。調査会社のデータには利用条件があることが多く、順位を機械的に取り込んで配ることには確認が要る |
| 局の表を本体の外（SDカードやUSBの中継から渡すファイル）に持ち、ファームウェアを作り直さずに差し替える | 将来の案。今は `stations.h` に組み込み（作り直しが必要） |

順位が変わると、局が入れ替わる・並びが変わる。保存しているのはLocationだけなので、互換性の問題はない（局の番号は保存していない）。

## 5. 人気の順位と再生できるかの管理

局ごとに、次の項目を**別々に**持つ（資料の表、将来の局の表）。

| 項目 | 内容 | 例 |
| --- | --- | --- |
| 順位 | 調査での順位（記録した指標で） | 2 |
| 根拠 | 第3節の1組 | Médiamétrie EAR National 2026年4〜6月 |
| 公式の配信 | 方式（HTTP・HTTPS、MP3・AAC、HLS） | HTTP、MP3 |
| 技術的に再生できるか | 今のRADIO DAYSで | ○／×（理由：HTTPS、HLS、AACなど） |
| 利用の許可 | 利用条件の確認の結果 | 未確認／確認済み／規約上不可 |

日本の局は、**技術的に再生できるか**（Cardputer-Adv-Radikoのように、技術的にはCardputer単独でradikoを受信できることが示されている）と、**利用の許可**（radikoの規約上、公式の方法に当たらない）が分かれる例。両方を別の欄に書く。

### 上位の局が再生できない場合の扱い（案）

| 案 | 内容 | 評価 |
| --- | --- | --- |
| A：順位の番号を保ったまま、再生できる局だけ並べる | 例：`1st` が再生できなければ、`2nd`、`3rd`、`5th`… と番号が飛ぶ | **推奨**。正直で、番号の意味が変わらない |
| B：再生できない局もダイヤルに置き、合わせると「この局は再生できない」と表示して雑音 | 本当の順位の並びが分かる。ただし毎日聴く装置としては、聴けない位置が混ざる | 選択肢。表示の文と、再生しない処理が少し要る |
| C：再生できる局だけで1位から数え直す | 順位の意味が変わる（調査の1位ではない） | **勧めない**（推測の順位になる） |

## 6. U8のダイヤル・操作との関係

| 部分 | 維持できるか |
| --- | --- |
| 4段の目盛り、枠、指針、循環、局の印 | **そのまま** |
| 局の数でダイヤルの長さが決まる仕組み（3局→60段、4・5局→90段、7局→150段） | **そのまま**（局の並びを順位の順にするだけ） |
| AUTOの流れ（右に1つ＝次の順位）、MANUALの雑音、350ミリ秒 | **そのまま**。AUTOで右に回すと順位が下がり、最下位の次は1位に戻る |
| 局名の行（y 96、大きい文字、19文字まで） | 順位の表示を足す場所の候補：名前の左に小さく `1st`（6×8の字で3〜4文字、x 4〜27）、または名前の前に付ける（`1st Radio ZET`。名前が長いと19文字に収まらない） |
| 順位のないLocation（JAPAN、THAI、TAIWAN、WORLDなど） | 順位を出さない。今の表示と同じ |
| 容量 | 順位の1バイト×局数と表示の処理で、数百バイトの見込み【推測】。U8の残りは1,856 B |

## 7. 既存の調査計画の再評価

[station-lineup-survey-plan.md](station-lineup-survey-plan.md) の「優先して調べる局」は、この構想でも使える。変えること：

1. **各Locationの順位の根拠を先に決める**（第2節の表で「できる」のLocationから）。根拠が決まってから、その上位7局を「優先して調べる局」にする
2. 調べる順番を、「今の局との差が大きい順」から「**順位の資料がある順**」に変える：UK、FRANCE、ITALY、POLAND、CZECHIA、GERMANY、USA（ニューヨーク）、AUSSIE（シドニー）→ CANADA、NZ、HUNGARY、HOLLAND、SWEDEN、BRAZIL、ARGENT → JAPAN、THAI、TAIWAN（順位なし）
3. JAPANの調べる局に、NHKラジオ第1、NHK-FM、ニッポン放送、文化放送も含める（ビデオリサーチの調査で名前が挙がる主要局）。日本は公開の全順位がないので、順位の表示はしない
4. 先行事例 Cardputer-Adv-Radiko は、日本の局の「技術的に再生できるか」と「利用の許可」を分けて評価するための資料として扱う（[cardputer-adv-radiko-review.md](cardputer-adv-radiko-review.md)）

## 8. 作者に決めていただきたいこと

1. 順位の指標：国ごとに調査の主な指標に合わせるか（例：占有率）、できるだけ「週の到達」にそろえるか
2. 国の大きいLocation（USA、AUSSIE、CANADA）は、代表都市の市場で順位を取るか
3. 上位の局が再生できない場合の扱い（第5節の案A／B）
4. 順位の表示の位置（名前の左に小さく／名前の前に）
5. 最初に詳しく調べるLocation（推奨：UK、FRANCE、ITALY）

## 参考
- RAJAR 2026年第2四半期：https://www.rajar.co.uk/docs/2026_06/All%20Radio%20Listening%20Charts%20-%20Q2%202026.pdf 、Globalの発表：https://lbbonline.com/news/Global-RAJAR-Q2-2026
- Médiamétrie EAR National 2026年4〜6月：https://www.mediametrie.fr/system/files/2026-07/C2%20O%20COM%202026%2007%2010%20CP%20EAR%20National%20AJ26%20EN.pdf 、報道：https://fr.themedialeader.com/?p=217337
- ma 2026 Audio：https://www.agma-mmc.de/fileadmin/user_upload/agma_PM_ma_2026_Audio_I.pdf 、https://www.radioszene.de/wp-content/uploads/2026/03/ma-2026-Audio-I-Pressetabellen.pdf
- Radio Track 2026（報道）：https://antyweb.pl/najpopularniejsze-stacje-radiowe-fm-2026
- Radioprojekt 2026年1〜6月：https://www.mediaguru.cz/data/files/765-rp-tiskova-zprava-2kv-2026.pdf 、https://www.lupa.cz/clanky/v-poslechovosti-radii-si-polepsila-evropa-2-v-dennich-datech-je-za-radiozurnalem/
- Audiradio 2026年第1四半期（報道）：https://www.italiaoggi.it/marketing-e-media/media/ascolti-radio-rtl-1025-si-conferma-prima-ecco-la-classifica-los12puz
- Nielsen ニューヨーク 2026年8月（報道）：https://www.insideradio.com/free/changes-on-top-in-four-markets-on-august-ppms-day-1/article_cbbbbfc3-1809-49ee-9963-a11216f6f29f.html
- GfK シドニー 2026年第6回（報道）：https://www.mediaweek.com.au/sydney-radio-ratings-survey-6-2026-smoothfm-holds-the-crown-while-kiis-plateaus
- GfK NZ 2026年第1回（報道）：https://radioinfo.com.au/surveys/newstalk-zb-no-1-the-breeze-top-music-station-new-zealand-gfk-survey-1/
- Numeris（報道、年不明）：https://mediaincanada.com/?p=125350
- NMHH（ハンガリー）：https://nmhh.hu/dokumentum/260817/megyei_jogu_varosi_radios_hallgatottsag_2026_masodik_negyedev.pdf 、全国（2019年）：https://english.nmhh.hu/article/209724/The_three_most_popular_radios_have_four_million_listeners
- NLOからNMOへ：https://www.broadcastmagazine.nl/wp-content/uploads/2023/12/008-013-Radiojaaroverzicht-en-Adv.pdf
- Kantar Sifo（スウェーデン、過去の報告）：https://www.kantarsifo.se/sites/default/files/reports/documents/kantar_sifo_nationell_radio_2016_ppm.pdf
- ビデオリサーチと各局の発表：[japan-popular-stations-review.md](japan-popular-stations-review.md)
- アルゼンチン（報道、日付不明）：https://www.c5n.com/ratingcero/la-radio-mas-viva-que-nunca-cuales-son-las-mas-escuchadas-los-argentinos-n10793
