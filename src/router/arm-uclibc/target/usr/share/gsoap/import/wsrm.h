/*

wsrm.h

Usage: See import/wsrx.h and plugin/wsrmapi.c

Generated with:
wsdl2h -cgyex -o wsrm.h -t WS/WS-typemap.dat WS/WS-ReliableMessaging.xsd

Modified by Robert van Engelen:

- Removed //gsoapopt
- Changed //gsoap schema namespace directive to import directive
- Added #import "wsrx.h"

*/

/******************************************************************************\
 *                                                                            *
 * http://docs.oasis-open.org/ws-rx/wsrm/200702                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/

#import "wsa5.h"	// wsa5 = <http://www.w3.org/2005/08/addressing>

/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/

//gsoap wsrm  schema import:	http://docs.oasis-open.org/ws-rx/wsrm/200702
//gsoap wsrm  schema elementForm:	qualified
//gsoap wsrm  schema attributeForm:	unqualified

/******************************************************************************\
 *                                                                            *
 * Schema Types                                                               *
 *                                                                            *
\******************************************************************************/


// Imported type "http://www.w3.org/2005/08/addressing":EndpointReferenceType defined by wsa5__EndpointReferenceType


/// Typedef synonym for struct wsrm__SequenceType.
typedef struct wsrm__SequenceType wsrm__SequenceType;

/// Typedef synonym for struct wsrm__AckRequestedType.
typedef struct wsrm__AckRequestedType wsrm__AckRequestedType;

/// Typedef synonym for struct wsrm__SequenceFaultType.
typedef struct wsrm__SequenceFaultType wsrm__SequenceFaultType;

/// Imported complexType "http://docs.oasis-open.org/ws-rx/wsrm/200702":DetailType from typemap WS/WS-typemap.dat.
// complexType definition intentionally left blank.

/// Typedef synonym for struct wsrm__CreateSequenceType.
typedef struct wsrm__CreateSequenceType wsrm__CreateSequenceType;

/// Typedef synonym for struct wsrm__CreateSequenceResponseType.
typedef struct wsrm__CreateSequenceResponseType wsrm__CreateSequenceResponseType;

/// Typedef synonym for struct wsrm__CloseSequenceType.
typedef struct wsrm__CloseSequenceType wsrm__CloseSequenceType;

/// Typedef synonym for struct wsrm__CloseSequenceResponseType.
typedef struct wsrm__CloseSequenceResponseType wsrm__CloseSequenceResponseType;

/// Typedef synonym for struct wsrm__TerminateSequenceType.
typedef struct wsrm__TerminateSequenceType wsrm__TerminateSequenceType;

/// Typedef synonym for struct wsrm__TerminateSequenceResponseType.
typedef struct wsrm__TerminateSequenceResponseType wsrm__TerminateSequenceResponseType;

/// Typedef synonym for struct wsrm__OfferType.
typedef struct wsrm__OfferType wsrm__OfferType;

/// Typedef synonym for struct wsrm__AcceptType.
typedef struct wsrm__AcceptType wsrm__AcceptType;

/// Typedef synonym for struct _wsrm__SequenceAcknowledgement.
typedef struct _wsrm__SequenceAcknowledgement _wsrm__SequenceAcknowledgement;

/// Imported complexType "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier from typemap WS/WS-typemap.dat.
/// @brief This type is for elements whose [children] is an anyURI and can have arbitrary attributes.
// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/ws-rx/wsrm/200702":Address from typemap WS/WS-typemap.dat.
// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/ws-rx/wsrm/200702":Expires from typemap WS/WS-typemap.dat.
// complexType definition intentionally left blank.

/// Typedef synonym for struct _wsrm__UsesSequenceSTR.
typedef struct _wsrm__UsesSequenceSTR _wsrm__UsesSequenceSTR;

/// Typedef synonym for struct _wsrm__UsesSequenceSSL.
typedef struct _wsrm__UsesSequenceSSL _wsrm__UsesSequenceSSL;

/// Imported simpleType "http://docs.oasis-open.org/ws-rx/wsrm/200702":MessageNumberType from typemap WS/WS-typemap.dat.
// simpleType definition intentionally left blank.

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":FaultCodes is a simpleType restriction of xs:QName.
enum wsrm__FaultCodes
{
	wsrm__SequenceTerminated,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceTerminated"
	wsrm__UnknownSequence,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":UnknownSequence"
	wsrm__InvalidAcknowledgement,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":InvalidAcknowledgement"
	wsrm__MessageNumberRollover,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":MessageNumberRollover"
	wsrm__CreateSequenceRefused,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequenceRefused"
	wsrm__SequenceClosed,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceClosed"
	wsrm__WSRMRequired,	///< xs:QName value=""http://docs.oasis-open.org/ws-rx/wsrm/200702":WSRMRequired"
};
/// Typedef synonym for enum wsrm__FaultCodes.
typedef enum wsrm__FaultCodes wsrm__FaultCodes;

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":IncompleteSequenceBehaviorType is a simpleType restriction of xs:string.
enum wsrm__IncompleteSequenceBehaviorType
{
	DiscardEntireSequence,	///< xs:string value="DiscardEntireSequence"
	DiscardFollowingFirstGap,	///< xs:string value="DiscardFollowingFirstGap"
	NoDiscard,	///< xs:string value="NoDiscard"
};
/// Typedef synonym for enum wsrm__IncompleteSequenceBehaviorType.
typedef enum wsrm__IncompleteSequenceBehaviorType wsrm__IncompleteSequenceBehaviorType;

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":UnsupportedElement is a simpleType restriction of xs:QName.

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceType is a complexType.
struct wsrm__SequenceType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// Element MessageNumber of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":MessageNumberType.
    ULONG64                              MessageNumber                  1;	///< Required element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":AckRequestedType is a complexType.
struct wsrm__AckRequestedType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceFaultType is a complexType.
struct wsrm__SequenceFaultType
{
/// Element FaultCode of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":FaultCodes.
    enum wsrm__FaultCodes                FaultCode                      1;	///< Required element.
/// Element Detail of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":DetailType.
    struct SOAP_ENV__Detail*             Detail                         0;	///< Optional element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequenceType is a complexType.
struct wsrm__CreateSequenceType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":AcksTo.
    wsa5__EndpointReferenceType          wsrm__AcksTo                   1;	///< Required element.
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Expires.
    char* /*duration*/                   wsrm__Expires                  0;	///< Optional element.
/// Element Offer of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":OfferType.
    struct wsrm__OfferType*              Offer                          0;	///< Optional element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequenceResponseType is a complexType.
struct wsrm__CreateSequenceResponseType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Expires.
    char* /*duration*/                   wsrm__Expires                  0;	///< Optional element.
/// Element IncompleteSequenceBehavior of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":IncompleteSequenceBehaviorType.
    enum wsrm__IncompleteSequenceBehaviorType*  IncompleteSequenceBehavior     0;	///< Optional element.
/// Element Accept of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":AcceptType.
    struct wsrm__AcceptType*             Accept                         0;	///< Optional element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":CloseSequenceType is a complexType.
struct wsrm__CloseSequenceType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// Element LastMsgNumber of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":MessageNumberType.
    ULONG64*                             LastMsgNumber                  0;	///< Optional element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":CloseSequenceResponseType is a complexType.
struct wsrm__CloseSequenceResponseType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":TerminateSequenceType is a complexType.
struct wsrm__TerminateSequenceType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// Element LastMsgNumber of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":MessageNumberType.
    ULONG64*                             LastMsgNumber                  0;	///< Optional element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":TerminateSequenceResponseType is a complexType.
struct wsrm__TerminateSequenceResponseType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":OfferType is a complexType.
struct wsrm__OfferType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// Element Endpoint of type "http://www.w3.org/2005/08/addressing":EndpointReferenceType.
    wsa5__EndpointReferenceType          Endpoint                       1;	///< Required element.
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Expires.
    char* /*duration*/                   wsrm__Expires                  0;	///< Optional element.
/// Element IncompleteSequenceBehavior of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":IncompleteSequenceBehaviorType.
    enum wsrm__IncompleteSequenceBehaviorType*  IncompleteSequenceBehavior     0;	///< Optional element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":AcceptType is a complexType.
struct wsrm__AcceptType
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":AcksTo.
    wsa5__EndpointReferenceType          wsrm__AcksTo                   1;	///< Required element.
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};


/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceAcknowledgement of complexType.

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceAcknowledgement is a complexType.
struct _wsrm__SequenceAcknowledgement
{
/// Element reference "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier.
    char* /*URI*/                        wsrm__Identifier               1;	///< Required element.
/// CHOICE OF ELEMENTS FOR choice
/// Size of array of ULONG64* is 1..unbounded
    int                                  __sizeNack                    ;
/// Array of length 1..unbounded
    ULONG64*                             Nack                           1;
    struct _wsrm__SequenceAcknowledgement_Final
    {
    }                                   *Final                          0;	///< Optional element.
/// CHOICE OF ELEMENTS FOR choice
/// Size of AcknowledgementRange array is 1..unbounded
    int                                  __sizeAcknowledgementRange    ;
    struct _wsrm__SequenceAcknowledgement_AcknowledgementRange
    {
/// Attribute Upper of type xs:unsignedLong.
   @ULONG64                              Upper                          1;	///< Required attribute.
/// Attribute Lower of type xs:unsignedLong.
   @ULONG64                              Lower                          1;	///< Required attribute.
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
    }                                   *AcknowledgementRange           1;
    struct _wsrm__SequenceAcknowledgement_None
    {
    }                                   *None                           1;	///< Required element.
//  END OF CHOICE
//  END OF CHOICE
/// TODO: <any namespace="##other" minOccurs="0" maxOccurs="unbounded">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this element.
///       Use wsdl2h option -d for xsd__anyType DOM (soap_dom_element).
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};


/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":UsesSequenceSTR of complexType.

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":UsesSequenceSTR is a complexType.
struct _wsrm__UsesSequenceSTR
{
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};


/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":UsesSequenceSSL of complexType.

/// "http://docs.oasis-open.org/ws-rx/wsrm/200702":UsesSequenceSSL is a complexType.
struct _wsrm__UsesSequenceSSL
{
/// <anyAttribute namespace="##other">
/// TODO: Schema extensibility is user-definable.
///       Consult the protocol documentation to change or insert declarations.
///       Use wsdl2h option -x to remove this attribute.
///       Use wsdl2h option -d for xsd__anyAttribute DOM (soap_dom_attribute).
};


/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":Identifier of complexType.


/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":Address of complexType.


/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":Expires of complexType.

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":Sequence of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceType.
typedef struct wsrm__SequenceType _wsrm__Sequence;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":AckRequested of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":AckRequestedType.
typedef struct wsrm__AckRequestedType _wsrm__AckRequested;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceFault of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":SequenceFaultType.
typedef struct wsrm__SequenceFaultType _wsrm__SequenceFault;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequence of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequenceType.
typedef struct wsrm__CreateSequenceType _wsrm__CreateSequence;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequenceResponse of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":CreateSequenceResponseType.
typedef struct wsrm__CreateSequenceResponseType _wsrm__CreateSequenceResponse;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":CloseSequence of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":CloseSequenceType.
typedef struct wsrm__CloseSequenceType _wsrm__CloseSequence;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":CloseSequenceResponse of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":CloseSequenceResponseType.
typedef struct wsrm__CloseSequenceResponseType _wsrm__CloseSequenceResponse;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":TerminateSequence of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":TerminateSequenceType.
typedef struct wsrm__TerminateSequenceType _wsrm__TerminateSequence;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":TerminateSequenceResponse of type "http://docs.oasis-open.org/ws-rx/wsrm/200702":TerminateSequenceResponseType.
typedef struct wsrm__TerminateSequenceResponseType _wsrm__TerminateSequenceResponse;

/// Element "http://docs.oasis-open.org/ws-rx/wsrm/200702":AcksTo of type "http://www.w3.org/2005/08/addressing":EndpointReferenceType.
typedef wsa5__EndpointReferenceType _wsrm__AcksTo;

#import "wsrx.h"

/* End of wsrm1.h */
