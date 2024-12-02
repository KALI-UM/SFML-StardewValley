#include "pch.h"
#include "TileObject.h"
#include "DTile.h"

TileObject::TileObject()
{
}

TileObject::~TileObject()
{
}

bool TileObject::Initialize()
{
	m_TileSprite = new DTile();
	SetDrawableObj(m_TileSprite, false);
	return true;
}

ColliderType TileObject::GetColliderType(const CellIndex& currIndex) const
{
	CellIndex localIndex = currIndex - m_TLIndex;
	return m_TLOffsets[localIndex.y][localIndex.x];
}

void TileObject::OnColliderEnter(const CellIndex& currIndex)
{
	CellIndex localIndex = currIndex - m_TLIndex;

	switch (m_TLOffsets[localIndex.y][localIndex.x])
	{
	case ColliderType::Block:
		break;
	case ColliderType::Trigger:
		OnTriggerEnter(localIndex);
		break;
	case ColliderType::None:
		break;
	}
}

void TileObject::OnColliderStay(const CellIndex& currIndex)
{
	CellIndex localIndex = currIndex - m_TLIndex;

	switch (m_TLOffsets[localIndex.y][localIndex.x])
	{
	case ColliderType::Block:
		break;
	case ColliderType::Trigger:
		OnTriggerStay(localIndex);
		break;
	case ColliderType::None:
		break;
	}
}

void TileObject::OnColliderExit(const CellIndex& currIndex)
{
	CellIndex localIndex = currIndex - m_TLIndex;

	switch (m_TLOffsets[localIndex.y][localIndex.x])
	{
	case ColliderType::Block:
		break;
	case ColliderType::Trigger:
		OnTriggerExit(localIndex);
		break;
	case ColliderType::None:
		break;
	}
}
