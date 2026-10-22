# JAPAN（Tokyo）の選局の調査

- 作成日：2026-10-22
- 状態：**調査と提案だけ**。`stations.h`、U3〜U8、USB中継、README、CHANGELOGは変えていない
- ひな型：[uk-station-selection-review.md](uk-station-selection-review.md)。全体：[location-selection-review.md](location-selection-review.md)
- もとにした調査：[japan-stations-review.md](japan-stations-review.md)（今の7局）、[japan-candidates-review.md](japan-candidates-review.md)（代わりの候補）、[japan-popular-stations-review.md](japan-popular-stations-review.md)（主要局とradiko）、[cardputer-adv-radiko-review.md](cardputer-adv-radiko-review.md)
- 書き方：【接続】＝このMacからの接続（2026-10-21〜22 UTC）、【推測】、**未確認**。**Macでの確認は実機の確認ではない**。NHK・フジサンケイグループを含め、どの放送局のグループも除かずに同じ基準で評価した

## 1. 主要な局と根拠

- 日本には**全局の順位表が公開されていない**（ビデオリサーチの首都圏ラジオ調査は、各局が自局の首位の区分だけを発表）
- 発表の例：ニッポン放送が2026年の個人全体・週平均で首位、TOKYO FMが12〜69歳・全日平均で首位を続けた、文化放送の番組が個人全体1位（[japan-popular-stations-review.md](japan-popular-stations-review.md)）
- 知名度の高い局：NHKラジオ第1、NHK-FM、TBSラジオ、ニッポン放送、文化放送、TOKYO FM、J-WAVE、関西のMBS・ABC・FM802など

## 2. 比較表

再生：◎＝HTTP・MP3・192k以下で応答【接続】、×＝合う配信がない。実機は**未確認**（JAPANの局が実機で鳴ったという記録は見つからない。Stage 3の時点で実機で鳴った記録があるのは Radio Paradise、FIP、KEXP、BBC World Service の4局だけ。[stage-3.md](stage-3.md)）。

| 局 | 知名度 | 内容 | 日本らしさ | 再生（配信） | 利用条件 | 判定 |
| --- | --- | --- | --- | --- | --- | --- |
| NHKラジオ第1、NHK-FM | とても高い | 公共放送 | とても高い | × らじる★らじる（HTTPS・HLS・AAC、国内向け） | 未確認 | 将来の技術 |
| TBSラジオ、ニッポン放送、文化放送、TOKYO FM、J-WAVE ほか民放 | とても高い | トーク、音楽 | とても高い | × radiko（HTTPS・HLS・AAC、地域の判定） | radikoの規約は公式以外の方法での聴取を認めない（第4条(3)） | 将来の技術（**正規の方法がない**） |
| 湘南ビーチFM 78.9（今の局） | 地域 | ジャズ、オールディーズ、地域情報 | 高い（海辺の町のコミュニティFM） | ◎ Airtime.pro 128k、局名を名乗る | 公式サイトに記載なし。URLは公式サイトに載っていない | **推奨** |
| FM世田谷 83.4（新） | 地域 | 生ワイド、区政、演劇、子育て | 高い（東京の生活感） | ◎ Airtime.pro 128k（名乗りは「Airtime!」） | 公式サイトが同じAirtime.proのアカウントのプレーヤーを案内（**このURLそのものは載っていない**） | **推奨** |
| FMかほく 78.7（今の局） | 地域 | ボランティアの番組、演歌・歌謡曲、防災 | 高い（地方の小さな局） | ◎ 96k、局名を名乗る | 記載なし（音楽はJASRAC・NexToneの許諾と記載） | **推奨** |
| OTTAVA（今の局） | ネット局として知られる | クラシック | 中 | ◎ 128k（名乗りは「Airtime!」） | 未確認。**公式の配信か未確認** | **推奨**（配信元の確認を条件に） |
| 大槌・海辺（Otsuchi Seashore、今の局） | ― | 海辺の環境音（東京大学サイバーフォレスト） | 高い（作品の個性） | ◎ 192k 48kHz | CC BY-NC-SA 4.0（公式サイト）。出典の表示と非営利が条件【推測：一般的な読み方】 | **推奨** |
| NHK WORLD (WRN)（今の局） | NHKの名前 | 国際放送 | 中 | ◎ 112k | **NHKの正式な配信か未確認** | 保留 |
| Japan Hits（今の局） | ― | J-POP（配信はトロント） | 低い | ◎ 128k | 運営者が未確認 | 外す |
| J1 Gold（今の局） | ― | 昔の歌謡曲 | 中 | ◎ 128k（名乗りは「Stream」） | 運営者・配信元が未確認 | 外す（保留の候補） |
| 山中湖（富士山麓の森） | ― | 森の環境音（サイバーフォレスト） | 高い | 前の調査で ◎ 192k（今回は試したURLが違い404。**再確認が必要**） | CC BY-NC-SA 4.0 | 保留 |
| City Pop One | ― | 昭和のシティポップ | 高い | 公式のURLはHTTPS（証明書の期限切れ）。HTTPはIPアドレスのURLだけ | 記載なし | 保留（公式のHTTPのURLがあれば） |

## 3. 推奨する局：5局

| 表示 | 局 | 種類 | 理由 |
| --- | --- | --- | --- |
| 1/5 | 湘南ビーチFM | 音楽（ジャズ、オールディーズ） | 聴きやすい地域局。今と同じ |
| 2/5 | FM世田谷 | 言葉・地域（東京） | 代表都市Tokyoの地域局。公式が同じ配信のサービスを案内 |
| 3/5 | OTTAVA | 音楽（クラシック） | 知られたネット局。今と同じ |
| 4/5 | FMかほく | 言葉・地域（北陸） | 地方の小さな局の手ざわり。今と同じ |
| 5/5 | 大槌・海辺 | 環境音 | 作品の個性。5→1で音楽に戻る |

**問題**：日本の主要局（NHK、民放のAM・FM）は、今のRADIO DAYSでは**技術的にも、radikoの規約の上でも**鳴らせない。推奨の5局は「地域局・ネット局・環境音」で日本らしさを出す構成で、知名度の高い局は入らない。作者の国なので、Locationとしては残す価値が高い。

- 今の7局から外す：Japan Hits、J1 Gold（配信はカナダ、運営者が未確認）、NHK WORLD (WRN)（正式な配信か未確認）。NHK WORLD (WRN)は、NHKの正式な配信と確認できれば戻す候補
- 加える：FM世田谷

## 4. 将来の技術の拡張の候補
- らじる★らじる（NHK）：HTTPS・HLS・AAC。利用条件は未確認
- radikoの局：技術的にはCardputer単独での受信が示されている（Cardputer-Adv-Radiko）が、**radikoの規約が認める方法ではない**。正規の方法は、PCで公式のradikoを鳴らし、Cardputerを選局の装置にする形（音はPCから）【[japan-popular-stations-review.md](japan-popular-stations-review.md)】
- 無理にHTTPS・HLSへの対応は進めない（今回の方針）

## 5. 未確認のことと実機の確認
1. 各局への問い合わせ（このURLで機器から聴いてよいか）：湘南ビーチFM、FM世田谷、FMかほく、OTTAVA
2. OTTAVAの配信が公式か、NHK WORLD (WRN) がNHKの正式な配信か
3. 山中湖の配信のURL（今回の確認では別のURLを試した）
4. USB中継：`fmsetagaya834.out.airtime.pro:8000` を許可の一覧に加える必要がある
5. 実機：FMかほく（96k）、大槌（192k 48kHz）

## 参考
- 前の調査の資料（上の4つ）
- サイバーフォレスト：http://cyberforest.nenv.k.u-tokyo.ac.jp/
- FM世田谷：https://fmsetagaya.com/
