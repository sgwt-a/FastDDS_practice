Fast DDS Security（DDS Security）について、技術者向けの教育資料としてスライドを作成してください。

## 目的
Fast DDSのDDS Securityを理解し、既存システムへ新しいParticipantを追加する際に必要となる証明書、鍵、Permissionsの役割と運用方法を理解すること。

## 想定読者
・DDSは利用経験がある
・Discovery Serverは理解している
・DDS Securityは初心者
・PKI（公開鍵暗号）はあまり詳しくないソフトウェアエンジニア

## スライド作成方針

・1ページあたりの情報量を多めにし、なるべくページ数を少なくする
・図を多用する
・文章だけではなく構成図、シーケンス図、関係図を入れる
・「何のために存在するのか」を最初に説明してから各機能を説明する
・Fast DDSの仕様だけでなく、実際の運用方法も説明する
・最後は実案件での追加手順をまとめる

---

## スライド1
DDS Securityとは

説明してほしい内容

・DDS Securityとは何か
・Securityなしの場合の問題点
・認証(Authentication)
・認可(Authorization)
・暗号化(Cryptographic)
・アクセス制御
・Fast DDS Securityの構成
・PKI-DHとは何か
・DDS通信開始までの流れ

図を入れること

Participant
↓
認証
↓
認可
↓
暗号鍵共有
↓
通信開始

---

## スライド2
DDS Securityで利用するファイル一覧

以下について説明

identity_ca.pem
identity_ca.key
permissions_ca.pem
permissions_ca.key
participant.pem
participant.key
governance.xml
governance.p7s
permissions.xml
permissions.p7s

説明内容

・誰が作るのか
・誰が持つのか
・どこへ配布するのか
・役割
・漏洩すると何が起こるか

表形式でまとめる

さらに

CA
↓
Participant証明書発行

の関係図を作る

---

## スライド3
証明書・鍵の作成方法

以下を順番に説明

①Identity CA作成

②Permissions CA作成

③Participant秘密鍵作成

④CSR作成

⑤Participant証明書発行

⑥Governance作成

⑦Permissions作成

⑧Governance署名

⑨Permissions署名

OpenSSLコマンド例も掲載する

CSRとは何かも説明する

---

## スライド4
Participant認証の流れ

シーケンス図を作成

PC-B

↓

ECU

で

Participant証明書送信

↓

Challenge

↓

秘密鍵で署名

↓

公開鍵で検証

↓

認証成功

を図解する

公開鍵・秘密鍵・CA証明書がどこで使われるかも示す

---

## スライド5
Permissions.xmlとGovernance.xml

それぞれの役割を比較

Governance

・ドメイン全体のルール

Permissions

・Participantごとの権限

比較表を作る

また

PermissionsはParticipant単位

Static DiscoveryはDataWriter/DataReader単位

であることも比較して説明する

---

## スライド6（最重要）
既存システムへ自社PCを追加する場合

前提

既存

ECU（Publisher）
ECU（Subscriber）

↓

新規

PC-B（Subscriber）

追加したい

目的

ECUが送信するVehicleStatusをPC-Bでも受信したい

構成図を描く

ECU(Publisher)

↓

LAN Switch

↓

ECU(Subscriber)

↓

PC-B(Subscriber)

Discovery ServerはECU

DDS Security有効

説明内容

他社が管理しているもの

・Identity CA
・Permissions CA
・Governance
・Permissions

自社が作成するもの

・pc_b.key
・pc_b.csr

他社へ依頼するもの

・pc_b.pem
・permissions.p7s更新

PC-Bへ配置するもの

identity_ca.pem
permissions_ca.pem
pc_b.pem
pc_b.key
governance.p7s
permissions.p7s

Discovery Server設定

Server IP
Server Port
Server GUID Prefix

も必要であることを説明する

最後に

「他社に依頼するもの」
「自社で準備するもの」

を一覧表でまとめる

---

## スライド7
まとめ

以下を一覧表でまとめる

・各ファイルの役割
・誰が管理するか
・Participant追加時に更新が必要か
・配布先
・秘密にする必要があるか

最後に

Participant追加時のチェックリスト

□ Participant秘密鍵作成

□ CSR作成

□ 他社へCSR提出

□ Participant証明書受領

□ Permissions更新

□ permissions.p7s受領

□ Discovery Server設定

□ 動作確認

までを一覧化する。

## 出力形式

・PowerPointでそのまま利用できる内容
・図や表を積極的に利用
・ページ数は6〜7ページ程度に抑える
・各ページは情報量を多めにし、内容が薄くならないようにする
・Fast DDS Securityの仕様だけでなく、実際の開発・運用でどのように扱われるかを重視して説明する
