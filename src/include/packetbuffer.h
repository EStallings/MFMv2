#ifndef PACKETBUFFER_H      /* -*- C++ -*- */
#define PACKETBUFFER_H

#include "packet.h"
#include "itype.h"

#define PACKETBUFFER_SIZE 128

template <class T>
class PacketBuffer
{
private:
  Packet<T> m_buffer[PACKETBUFFER_SIZE];

  u32 m_heldPackets;

public:

  PacketBuffer();

  void PushPacket(Packet<T>& packet);

  Packet<T>* PopPacket();

  u32 PacketsHeld()
  { return m_heldPackets; }
  
};

#include "packetbuffer.tcc"

#endif /*PACKETBUFFER_H*/