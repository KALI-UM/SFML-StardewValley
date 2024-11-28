#include "pch.h"


int main()
{
	Framework::GetInstance()->Initialize(1080, 920, "StardewValley");
	Framework::GetInstance()->Do();
	Framework::GetInstance()->Release();
	return 0;
}

