#ifndef __SceneManagerCofing_H_
#define __SceneManagerCofing_H_
/*#include "Scene/Main/Interface_Main.h"
#include "Scene/Game/Interface_Game.h"
#include "Scene/Choice/Interface_GameChoice.h"*/

#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Scene/Main/Interface_Main.h"
#include "Scene/Game/Interface_Game_Scene_1"
#include "Scene/Game/Interface_Game_Scene_2"
#include "Scene/Game/Interface_Game_Scene_3"
#include "Scene/Game/Interface_Game_Scene_4"
#include "Scene/Game/Interface_Game_Scene_5"
#include "Scene/Game/Interface_Game_Scene_7"
#else
#include "Interface_Main.h"
#include "Interface_Game_Scene_1.h"
#include "Interface_Game_Scene_2.h"
#include "Interface_Game_Scene_3.h"
#include "Interface_Game_Scene_4.h"
#include "Interface_Game_Scene_5.h"
#include "Interface_Game_Scene_7.h"
#endif//判断路径的引用

#endif

