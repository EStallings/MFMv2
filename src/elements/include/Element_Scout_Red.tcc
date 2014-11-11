	/* -*- C++ -*- */

	#include "Element_Tower_Red.h"
	#include "Element_Colonist_Red.h"
	#include "Element_Scout_Red.h"
	#include "Element_Breadcrumb_Red.h"

	namespace MFM
	{

		template <class CC>
		const Abstract_Element_Breadcrumb<CC>& Element_Scout_Red<CC>::GetBreadcrumbElement() const
		{	
		  return Element_Breadcrumb_Red<CC>::THE_INSTANCE;
		}
		
	    template <class CC>
	    const bool Element_Scout_Red<CC>::IsAtomInteresting(EventWindow<CC>& window, const T& atom) const
	    {

	    	const u32 type = atom.GetType();
	    	if(Element_Tower_Red<CC>::THE_INSTANCE.GetType() == type)
    		{
    			return true;
    		}
    		return false;
	    	
	    }


	}
    