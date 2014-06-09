/*                                              -*- mode:C++ -*-
  ByteSource.h Abstract stream
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file ByteSource.h Abstract stream
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef BYTESOURCE_H
#define BYTESOURCE_H

#include "itype.h"
#include "Fail.h"
#include "Format.h"
#include "ByteSink.h"
#include <stdarg.h>    /* For ... */

namespace MFM {

  class ByteSource {
  public:
    ByteSource() : m_read(0), m_lastRead(-1), m_unread(false) { }

    s32 Read() {
      if (m_unread) m_unread = false;
      else m_lastRead = ReadByte();

      ++m_read;
      return m_lastRead;
    }

    void Unread() {
      if (m_unread) FAIL(ILLEGAL_STATE);
      m_unread = true;
      --m_read;
    }

    s32 Peek() {
      s32 read = Read();
      Unread();
      return read;
    }

    virtual int ReadByte() = 0;
    virtual ~ByteSource() { }

    u32 GetBytesRead() {
      return m_read;
    }

    bool Scan(s32 & result, Format::Type code = Format::DEC, u32 maxLen = U32_MAX) ;
    bool Scan(u32 & result, Format::Type code = Format::DEC, u32 maxLen = U32_MAX) ;
    s32 Scan(ByteSink & result, const char * stopOn, const char * continueOn = NULL) ;
    s32 ScanSet(ByteSink & result, const char * setSpec) {
      return ScanSetFormat(result, setSpec);
    }
    s32 SkipSet(const char * setSpec) {
      return ScanSet(DevNull, setSpec);
    }

    s32 ScanSetFormat(ByteSink & result, const char * & setSpec) ;

    static const char * WHITESPACE_CHARS;
    static const char * WHITESPACE_SET;
    static const char * NON_WHITESPACE_SET;

    s32 SkipWhitespace() {
      return SkipSet(WHITESPACE_SET);
    }

    s32 Scanf(const char * format, ...) ;  // NYI
    s32 Vscanf(const char * format, va_list & ap) ;

  private:
    u32 m_read;
    s32 m_lastRead;
    bool m_unread;
  };

}

#endif /* BYTESOURCE_H */
