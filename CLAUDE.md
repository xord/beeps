# Beeps

音声合成・再生ライブラリ。オシレーター、フィルター、エンベロープ、エフェクトによるオーディオ処理チェーンを提供する。

## プロセッサチェーン

`>>` 演算子でプロセッサを接続する:
```ruby
Beeps::Oscillator.new >> Beeps::Gain.new(gain: 0.5)
```

## 外部ライブラリ

ビルド時に以下を自動取得・静的リンクする:
- STK 5.0.1 — DSP ツールキット
- AudioFile 1.1.1 — オーディオファイル I/O
- r8brain-free-src 6.2 — サンプルレート変換
- signalsmith-stretch — タイムストレッチ

## プラットフォーム依存

- macOS: OpenAL, AVFoundation
- Windows: OpenAL, Media Foundation
- Linux: libopenal-dev
