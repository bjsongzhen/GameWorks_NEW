// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#include "libcef_dll/ctocpp/waitable_event_ctocpp.h"


// STATIC METHODS - Body may be edited by hand.

CefRefPtr<CefWaitableEvent> CefWaitableEvent::CreateWaitableEvent(
    bool automatic_reset, bool initially_signaled) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_waitable_event_t* _retval = cef_waitable_event_create(
      automatic_reset,
      initially_signaled);

  // Return type: refptr_same
  return CefWaitableEventCToCpp::Wrap(_retval);
}


// VIRTUAL METHODS - Body may be edited by hand.

void CefWaitableEventCToCpp::Reset() {
  cef_waitable_event_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, reset))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->reset(_struct);
}

void CefWaitableEventCToCpp::Signal() {
  cef_waitable_event_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, signal))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->signal(_struct);
}

bool CefWaitableEventCToCpp::IsSignaled() {
  cef_waitable_event_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_signaled))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_signaled(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWaitableEventCToCpp::Wait() {
  cef_waitable_event_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, wait))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->wait(_struct);
}

bool CefWaitableEventCToCpp::TimedWait(int64 max_ms) {
  cef_waitable_event_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, timed_wait))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->timed_wait(_struct,
      max_ms);

  // Return type: bool
  return _retval?true:false;
}


// CONSTRUCTOR - Do not edit by hand.

CefWaitableEventCToCpp::CefWaitableEventCToCpp() {
}

template<> cef_waitable_event_t* CefCToCppRefCounted<CefWaitableEventCToCpp,
    CefWaitableEvent, cef_waitable_event_t>::UnwrapDerived(CefWrapperType type,
    CefWaitableEvent* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCppRefCounted<CefWaitableEventCToCpp,
    CefWaitableEvent, cef_waitable_event_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCppRefCounted<CefWaitableEventCToCpp,
    CefWaitableEvent, cef_waitable_event_t>::kWrapperType = WT_WAITABLE_EVENT;
