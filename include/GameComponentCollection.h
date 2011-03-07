/********************************************************
 *	GameComponentCollection.h							*
 *														*
 *	XFX GameComponentCollection definition file			*
 *	Copyright � XFX Team. All Rights Reserved			*
 ********************************************************/
#ifndef _XFX_GAMECOMPONENTCOLLECTION_
#define _XFX_GAMECOMPONENTCOLLECTION_

#include "Interfaces.h"
#include <System/Collections/Generic/List.h>

using namespace System::Collections::Generic;

namespace XFX
{

	/// <summary>
	/// A collection of game components.
	/// </summary>
	class GameComponentCollection
	{
	private:
		List<IGameComponent*> _components;

	protected:
		void ClearItems();
		void InsertItem(int index, IGameComponent* item);
		void RemoveItem(int index);
		void SetItem(int index, IGameComponent* item);
		
	public:
		GameComponentCollection();
	};
}

#endif //_XFX_GAMECOMPONENTCOLLECTION_
