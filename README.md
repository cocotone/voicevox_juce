# voicevox_juce

VOICEVOX wrapper library for JUCE framework.

## Overview

`voicevox_juce` is a wrapper library that enables the use of [voicevox_core](https://github.com/VOICEVOX/voicevox_core) within the [JUCE](https://juce.com/) framework. This library aims to simplify the integration of VOICEVOX's text-to-speech capabilities into JUCE-based applications.

## ⚠️ Important Notes

- **This repository is an unofficial wrapper library for [VOICEVOX](https://voicevox.hiroshiba.jp/).**
- **This project is in an experimental stage and its design may change without prior notice.**

## Structure

The library is structured as follows:

- `open_jtalk_dic_utf_8/`: Placement of OpenJTalk dictionary files (Need to be installed by developer)
- `voicevox_core/`: Placement of voicevox_core files (Need to be installed by developer)
- `voicevox_juce/`: Wrapper library that can be imported as a JUCE Module Format
  - `voicevox_client/`: Base classes for client-side implementation
  - `voicevox_core_host/`: Hosting class of voicevox_core library

## Prerequisites

- JUCE framework
- VOICEVOX core library: `voicevox_core`
- C++ compiler with C++17 support

## Installation

1. Clone this repository:
   ```
   git clone https://github.com/cocotone/voicevox_juce.git
   ```
2. Add the `voicevox_juce` directory to your JUCE project's include paths.
3. Add the `voicevox_juce` directory using the `juce_add_module` command in the JUCE CMake module.

## Environment Setup

To use the voicevox_juce library, you need Open JTalk dictionary files, voicevox_core dynamic library, header files, and model files. Here are the steps to download these files and set them up for use with the library:

### 1. Download and Place voicevox_core

1. Go to the [voicevox_core releases](https://github.com/VOICEVOX/voicevox_core/releases) page.
2. Download the downloader appropriate for your OS and architecture.
3. Run the downloaded file and extract its contents.
4. Place the extracted contents in the appropriate OS folder within the `voicevox_juce/voicevox_core` directory:
   - For Windows: `voicevox_juce/voicevox_core/windows`
   - For macOS: `voicevox_juce/voicevox_core/macos`
   - For Linux: `voicevox_juce/voicevox_core/linux`

### 2. Download and Place Open JTalk Dictionary Files

1. Go to the [Open JTalk website](http://open-jtalk.sourceforge.net/).
2. Download the "Binary Package (UTF-8)" from the "Dictionary" section.
3. Extract the downloaded file.
4. Place the contents of the extracted file in the `voicevox_juce/open_jtalk_dic_utf_8` directory.

Once you complete these steps, you'll have the basic environment set up to use the voicevox_juce library. You can then utilize the library's functionality by adding the `voicevox_juce` directory to your project and including the necessary header files.

## License

This project is licensed under [LICENSE](LICENSE). Please see the LICENSE file for more details.

## Attention Regarding Licenses

If you are using this project to create and distribute software, please ensure compliance with the licenses of the following software libraries:

- [voicevox_core](https://github.com/VOICEVOX/voicevox_core)
- [JUCE](https://github.com/juce-framework/JUCE)

---

# voicevox_juce

JUCE フレームワーク用 VOICEVOX ラッパーライブラリ。

## 概要

`voicevox_juce` は、[JUCE](https://juce.com/) フレームワーク内で [voicevox_core](https://github.com/VOICEVOX/voicevox_core) を使用できるようにするラッパーライブラリです。このライブラリは、JUCEベースのアプリケーションにVOICEVOXのテキスト読み上げ機能を簡単に統合することを目的としています。

## ⚠️ 重要な注意事項

- **このリポジトリは、[VOICEVOX](https://voicevox.hiroshiba.jp/) の非公式のラッパーライブラリです。**
- **このプロジェクトは実験段階であり、予告なく設計を変更する可能性があります。**

## 構造

ライブラリは以下のように構成されています：

- `open_jtalk_dic_utf_8/`: OpenJTalk辞書ファイルの配置場所（開発者によるインストールが必要）
- `voicevox_core/`: voicevox_coreファイルの配置場所（開発者によるインストールが必要）
- `voicevox_juce/`: JUCE Module Format としてインポート可能なラッパーライブラリ
  - `voicevox_client/`: クライアント側実装のためのベースクラス
  - `voicevox_core_host/`: voicevox_coreライブラリのホスティングクラス

## 前提条件

- JUCEフレームワーク
- VOICEVOXコアライブラリ: `voicevox_core`
- C++17をサポートするC++コンパイラ

## インストール

1. このリポジトリをクローンします：
   ```
   git clone https://github.com/cocotone/voicevox_juce.git
   ```
2. `voicevox_juce`ディレクトリをJUCEプロジェクトのインクルードパスに追加します。
3. JUCE CMakeモジュールの `juce_add_module` コマンドで `voicevox_juce` ディレクトリを追加してください。

## 環境構築方法

voicevox_juceライブラリを使用するには、Open JTalk の辞書ファイル、voicevox_coreの動的ライブラリ、ヘッダファイル、そしてモデルファイルが必要です。以下に、これらのファイルをダウンロードし、ライブラリで使用できるようにするための簡単なセットアップ手順を説明します。

### 1. voicevox_coreのダウンロードと配置

1. [voicevox_coreのreleases](https://github.com/VOICEVOX/voicevox_core/releases)ページにアクセスします。
2. お使いのOS、アーキテクチャに合ったダウンローダをダウンロードします。
3. ダウンロードしたファイルを実行し、展開します。
4. 展開されたファイルの内容を`voicevox_juce/voicevox_core`ディレクトリ内の、該当するOSのフォルダに配置します。
   - Windowsの場合：`voicevox_juce/voicevox_core/windows`
   - macOSの場合：`voicevox_juce/voicevox_core/macos`
   - Linuxの場合：`voicevox_juce/voicevox_core/linux`

### 2. Open JTalkの辞書ファイルのダウンロードと配置

1. [Open JTalkのウェブサイト](http://open-jtalk.sourceforge.net/)にアクセスします。
2. "Dictionary" セクションから "Binary Package (UTF-8)" をダウンロードします。
3. ダウンロードしたファイルを解凍します。
4. 解凍したファイルの内容を`voicevox_juce/open_jtalk_dic_utf_8`ディレクトリに配置します。

これらの手順を完了すると、voicevox_juceライブラリを使用するための基本的な環境が整います。プロジェクトに`voicevox_juce`ディレクトリを追加し、必要なヘッダファイルをインクルードすることで、ライブラリの機能を利用できるようになります。

## ライセンス

このプロジェクトは [LICENSE](LICENSE) の下でライセンスされています。詳細については、LICENSE ファイルをご覧ください。

## ライセンスに関する注意

このプロジェクトを使用してソフトウェアを作成・配布する場合、以下のソフトウェアライブラリのライセンスに準拠していることを確認してください：

- [voicevox_core](https://github.com/VOICEVOX/voicevox_core)
- [JUCE](https://github.com/juce-framework/JUCE)
