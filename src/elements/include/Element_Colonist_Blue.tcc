	/* -*- C++ -*- */

	#include "Element_Tower_Blue.h"

	namespace MFM
	{
		template <class CC>
		const typename CC::ATOM_TYPE& Element_Colonist_Blue<CC>::GetDefaultTower() const
		{
	    	return Element_Tower_Blue<CC>::THE_INSTANCE.GetDefaultAtom();
	  	}	
	}
    