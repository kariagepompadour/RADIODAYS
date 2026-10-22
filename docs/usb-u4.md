# U4：昼の空、USB待機の案内、国別Location、人工衛星、Wi-Fiの3件保存

- 作成日：2026-10-21
- 状態：**実装、ビルド、ホスト試験、シミュレーション、中継の規則の試験まで。実機ではまだ試していない**
- 設計：[u4-proposal.md](u4-proposal.md)（作者の承認：2026-10-21）
- もとにした版：U3（作者が正常動作を確認した保存版。`firmware/RadioDays_U3/` と `RadioDaysRelay_U3.command` は変更していない）
- Stage 3-3以前、U1、U2a、U2a2、U2a3、U2a4、U3、Macの中継U2・U2a2・U3のファイルは変更していない（作業の前後でSHA-256が一致。第6.6節）

---

## 1. 変更の内容（作者の指示の順）

| # | 項目 | 結果 |
| --- | --- | --- |
| ① | 昼の空 | 実装した。背景 `#344F65`、雲は約55%の大きさ、色は落ち着いた白 `(200,206,212)` と青灰 `(130,148,165)`。太陽・夜空・月・周波数・オレンジの文字は変えていない（関数が同じことを比較で確認） |
| ② | USB専用の待機 | 実装した。状態の行 `USB RELAY REQUIRED`、局名の行 `Start the relay on your computer.`。中継が見つかると自動で通常の表示に戻る。本体の表示から「Mac」をなくした |
| ③ | 国別Locationと放送局 | **14のLocation、58局**を追加した（全体で19のLocation、85局）。表示名 `THAI`・`ARGENT`・`AUSSIE`・`NZ`（作者の指定）。**INDIAは見送った**（第3.3節）。USB中継のための「転送先だけのドメインの決まり」を、新しい中継 `RadioDaysRelay_U4.command` に入れた |
| ⑤ | UFO→人工衛星 | 実装した。**絵だけを差し替え**、移動・描画のタイミング・電波の処理はU2a3のUFOのまま。負荷は同じ（第6.4節） |
| ④ | Wi-Fiを最大3件 | 実装した。12秒／30秒、最後に成功したものから、必要なときだけ1回検索、強い順。NVSの旧形式と互換 |

実装の順番は指示どおり①→②→③→（容量の確認）→⑤→④。

## 2. 容量

| 時点 | アプリの大きさ | 0x150000までの残り |
| --- | --- | --- |
| U3 | 1,363,120 B | 13,136 B |
| ①〜③の後（実測） | 1,368,400 B（+5,280 B） | 7,856 B |
| ⑤・④の後（**U4**） | **1,374,080 B**（U3から+10,960 B） | **2,176 B** |

- ①〜③の後で測り、⑤（絵の差し替えだけ。大きさはほぼ0）と④（約5.7 KB）が入ることを確かめてから実装した
- **規定の容量（0x150000）以内に収まった。** 局数とWi-Fiの機能は削っていない。地球儀の地図の圧縮・記録の文字列の整理は、今回は不要だったので行っていない（次に機能を足すときは、まずこれを行う。見込みで約3 KB）
- ④は見込み（2〜3 KB）より大きかった（約5.7 KB：保存した一覧の画面、入れ替えの確認、検索の処理、記録の文字列）
- 静的RAM：59,740 B（U3は59,044 B。+696 B：保存した3件の写し2つ、検索の候補）
- SHA-256：`718dcf3295f2f47daf5018cf83a3db9215ff6e82756b38efb8bc61c902beed58`
- 自分のコードの警告：0件

## 3. ③ 国別Locationと放送局

### 3.1 Locationの並び（19）

JAPAN, USA, UK, FRANCE, WORLD（**U3のまま**）→ GERMANY, HOLLAND, POLAND, CZECHIA, SWEDEN, ITALY, HUNGARY, THAI, TAIWAN, AUSSIE, NZ, CANADA, BRAZIL, ARGENT

| 項目 | 内容 |
| --- | --- |
| 保存したLocationとの互換性 | 本体は番号と名前を保存し、名前で探し直す（`savedLocationIndex()`、U3と同じ）。新しいLocationはWORLDの後ろだけに足したので、U3で保存したJAPAN〜WORLDはそのまま開く。**ビルド時の検査**：最初の5つの名前と位置、Locationの名前の重複なし、7文字以内、局名19文字以内（`stations.h` の `static_assert`） |
| U3に戻した場合 | U4で新しいLocation（例：NZ）を保存してからU3を入れると、U3はその名前を知らないので既定のUSAで始まる（U3の作りのとおり。壊れたり書き換えられたりはしない） |
| ロータリーエンコーダー | 操作は変えていない（1目盛りで1つ、端で一周、`,`・`/` キーも同じ）。20近くになったので、実機で回しやすさを見てほしい |
| 地球儀を作れないときの文字の一覧 | 5行ずつのページにし、選んでいるLocationのページを出す（常に見える。ホスト試験で19すべてを確認） |
| 都市（地球儀の印と昼夜） | 各国の首都：Berlin、Amsterdam、Warsaw、Prague、Stockholm、Rome、Budapest、Bangkok、Taipei、Sydney（オーストラリアは最大都市）、Wellington、Ottawa、Brasília、Buenos Aires |
| 周波数 | 今までどおり仮想の周波数（80.0〜90.0）。局数に合わせて等間隔に置いた |

### 3.2 放送局（58局）

**区分**：A＝そのまま（`http://`、MP3、転送なし）／B＝転送先が毎回変わる（USB中継はU4の中継が必要、Wi-Fiは再生の部品が転送をたどる）。2026-10-21にこのMacから接続し、MP3のフレームを確かめた（**Cardputerでの再生は未確認**）。

| Location | 局（局の都市） | 区分 |
| --- | --- | --- |
| GERMANY | Deutschlandfunk（Cologne）、DLF Kultur（Berlin）、1LIVE（Cologne）、WDR 5（Cologne）、Hit Radio FFH（Frankfurt） | 前の4局はB（`*.rndfnk.com`）、FFHはA |
| HOLLAND | NPO Radio 1、NPO Radio 2、NPO 3FM、NPO Klassiek（Hilversum）、Radio Continu（Groningen） | A（NPOは192k） |
| POLAND | PR Jedynka、PR Dwojka、**PR Radio Chopin**（Warsaw）、RMF FM（Krakow）、Radio ZET（Warsaw） | Polskie Radioは `ICY 200`、Radio ZETはB（`*.cdn.eurozet.pl`、2回転送） |
| CZECHIA | CRo Radiozurnal、CRo Dvojka、CRo Vltava、Radio Beat、Radio Blanik（Prague） | A |
| SWEDEN | Pirate Rock（Gothenburg）、Country Rocks、FUN RADIO 95.3、**Radio Sotenas**（Sotenäs） | A |
| ITALY | Virgin Radio Italia、Radio 105、Radio Monte Carlo（Milan）、Radio Radicale（Rome） | A |
| HUNGARY | Kossuth Radio、Petofi Radio、Radio 1（Budapest） | A |
| THAI | FM 94 Bangkok、Smooth 105.5、Voice of Navy 88.5（Bangkok） | A |
| TAIWAN | City FM 92.9（Taipei）、City FM 90.1（Taichung）、City FM 97.1（Tainan）、Family 97.7（Taichung） | A |
| AUSSIE | Kiss FM（Melbourne）、Three D Radio（Adelaide）、Skylab Radio（Melbourne） | A |
| NZ | RNZ National、RNZ Concert（Wellington）、RNZ Pacific（Auckland）、The Most FM（Christchurch）、Compass FM（Kerikeri） | A。**RNZの3局は公式に公開されたURL**。RNZ NationalはWORLDにも残した（WORLDの5局と順番は変えていない） |
| CANADA | Soft Rock 97.7（CHUP、Calgary）、CHOI Radio X（Quebec City）、DJFM（Toronto） | A |
| BRAZIL | Radio Batuta、Batuta Classicos（Rio de Janeiro）、91 Rock（Curitiba）、Continental FM（Porto Alegre） | A |
| ARGENT | Nacional Folklorica、Nacional Rock、Nacional Clasica、Radio Nacional 870（Buenos Aires）、Nacional Cordoba（Córdoba） | A。配信の名前（`icy-name`：FOLK、ROCK-93.7、CLASICA、AM870、LRA7 Cordoba）で局を確かめた |

- 局名は、本体の文字（アクセントなし）に合わせてASCIIにした（例：Petőfi→Petofi、Rádió→Radio）
- 256〜320 kbpsの局は入れていない（Raiなど。設計書第3.5節）

### 3.3 設計書の61局からの変更

| 設計書 | U4 | 理由 |
| --- | --- | --- |
| **INDIA（3局）** | **見送り**（Locationも作らない） | 所在地と配信元をもう一度確かめた（作者の指示）。radioBollyFM：配信の名前とサイト（radiobollyfm.in）は一致するが、所在地はサイトにも書かれておらず、第三者の一覧が「インド」とするだけ。Chennai Live：配信は名前のない「RadioBOSS Stream」で、局のサイト（chennailive.fm）は名前が引けず、この配信がChennai Live 104.8 FMのものか確かめられない。2B! Radio Sangam：URLは `millenniumhits`、配信の名前は「Airtime!」で、局名とも合わない。**国別Locationとして妥当と言えないため、採用しなかった**。インドの公共放送（AIR）は `http://` のMP3が見つからない（設計書） |
| Polskie Radio Trójka | **PR Radio Chopin**（同じPolskie Radioの公共放送） | Trójkaのポート（8904）は、この日 `ICY 401 Service Unavailable` を返し、ほかのホストでも同じだった。Radio Chopin（ポート8910）は `ICY 200`、192k |
| Mix Megapol（B、`*.sharp-stream.com`） | **Radio Sotenas**（A） | この日、3回とも接続できなかった（10秒でタイムアウト）。Radio Sotenäsは `icy-name: Radio Sotenas`、サイト radiosotenas.se。これにより `*.sharp-stream.com` の決まりは不要になり、入れていない |

結果：**14のLocation、58局（A＝53、B＝5）**。Location全体は19、局は85。

### 3.4 USB中継：転送先だけのドメインの決まり（`RadioDaysRelay_U4.command`）

| 条件（作者の指示） | 実装 |
| --- | --- |
| 最初のTUNEのURLは完全一致 | 変えていない。許可の一覧の `host:port` と一致しなければ `RDNAK HOST`／`PORT`。決まりのドメインのホストでも、最初のURLとしては拒否（試験U4.3） |
| ワイルドカードは転送先にだけ | `check_url($url, 1)`（転送先）のときだけ決まりを見る |
| ドメインの境界を厳密に | ホスト名を小文字にし、今までの検査（使える文字、ラベルの形、IPアドレスでない）を通した後、「`.` ＋ドメイン」で終わり、その前に1つ以上のラベルがあるときだけ一致。`evilrndfnk.com`、`rndfnk.com`、`rndfnk.com.example.org`、末尾の `.` は不一致 |
| HTTPだけ、最大3回 | 変えていない（`https://` の転送先、4回目の転送は `RDNAK REDIR`） |
| IPアドレス、ユーザー情報 | 変えていない（拒否） |
| ポート | 決まりのポートと完全一致（`:80`） |
| 決まり | `*.rndfnk.com:80`（Deutschlandfunk、WDR）、`*.cdn.eurozet.pl:80`（Radio ZET）の2つだけ。ドメインは2ラベル以上（`*.com` のような決まりは読み込まない） |
| `allowed_hosts.txt` | 「`redirect *.domain:port`」の行も書ける（書いた場合は、組み込みの一覧と決まりの代わりに、そのファイルだけを使う。U3と同じ考え方） |
| 確認の命令 | `--check-url`（最初のURL）に加えて `--check-redirect`（転送先として）を追加 |
| U3の中継 | 変えていない。U3の中継でU4を使うと、U3の27局は今までどおり、U4の新しい局は `RDNAK HOST`（許可の一覧にない） |
| 時刻・クレジット・TUNE・RDSYNC | U3の中継のまま |

## 4. ⑤ 人工衛星（Location画面）

| 項目 | 内容 |
| --- | --- |
| 絵 | 24×12（UFOと同じ大きさ）。金色の本体、左右に青い太陽電池パネル（格子）と支柱、上にアンテナの灯り、下に地球へ向いたアンテナの皿。本体の3つの灯りが、UFOの縁の灯りと同じく順に点く |
| そのまま使ったもの | 位置（地球儀の右上）、浮遊（2つの波）、速さの変化と一時停止、交信（地球→衛星の水色の電波1.3秒、灯りの点滅0.6秒、衛星→地球の黄色の電波1.3秒、15〜25秒ごと）、電波の描き方、「姿が変わったときだけ地球儀を描き直す」決まり（`ufoPoseAt()`・`talkDraw()`・`globeService()` はU3と1文字も違わない） |
| 変えたもの | 絵（`UFO_ART`）、色の表、点滅する灯りの行（6→5）。変数名 `ufo…` はU2a3のコードであることがわかるように残した |
| 地球儀の回転、Locationの選択、音声、USB中継 | 変えていない |
| 双方向の電波 | もとから交互（地球→衛星、点滅、衛星→地球）。同時に両方向へは描かない（試験で確認） |

比較の図：[images/satellite-talk-preview-u4.png](images/satellite-talk-preview-u4.png)（左から：浮遊、地球→衛星、灯りの点滅、衛星→地球。Macで、スケッチのコードで描いたもの。地球儀は単色で代用）

## 5. ④ Wi-Fiを最大3件

### 5.1 背景の接続（ラジオの画面）

```text
探索の開始（起動、Wi-Fiが切れた後、やり直しの時刻）
  ① 最後に成功したネットワーク：WiFi.begin（中でチャンネルを探す）
       つながった → 完了
       「見つからない」「パスワードが違う」→ すぐに次へ（3秒後から信用する。設定の画面と同じ）
       12秒 → 次へ
  ② 保存が2件以上のときだけ：一覧を1回検索（非公開のSSIDは探さない）
  ③ 見つかった保存済みのものを電波の強い順に、チャンネルとBSSIDを指定して（12秒ずつ）
  ・探索全体は30秒まで。だめなら5→10→20→40→60秒あけて、次の探索も①から
```

| 項目 | 内容 |
| --- | --- |
| 待たないこと | `loop()` の中で待つ処理はない（状態を見て進むだけ。検索も非同期） |
| USB中継 | 中継が答えている間は探索しない（U2a2から）。検索の途中で中継が答えたら止め、残った検索の結果は捨てる |
| Uキー（USB専用） | Wi-Fiの接続も検索も一切しない（U2a4から変えていない。試験で3分間確認） |
| 別の保存済みネットワークにつながったとき | 「最後に成功した」をそれに変え、NVSに書く（`wifis` と `wifi`。1回だけ） |
| 設定の画面の接続（`W`） | 今までどおり20秒（利用者が見ている接続のため） |

### 5.2 NVS

| キー | 内容 | U4での扱い |
| --- | --- | --- |
| `wifi`（U3以前） | 1件（112バイト、形式は変えていない） | **最後に成功したネットワークを書き続ける**。U3以前に戻しても、そのネットワークで動く |
| `wifis`（新規） | 最大3件、最後に成功した番号、CRC（308バイト） | 追加・入れ替え・削除・「最後に成功した」が変わったときだけ書く |

- 引き継ぎ：`wifis` がなく `wifi` が正しく読めれば、それを1件目として使う。**起動時には何も書かない**
- `wifis` が壊れている：`wifi` のネットワークを使い、`wifis` は書かない（保存するときは、今までと同じく利用者に確かめる）。新しい版の `wifis`：書かない
- すべて削除した後：`wifis` は「0件」になり、U4は `wifi` を使わない。**`wifi` の中身は消さない（NVSを消す操作はしない約束）ので、U3に戻すと最後のネットワークが使われる**
- 非公開のSSID：今までどおり対応しない

### 5.3 本体での操作

| 画面 | 操作 |
| --- | --- |
| Wi-Fiの一覧（`W`） | 保存しているネットワークに `*`。`S` で保存した一覧へ |
| 保存した一覧（新規） | 最大3件（`*` は最初に試すもの）。ノブ／`;` `.` で選び、ok／押すで今すぐ接続、`del` で削除（`Y`／`N` で確認）、esc で一覧へ |
| 4件目に接続できたとき | 「どれと入れ替えるか」を出す。ok／押すで入れ替え、esc で保存せずに使う |
| 同じネットワークにもう一度接続 | パスワードを更新（件数は増えない） |

## 6. 試験

### 6.1 U4のホスト試験（新規：`tools/u4-test/`）

```bash
sh tools/u4-test/run_u4_test.sh
```

スケッチの関数（NVS、Wi-Fiの探索、START画面、保存・入れ替え・削除、案内の条件、Locationの照合、文字の一覧）を名前で切り出し、本物の `stations.h` と一緒にMacで動かす。Wi-Fiの代わりの部品は、ESP32の動き（見つからないと2.5秒で「見つからない」、検索は4秒、BSSID指定は1秒）をまねる。**55項目すべて合格。**

| 分類 | 確かめたこと |
| --- | --- |
| NVSの互換性 | `wifi` の形は112バイトのまま／U3の `wifi` だけ：1件目として使い、起動で書かない、同じネットワークにつながっても書かない／`wifis` が壊れている・新しい版：書かない／2件目を保存すると `wifi` が最後のネットワークになり、**U3の読み込みの関数（U3のスケッチから切り出したもの）がそれを読める** |
| 保存・入れ替え・削除 | 同じネットワークはパスワードの更新だけ／4件目は確認を出し、確認までは書かない／入れ替え／保存せずに使う／削除で順番と「最初に試すもの」が正しく変わる |
| 探索 | 1件で近くにない：約3秒で終わり、検索しない／最後のものが近くにない：検索1回、強いもの（BSSID指定）に8秒でつながり、NVSに1回書く／次の起動はそれに2.5秒で、書かない／パスワード違い：すぐ次へ／3件とも答えない：1回12秒、全体30秒で止まる／5分間何もない：探索7回（5→60秒の間隔）／非公開のSSIDは探さない |
| USB | Uキー：3件保存・1件が近くにあっても、3分間接続も検索もしない、何も書かない／検索中に中継が答えると止まり、検索の結果を捨てる／中継がいなくなると①から |
| 画面 | START画面「USB   : waiting for the relay」（「Mac」なし）、「Saved : 2 networks」／最下行は検索中も「joining」／案内（USB RELAY REQUIRED）の条件の7通り |
| Location | 19か所、85局／U3で保存したJAPAN〜WORLDは同じLocationで開く／番号が違っても名前で見つかる／壊れた記録は既定／文字の一覧で選んだLocationが常に見える |

### 6.2 中継の決まりの試験（新規：`tools/relay-rule-test/`）

```bash
sh tools/relay-rule-test/run_relay_rule_test.sh          # 手元だけ
sh tools/relay-rule-test/run_relay_rule_test.sh --live   # 転送する局に今つないで確かめる
```

**28項目すべて合格**（`--live` を含む）：U4の85局のURLはすべて最初のURLとして許可／決まりのドメインのホストは最初のURLとしては拒否／転送先：`d121.rndfnk.com`（長い問い合わせ付き）、2段下のホスト、`zt.cdn.eurozet.pl`、大文字は許可。`evilrndfnk.com`、`rndfnk.com`、`rndfnk.com.example.org`、`d121.rndfnk.com.evil.net`、別のポート、`eurozet.pl`・`x.eurozet.pl`、IPアドレス、ユーザー名、`https`、末尾の `.`、決まりにないホスト（Raiの配信網）は拒否／U3の中継はU4の新しいホストを知らない／**実際の転送**：Deutschlandfunk・DLF Kultur・1LIVE・WDR 5（→ `*.rndfnk.com`）、Radio ZET（→ `zt.cdn.eurozet.pl` → `zt05.cdn.eurozet.pl`、2回）を、U4の中継の検査がすべて許可

### 6.3 シミュレーション（`tools/usb-relay/sim4/`：sim3に転送の決まりの試験を加えたもの）

```bash
sh tools/usb-relay/sim4/run_sim.sh             # U4＋中継U4
sh tools/usb-relay/sim4/run_sim.sh --oldrelay  # U4＋中継U3
sh tools/usb-relay/sim4/run_sim.sh --negative  # 確認用
```

U4のリンク（`radiodays_link.h`）はU3と同じ（`cmp` で確認）。本物の中継のスクリプトを擬似端末で動かす。手元の試験局は `localhost`、決まりは `redirect *.cdn.localhost:ポート`（curlは `*.localhost` をこのMacとして扱う）。

（結果は第6.5節）

追加した項目：U4.1 決まりのドメインへの転送で再生／U4.2 決まりのドメインの中で2回転送（Radio ZETと同じ）／U4.3 最初のURLには決まりを使わない／U4.4 境界のないホスト、ドメインそのもの、別の後ろ／U4.5 別のポート（その試験局には1回もつながない）／U4.6 ユーザー名、https／U4.7 4回以上の転送／U4.8 拒否の後も普通の局が鳴る。中継U3では、決まりのドメインへの転送が拒否されること。

**試験の直し（sim4だけ）**：「Cardputerが読まないと、中継の記録が『クレジット待ち』になる」項目は、中継の10秒ごとの記録と12秒の停止の時間の重なりで結果が変わる作りだった（重なりが少ないと最小25%）。sim3の順番に試験を足したことで重なりが変わり、2回とも35%で不合格になった。中継の処理は変えていない（U3と同じ部分）。sim4では、中継の記録が出た直後から停止を始めるようにし、10秒の記録が停止の中に入るようにした。sim3は変更していない。

### 6.4 人工衛星の試験（新規：`tools/sat-test/`。ufo-testを広げたもの）

```bash
sh tools/sat-test/run_sat_test.sh
```

スケッチのコードを切り出し、10分間（33ミリ秒ごと）を動かす。**同じ試験をU2a3のUFOにも行い、比べた。**

| 項目 | U4（人工衛星） | U2a3（UFO、実機試験合格） |
| --- | --- | --- |
| 地球儀の上に描いた点 | 0 | 0 |
| 絵の点（常にすべて見える） | 121／121 | 147／147 |
| 1こまの電波の点（最大） | 46 | 46 |
| 電波の向き | 1こまに1方向、地球→衛星と衛星→地球が交互（52回） | 同じ |
| 浮遊の幅、交信の間隔 | ±5／±3、18.2〜27.5秒 | 同じ |
| 描き直しの回数 | **毎秒8.2回** | **毎秒8.2回** |

描き直しの回数と電波の点の数がUFOと同じで、絵の点は少ない。**新たな待ち、計算、描画の回数の増加はない。** そのため、作者の指示どおり、新たな負荷の測定や大規模な実機試験は必要ないと判断した。

### 6.5 シミュレーションと回帰試験の結果

| 試験 | 結果 |
| --- | --- |
| `sh tools/usb-relay/sim4/run_sim.sh`（U4＋中継U4） | **57項目すべて合格**（sim3の45項目、U4.1〜U4.8の11項目、全体の確認。第6.3節の直しの前は、時間の重なりの1項目が35%で不合格だった） |
| `--oldrelay`（U4＋中継U3） | **9項目すべて合格**（時刻が届く、再生・選局、決まりのドメインへの転送は拒否） |
| `--negative`（確認用） | あふれを検出（試験が正しく働く） |
| `sh tools/usb-relay/sim3/run_sim.sh`（U3＋中継U3、比較用。変更なし） | **45項目すべて合格**（U3の記録と同じ） |
| `sh tools/time-test/run_time_test.sh`（U3の時刻。U4のリンクはU3と同じ） | リンク15項目・時計11項目すべて合格 |
| ビルド | 0x150000以内、自分のコードの警告0件 |

### 6.6 変更していないことの確認

作業の前に、`firmware/` と `tools/` のすべてのファイル（118個）のSHA-256を記録し、作業の後で比べた。**118個すべて一致**（Stage 3-3以前、U1、U2a〜U3、中継U1〜U3、sim〜sim3、start-test、ufo-test、time-test）。U4のファイルの一覧とSHA-256は `firmware/RadioDays_U4/SHA256SUMS.txt`

## 7. ⑧ 公開前の確認：公式の配信元と利用条件（途中経過）

**第三者の一覧（Radio Browser、radio.dubbeh.net など）で見つけたURLは、公式確認済みとして扱っていない。**

| 局 | 公式の配信元 | 利用条件 |
| --- | --- | --- |
| RNZ National・Concert・Pacific（NZ、WORLD） | **確認済み**：RNZのヘルプのページに、第三者のプレーヤー用として同じURLが載っている（[RNZ: What audio is available?](https://www.Rnz.Co.Nz/listen/audiohelp)） | 未確認 |
| Radio Swiss Jazz（WORLD） | **同じホストの別のURLが公式**：公式のページは `https://stream.srg-ssr.ch/srgssr/rsj/mp3/128`（MP3 128k）を載せている（[Radio Swiss Jazz: Internet](https://www.radioswissjazz.ch/en/reception/internet)）。RADIO DAYSの `/m/rsj/mp3_128` は載っていない。公式のURLは `http://` だと別のホストへ転送される（`livestreaming-node-3.srg-ssr.ch`）ので、使うには中継の決まりが要る | 未確認（ページに記載なし） |
| SomaFM（Groove Salad、Secret Agent。WORLD） | **RADIO DAYSのURL（`ice1.somafm.com`）は公式のページにない**。公式は `https://ice2/ice5.somafm.com/...-128-mp3`（[SomaFM: direct stream links](https://somafm.com/groovesalad/directstreamlinks.html)） | **公式のページに条件あり**：直接のリンクは「個人の利用だけ」、ゲームや配信などでの利用は不可、許可のない利用は遮断される |
| KEXP（USA） | 公式のヘルプのページに、配信の相手先として `kexp.streamguys1.com` の名前はあるが、URLは載っていない（[KEXP Help](https://www.kexp.org/help/)） | 未確認（利用規約のページは別） |
| Deutschlandfunk・WDR、NPO、Radio France、Český rozhlas、Radio Paradise | 公式のURLの一覧は見つからなかった（検索で出るのは第三者の一覧と掲示板だけ）。Radio Franceは公式のAPIがある、という第三者の情報がある | 未確認 |
| そのほかの局（U3の局を含む） | 未確認 | 未確認 |

公開の前に作者が判断すること：SomaFMの条件（個人の利用だけ）と、局のURLを入れたプログラムを配ることとの関係。Radio Swiss JazzとSomaFMのURLを公式のものに替えるかどうか（WORLDの局の並びは変えずにURLだけ替えることはできる。今回は替えていない）。

## 8. 実機で確かめる手順（導入は作者の判断で）

### 準備

1. PManで、空き（FREE）が0x150000以上あることを確かめる（U4の大きさは1,374,080 B）
2. Launcherで `firmware/RadioDays_U4/RadioDays_U4.ino.bin` を入れる（「Need …」が出たら「Cancel」）。START画面の右上が「U4」
3. Macでは `tools/usb-relay/RadioDaysRelay_U4.command` を使う（U3の中継でも、U3の27局とWi-Fiの局は使える。U4の新しい局はUSBでは拒否される）

### 手順

| # | 場面 | 期待すること |
| --- | --- | --- |
| 1 | START画面 | 「USB   : waiting for the relay」、「Saved : 1 network」（U3で保存したネットワーク）。Wi-Fiの名前はU3と同じ |
| 2 | 職場：`U` | 中継を起動する前は、状態が `USB RELAY REQUIRED`（黄）、局名の行に `Start the relay on your computer.`。中継を起動すると、局名と `CONNECTING`→`PLAYING` に変わる。シリアルの記録に `[WIFI] connecting` が出ない |
| 3 | 昼の空 | 空の窓が落ち着いた青灰色、小さな雲2つ。太陽・夜空・月はU3と同じ |
| 4 | `L`（Location画面） | 地球儀の右上に人工衛星。15〜25秒ごとに地球→衛星（水色）、灯りの点滅、衛星→地球（黄色）。ノブで19か所を回り、端で一周。U3で選んでいたLocationで始まる |
| 5 | 新しいLocationで再生（USB） | GERMANY（Deutschlandfunk：中継のターミナルに「Redirected to http://…rndfnk.com…」）、POLAND（Radio ZET：2回転送）、HOLLAND、NZ など |
| 6 | 家（Wi-Fi） | 今までどおり接続（ふだんは1回の接続で終わる）。新しいLocationの局（転送する局を含む）がWi-Fiで鳴るか |
| 7 | Wi-Fiの設定 | `W` で一覧（保存したものに `*`）、`S` で保存した一覧、2件目を追加、削除（`Y`／`N`）。2件を保存して、最後のネットワークが近くにない場所で起動すると、もう一方につながる |
| 8 | 音切れ、操作、USBの再生、時刻 | U3と同じ（人工衛星の画面でも） |

## 9. 未確認事項

| 項目 | 内容 |
| --- | --- |
| U4の実機での動作 | 未確認（第8節） |
| 新しい58局の再生 | Macからの接続で確かめただけ。Cardputerでは未確認（特に、48kHzの局、Polskie Radioの `ICY 200` のWi-Fiでの再生、Wi-Fiでの転送） |
| Wi-Fiの探索の実際の時間 | ESP32の「見つからない」が何秒で出るか、検索に何秒かかるかは、実機で測っていない（Arduinoのコードを読んだ見込み） |
| 20近いLocationでのノブの回しやすさ | 実機で確認が必要 |
| 人工衛星の見た目 | Macでの再現だけ。実機の画面の色は少し違う可能性がある |
| 局の公式の配信元と利用条件 | 第7節（ほとんどが未確認） |
| インドの局 | 見送り（第3.3節）。公式の局が見つかれば、後で加えられる（WORLDの後ろに足すので互換性は保てる） |
