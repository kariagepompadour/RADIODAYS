# Windows用のUSB中継（RadioDaysRelay_U9_Windows）

- 作成日：2026-10-22
- 状態：実装し、Macで試験（PowerShell 7）。**2026-10-22、作者がWindows 11とCardputer ADV（U10）で動作を確認**（第8節）。スリープの抑止、`--check-url`、長時間の安定性は未確認
- 対象：RADIO DAYS U9・U10（U2a2以降の通信方式。U10はU9と同じ局・同じ通信）
- もとにした版：macOS用の `tools/usb-relay/RadioDaysRelay_U9.command`（**変更していない**）

## 1. ファイル

| ファイル | 内容 |
| --- | --- |
| `tools/usb-relay/RadioDaysRelay_U9_Windows.ps1` | 中継の本体（PowerShellのスクリプト。ASCIIだけで書いている） |
| `tools/usb-relay/RadioDaysRelay_U9_Windows.cmd` | ダブルクリックで本体を起動する（改行はCRLF） |
| `tools/usb-relay-windows-test/run_compare_rules.sh` | Mac版と、URLの判定がすべて同じことを確かめる試験 |
| `tools/usb-relay-windows-test/run_sim_windows.sh` | 既存の中継シミュレーション（sim4）をWindows版で動かす試験（作者の環境のみ） |
| `tools/usb-relay-windows-test/live_stations.py` | U10の42局を、偽のCardputerからWindows版の中継で実際に受信する試験（インターネットを使用） |

## 2. 使い方（Windows 10・11）

1. CardputerをUSBの**データ**ケーブルでPCにつなぎ、RADIO DAYSを起動する。Arduino IDEのシリアルモニタなど、同じCOMポートを使うものは閉じる
2. `RadioDaysRelay_U9_Windows.cmd` と `RadioDaysRelay_U9_Windows.ps1` を**同じフォルダ**に置き、`.cmd` をダブルクリックする
3. CardputerのSTART画面で `U`（Wi-Fiを保存していなければ、どのキーでもよい）
4. 止めるときは、Control-Cかウィンドウを閉じる

PowerShellから直接起動する場合：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\RadioDaysRelay_U9_Windows.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File .\RadioDaysRelay_U9_Windows.ps1 --port COM5
powershell -NoProfile -ExecutionPolicy Bypass -File .\RadioDaysRelay_U9_Windows.ps1 --check-url http://stream.radioparadise.com/mp3-192
```

- `-ExecutionPolicy Bypass` は、その1回の起動だけに効く。PCの設定は変えない。会社のPCなどで、グループポリシーによりスクリプトの実行が禁止されている場合は動かない（その場所の規則に従う）
- インターネットからダウンロードしたファイルは、Windowsが「ブロック」の印を付けることがある。開けない場合は、ファイルのプロパティで「許可する」を選ぶ（未確認：`-ExecutionPolicy Bypass` ではこの印は問題にならない見込み）
- `allowed_hosts.txt` は、Mac版と同じ形式で、`.ps1` と同じフォルダに置く

## 3. Mac版と同じところ

- 通信方式：`@RD HELLO U2a2`、`RDHOST`、`TUNE`、`RDSYNC`／`RDNAK`、`CREDIT`（累計、2^32で一周）、`STOP`、`PING`、`TIME`（RDSYNCの前だけUTCの秒を返す）
- 局の受信と送信：RDSYNCの後は、Cardputerが許した分だけ、最大4,096バイトずつ送る。受け取ったバイトがあるうちは局から読まない（Mac版と同じ読み方）
- 安全の決まり：`http://` だけ、許可したホストとポートだけ、IPアドレスと利用者名は不可、URLは300文字まで、転送は最大3回で同じ規則で確認、転送の規則（`redirect *.domain:port`）は転送先だけに使う
- 許可するホスト：内蔵の33組と転送の規則3つはMac版と同じ（試験で確認）。`allowed_hosts.txt` があればそれだけを使う
- 時間：接続は8秒、RDSYNCかRDNAKまで10秒、途切れたら2秒後につなぎ直す
- 画面の記録（`[relay]` の行など）の文面は、ほぼMac版と同じ

## 4. Mac版と違うところ

| 項目 | Mac版 | Windows版 |
| --- | --- | --- |
| 必要なもの | Perl、curl、caffeinate（macOSに標準） | Windows PowerShell 5.1（Windows 10・11に標準）。インストールは不要 |
| 局の受信 | curl（HTTP/1.1。チャンク形式はcurlが戻す） | .NETの `TcpClient` で、自分でHTTP/1.1の要求を送る（`Connection: close`）。チャンク形式の返事は自分で戻す。`ICY 200 OK` の返事も受け付ける |
| Cardputerのポート | `/dev/cu.usbmodem*` | COMポートのうち、USBのベンダーIDがEspressif（`VID_303A`）のもの。`--port COM5` で指定もできる |
| DTR・RTS | macOSの既定 | DTRを入れ、RTSは入れない（ESP32-S3はRTSだけを入れるとリセットされることがあるため） |
| スリープ | caffeinate `-i` | `SetThreadExecutionState`（ES_SYSTEM_REQUIRED）。中継の動作中だけ、PCが自動でスリープしない。画面は消えることがある |
| ポートが消えたとき | 読み書きの失敗で検出 | 読み書きの失敗に加えて、2秒ごとにCOMポートの一覧を確認 |
| `localhost` と `*.localhost` | curlがこのPCとして扱う | 同じ扱い（試験のため） |
| User-Agent | `RADIODAYS-relay/U9` | `RADIODAYS-relay/U9 (Windows)` |

## 5. 試験の結果（Mac、2026-10-22）

試験に使ったもの：PowerShell 7.6.7（Microsoftの公式配布、macOS arm64。作業用フォルダに展開しただけで、Macにはインストールしていない。SHA-256は公式の値と一致）。

**Windows PowerShell 5.1とWindows自体では試していない。** スクリプトは5.1でも動く書き方にした（PowerShell 7だけの書き方を使っていない、文字はASCIIだけ）が、未確認。

### 5.1 URLの判定（`run_compare_rules.sh`）：10項目すべて合格

- U10の42局、中継の規則の試験で使っているURL、境界の例（大文字、ポート0・65536、`@`・`#`・空白・引用符、IPアドレス、`localhost`、270〜273文字の長いURLなど）の122件を、Mac版とWindows版の `--check-url` と `--check-redirect` にかけ、**出力が1行も違わない**ことを確認
- 内蔵の許可ホスト33組と、転送の規則3つが同じ
- 良い行・悪い行を混ぜた `allowed_hosts.txt` でも、判定と警告（7件）が同じ

### 5.2 中継のシミュレーション（`run_sim_windows.sh`）：57項目すべて合格

Mac版の中継の試験（`tools/usb-relay/sim4`、変更なし）を、U10の `radiodays_link.h` と組み合わせ、中継だけをWindows版に替えて動かした。Mac版と同じ57項目：つなぎ直し、局の切り替えで前の局のバイトが混ざらないこと、CREDITによる流量の制御、Cardputerが読むのを止めたとき、転送（同じホスト、規則による転送、ループ）、`ICY 200 OK`、応答しない局、途中で切れる局、拒否の理由（PORT、HOST、REDIR、HTTP、URL）、ケーブルを抜いたとき、行の消失、中継の停止と再起動、時刻（TIME）。受信の待ち行列は一度もあふれなかった。

### 5.3 チャンク形式の返事

ランダムな長さのチャンク（拡張つきを含む）を、ランダムな区切りで200回渡し、戻した内容が元と完全に一致することを確認。不正なチャンクの長さは `HTTP` として断る。

### 5.4 実際の局（`live_stations.py`、インターネットを使用）：42局すべて合格

簡単な偽のCardputer（擬似端末で HELLO、TUNE、CREDIT を送る）から、U10の42局を順にWindows版の中継へ要求し、すべての局で `RDSYNC` が返り、続くデータがMP3のフレームであることを確認（転送される局も含む）。**MacのPowerShell 7とMacのネットワークでの確認で、Cardputerでの再生ではない。**

## 6. 未確認のこと

- スリープの抑止（長く聴いてもPCがスリープしないこと）
- Windows PowerShell 5.1で `--check-url`・`--check-redirect`・`--port` の引数がそのまま渡ること
- 長時間の安定性
- Windows 10での動作、会社のPCなど実行ポリシーやウイルス対策ソフトの制限がある環境
- 局の利用条件（Mac版と同じく未確認）

## 7. Windowsで確かめること

| # | 項目 |
| --- | --- |
| 1 | `.cmd` のダブルクリックで起動し、最初の行（許可するホストの数）が出る |
| 2 | CardputerのCOMポートが自動で見つかる（デバイスマネージャーの「ポート (COM と LPT)」の番号と同じ） |
| 3 | 起動したときにCardputerがリセットされない（DTR・RTS） |
| 4 | `U` で開始してから、42局のいくつかで音が出る。AUTOとMANUALで局を替えても前の局の音が混ざらない |
| 5 | 時計が合う（`Sent the time to the Cardputer (UTC).`） |
| 6 | USBケーブルを抜いて差し直すと、つなぎ直す |
| 7 | 長く聴いてもPCがスリープしない |
| 8 | `--check-url` が使える（Windows PowerShell 5.1での引数の渡り方） |
| 9 | 会社のPCなど、実行ポリシーやウイルス対策ソフトで止められないか |

## 8. Windowsでの確認（2026-10-22、作者）

Windows 11、Cardputer ADV、RADIO DAYS U10。第7節の項目のうち、確認できたもの：

| # | 項目 | 結果 |
| --- | --- | --- |
| 1 | `.cmd` のダブルクリックで起動 | 確認 |
| 2 | CardputerのCOMポートを自動で検出 | 確認 |
| 3 | 起動したときにCardputerがリセットされない | 確認 |
| 4 | 何局か切り替えて再生（問題なし） | 確認 |
| 5 | 時計が合う | 確認 |
| 6 | USBケーブルの抜き差しで再接続 | 確認 |
| 7 | 長く聴いてもPCがスリープしない | 未確認 |
| 8 | `--check-url` など引数の受け渡し | 未確認 |
| 9 | 実行ポリシーやウイルス対策ソフトの制限がある環境 | 未確認 |

