#include "pch.h"
#include "AnimationClip.h"
#include "rapidcsv.h"
#include "TexCoordTable.h"

bool AnimationClip::loadFromFile(const std::string& filePath)
{
	rapidcsv::Document doc(filePath);
	id = doc.GetCell<std::string>(0, 0);
	fps = doc.GetCell<int>(1, 0);	
	loopType = (AnimationLoopTypes)doc.GetCell<int>(2, 0);

	frames.clear();
	for (int i = 3; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		frames.push_back(
			{ 
				row[0], 
				{ std::stoi(row[1]), std::stoi(row[2]), std::stoi(row[3]), std::stoi(row[4]) }
			});
	}

	return true;
}

bool AnimationClip::loadFromTexId(const std::string& clipId, AnimationLoopTypes looptype, int fps, const std::string& texId)
{
	id = clipId;
	this->fps = fps;
	loopType = looptype;

	frames.clear();
	const TexRes& currTexres = TEXRESTABLE_MGR->GetTileTexRes(texId);
	if (currTexres.children.size() == 0) {
		frames.push_back(AnimationFrame(currTexres.filepath, currTexres.texcoord));
	}

	else {
		for (int i = 0; i < currTexres.children.size(); i++) {
			const TexRes& childTexres = TEXRESTABLE_MGR->GetTileTexRes(currTexres.children[0][i]);
			frames.push_back(AnimationFrame(childTexres.filepath, childTexres.texcoord));
		}
	}
	return false;
}

