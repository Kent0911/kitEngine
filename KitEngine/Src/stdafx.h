// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <assert.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <list>
#include <map>

#include "../Library/KitLib/include/Kitlib.h"



// TODO: プログラムに必要な追加ヘッダーをここで参照してください

// C4091 externの警告の非表示
#pragma warning (disable:4091)