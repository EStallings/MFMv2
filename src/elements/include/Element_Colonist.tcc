	/* -*- C++ -*- */

	#include "Element_Tower.h"

	namespace MFM
	{
		template <class CC>
		const typename CC::ATOM_TYPE& Element_Colonist<CC>::GetDefaultTower() const
		{
	    	return Element_Tower<CC>::THE_INSTANCE.GetDefaultAtom();
	  	}	
	}
    