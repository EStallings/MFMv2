/* -*- C++ -*- */

#include "Element_Scout_Red.h"
#include "Element_Breadcrumb_Red.h"
#include "Element_Empty.h"
#include "Element_Res.h"

namespace MFM
{

  template <class CC>
  void Element_Tower_Red<CC>::Behavior(EventWindow<CC>& window) const
  {
    typedef typename CC::ATOM_TYPE T;

    Random& rand = window.GetRandom();
    u32 resDemand = GetCurrentDemand(window.GetCenterAtom());
    u32 resSupply = GetCurrentSupply(window.GetCenterAtom());
    MDist<R> md = MDist<R>::get();
    bool didEat = false;

    //Consume Res
    if(rand.OneIn(resDemand+1))
    {
      for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
      {
        SPoint pt = md.GetPoint(i);
        if(window.GetRelativeAtom(pt).GetType() == Element_Res<CC>::THE_INSTANCE.GetType())
        {
          window.SetRelativeAtom(pt, Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
          didEat = true;
        }
      }
    }

    if(!didEat){
      LOG.Debug("RESNOTFOUND Village [%d] with supply=[%d] demand=[%d] was unable to find res!", GetID(window.GetCenterAtom()), resSupply, resDemand);
    }
    else{
      LOG.Debug("RESFOUND Village [%d] with supply=[%d] demand=[%d] found res.", GetID(window.GetCenterAtom()), resSupply, resDemand); 
    }


    //LOG.Debug("Village %d ate %d res, demand before = %d", GetID(window.GetCenterAtom()), numEaten, resDemand);

    if(rand.OneIn(100) && GetNumConnections(window.GetCenterAtom()) < 4 && Abstract_Element_Tower<CC>::m_mode.GetValue() > 1)
    {
      const T& scout = Element_Scout_Red<CC>::THE_INSTANCE.GetDefaultAtom();
      T mutableScout = Element_Scout_Red<CC>::THE_INSTANCE.GetMutableAtom(scout);
      Element_Scout_Red<CC>::THE_INSTANCE.SetTowerID(mutableScout, GetID(window.GetCenterAtom()));
      PlaceAtomRandomly(window, mutableScout);
    }

    if(rand.OneIn(resSupply+1))
    {
      PlaceAtomRandomly(window, Element_Res<CC>::THE_INSTANCE.GetDefaultAtom());
    }

    //TODO Adjust supply and demand based on nearby breadcrumbs
    u32 localDemand = 0;
    u32 numConnections = 0;
    for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
    {
      SPoint pt = md.GetPoint(i);

      if(window.GetRelativeAtom(pt).GetType() == Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetType())
      {

        if(Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetIsEndpoint(window.GetRelativeAtom(pt)))
        {
          numConnections++;

          if(Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetEndpointSwitch(window.GetRelativeAtom(pt)) == 0)
          {
            localDemand += Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetDownstreamDemand(window.GetRelativeAtom(pt));
          }
          else
          {
            localDemand += Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetUpstreamDemand(window.GetRelativeAtom(pt));
          }
        }
      }
    }
    localDemand = (int)((float)localDemand/(float)numConnections);
    T mutableMe = GetMutableAtom(window.GetCenterAtom());
    SetLocalDemand(mutableMe, GetCurrentDemand(window.GetCenterAtom()) + localDemand);
    SetNumConnections(mutableMe, numConnections);
    window.SetCenterAtom(mutableMe);
  }
}
