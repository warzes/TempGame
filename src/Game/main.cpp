#include "stdafx.h"
#include "GameApp01.h"
//
//СОЗДАТЬ ПРОЕКТ
//
//идея
//
//
//простой рогалик от первого лица в стиле DungeonsOfMysteria
//для визуала
//
//https ://lindou.itch.io/modular-dungeon - это можно и в магикавокселе
//https://maxparata.itch.io
//
// voxedit - https://www.youtube.com/@metaseven/videos
// 
//блендер
//и https ://www.youtube.com/watch?v=d7hqdqfmeiw&t=553s
//
//в блокбенче(посмотреть какие там есть плагины)
//https ://www.youtube.com/watch?v=WmYwvtOo4W0
//
//
//	https ://itch.io/game-assets/tag-voxel
//


//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "Engine.lib" )
#endif
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	EngineDeviceCreateInfo createInfo;
	createInfo.window.maximized = false;
	//createInfo.window.width = 1280;
	//createInfo.window.height = 960;
	createInfo.window.vsyncEnabled = true;
	auto engineDevice = EngineDevice::Create(createInfo);
	engineDevice->RunApp(std::make_shared<GameApp01>());
}
//-----------------------------------------------------------------------------