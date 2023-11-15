#include "stdafx.h"
#include "GameApp01.h"


поискать тот воксельный редактор на юнити
сделать из вокселей такие блоки - https://kenney.nl/assets/mini-dungeon
	а потом дальше
	вот еще варианты блоков - https://devilsworkshop.itch.io/big-pixel-isometric-block-pack-free-2d-sprites
	https://devilsworkshop.itch.io/essential-low-poly-isometric-3d-block-and-hex-pack
	https://shallow-lagoon.itch.io/mega-isometric-block-pack
	https://devilsworkshop.itch.io/essential-low-poly-isometric-3d-block-and-hex-pack

найти dnd картк/прикл и в trenchbroom

для тек проекта
взять вот этот ассет - https://kenney.nl/assets/mini-dungeon
не забыть про voxel farm island
еще для общих идей тайлов - https://maxparata.itch.io/monogon-isometricdungeon
еще красиво - https://maxparata.itch.io/medievaltown-monogon
возможно не в магикавоселе, а в тренчбруме или скетчап
для опенворда - https://krishna-palacio.itch.io/minifantasy-tiny-overworld

еще и это - https://www.youtube.com/watch?v=Q1yBrmIf1Yw
	https://www.youtube.com/watch?v=BhX48pe-6zc
	
	https://itch.io/game-assets/tag-medieval/tag-voxel
	
вид сверху как дьябло???



по https://maxparata.itch.io/medievaltown-monogon
	если разделять, то то вот пример - https://filipevoxel.itch.io/voxel-house-contruction
	
а для персов - https://www.youtube.com/watch?v=_DkgWy_KscU и https://www.youtube.com/watch?v=U7IjOxc4skg


https://shallow-lagoon.itch.io/mega-isometric-block-pack

















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

// Overload - простой редактор


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