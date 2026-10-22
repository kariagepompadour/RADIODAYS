# U7：時計をLocationの現地時刻に

- 作成日：2026-10-22
- 状態：**実装、ビルド、ホスト試験まで。実機ではまだ試していない**
- 設計：[clock-local-time-proposal.md](clock-local-time-proposal.md)（作者の決定：現地時刻にする、WORLDは日本時間、タイムゾーン名や時差は表示しない、版名U7）
- もとにした版：U6（変更なし）。中継は `RadioDaysRelay_U4.command`

## 変更
- `stations.h`：`Location` に `tz`（POSIXのタイムゾーン、夏時間の規則を含む）を加え、19か所に設定（JAPAN・WORLD：JST-9、USA・CANADA：米国東部、UK、ヨーロッパの8か所：中央ヨーロッパ、THAI、TAIWAN、AUSSIE：シドニー、NZ：ウェリントン、BRAZIL、ARGENT）。局名・URL・順番、局の位置は同じ
- `RadioDays_U7.ino`：
  - `clockZone()`（新規）：今のLocationの `tz` を `TZ` に設定
  - 起動時（`clockInit()`）：保存したLocationの時刻帯
  - Locationの切り替え（`applyLocation()`）：新しい時刻帯。画面全体の描き直しで時計と日付がすぐ現地時刻になる
  - SNTPの開始（`clockService()`）：U6は日本時間に戻していた。U7は今のLocationの時刻帯のまま
  - シリアルの記録：`[CLOCK] zone …`
- 変えていないもの：太陽・月（UTCと都市の緯度・経度。U6と1文字も違わないことを試験で確認）、時計の画面のデザイン、ダイヤル、選局、音声、USB、Wi-Fi、Location画面（`decodeTask`・`drawDial`・`tuneBy` などはU6と同じ）

## 容量
- **1,373,824 B（U6より+416 B）、0x150000までの残り2,432 B**。静的RAM 59,756 B。自分のコードの警告0件

## 試験（`sh tools/u7-test/run_u7_test.sh`）
| 試験 | 結果 |
| --- | --- |
| 時計（新規、スケッチの時計の関数そのもの） | **24項目すべて合格**：19か所の1月・7月のUTCとの差／起動時に保存したLocationの時刻帯／UTC 2026-10-22 00:30で UK 01:30、JAPAN 09:30、USA 前日20:30、NZ 13:30、WORLD 09:30（Locationを替えた直後）／BRAZILの日付の変わり目／夏時間の始まり・終わりの1分前と後（ニューヨーク、ロンドン、ベルリン、シドニー、ウェリントン、オタワ。2026年）／SNTPを始めてもLocationの時刻帯のまま、その後のLocationの切り替え／USB中継の時刻（UTC）がパリ時間で表示される |
| 試験の確認 | Locationを替えたときの時刻帯の設定を消すと、5項目が不合格になる |
| 太陽・月 | `solarAltitudeDeg`・`skyPlace`・`updateSky`・`moonPhase01` がU6と同じ |
| U6のダイヤルの試験をU7で | 22項目合格 |
| U4のホスト試験をU7で | 55項目合格 |
| U6のプレビューとの比較をU7で | 9場面とも0画素の違い |

時刻帯の計算は、MacのCライブラリで試した。Cardputerの中のCライブラリ（newlib）は同じ形式に対応しているが、**実機では未確認**。

## 実機で確かめること
1. 起動時：保存したLocationの現地時刻（例：UKならロンドン時刻）
2. Locationを替えたとき：時計と日付がすぐ変わる（例：JAPAN→USAで日付が前日になることがある）
3. Wi-Fi（SNTP）で時刻が合ったときに日本時間へ戻らない。USB中継の時刻でも現地時刻
4. シリアルの記録の `[CLOCK] zone …`
5. 太陽・月、ダイヤル、再生がU6と同じ
