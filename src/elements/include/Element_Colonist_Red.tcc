	/* -*- C++ -*- */

	#include "Element_Tower_Red.h"

	namespace MFM
	{
		template <class CC>
		const typename CC::ATOM_TYPE& Element_Colonist_Red<CC>::GetDefaultTower() const
		{
	    	return Element_Tower_Red<CC>::THE_INSTANCE.GetDefaultAtom();
	  	}	
	}
    