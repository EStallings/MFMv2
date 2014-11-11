	/* -*- C++ -*- */

	#include "Element_Tower_Red.h"

	namespace MFM
	{
	    template <class CC>
	    u32 Element_Breadcrumb_Red<CC>::GetMyTowerType() const
	    {
      		return Element_Tower_Red<CC>::THE_INSTANCE.GetType();
	    }
	}
    