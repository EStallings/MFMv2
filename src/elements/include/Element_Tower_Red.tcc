/* -*- C++ -*- */

#include "Element_Scout_Red.h"
#include "Element_Colonist_Red.h"
#include "Element_Breadcrumb_Red.h"
#include "Element_Empty.h"
#include "Element_Res.h"

namespace MFM
{
  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Red<CC>::GetDefaultScout() const
  {
    return Element_Scout_Red<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Red<CC>::GetDefaultColonist() const
  {
    return Element_Colonist_Red<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  void Element_Tower_Red<CC>::Behavior(EventWindow<CC>& window) const
  {
    typedef typename CC::ATOM_TYPE T;

    const T& self = window.GetCenterAtom();
    Random& rand = window.GetRandom();
    u32 resDemand = GetCurrentDemand(self);
    MDist<R> md = MDist<R>::get();
    u32 numEaten = 0;


    //Consume Res
    for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
    {
      SPoint pt = md.GetPoint(i);
      if(window.GetRelativeAtom(pt).GetType() == Element_Res<CC>::THE_INSTANCE.GetType())
      {
        if(rand.OneIn(11-resDemand)){
          window.SetRelativeAtom(pt, Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
          numEaten++;
        }
      }
    }

    LOG.Debug("Village %d ate %d res, demand before = %d", GetID(self), numEaten, resDemand);
    
    if(rand.OneIn(Abstract_Element_Tower<CC>::m_scoutSpawnChance.GetValue()) && GetNumConnections(self) < 4)
    {
      const T& scout = Element_Scout_Red<CC>::THE_INSTANCE.GetDefaultAtom();
      T mutableScout = Element_Scout_Red<CC>::THE_INSTANCE.GetMutableAtom(scout);
      Element_Scout_Red<CC>::THE_INSTANCE.SetTowerID(mutableScout, GetID(self));
      PlaceAtomRandomly(window, mutableScout);
    }

    if(rand.OneIn(Abstract_Element_Tower<CC>::m_resSpawnChance.GetValue()))
    {
      PlaceAtomRandomly(window, Element_Res<CC>::THE_INSTANCE.GetDefaultAtom());
    }

    //TODO Adjust supply and demand based on nearby breadcrumbs
    u32 localDemand = 0;
    u32 numConnections = 0;
    for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
    {
      SPoint pt = md.GetPoint(i);
      
      if(window.GetRelativeAtom(pt).GetType() == Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetType()){
        LOG.Debug("Village %d saw %d, upstream=%d, downstream=%d", GetID(self), 
          Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetIsEndpoint(window.GetRelativeAtom(pt)), 
          Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetDownstreamDemand(window.GetRelativeAtom(pt)), 
            Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetUpstreamDemand(window.GetRelativeAtom(pt))) ;

        if(Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetIsEndpoint(window.GetRelativeAtom(pt))){
          numConnections++;
          if(Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetEndpointSwitch(window.GetRelativeAtom(pt)) == 0){
            localDemand += Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetUpstreamDemand(window.GetRelativeAtom(pt));
          }
          else{
            localDemand += Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetDownstreamDemand(window.GetRelativeAtom(pt)); 
          }
        }
      }
    }
    
    T mutableMe = GetMutableAtom(window.GetCenterAtom());
    SetLocalDemand(mutableMe, GetCurrentDemand(self) + localDemand);
    SetNumConnections(mutableMe, numConnections);
    window.SetCenterAtom(mutableMe);

    //disabled for now
    if(rand.OneIn(Abstract_Element_Tower<CC>::m_colonistSpawnChance.GetValue()))
    {
      //PlaceAtomRandomly(window, GetDefaultColonist());
    }

  }
}
