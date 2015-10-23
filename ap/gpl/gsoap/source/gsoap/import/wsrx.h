/*
	wsrx.h
*/

struct SOAP_ENV__Header
{
  _wsrm__Sequence                 *wsrm__Sequence                0;
  _wsrm__AckRequested             *wsrm__AckRequested            0;
  _wsrm__SequenceAcknowledgement  *wsrm__SequenceAcknowledgement 0;
  _wsrm__SequenceFault            *wsrm__SequenceFault           0;
};

//gsoap wsrm service name: wsrm

//gsoap wsrm service method-action: CreateSequence http://docs.oasis-open.org/ws-rx/wsrm/200702/CreateSequence
//gsoap wsrm service method-response-action: CreateSequence http://docs.oasis-open.org/ws-rx/wsrm/200702/CreateSequenceResponse
int __wsrm__CreateSequence(
  struct wsrm__CreateSequenceType *wsrm__CreateSequence,
  struct wsrm__CreateSequenceResponseType *wsrm__CreateSequenceResponse);

//gsoap wsrm service method-action: CloseSequence http://docs.oasis-open.org/ws-rx/wsrm/200702/CloseSequence
//gsoap wsrm service method-response-action: CloseSequence http://docs.oasis-open.org/ws-rx/wsrm/200702/CloseSequenceResponse
int __wsrm__CloseSequence(
  struct wsrm__CloseSequenceType *wsrm__CloseSequence,
  struct wsrm__CloseSequenceResponseType *wsrm__CloseSequenceResponse);

//gsoap wsrm service method-action: TerminateSequence http://docs.oasis-open.org/ws-rx/wsrm/200702/TerminateSequence
//gsoap wsrm service method-response-action: TerminateSequence http://docs.oasis-open.org/ws-rx/wsrm/200702/TerminateSequenceResponse
int __wsrm__TerminateSequence(
  struct wsrm__TerminateSequenceType *wsrm__TerminateSequence,
  struct wsrm__TerminateSequenceResponseType *wsrm__TerminateSequenceResponse);

